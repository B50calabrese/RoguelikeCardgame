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

  // Resolution
  int GetWindowWidth() const { return window_width_; }
  void SetWindowWidth(int width) { window_width_ = width; }

  int GetWindowHeight() const { return window_height_; }
  void SetWindowHeight(int height) { window_height_ = height; }

  // Gameplay
  int GetStartingHandSize() const { return starting_hand_size_; }
  void SetStartingHandSize(int size) { starting_hand_size_ = size; }

  int GetStartingDeckSize() const { return starting_deck_size_; }
  void SetStartingDeckSize(int size) { starting_deck_size_ = size; }

  // Assets
  const std::string& GetAssetPath() const { return asset_path_; }
  void SetAssetPath(const std::string& path) { asset_path_ = path; }

 private:
  GameConfig();
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
