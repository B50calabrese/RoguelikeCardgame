#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMMAND_SYSTEM_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMMAND_SYSTEM_H_

#include <string>
#include <vector>

#include "core/state/combat_state.h"

namespace scenes {

/**
 * @brief Handles registration and execution of console commands for
 * CombatScene.
 */
class CombatCommandSystem {
 public:
  static void Register(core::CombatState& state);

 private:
  static void AddCardCommand(core::CombatState& state,
                             const std::vector<std::string>& args);
  static void RemoveCardCommand(core::CombatState& state,
                                const std::vector<std::string>& args);
  static void ListCardsCommand(const std::vector<std::string>& args);
  static void SetHealthCommand(core::CombatState& state,
                               const std::vector<std::string>& args);
  static void DrawCardCommand(core::CombatState& state,
                              const std::vector<std::string>& args);
  static void SetManaCommand(core::CombatState& state,
                             const std::vector<std::string>& args);
  static void SetTurnCommand(core::CombatState& state,
                             const std::vector<std::string>& args);
  static void PlayCardCommand(core::CombatState& state,
                              const std::vector<std::string>& args);
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMMAND_SYSTEM_H_
