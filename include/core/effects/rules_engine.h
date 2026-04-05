#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ENGINE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ENGINE_H_

#include <vector>
#include <string>
#include <memory>

#include "core/effects/action.h"
#include "core/state/game_state.h"
#include "core/effects/target_filter.h"
#include "core/effects/rule_result.h"
#include "core/effects/rule.h"

namespace core::effects {

/**
 * @brief Singleton for validating gameplay actions against rules.
 */
class RulesEngine {
 public:
  static RulesEngine& Get() {
    static RulesEngine instance;
    return instance;
  }

  /**
   * @brief Validates an action against all registered rules.
   */
  RuleResult ValidateAction(const state::GameState& state, const Action& action) const {
    // 1. Core action validation (basic structure)
    RuleResult result = action->Validate(state);
    if (!result.success) return result;

    // 2. Business rules
    for (const auto& rule : rules_) {
      result = rule->Validate(state, action);
      if (!result.success) return result;
    }

    return {true, "All rules passed", false};
  }

  /**
   * @brief Registers a new gameplay rule.
   */
  void RegisterRule(std::unique_ptr<IRule> rule) {
    rules_.push_back(std::move(rule));
  }

  /**
   * @brief Checks if a set of targets meets the requirements of a filter.
   */
  static RuleResult CheckTargeting(const state::GameState& state, int actor_id, const std::vector<Target>& targets, const TargetFilter& filter) {
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

 private:
  RulesEngine();
  std::vector<std::unique_ptr<IRule>> rules_;
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ENGINE_H_
