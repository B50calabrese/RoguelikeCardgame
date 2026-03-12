#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_

#include <string>

namespace core {

/**
 * @brief Singleton class to manage global game configuration and flags.
 */
class GameConfig {
 public:
  /**
   * @brief Access the singleton instance of GameConfig.
   * @return Reference to the GameConfig instance.
   */
  static GameConfig& Get();

  // Resolution
  int window_width() const { return window_width_; }
  void set_window_width(int width) { window_width_ = width; }

  int window_height() const { return window_height_; }
  void set_window_height(int height) { window_height_ = height; }

  // Gameplay
  int starting_hand_size() const { return starting_hand_size_; }
  void set_starting_hand_size(int size) { starting_hand_size_ = size; }

  int starting_deck_size() const { return starting_deck_size_; }
  void set_starting_deck_size(int size) { starting_deck_size_ = size; }

  // Assets
  const std::string& asset_path() const { return asset_path_; }
  void set_asset_path(const std::string& path) { asset_path_ = path; }

 private:
  GameConfig() = default;
  ~GameConfig() = default;
  GameConfig(const GameConfig&) = delete;
  GameConfig& operator=(const GameConfig&) = delete;

  int window_width_ = 1600;
  int window_height_ = 1200;
  int starting_hand_size_ = 5;
  int starting_deck_size_ = 10;
  std::string asset_path_ = "";
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_
