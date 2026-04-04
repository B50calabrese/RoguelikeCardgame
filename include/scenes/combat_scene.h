#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_

#include <optional>
#include <vector>

#include <glm/vec2.hpp>

#include "core/card_data.h"
#include "engine/scene/scene.h"

namespace scenes {

/**
 * @brief Visual representation of a card in the hand with animation state.
 */
struct VisualCard {
  core::CardData data;

  // Current visual state (for rendering)
  glm::vec2 current_pos;
  float current_scale;
  float current_rotation;

  // Target visual state (for animation)
  glm::vec2 target_pos;
  float target_scale;
  float target_rotation;

  bool is_held = false;
};

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
  void UpdateHandLayout();
  void HandleCardInteraction(float delta_time_seconds);
  void AnimateCards(float delta_time_seconds);

  std::vector<VisualCard> hand_visuals_;
  std::optional<size_t> hovered_card_index_;
  std::optional<size_t> held_card_index_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SCENE_H_
