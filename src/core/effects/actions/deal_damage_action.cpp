#include "core/effects/actions/deal_damage_action.h"
#include "core/effects/actions/kill_creature_action.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/event_bus.h"
#include "core/effects/game_event.h"
#include "core/card_data.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

void DealDamageAction::Apply(GameState& state) const {
    GameEvent event;
    event.type = GameEventType::DamageDealt;
    event.source_id = source_id_;
    event.target_id = target_.id;
    event.value = amount_;

    if (target_.type == Target::Type::Player) {
        state.player->health -= amount_;
        LOG_INFO("[EffectResolver] Player takes %d damage. Health: %d", amount_, state.player->health);

        event.type = GameEventType::PlayerTookDamage;
        EventBus::Get().Publish(state, event);

        // Also publish that the source dealt damage
        GameEvent dealt_event = event;
        dealt_event.type = GameEventType::CreatureDealtDamage;
        EventBus::Get().Publish(state, dealt_event);
    } else if (target_.type == Target::Type::Enemy) {
        state.enemy->health -= amount_;
        LOG_INFO("[EffectResolver] Enemy takes %d damage. Health: %d", amount_, state.enemy->health);

        event.type = GameEventType::PlayerTookDamage;
        EventBus::Get().Publish(state, event);

        // Also publish that the source dealt damage
        GameEvent dealt_event = event;
        dealt_event.type = GameEventType::CreatureDealtDamage;
        EventBus::Get().Publish(state, dealt_event);
    } else if (target_.type == Target::Type::Creature) {
        CardInstance* target_inst = state.FindCardInstance(target_.id);
        if (target_inst && target_inst->location == CardLocation::Board) {
            target_inst->current_health -= amount_;
            LOG_INFO("[EffectResolver] Creature %s takes %d damage. Health: %d", target_inst->data->name.c_str(), amount_, target_inst->current_health);

            event.type = GameEventType::CreatureTookDamage;
            EventBus::Get().Publish(state, event);

            // Also publish that the source dealt damage
            GameEvent dealt_event = event;
            dealt_event.type = GameEventType::CreatureDealtDamage;
            EventBus::Get().Publish(state, dealt_event);

            if (target_inst->current_health <= 0) {
                EffectResolver::Get().QueueAction(std::make_shared<KillCreatureAction>(target_inst->instance_id));
            }
        }
    }
}

}  // namespace core::effects::actions
