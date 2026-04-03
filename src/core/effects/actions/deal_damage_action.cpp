#include "core/effects/actions/deal_damage_action.h"
#include "core/effects/actions/kill_creature_action.h"
#include "core/effects/effect_resolver.h"
#include "core/card_data.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

void DealDamageAction::Apply(GameState& state) const {
    if (target_.type == Target::Type::Player) {
        state.player->health -= amount_;
        LOG_INFO("[EffectResolver] Player takes %d damage. Health: %d", amount_, state.player->health);
    } else if (target_.type == Target::Type::Enemy) {
        state.enemy->health -= amount_;
        LOG_INFO("[EffectResolver] Enemy takes %d damage. Health: %d", amount_, state.enemy->health);
    } else if (target_.type == Target::Type::Creature) {
        CardInstance* target_inst = state.FindCardInstance(target_.id);
        if (target_inst && target_inst->location == CardLocation::Board) {
            target_inst->current_health -= amount_;
            LOG_INFO("[EffectResolver] Creature %s takes %d damage. Health: %d", target_inst->data->name.c_str(), amount_, target_inst->current_health);
            if (target_inst->current_health <= 0) {
                EffectResolver::Get().QueueAction(std::make_shared<KillCreatureAction>(target_inst->instance_id));
            }
        }
    }
}

}  // namespace core::effects::actions
