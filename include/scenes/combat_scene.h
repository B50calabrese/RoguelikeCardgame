#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_

#include <optional>
#include <vector>

#include <glm/vec2.hpp>

#include "core/card_data.h"
#include "core/state/game_state.h"
#include "core/graphics/battle_ui.h"
#include "core/ai/battle_ai.h"
#include "core/controllers/hand_controller.h"
#include "engine/ecs/components/transform.h"
#include "engine/scene/scene.h"

namespace scenes {

/**
 * Scene containing the combat section of the game.
 */
class CombatScene : public engine::Scene {
 public:
  CombatScene() : Scene("CombatScene") {}

  void OnAttach() override;

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

  // UI Constants (Relative to window size)
  float border_thickness_ = 0.0f;
  float icon_size_ = 0.0f;
  float icon_top_ = 0.0f;
  float enemy_icon_bottom_ = 0.0f;

  glm::vec2 hand_bounds_size_ = {0.0f, 0.0f};
  glm::vec2 player_hand_pos_ = {0.0f, 0.0f};
  glm::vec2 enemy_hand_pos_ = {0.0f, 0.0f};

  core::GameState game_state_;
  core::graphics::BattleUI battle_ui_;
  std::unique_ptr<core::ai::IBattleAI> enemy_ai_;

  std::unique_ptr<core::controllers::HandController> player_hand_;
  std::unique_ptr<core::controllers::HandController> enemy_hand_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
