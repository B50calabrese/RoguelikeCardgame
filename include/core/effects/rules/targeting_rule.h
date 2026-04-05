#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_TARGETING_RULE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_TARGETING_RULE_H_

#include "core/effects/rule.h"
#include "core/effects/actions/play_card_action.h"
#include "core/state/player_state.h"
#include "core/card_instance.h"

namespace core::effects::rules {

/**
 * @brief Ensures a spell has legal targets before it's played.
 */
class TargetingRule : public IRule {
 public:
  RuleResult Validate(const state::GameState& state, const Action& action) const override {
    auto play_action = std::dynamic_pointer_cast<actions::PlayCardAction>(action);
    if (!play_action) return RuleResult::Success();

    int player_id = play_action->GetActorId();
    int card_id = play_action->card_instance_id();

    const PlayerState& p = (player_id == static_cast<int>(ActorId::Player)) ? *state.player : *state.enemy;

    const CardInstance* inst = nullptr;
    for (const auto& c : p.hand) {
      if (c->instance_id == card_id) {
        inst = c.get();
        break;
      }
    }

    if (!inst) return RuleResult::Failure("Card not in hand");

    // If it's a spell, check if there are legal targets for EVERY effect that requires one.
    if (inst->data->type == CardType::Spell) {
      for (const auto& effect_def : inst->data->effects) {
        if (!effect_def.filter.is_required) continue;

        bool found_legal_target = false;
        if (effect_def.filter.IsValid(state, player_id, {Target::Type::Player, state.player->id})) found_legal_target = true;
        if (!found_legal_target && effect_def.filter.IsValid(state, player_id, {Target::Type::Enemy, state.enemy->id})) found_legal_target = true;

        if (!found_legal_target) {
          for (const auto& c : state.player->board) {
            if (effect_def.filter.IsValid(state, player_id, {Target::Type::Creature, c->instance_id})) {
              found_legal_target = true;
              break;
            }
          }
        }
        if (!found_legal_target) {
          for (const auto& c : state.enemy->board) {
            if (effect_def.filter.IsValid(state, player_id, {Target::Type::Creature, c->instance_id})) {
              found_legal_target = true;
              break;
            }
          }
        }

        if (!found_legal_target) {
          return RuleResult::Failure("No legal targets for spell effects");
        }
      }
    }

    return RuleResult::Success("Targeting valid");
  }
};

}  // namespace core::effects::rules

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_TARGETING_RULE_H_
