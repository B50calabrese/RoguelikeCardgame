#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_CARD_DATA_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_CARD_DATA_H_

#include <string>

namespace core {

/**
 * @brief The static, immutable definition of a card.
 *
 * CardData objects are loaded from XML definitions and contain all the
 * necessary information for gameplay logic and visual rendering of a card.
 */
struct CardData {
  /**
   * @brief Unique internal identifier for the card type.
   */
  int id;

  /**
   * @brief The display name of the card.
   */
  std::string name;

  /**
   * @brief A textual description of the card's effects and abilities.
   */
  std::string description;

  /**
   * @brief Gameplay attribute representing the card's attack value.
   */
  int power = 0;

  /**
   * @brief Gameplay attribute representing the card's health or defense value.
   */
  int health = 0;

  /**
   * @brief The texture ID for the card's frame or template border.
   */
  unsigned int frame_texture_id;

  /**
   * @brief The texture ID for the card's unique illustration artwork.
   */
  unsigned int art_texture_id;
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_CARD_DATA_H_
