#include "core/effects/actions/start_turn_action.h"
#include "core/state/game_state.h"
#include "core/effects/rule_result.h"
#include <algorithm>

namespace core::effects::actions {

RuleResult StartTurnAction::Validate(const state::GameState& state) const {
    return {true, "OK", false};
}

void StartTurnAction::Apply(state::GameState& state) const {
    state::PlayerState* p = (state.player->id == player_id_) ? state.player.get() : state.enemy.get();

    // Refresh mana and add one more up to ten
    p->max_mana = std::min(10, p->max_mana + 1);
    p->mana = p->max_mana;
}

}  // namespace core::effects::actions
