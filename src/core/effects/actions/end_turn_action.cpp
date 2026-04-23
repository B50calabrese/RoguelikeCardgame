#include "core/effects/actions/end_turn_action.h"
#include "core/state/game_state.h"
#include "core/effects/rule_result.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/start_turn_action.h"

namespace core::effects::actions {

RuleResult EndTurnAction::Validate(const state::GameState& state) const {
    if (state.current_turn_player_id != player_id_) {
        return {false, "Not current player's turn", false};
    }
    return {true, "OK", false};
}

void EndTurnAction::Apply(state::GameState& state) const {
    // Reset temporary modifiers for the player whose turn is ending
    PlayerState& p = (player_id_ == state.player->id) ? *state.player : *state.enemy;
    for (auto& inst : p.board) {
        if (inst->temp_power_modifier != 0 || inst->temp_health_modifier != 0) {
            inst->current_power -= inst->temp_power_modifier;
            inst->current_health -= inst->temp_health_modifier;
            inst->temp_power_modifier = 0;
            inst->temp_health_modifier = 0;
        }
    }
    // Switch active player
    state.current_turn_player_id = (state.current_turn_player_id == state.player->id)
                                   ? state.enemy->id
                                   : state.player->id;

    // Queue StartTurnAction for the new active player
    EffectResolver::Get().QueueAction(std::make_shared<StartTurnAction>(state.current_turn_player_id));
}

}  // namespace core::effects::actions
