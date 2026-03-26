#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ENGINE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ENGINE_H_

#include <vector>
#include <string>

#include "core/effects/action.h"
#include "core/state/game_state.h"
#include "core/effects/target_filter.h"
#include "core/effects/rule_result.h"

namespace core::effects {

/**
 * @brief Singleton for validating gameplay actions against rules.
 */
class RulesEngine {
 public:
  static RuleResult CanPerformAction(const GameState& state, const Action& action) {
    return action->Validate(state);
  }

  /**
   * @brief Checks if a set of targets meets the requirements of a filter.
   */
  static RuleResult CheckTargeting(const GameState& state, int actor_id, const std::vector<Target>& targets, const TargetFilter& filter) {
    if (filter.is_required && targets.empty()) {
        return {false, "Targets required", false};
    }

    for (const auto& t : targets) {
        if (!filter.IsValid(state, actor_id, t)) {
            return {false, "Invalid target for filter", false};
        }
    }

    return {true, "Targets valid", false};
  }
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ENGINE_H_
