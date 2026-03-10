#include "core/game_config.h"

namespace core {

GameConfig& GameConfig::Get() {
  static GameConfig instance;
  return instance;
}

GameConfig::GameConfig() {
  // Use default values as initialized in the header
}

}  // namespace core
