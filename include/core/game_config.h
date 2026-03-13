#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_

#include <string>

namespace core {

/**
 * @brief Singleton class to manage global game configuration and flags.
 */
class GameConfig {
 public:
  static GameConfig& Get();

  // Getters
  int window_width() const { return window_width_; }
  int window_height() const { return window_height_; }
  int starting_hand_size() const { return starting_hand_size_; }
  int starting_deck_size() const { return starting_deck_size_; }
  const std::string& asset_path() const { return asset_path_; }

  // Setters
  void set_window_width(int w) { window_width_ = w; }
  void set_window_height(int h) { window_height_ = h; }
  void set_starting_hand_size(int s) { starting_hand_size_ = s; }
  void set_starting_deck_size(int s) { starting_deck_size_ = s; }
  void set_asset_path(const std::string& p) { asset_path_ = p; }

 private:
  GameConfig() = default;
  ~GameConfig() = default;
  GameConfig(const GameConfig&) = delete;
  GameConfig& operator=(const GameConfig&) = delete;

  // Resolution
  int window_width_ = 1600;
  int window_height_ = 1200;

  // Gameplay
  int starting_hand_size_ = 5;
  int starting_deck_size_ = 10;

  // Assets
  std::string asset_path_ = "";
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_
