#include "core/effects/actions/modify_stats_action.h"

#include "core/card_data.h"
#include "core/effects/actions/kill_creature_action.h"
#include "core/effects/effect_resolver.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

void ModifyStatsAction::Apply(CombatState& state) const {
  CardInstance* inst = state.FindCardInstance(card_instance_id_);
  if (inst && inst->location == CardLocation::Board) {
    if (duration_ == core::ModifierDuration::Permanent) {
      inst->permanent_power_offset += power_change_;
      inst->permanent_health_offset += health_change_;
      inst->current_health += health_change_;
      inst->RecalculateStats();
      LOG_INFO("[EffectResolver] Modified %s stats permanently: P=%d, H=%d",
               inst->data->name.c_str(), inst->current_power,
               inst->current_health);
    } else {
      inst->modifiers.push_back({power_change_, health_change_, duration_});
      // For temporary health reduction, it's usually treated as damage or
      // reducing max health. If it's a -X/-H effect, we apply it to current
      // health too.
      if (health_change_ < 0) {
        inst->current_health += health_change_;
      }
      inst->RecalculateStats();
      LOG_INFO("[EffectResolver] Modified %s stats temporarily: P=%d, H=%d",
               inst->data->name.c_str(), inst->current_power,
               inst->current_health);
    }

    if (inst->current_health <= 0) {
      EffectResolver::Get().QueueAction(
          std::make_shared<KillCreatureAction>(inst->instance_id));
    }
  }
}

}  // namespace core::effects::actions
