#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMBAT_ANIMATOR_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMBAT_ANIMATOR_H_

#include <optional>
#include <glm/vec2.hpp>
#include "scenes/combat/attack_animation.h"
#include "core/state/game_state.h"

namespace scenes::combat {

class CombatAnimator {
 public:
  void Update(float delta_time, core::state::GameState& state);
  void StartAnimation(const AttackAnimation& anim);

  glm::vec2 GetAnimatedPosition(int instance_id, glm::vec2 default_pos) const;
  const std::optional<AttackAnimation>& active_animation() const { return active_animation_; }

 private:
  std::optional<AttackAnimation> active_animation_;
};

} // namespace scenes::combat

#endif // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMBAT_ANIMATOR_H_
