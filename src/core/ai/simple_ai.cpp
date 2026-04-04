#include "core/ai/simple_ai.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/end_turn_action.h"

namespace core::ai {

void SimpleAI::Update(float delta_time, GameState& state) {
  if (state.current_turn_player_id != ai_player_id_) {
    wait_timer_ = 0.0f;
    return;
  }

  wait_timer_ += delta_time;
  if (wait_timer_ >= kWaitDuration) {
    effects::EffectResolver::Get().QueueAction(
        std::make_shared<effects::actions::EndTurnAction>(ai_player_id_));
    wait_timer_ = 0.0f;
  }
}

void SimpleAI::Reset() {
  wait_timer_ = 0.0f;
}

}  // namespace core::ai
