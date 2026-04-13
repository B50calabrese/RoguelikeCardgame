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

/**
 * @brief Identifies a player or game-driven actor.
 */
enum class ActorId : int {
  Player = 0,
  Enemy = 1,
  None = -1
};

/**
 * @brief Defines the subtypes (creature types) of a card.
 */
enum class CreatureType {
  None,
  Human,
  Soldier,
  Knight,
  Cleric,
  Elf,
  Warrior,
  Dwarf,
  Elemental,
  Cat,
  Wizard,
  Angel,
  Avatar,
  Placeholder
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_ENUMS_H_
