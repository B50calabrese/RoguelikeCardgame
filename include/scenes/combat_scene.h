#ifndef DECK_BUILDER_GAME_SCENES_COMBAT_SCENE_H_
#define DECK_BUILDER_GAME_SCENES_COMBAT_SCENE_H_

#include <engine/scene/scene.h>

namespace scenes {

// Scene containing the combat section of the game.
class CombatScene : public engine::Scene {
 public:
  CombatScene() : Scene("CombatScene") {}

  void OnAttach() override {}

  void OnUpdate(float delta_time_seconds) override {
    // Game logic (AI, Card movement) goes here
  }

  void OnRender() override {}
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_SCENES_COMBAT_SCENE_H_
