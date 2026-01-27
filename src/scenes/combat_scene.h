#ifndef COMBAT_SCENE_H
#define COMBAT_SCENE_H

#include "scene.h"

namespace scenes {

/**
 * Scene containing the combat section of the game.
 */
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

#endif  // COMBAT_SCENE_H