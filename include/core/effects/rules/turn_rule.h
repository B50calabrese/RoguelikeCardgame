#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_TURN_RULE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_TURN_RULE_H_

#include "core/effects/rule.h"
#include "core/effects/actions/action_base.h"

namespace core::effects::rules {

/**
 * @brief Ensures the action is performed by the current player.
 */
class TurnRule : public IRule {
 public:
  RuleResult Validate(const state::GameState& state, const Action& action) const override {
    int actor_id = action->GetActorId();
    if (actor_id != -1 && state.current_turn_player_id != actor_id) {
      return {false, "Not your turn", false};
    }
    return {true, "Correct turn", false};
  }
};

}  // namespace core::effects::rules

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_TURN_RULE_H_
