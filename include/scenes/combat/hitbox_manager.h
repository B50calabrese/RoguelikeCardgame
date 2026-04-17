#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_HITBOX_MANAGER_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_HITBOX_MANAGER_H_

#include <vector>
#include <optional>
#include <glm/vec2.hpp>
#include "scenes/combat/board_hitbox.h"

namespace scenes::combat {

class HitboxManager {
 public:
  void Clear() { hitboxes_.clear(); }
  void AddHitbox(const BoardHitbox& hitbox) { hitboxes_.push_back(hitbox); }

  std::optional<BoardHitbox> GetHitboxAt(glm::vec2 mouse_pos) const;
  std::optional<BoardHitbox> GetHitboxFor(int instance_id) const;

  const std::vector<BoardHitbox>& hitboxes() const { return hitboxes_; }

 private:
  std::vector<BoardHitbox> hitboxes_;
};

} // namespace scenes::combat

#endif // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_HITBOX_MANAGER_H_
