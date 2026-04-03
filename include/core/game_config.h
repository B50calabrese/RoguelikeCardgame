#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_

#include <string>

namespace core {

/**
 * @brief Singleton struct to manage global game configuration and flags.
 * Using a struct for simple data storage of configuration parameters.
 */
struct GameConfig {
 public:
  static GameConfig& Get();

  // Resolution
  int window_width = 1600;
  int window_height = 1200;

  // Gameplay
  int starting_hand_size = 5;
  int starting_deck_size = 10;

  // Assets
  std::string asset_path = "";

  // Card Viewer
  int card_viewer_columns = 4;
  float card_viewer_hover_zoom = 1.2f;
  float card_viewer_fullscreen_zoom = 1.6f;

 private:
  GameConfig() = default;
  ~GameConfig() = default;
  GameConfig(const GameConfig&) = delete;
  GameConfig& operator=(const GameConfig&) = delete;
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_
