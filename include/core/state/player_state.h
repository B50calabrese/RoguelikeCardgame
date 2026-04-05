#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_STATE_PLAYER_STATE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_STATE_PLAYER_STATE_H_

#include <vector>
#include <memory>

#include "core/enums.h"
#include "core/card_data.h"
#include "core/card_instance.h"

namespace core::state {

/**
 * @brief Represents a single player's game-relevant resources and zones.
 */
struct PlayerState {
  int id;
  int health;
  int max_health;
  int mana;
  int max_mana;

  std::vector<CardColor> colors;

  std::vector<std::unique_ptr<CardInstance>> deck;
  std::vector<std::unique_ptr<CardInstance>> hand;
  std::vector<std::unique_ptr<CardInstance>> board;
  std::vector<std::unique_ptr<CardInstance>> graveyard;
  std::vector<std::unique_ptr<CardInstance>> stack;

  PlayerState(int player_id, int starting_health, int starting_mana)
      : id(player_id),
        health(starting_health),
        max_health(starting_health),
        mana(starting_mana),
        max_mana(starting_mana) {}
};

}  // namespace core::state

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_STATE_PLAYER_STATE_H_
