#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_CARD_DATA_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_CARD_DATA_H_

#include <string>
#include <vector>

#include "core/effects/effect.h"

namespace core {

/**
 * @brief Defines the type of card.
 */
enum class CardType {
  Creature,
  Spell
};

/**
 * @brief The static, immutable definition of a card.
 */
struct CardData {
  // Unique internal identifier.
  int id;

  // Display properties
  std::string name;
  std::string description;

  // Gameplay attributes
  CardType type = CardType::Spell;
  int cost = 0;
  int power = 0;
  int health = 0;

  // Effects
  std::vector<CardEffectDefinition> effects;

  // Visuals
  unsigned int frame_texture_id;  // The border/template
  unsigned int art_texture_id;    // The unique illustration
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_CARD_DATA_H_
