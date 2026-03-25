#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULE_RESULT_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULE_RESULT_H_

#include <string>

namespace core::effects {

/**
 * @brief Represents the result of a rule check.
 */
struct RuleResult {
  bool success;
  std::string message;
  bool fail_open;  // If true, the action fails but doesn't necessarily block.
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULE_RESULT_H_
