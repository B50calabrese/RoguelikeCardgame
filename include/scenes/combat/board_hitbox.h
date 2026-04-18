#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_BOARD_HITBOX_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_BOARD_HITBOX_H_

#include <glm/vec2.hpp>

namespace scenes {

struct BoardHitbox {
    int instance_id;
    glm::vec2 position;
    glm::vec2 size;
    bool is_enemy;
};

} // namespace scenes

#endif // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_BOARD_HITBOX_H_
