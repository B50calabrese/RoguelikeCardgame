#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_TURN_RULE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_TURN_RULE_H_

#include "core/effects/actions/action_base.h"
#include "core/effects/rule.h"

namespace core::effects::rules {

/**
 * @brief Ensures the action is performed by the current player.
 */
class TurnRule : public IRule {
 public:
  RuleResult Validate(const state::CombatState& state,
                      const Action& action) const override {
    int actor_id = action->actor_id();
    if (actor_id != static_cast<int>(ActorId::None) &&
        state.current_turn_player_id != actor_id) {
      return RuleResult::Failure("Not your turn");
    }
    return RuleResult::Success("Correct turn");
  }
};

}  // namespace core::effects::rules

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_TURN_RULE_H_
