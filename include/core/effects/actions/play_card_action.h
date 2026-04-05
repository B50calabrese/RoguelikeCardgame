#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_PLAY_CARD_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_PLAY_CARD_ACTION_H_

#include <vector>
#include <algorithm>

#include "core/effects/actions/action_base.h"
#include "core/effects/actions/resolve_spell_action.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/event_bus.h"
#include "core/effects/game_event.h"
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
    // Basic structure validation only. Logic-based rules are now in RulesEngine.
    return {true, "OK", false};
  }

  void Apply(state::GameState& state) const override {
    PlayerState& p = (player_id_ == 0) ? *state.player : *state.enemy;

    auto it = std::find_if(p.hand.begin(), p.hand.end(),
        [id = card_instance_id_](const auto& c) { return c->instance_id == id; });

    if (it != p.hand.end()) {
        LOG_INFO("[EffectResolver] Playing card: %s", (*it)->data->name.c_str());
        p.mana -= (*it)->current_cost;

        int card_id = (*it)->instance_id;
        if ((*it)->data->type == CardType::Creature) {
            (*it)->location = CardLocation::Board;
            p.board.push_back(std::move(*it));
        } else {
            (*it)->location = CardLocation::Stack;
            p.stack.push_back(std::move(*it));
            // Queue resolution
            EffectResolver::Get().QueueAction(std::make_shared<ResolveSpellAction>(player_id_, card_id, targets_));
        }
        p.hand.erase(it);

        // Broadcast card played event
        EventBus::Get().Publish(state, {GameEventType::CardPlayed, player_id_, card_id});
    }
  }

  std::string name() const override { return "PlayCard"; }

  int GetActorId() const override { return player_id_; }
  int card_instance_id() const { return card_instance_id_; }
  const std::vector<Target>& targets() const { return targets_; }

 private:
  int player_id_;
  int card_instance_id_;
  std::vector<Target> targets_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_PLAY_CARD_ACTION_H_
