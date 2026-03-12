#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_

#include <vector>

#include "core/card_data.h"
#include "engine/scene/scene.h"

namespace scenes {

/**
 * @brief Scene containing the combat section of the game.
 *
 * This scene handles the gameplay logic and rendering for card-based combat,
 * including managing the player's hand and interacting with the game engine.
 */
class CombatScene : public engine::Scene {
 public:
  /**
   * @brief Constructs a new CombatScene.
   */
  CombatScene() : Scene("CombatScene") {}

  /**
   * @brief Called when the scene is attached to the SceneManager.
   *
   * Initializes the scene by loading cards and setting up the initial hand.
   */
  void OnAttach() override;

  /**
   * @brief Called every frame to update the scene's logic.
   * @param delta_time_seconds Time elapsed since the last frame in seconds.
   */
  void OnUpdate(float delta_time_seconds) override;

  /**
   * @brief Called every frame to render the scene.
   */
  void OnRender() override;

 private:
  /// The collection of cards currently in the player's hand.
  std::vector<core::CardData> hand_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
