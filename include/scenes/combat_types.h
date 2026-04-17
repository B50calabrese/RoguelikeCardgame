#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_TYPES_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_TYPES_H_

#include <glm/vec2.hpp>
#include "core/effects/target.h"

namespace scenes {

enum class CombatState {
    Idle,
    PickingTarget,
    AnimatingAttack
};

struct AttackAnimation {
    int attacker_id;
    core::effects::Target target;
    glm::vec2 start_pos;
    glm::vec2 target_pos;
    float elapsed_time;
    float duration;
    bool moving_to_target;
};

struct BoardHitbox {
    int instance_id;
    glm::vec2 position;
    glm::vec2 size;
    bool is_enemy;
};

} // namespace scenes

#endif // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_TYPES_H_
