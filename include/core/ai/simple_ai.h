#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_AI_SIMPLE_AI_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_AI_SIMPLE_AI_H_

#include "core/ai/battle_ai.h"

namespace core::ai {

class SimpleAI : public IBattleAI {
 public:
  SimpleAI(int ai_player_id) : ai_player_id_(ai_player_id) {}
  void Update(float delta_time, GameState& state) override;
  void Reset() override;

 private:
  int ai_player_id_;
  float wait_timer_ = 0.0f;
  const float kWaitDuration = 2.0f;
};

}  // namespace core::ai

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_AI_SIMPLE_AI_H_
