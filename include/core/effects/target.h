#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TARGET_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TARGET_H_

namespace core::effects {

/**
 * @brief Represents a target in the game.
 */
struct Target {
  enum class Type { kPlayer, kEnemy, kCreature, kCardInHand, kCardInGraveyard };
  Type type;
  int id;  // Player ID or CardInstance ID
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TARGET_H_
