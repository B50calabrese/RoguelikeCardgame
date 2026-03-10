#ifndef DECK_BUILDER_GAME_CORE_CARD_DATA_H_
#define DECK_BUILDER_GAME_CORE_CARD_DATA_H_

#include <cstdint>
#include <string>

namespace core {

// The static, immutable definition of a card.
struct CardData {
  // Unique internal identifier.
  int id;

  // Display properties
  std::string name;
  std::string description;

  // Gameplay attributes
  int power = 0;
  int health = 0;

  // Visuals
  uint32_t frame_texture_id;  // The border/template
  uint32_t art_texture_id;    // The unique illustration
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_CORE_CARD_DATA_H_
