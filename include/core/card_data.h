#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_CARD_DATA_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_CARD_DATA_H_

#include <string>

namespace core {

/**
 * @brief The static, immutable definition of a card.
 *
 * This structure holds all the baseline information for a card, including its
 * gameplay statistics, display information, and visual assets.
 */
struct CardData {
  /// Unique internal identifier for the card.
  int id;

  /// Display name of the card.
  std::string name;

  /// Descriptive text for the card's effect or flavor.
  std::string description;

  /// The card's attack power.
  int power = 0;

  /// The card's health or durability.
  int health = 0;

  /// The OpenGL texture ID for the card's border/frame.
  unsigned int frame_texture_id;

  /// The OpenGL texture ID for the card's unique illustration.
  unsigned int art_texture_id;
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_CARD_DATA_H_
