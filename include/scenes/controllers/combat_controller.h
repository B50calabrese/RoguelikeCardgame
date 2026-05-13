#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_CONTROLLERS_COMBAT_CONTROLLER_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_CONTROLLERS_COMBAT_CONTROLLER_H_

#include <glm/vec2.hpp>
#include <optional>
#include <vector>

#include "core/effects/game_event.h"
#include "core/state/combat_state.h"
#include "scenes/combat/attack_animation.h"
#include "scenes/combat/board_hitbox.h"
#include "scenes/combat/combat_animator.h"
#include "scenes/combat/combat_ui_state.h"
#include "scenes/combat/hitbox_manager.h"

namespace scenes::controllers {

class CombatController {
 public:
  void Update(float delta_time, core::state::CombatState& state, float icon_top,
              float icon_size);
  void HandleInput(core::state::CombatState& state, float icon_top,
                   float icon_size);
  void OnCreatureAttacked(core::state::CombatState& state,
                          const core::effects::GameEvent& event, float icon_top,
                          float icon_size);

  CombatUIState current_state() const { return current_state_; }
  std::optional<int> selected_attacker_id() const {
    return selected_attacker_id_;
  }

  combat::CombatAnimator& animator() { return animator_; }
  combat::HitboxManager& hitbox_manager() { return hitbox_manager_; }

 private:
  CombatUIState current_state_ = CombatUIState::Idle;
  std::optional<int> selected_attacker_id_;

  combat::CombatAnimator animator_;
  combat::HitboxManager hitbox_manager_;
};

}  // namespace scenes::controllers

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_CONTROLLERS_COMBAT_CONTROLLER_H_
