#include "core/state/game_state.h"

namespace core {

GameState& GameState::Get() {
  static GameState instance;
  return instance;
}

GameState::GameState() : character_type_(CharacterType::None) {}

void GameState::Reset() {
  character_type_ = CharacterType::None;
  colors_.clear();
  deck_.clear();
}

}  // namespace core
