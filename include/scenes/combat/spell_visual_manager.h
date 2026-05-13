#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SPELL_VISUAL_MANAGER_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SPELL_VISUAL_MANAGER_H_

#include <glm/vec2.hpp>
#include <vector>

#include "core/state/combat_state.h"

namespace scenes::combat {

/**
 * @brief Manages the visual effects for spells being played.
 */
class SpellVisualManager {
 public:
  struct ActiveSpellVisual {
    int instance_id;
    float elapsed_time;
    glm::vec2 current_pos;
  };

  void Update(float delta_time, core::state::CombatState& state);
  void Render(const core::state::CombatState& state) const;

 private:
  std::vector<ActiveSpellVisual> active_visuals_;
};

}  // namespace scenes::combat

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_SPELL_VISUAL_MANAGER_H_
