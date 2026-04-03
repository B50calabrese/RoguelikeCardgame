#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TARGET_FILTER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TARGET_FILTER_H_

#include <string>
#include <vector>
#include <optional>

#include "core/effects/target.h"
#include "core/state/game_state.h"

namespace core::effects {

/**
 * @brief Simple filter to specify allowed targets for an effect.
 */
struct TargetFilter {
  bool is_required = true;

  // Types to allow
  bool allow_creature = true;
  bool allow_player = true;
  bool allow_enemy = true;
  bool allow_self = true;

  // Numerical constraints (if applicable)
  std::optional<int> max_cost;
  std::optional<int> min_cost;
  std::optional<int> max_power;
  std::optional<int> min_power;
  std::optional<int> max_health;
  std::optional<int> min_health;

  // Tag constraints (for creature types, etc)
  std::vector<std::string> required_tags;

  /**
   * @brief Checks if a given target is valid according to this filter.
   */
  bool IsValid(const state::GameState& state, int actor_player_id, const Target& target) const {
    if (target.type == Target::Type::Player || target.type == Target::Type::Enemy) {
      if (!allow_player) return false;
      bool is_actor = (target.id == actor_player_id);
      if (is_actor && !allow_self) return false;
      if (!is_actor && !allow_enemy) return false;
      return true;
    }

    if (target.type == Target::Type::Creature) {
      if (!allow_creature) return false;
      CardInstance* inst = state.FindCardInstance(target.id);
      if (!inst || inst->location != CardLocation::Board) return false;

      bool is_actor_creature = (inst->owner_id == actor_player_id);
      if (is_actor_creature && !allow_self) return false;
      if (!is_actor_creature && !allow_enemy) return false;

      // Stats check
      if (max_cost && inst->current_cost > *max_cost) return false;
      if (min_cost && inst->current_cost < *min_cost) return false;
      if (max_power && inst->current_power > *max_power) return false;
      if (min_power && inst->current_power < *min_power) return false;
      if (max_health && inst->current_health > *max_health) return false;
      if (min_health && inst->current_health < *min_health) return false;

      return true;
    }

    return false;
  }
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TARGET_FILTER_H_
