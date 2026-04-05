#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_AI_BATTLE_AI_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_AI_BATTLE_AI_H_

#include "core/state/game_state.h"

namespace core::ai {

class IBattleAI {
 public:
  virtual ~IBattleAI() = default;
  virtual void Update(float delta_time, GameState& state) = 0;
  virtual void Reset() = 0;
};

}  // namespace core::ai

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_AI_BATTLE_AI_H_
