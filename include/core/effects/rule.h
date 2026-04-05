#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULE_H_

#include "core/effects/rule_result.h"
#include "core/state/game_state.h"
#include "core/effects/action.h"

namespace core::effects {

/**
 * @brief Interface for a gameplay rule.
 */
class IRule {
 public:
  virtual ~IRule() = default;

  /**
   * @brief Validates an action against this rule.
   * @return RuleResult indicating if the rule was satisfied.
   */
  virtual RuleResult Validate(const state::GameState& state, const Action& action) const = 0;
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULE_H_
