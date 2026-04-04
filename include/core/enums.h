#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_ENUMS_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_ENUMS_H_

namespace core {

/**
 * @brief Defines the type of card.
 */
enum class CardType {
  Creature,
  Spell
};

/**
 * @brief Defines the color of the card.
 */
enum class CardColor {
  White,
  Blue,
  Black,
  Red,
  Green,
  Colorless
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_ENUMS_H_
