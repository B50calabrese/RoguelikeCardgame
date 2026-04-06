#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMMANDER_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMMANDER_H_

#include <string>
#include <vector>

#include "core/state/game_state.h"

namespace scenes {

/**
 * @brief Handles registration and execution of console commands for CombatScene.
 */
class CombatCommander {
 public:
  static void Register(core::GameState& state);

 private:
  static void AddCardCommand(core::GameState& state, const std::vector<std::string>& args);
  static void RemoveCardCommand(core::GameState& state, const std::vector<std::string>& args);
  static void ListCardsCommand(const std::vector<std::string>& args);
  static void SetHealthCommand(core::GameState& state, const std::vector<std::string>& args);
  static void DrawCardCommand(core::GameState& state, const std::vector<std::string>& args);
  static void SetManaCommand(core::GameState& state, const std::vector<std::string>& args);
  static void SetTurnCommand(core::GameState& state, const std::vector<std::string>& args);
  static void PlayCardCommand(core::GameState& state, const std::vector<std::string>& args);
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMMANDER_H_
