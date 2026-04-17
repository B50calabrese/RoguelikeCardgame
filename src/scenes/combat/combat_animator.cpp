#include "scenes/combat/combat_animator.h"
#include <glm/glm.hpp>
#include "core/effects/visual_blocker.h"

namespace scenes::combat {

void CombatAnimator::Update(float delta_time, core::state::GameState& state) {
    if (active_animation_) {
        active_animation_->elapsed_time += delta_time;
        if (active_animation_->moving_to_target && active_animation_->elapsed_time >= active_animation_->duration * 0.5f) {
            active_animation_->moving_to_target = false;
        }

        if (active_animation_->elapsed_time >= active_animation_->duration) {
            core::effects::VisualBlocker::Get().RemoveBlocker("CreatureAttack_" + std::to_string(active_animation_->attacker_id));
            active_animation_.reset();
        }
    }
}

void CombatAnimator::StartAnimation(const AttackAnimation& anim) {
    active_animation_ = anim;
}

glm::vec2 CombatAnimator::GetAnimatedPosition(int instance_id, glm::vec2 default_pos) const {
    if (active_animation_ && active_animation_->attacker_id == instance_id) {
        if (active_animation_->moving_to_target) {
            float local_t = active_animation_->elapsed_time / (active_animation_->duration * 0.5f);
            return glm::mix(active_animation_->start_pos, active_animation_->target_pos, local_t * local_t);
        } else {
            float local_t = (active_animation_->elapsed_time - active_animation_->duration * 0.5f) / (active_animation_->duration * 0.5f);
            return glm::mix(active_animation_->target_pos, active_animation_->start_pos, local_t);
        }
    }
    return default_pos;
}

} // namespace scenes::combat
