#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_CONTROLLERS_HAND_CONTROLLER_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_CONTROLLERS_HAND_CONTROLLER_H_

#include <glm/vec2.hpp>
#include <optional>
#include <vector>

#include "core/card_data.h"
#include "core/state/game_state.h"
#include "engine/ecs/components/transform.h"
#include "scenes/combat/hitbox_manager.h"

namespace scenes::controllers {

/**
 * @brief Visual representation of a card in the hand with animation state.
 */
struct VisualCard {
  core::CardData data;
  int instance_id;

  // Current visual state (for rendering)
  engine::ecs::components::Transform current_transform;

  // Target visual state (for animation)
  engine::ecs::components::Transform target_transform;

  bool is_held = false;
};

/**
 * @brief Manages input and logic for a player's hand.
 */
class HandController {
 public:
  HandController(int player_id);

  void Update(float delta_time_seconds, core::state::GameState& state,
              combat::HitboxManager* hitbox_manager = nullptr);
  void Render();

  void SetBounds(glm::vec2 pos, glm::vec2 size) {
    bounds_pos_ = pos;
    bounds_size_ = size;
  }
  void SetArcAngle(float angle) { arc_angle_ = angle; }
  void SetInteractive(bool interactive) { is_interactive_ = interactive; }
  void SetFaceDown(bool face_down) { is_face_down_ = face_down; }
  void SetPlayZone(glm::vec4 zone) { play_zone_ = zone; }

  std::optional<size_t> hovered_card_index() const {
    return hovered_card_index_;
  }
  std::optional<size_t> held_card_index() const { return held_card_index_; }

  bool is_picking_target() const { return is_picking_target_; }

 private:
  void SyncHandWithState(
      const std::vector<std::unique_ptr<core::CardInstance>>& hand_state);
  void HandleInteraction(core::state::GameState& state);
  void UpdateLayout();
  void AnimateCards(float delta_time_seconds);
  void DrawTargetingLine();
  std::optional<core::effects::Target> FindTargetAt(
      const core::state::GameState& state, glm::vec2 mouse_pos,
      combat::HitboxManager* hitbox_manager);

  int player_id_;
  glm::vec2 bounds_pos_;
  glm::vec2 bounds_size_;
  float arc_angle_;
  bool is_interactive_ = true;
  bool is_face_down_ = false;
  std::optional<glm::vec4> play_zone_;

  std::vector<VisualCard> hand_visuals_;
  std::optional<size_t> hovered_card_index_;
  std::optional<size_t> held_card_index_;

  bool is_picking_target_ = false;
  core::effects::TargetFilter current_target_filter_;
};

}  // namespace scenes::controllers

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_CONTROLLERS_HAND_CONTROLLER_H_
