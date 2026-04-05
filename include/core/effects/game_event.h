#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_GAME_EVENT_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_GAME_EVENT_H_

namespace core::effects {

/**
 * @brief Types of events that can occur in the game.
 */
enum class GameEventType {
  CardPlayed,
  CreatureDied,
  TurnStarted,
  TurnEnded,
  DamageDealt
};

/**
 * @brief Data associated with a game event.
 */
struct GameEvent {
  GameEventType type;
  int actor_id;         // Player who caused the event (if any)
  int source_id;        // Instance ID of the card/creature that triggered the event
  int target_id = -1;   // Instance ID of the target (if any)
  int value = 0;        // Amount (damage, mana, etc.)
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_GAME_EVENT_H_
