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
   * @brief Accesses the singleton instance.
   * @return Reference to the global GameConfig.
   */
  static GameConfig& Get();

  /** @name Resolution Accessors */
  /**@{*/
  /** @brief Gets the configured window width in pixels. */
  int window_width() const { return window_width_; }
  /** @brief Sets the window width in pixels. */
  void set_window_width(int width) { window_width_ = width; }

  /** @brief Gets the configured window height in pixels. */
  int window_height() const { return window_height_; }
  /** @brief Sets the window height in pixels. */
  void set_window_height(int height) { window_height_ = height; }
  /**@}*/

  /** @name Gameplay Accessors */
  /**@{*/
  /** @brief Gets the number of cards drawn at the start of a match. */
  int starting_hand_size() const { return starting_hand_size_; }
  /** @brief Sets the number of cards drawn at the start of a match. */
  void set_starting_hand_size(int size) { starting_hand_size_ = size; }

  /** @brief Gets the number of cards in the initial deck. */
  int starting_deck_size() const { return starting_deck_size_; }
  /** @brief Sets the number of cards in the initial deck. */
  void set_starting_deck_size(int size) { starting_deck_size_ = size; }
  /**@}*/

  /** @name Asset Accessors */
  /**@{*/
  /** @brief Gets the base path for game assets. */
  const std::string& asset_path() const { return asset_path_; }
  /** @brief Sets the base path for game assets. */
  void set_asset_path(const std::string& path) { asset_path_ = path; }
  /**@}*/

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
