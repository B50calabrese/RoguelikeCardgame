#include "core/effects/actions/modify_stats_action.h"
#include "core/effects/actions/kill_creature_action.h"
#include "core/effects/effect_resolver.h"
#include "core/card_data.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

void ModifyStatsAction::Apply(GameState& state) const {
    CardInstance* inst = state.FindCardInstance(card_instance_id_);
    if (inst && inst->location == CardLocation::Board) {
        inst->current_power += power_change_;
        inst->current_health += health_change_;
        inst->max_health += health_change_;
        LOG_INFO("[EffectResolver] Modified %s stats: P=%d, H=%d", inst->data->name.c_str(), inst->current_power, inst->current_health);

        if (inst->current_health <= 0) {
            EffectResolver::Get().QueueAction(std::make_shared<KillCreatureAction>(inst->instance_id));
        }
    }
}

}  // namespace core::effects::actions
