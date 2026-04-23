#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_

#include <glm/vec2.hpp>
#include <optional>
#include <vector>

#include "core/ai/battle_ai.h"
#include "core/card_data.h"
#include "core/effects/game_event.h"
#include "core/graphics/battle_ui.h"
#include "core/state/game_state.h"
#include "engine/ecs/components/transform.h"
#include "engine/scene/scene.h"
#include "scenes/combat/attack_animation.h"
#include "scenes/combat/board_hitbox.h"
#include "scenes/combat/combat_state.h"
#include "scenes/combat/combat_ui_constants.h"
#include "scenes/controllers/combat_controller.h"
#include "scenes/controllers/hand_controller.h"

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
  void DrawTargetingLine();

  // UI Constants (Relative to window size)
  float kBorderThickness = 0.0f;
  float kIconSize = 0.0f;
  float kIconTop = 0.0f;
  float kEnemyIconBottom = 0.0f;

  glm::vec2 kHandBoundsSize = {0.0f, 0.0f};
  glm::vec2 kPlayerHandPos = {0.0f, 0.0f};
  glm::vec2 kEnemyHandPos = {0.0f, 0.0f};

  glm::vec2 kBoardBoundsSize = {0.0f, 0.0f};
  glm::vec2 kPlayerBoardPos = {0.0f, 0.0f};
  glm::vec2 kEnemyBoardPos = {0.0f, 0.0f};

  core::GameState game_state_;
  core::graphics::BattleUI battle_ui_;
  std::unique_ptr<core::ai::IBattleAI> enemy_ai_;

  std::unique_ptr<controllers::HandController> player_hand_;
  std::unique_ptr<controllers::HandController> enemy_hand_;
  std::unique_ptr<controllers::CombatController> combat_controller_;

  // Hitbox cache for board creatures
  std::vector<BoardHitbox> board_hitboxes_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
