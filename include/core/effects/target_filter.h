#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TARGET_FILTER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TARGET_FILTER_H_

#include <string>
#include <vector>
#include <optional>

#include "core/effects/target.h"

namespace core::state { struct GameState; }
namespace core { struct CardInstance; }

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
  bool IsValid(const state::GameState& state, int actor_player_id, const Target& target) const;
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TARGET_FILTER_H_
