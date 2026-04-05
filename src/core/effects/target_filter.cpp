#include "core/effects/target_filter.h"
#include "core/state/game_state.h"
#include "core/card_instance.h"

namespace core::effects {

bool TargetFilter::IsValid(const state::GameState& state, int actor_player_id, const Target& target) const {
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

}  // namespace core::effects
