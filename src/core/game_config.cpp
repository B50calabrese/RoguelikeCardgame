#include "core/game_config.h"

namespace core {

GameConfig& GameConfig::Get() {
  static GameConfig instance;
  return instance;
}

}  // namespace core
