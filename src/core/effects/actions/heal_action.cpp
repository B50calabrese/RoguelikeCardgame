#include "core/effects/actions/heal_action.h"
#include "core/card_instance.h"
#include "core/state/game_state.h"
#include "engine/util/logger.h"
#include <algorithm>

namespace core::effects::actions {

void HealAction::Apply(state::GameState& state) const {
    if (target_.type == Target::Type::Player || target_.type == Target::Type::Enemy) {
        PlayerState& p = (target_.type == Target::Type::Player) ? *state.player : *state.enemy;
        int old_health = p.health;
        p.health = std::min(p.max_health, p.health + amount_);
        LOG_INFO("[EffectResolver] Healed player %d for %d. Health: %d -> %d", p.id, amount_, old_health, p.health);
        return;
    }

    CardInstance* inst = state.FindCardInstance(target_.id);
    if (inst && inst->location == CardLocation::Board) {
        int old_health = inst->current_health;
        inst->current_health = std::min(inst->max_health, inst->current_health + amount_);
        LOG_INFO("[EffectResolver] Healed %s for %d. Health: %d -> %d", inst->data->name.c_str(), amount_, old_health, inst->current_health);
    }
}

}  // namespace core::effects::actions
