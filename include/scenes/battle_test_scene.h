#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_BATTLE_TEST_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_BATTLE_TEST_SCENE_H_

#include <memory>
#include <vector>
#include <string>

#include "engine/scene/scene.h"
#include "core/state/game_state.h"

namespace scenes {

/**
 * @brief A scene for testing battle mechanics and state manipulation via console commands.
 */
class BattleTestScene : public engine::Scene {
 public:
  BattleTestScene();
  ~BattleTestScene() override = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;

 private:
  /** @brief Registers console commands for this scene. */
  void RegisterCommands();

  /** @brief Adds a card to a player's hand or deck. */
  void AddCardCommand(const std::vector<std::string>& args);

  /** @brief Removes a card by instance ID. */
  void RemoveCardCommand(const std::vector<std::string>& args);

  /** @brief Lists all available cards in the registry. */
  void ListCardsCommand(const std::vector<std::string>& args);

  /** @brief Sets the health of a player. */
  void SetHealthCommand(const std::vector<std::string>& args);

  /** @brief Draws a card from the deck to the hand. */
  void DrawCardCommand(const std::vector<std::string>& args);

  /** @brief Sets the mana of a player. */
  void SetManaCommand(const std::vector<std::string>& args);

  /** @brief Sets the current turn player. */
  void SetTurnCommand(const std::vector<std::string>& args);

  /** @brief Plays a card from hand. */
  void PlayCardCommand(const std::vector<std::string>& args);

  core::GameState game_state_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_BATTLE_TEST_SCENE_H_
