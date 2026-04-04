#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_CARD_INSTANCE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_CARD_INSTANCE_H_

namespace core {

struct CardData; // Forward declaration

/**
 * @brief Represents where a card is currently located.
 */
enum class CardLocation {
  Deck,
  Hand,
  Board,
  Graveyard,
  Exile,
  Limbo
};

/**
 * @brief The runtime instance of a card, which may have modified stats or additional effects.
 */
struct CardInstance {
  const CardData* data;
  int instance_id;
  int owner_id; // 0 for player, 1 for enemy

  CardLocation location;

  // Current stats (may be modified from base CardData)
  int current_cost;
  int current_power;
  int current_health;
  int max_health;

  CardInstance(const CardData* card_data, int inst_id, int owner);
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_CARD_INSTANCE_H_
