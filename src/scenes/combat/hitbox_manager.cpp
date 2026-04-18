#include "scenes/combat/hitbox_manager.h"
#include "core/util/math_util.h"

namespace scenes::combat {

std::optional<BoardHitbox> HitboxManager::GetHitboxAt(glm::vec2 mouse_pos) const {
    for (const auto& hitbox : hitboxes_) {
        if (core::util::PointInRect(mouse_pos, hitbox.position, hitbox.size, true)) {
            return hitbox;
        }
    }
    return std::nullopt;
}

std::optional<BoardHitbox> HitboxManager::GetHitboxFor(int instance_id) const {
    for (const auto& hitbox : hitboxes_) {
        if (hitbox.instance_id == instance_id) {
            return hitbox;
        }
    }
    return std::nullopt;
}

} // namespace scenes::combat
