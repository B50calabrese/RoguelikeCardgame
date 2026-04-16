#include "core/effects/actions/creature_attack_action.h"
#include "core/effects/actions/deal_damage_action.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/event_bus.h"
#include "core/effects/visual_blocker.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

RuleResult CreatureAttackAction::Validate(const state::GameState& state) const {
    CardInstance* attacker = state.FindCardInstance(attacker_id_);
    if (!attacker || attacker->location != CardLocation::Board) {
        return {false, "Attacker not on board", true};
    }

    if (target_.type == Target::Type::Creature) {
        CardInstance* target_inst = state.FindCardInstance(target_.id);
        if (!target_inst || target_inst->location != CardLocation::Board) {
            return {false, "Target creature not on board", true};
        }
    } else if (target_.type != Target::Type::Player && target_.type != Target::Type::Enemy) {
        return {false, "Invalid target type for attack", true};
    }

    return {true, "OK", false};
}

void CreatureAttackAction::Apply(state::GameState& state) const {
    CardInstance* attacker = state.FindCardInstance(attacker_id_);
    if (!attacker) return;

    LOG_INFO("[CreatureAttackAction] %s (ID:%d) attacking target (ID:%d, Type:%d)",
             attacker->data->name.c_str(), attacker_id_, target_.id, (int)target_.type);

    attacker->has_attacked = true;

    // Publish attack event
    GameEvent event;
    event.type = GameEventType::CreatureAttacked;
    event.actor_id = attacker->owner_id;
    event.source_id = attacker_id_;
    event.target_id = target_.id;
    EventBus::Get().Publish(state, event);

    // Queue damage resolution
    // When a creature attacks, it deals damage to the target.
    // If the target is a creature, that creature also deals damage back.

    // Attacker deals damage to target
    EffectResolver::Get().QueueAction(std::make_shared<DealDamageAction>(attacker_id_, target_, attacker->current_power));

    // Counter-attack if target is a creature
    if (target_.type == Target::Type::Creature) {
        CardInstance* target_inst = state.FindCardInstance(target_.id);
        if (target_inst) {
            Target attacker_target;
            attacker_target.type = Target::Type::Creature;
            attacker_target.id = attacker_id_;
            EffectResolver::Get().QueueAction(std::make_shared<DealDamageAction>(target_inst->instance_id, attacker_target, target_inst->current_power));
        }
    }

    // Add a visual blocker for the animation
    // The Scene will be responsible for handling the animation and removing this blocker
    VisualBlocker::Get().AddBlocker("CreatureAttack_" + std::to_string(attacker_id_));
}

bool CreatureAttackAction::IsComplete(const state::GameState& state) const {
    return !VisualBlocker::Get().IsBlocking();
}

}  // namespace core::effects::actions
