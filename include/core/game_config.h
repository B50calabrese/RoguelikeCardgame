#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GAME_CONFIG_H_

#include <string>

namespace core {

/**
 * @brief Singleton class to manage global game configuration and flags.
 *
 * This class provides access to global configuration settings used throughout
 * the application, such as window dimensions and gameplay constants.
 */
class GameConfig {
 public:
  /**
   * @brief Access the singleton instance of GameConfig.
   * @return Reference to the singleton instance.
   */
  static GameConfig& Get();

  /**
   * @brief Returns the desired width of the application window in pixels.
   * @return The window width.
   */
  int window_width() const { return window_width_; }

  /**
   * @brief Sets the desired width of the application window in pixels.
   * @param width The new window width.
   */
  void set_window_width(int width) { window_width_ = width; }

  /**
   * @brief Returns the desired height of the application window in pixels.
   * @return The window height.
   */
  int window_height() const { return window_height_; }

  /**
   * @brief Sets the desired height of the application window in pixels.
   * @param height The new window height.
   */
  void set_window_height(int height) { window_height_ = height; }

  /**
   * @brief Returns the initial number of cards a player draws when starting a
   * game.
   * @return The starting hand size.
   */
  int starting_hand_size() const { return starting_hand_size_; }

  /**
   * @brief Sets the initial number of cards a player draws when starting a
   * game.
   * @param size The new starting hand size.
   */
  void set_starting_hand_size(int size) { starting_hand_size_ = size; }

  /**
   * @brief Returns the initial number of cards in the player's deck at game
   * start.
   * @return The starting deck size.
   */
  int starting_deck_size() const { return starting_deck_size_; }

  /**
   * @brief Sets the initial number of cards in the player's deck at game start.
   * @param size The new starting deck size.
   */
  void set_starting_deck_size(int size) { starting_deck_size_ = size; }

  /**
   * @brief Returns the root directory where assets (textures, cards, etc.) are
   * located.
   * @return The root asset path.
   */
  const std::string& asset_path() const { return asset_path_; }

  /**
   * @brief Sets the root directory where assets are located.
   * @param path The new root asset path.
   */
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
