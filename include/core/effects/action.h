#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTION_H_

#include <variant>
#include <vector>
#include <string>

namespace core {

/**
 * @brief Represents a target in the game.
 */
struct Target {
  enum class Type { Player, Enemy, Creature, CardInHand, CardInGraveyard };
  Type type;
  int id;  // Player ID or CardInstance ID
};

/**
 * @brief Base definitions for all game actions.
 */
namespace action {

struct PlayCard {
  int player_id;
  int card_instance_id;
  std::vector<Target> targets;
};

struct DealDamage {
  int source_id; // CardInstance ID or player_id
  Target target;
  int amount;
};

struct DrawCard {
  int player_id;
  int amount;
};

struct KillCreature {
  int card_instance_id;
};

struct ModifyStats {
  int card_instance_id;
  int power_change;
  int health_change;
};

} // namespace action

/**
 * @brief Variant representing any supported game action.
 */
using Action = std::variant<
    action::PlayCard,
    action::DealDamage,
    action::DrawCard,
    action::KillCreature,
    action::ModifyStats
>;

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTION_H_
