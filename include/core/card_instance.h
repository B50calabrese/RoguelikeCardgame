#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_CARD_INSTANCE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_CARD_INSTANCE_H_

#include <vector>

namespace core {

struct CardData;  // Forward declaration

/**
 * @brief Represents where a card is currently located.
 */
enum class CardLocation { Deck, Hand, Board, Graveyard, Exile, Stack };

/**
 * @brief Defines the duration of an effect or modifier.
 */
enum class ModifierDuration { Permanent, UntilEndOfTurn };

/**
 * @brief A modifier applied to a card's stats.
 */
struct StatModifier {
  int power_add;
  int health_add;
  ModifierDuration duration;
};

/**
 * @brief The runtime instance of a card, which may have modified stats or
 * additional effects.
 */
struct CardInstance {
  const CardData* data;
  int instance_id;
  int owner_id;  // 0 for player, 1 for enemy

  CardLocation location;

  // Current stats (may be modified from base CardData)
  int current_cost;
  int current_power;
  int current_health;
  int max_health;

  // Base stats after permanent modifications but before temporary ones
  int permanent_power_offset;
  int permanent_health_offset;

  bool has_attacked;
  bool can_attack;  // Used for summoning sickness and other "can't attack"
                    // effects

  bool has_haste;
  bool is_blocker;

  std::vector<StatModifier> modifiers;

  CardInstance(const CardData* card_data, int inst_id, int owner);

  /**
   * @brief Recalculates current stats based on base data and active modifiers.
   */
  void RecalculateStats();

  /**
   * @brief Removes all modifiers with the specified duration.
   */
  void ClearModifiers(ModifierDuration duration);
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_CARD_INSTANCE_H_
