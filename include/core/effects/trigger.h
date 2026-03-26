#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TRIGGER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TRIGGER_H_

namespace core::effects {

/**
 * @brief Common triggers for card effects.
 */
enum class Trigger {
  OnPlay,
  OnDeath,
  AtStartOfTurn,
  AtEndOfTurn,
  Manual  // For effects that don't trigger automatically
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TRIGGER_H_
