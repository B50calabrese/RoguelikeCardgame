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
    // Switch active player
    state.current_turn_player_id = (state.current_turn_player_id == state.player->id)
                                   ? state.enemy->id
                                   : state.player->id;

    // Queue StartTurnAction for the new active player
    EffectResolver::Get().QueueAction(std::make_shared<StartTurnAction>(state.current_turn_player_id));
}

}  // namespace core::effects::actions
