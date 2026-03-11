#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_

#include <vector>

#include "core/card_data.h"
#include "engine/scene/scene.h"

namespace scenes {

/**
 * @brief Scene containing the combat section of the game.
 *
 * This scene handles the loading of cards, management of the player's hand,
 * and rendering of the combat interface.
 */
class CombatScene : public engine::Scene {
 public:
  /**
   * @brief Constructs a new CombatScene.
   */
  CombatScene() : Scene("CombatScene") {}

  /**
   * @brief Called when the scene is attached to the SceneManager.
   * Loads initial card data and populates the hand.
   */
  void OnAttach() override;

  /**
   * @brief Updates the scene logic every frame.
   * @param delta_time_seconds Time elapsed since the last frame in seconds.
   */
  void OnUpdate(float delta_time_seconds) override;

  /**
   * @brief Renders the scene's visual components.
   */
  void OnRender() override;

 private:
  std::vector<core::CardData> hand_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
