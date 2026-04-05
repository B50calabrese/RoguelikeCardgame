#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_

#include <optional>
#include <vector>

#include <glm/vec2.hpp>

#include "core/card_data.h"
#include "core/state/game_state.h"
#include "core/graphics/battle_ui.h"
#include "core/ai/battle_ai.h"
#include "engine/ecs/components/transform.h"
#include "engine/scene/scene.h"
#include "scenes/hand_controller.h"

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
  core::GameState game_state_;
  core::graphics::BattleUI battle_ui_;
  std::unique_ptr<core::ai::IBattleAI> enemy_ai_;

  std::unique_ptr<HandController> player_hand_;
  std::unique_ptr<HandController> enemy_hand_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
