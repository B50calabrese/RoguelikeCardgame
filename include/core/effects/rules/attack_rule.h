#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ATTACK_RULE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ATTACK_RULE_H_

#include "core/effects/rule.h"
#include "core/effects/actions/creature_attack_action.h"

namespace core::effects::rules {

/**
 * @brief Rule enforcing attack constraints (turn, exhausted, summoning sickness).
 */
class AttackRule : public IRule {
 public:
  RuleResult Validate(const state::GameState& state, const Action& action) const override {
    auto attack_action = std::dynamic_pointer_cast<actions::CreatureAttackAction>(action);
    if (!attack_action) return {true, "Not an attack action", false};

    CardInstance* attacker = state.FindCardInstance(attack_action->attacker_id());
    if (!attacker) return {false, "Attacker not found", true};

    if (attacker->owner_id != state.current_turn_player_id) {
        return {false, "Cannot attack on opponent's turn", false};
    }

    if (attacker->location != CardLocation::Board) {
        return {false, "Attacker must be on board", true};
    }

    if (attacker->has_attacked) {
        return {false, "Creature already attacked this turn", false};
    }

    if (!attacker->can_attack) {
        return {false, "Creature cannot attack this turn (Summoning Sickness?)", false};
    }

    // Check that target is an opposing creature or player
    if (attack_action->target().type == Target::Type::Creature) {
        CardInstance* target_inst = state.FindCardInstance(attack_action->target().id);
        if (target_inst && target_inst->owner_id == attacker->owner_id) {
            return {false, "Cannot attack your own creatures", false};
        }
    } else if (attack_action->target().type == Target::Type::Player) {
        if (attacker->owner_id == state.player->id) {
            return {false, "Cannot attack yourself", false};
        }
    } else if (attack_action->target().type == Target::Type::Enemy) {
        if (attacker->owner_id == state.enemy->id) {
            return {false, "Cannot attack yourself", false};
        }
    }

    return {true, "Attack valid", false};
  }
};

}  // namespace core::effects::rules

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ATTACK_RULE_H_
