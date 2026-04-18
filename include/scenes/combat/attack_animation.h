#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_ATTACK_ANIMATION_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_ATTACK_ANIMATION_H_

#include <glm/vec2.hpp>
#include "core/effects/target.h"

namespace scenes {

struct AttackAnimation {
    int attacker_id;
    core::effects::Target target;
    glm::vec2 start_pos;
    glm::vec2 target_pos;
    float elapsed_time;
    float duration;
    bool moving_to_target;
};

} // namespace scenes

#endif // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_ATTACK_ANIMATION_H_
