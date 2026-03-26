#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_STATE_GAME_STATE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_STATE_GAME_STATE_H_

#include <memory>

#include "core/state/player_state.h"
#include "core/card_instance.h"

namespace core::state {

/**
 * @brief Central storage for the game's runtime state.
 */
struct GameState {
  std::unique_ptr<PlayerState> player;
  std::unique_ptr<PlayerState> enemy;

  int current_turn_player_id;
  int next_instance_id = 1;

  GameState() {
    player = std::make_unique<PlayerState>(0, 30, 1);
    enemy = std::make_unique<PlayerState>(1, 30, 1);
    current_turn_player_id = 0;
  }

  /**
   * @brief Helper to find a card instance by its ID across all zones.
   */
  CardInstance* FindCardInstance(int instance_id) const {
    auto search_zones = [](const PlayerState& p, int id) -> CardInstance* {
      for (const auto& c : p.hand) if (c->instance_id == id) return c.get();
      for (const auto& c : p.board) if (c->instance_id == id) return c.get();
      for (const auto& c : p.deck) if (c->instance_id == id) return c.get();
      for (const auto& c : p.graveyard) if (c->instance_id == id) return c.get();
      return nullptr;
    };

    if (auto c = search_zones(*player, instance_id)) return c;
    if (auto c = search_zones(*enemy, instance_id)) return c;
    return nullptr;
  }
};

}  // namespace core::state

namespace core {
  using GameState = state::GameState;
  using PlayerState = state::PlayerState;
}

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_STATE_GAME_STATE_H_
