#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_HAND_CONTROLLER_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_HAND_CONTROLLER_H_

#include <vector>
#include <optional>
#include <glm/vec2.hpp>
#include "core/card_data.h"
#include "core/state/game_state.h"
#include "engine/ecs/components/transform.h"

namespace scenes {

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
 * @brief Manages input and logic for the player's hand.
 */
class HandController {
 public:
  HandController();

  void Update(float delta_time_seconds, core::state::GameState& state);
  void Render();

  std::optional<size_t> hovered_card_index() const { return hovered_card_index_; }
  std::optional<size_t> held_card_index() const { return held_card_index_; }

 private:
  void SyncHandWithState(const std::vector<std::unique_ptr<core::CardInstance>>& hand_state);
  void HandleInteraction(core::state::GameState& state);
  void UpdateLayout();
  void AnimateCards(float delta_time_seconds);

  std::vector<VisualCard> hand_visuals_;
  std::optional<size_t> hovered_card_index_;
  std::optional<size_t> held_card_index_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_HAND_CONTROLLER_H_
