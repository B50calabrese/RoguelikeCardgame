#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_PLAY_CARD_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_PLAY_CARD_ACTION_H_

#include <vector>
#include <algorithm>

#include "core/effects/actions/action_base.h"
#include "core/effects/actions/resolve_spell_action.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/target.h"
#include "core/effects/rule_result.h"
#include "core/state/game_state.h"
#include "core/card_data.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

class PlayCardAction : public ActionBase {
 public:
  PlayCardAction(int player_id, int card_instance_id, std::vector<Target> targets)
      : player_id_(player_id), card_instance_id_(card_instance_id), targets_(std::move(targets)) {}

  RuleResult Validate(const state::GameState& state) const override {
    if (state.current_turn_player_id != player_id_) {
        return {false, "Not your turn", false};
    }

    const PlayerState& p = (player_id_ == 0) ? *state.player : *state.enemy;

    CardInstance* inst = nullptr;
    for (const auto& c : p.hand) {
      if (c->instance_id == card_instance_id_) {
        inst = c.get();
        break;
      }
    }

    if (!inst) return {false, "Card not in hand", false};
    if (p.mana < inst->current_cost) return {false, "Insufficient mana", false};

    // If it's a spell, check if there are legal targets for EVERY effect that requires one.
    if (inst->data->type == CardType::Spell) {
      for (const auto& effect_def : inst->data->effects) {
        if (!effect_def.filter.is_required) continue;

        // Check if there is AT LEAST ONE legal target in the current state.
        bool found_legal_target = false;

        // 1. Check players
        if (effect_def.filter.IsValid(state, player_id_, {Target::Type::Player, state.player->id})) found_legal_target = true;
        if (!found_legal_target && effect_def.filter.IsValid(state, player_id_, {Target::Type::Enemy, state.enemy->id})) found_legal_target = true;

        // 2. Check creatures on board
        if (!found_legal_target) {
          for (const auto& c : state.player->board) {
            if (effect_def.filter.IsValid(state, player_id_, {Target::Type::Creature, c->instance_id})) {
              found_legal_target = true;
              break;
            }
          }
        }
        if (!found_legal_target) {
          for (const auto& c : state.enemy->board) {
            if (effect_def.filter.IsValid(state, player_id_, {Target::Type::Creature, c->instance_id})) {
              found_legal_target = true;
              break;
            }
          }
        }

        if (!found_legal_target) {
          return {false, "No legal targets for spell effects", false};
        }
      }
    }

    return {true, "Valid play", false};
  }

  void Apply(state::GameState& state) const override {
    PlayerState& p = (player_id_ == 0) ? *state.player : *state.enemy;

    auto it = std::find_if(p.hand.begin(), p.hand.end(),
        [id = card_instance_id_](const auto& c) { return c->instance_id == id; });

    if (it != p.hand.end()) {
        LOG_INFO("[EffectResolver] Playing card: %s", (*it)->data->name.c_str());
        p.mana -= (*it)->current_cost;

        if ((*it)->data->type == CardType::Creature) {
            (*it)->location = CardLocation::Board;
            p.board.push_back(std::move(*it));
        } else {
            (*it)->location = CardLocation::Stack;
            int card_id = (*it)->instance_id;
            p.stack.push_back(std::move(*it));
            // Queue resolution
            EffectResolver::Get().QueueAction(std::make_shared<ResolveSpellAction>(player_id_, card_id, targets_));
        }
        p.hand.erase(it);
    }
  }

  std::string name() const override { return "PlayCard"; }

 private:
  int player_id_;
  int card_instance_id_;
  std::vector<Target> targets_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_PLAY_CARD_ACTION_H_
