#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_GAME_SETUP_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_GAME_SETUP_H_

#include <algorithm>
#include <random>
#include <vector>

#include "core/card_instance.h"
#include "core/card_registry.h"
#include "core/state/combat_state.h"

namespace core::util {

/**
 * @brief Utilities for setting up a game or test state.
 */
class GameSetup {
 public:
  /**
   * @brief Initializes a player's deck with the first 10 cards of each of their
   * color pairs.
   * @param state The game state to modify.
   * @param player_id The player whose deck to initialize.
   * @param seed Optional seed for shuffling.
   */
  static void SetupDefaultDeck(CombatState& state, int player_id,
                               std::optional<unsigned int> seed = std::nullopt) {
    PlayerState& p = state.GetPlayerById(player_id);
    p.deck.clear();

    for (auto color : p.colors) {
      int base_id = 0;
      switch (color) {
        case CardColor::White: base_id = 0; break;
        case CardColor::Blue: base_id = 20; break;
        case CardColor::Black: base_id = 40; break;
        case CardColor::Red: base_id = 60; break;
        case CardColor::Green: base_id = 80; break;
        default: break;
      }
      for (int i = 1; i <= 10; ++i) {
        const CardData* data = CardRegistry::Get().GetCardById(base_id + i);
        if (data) {
          auto inst = std::make_unique<CardInstance>(
              data, state.next_instance_id++, p.id);
          inst->location = CardLocation::Deck;
          p.deck.push_back(std::move(inst));
        }
      }
    }

    if (seed) {
      std::mt19937 g(*seed);
      std::shuffle(p.deck.begin(), p.deck.end(), g);
    } else {
      std::random_device rd;
      std::mt19937 g(rd());
      std::shuffle(p.deck.begin(), p.deck.end(), g);
    }
  }

  /**
   * @brief Draws an initial hand of cards.
   */
  static void DrawInitialHand(CombatState& state, int player_id, int amount = 5) {
    PlayerState& p = state.GetPlayerById(player_id);
    for (int i = 0; i < amount && !p.deck.empty(); ++i) {
      auto inst = std::move(p.deck.back());
      p.deck.pop_back();
      inst->location = CardLocation::Hand;
      p.hand.push_back(std::move(inst));
    }
  }
};

}  // namespace core::util

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_GAME_SETUP_H_
