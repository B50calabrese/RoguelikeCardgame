#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_ACTION_BASE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_ACTION_BASE_H_

#include <string>
#include "core/enums.h"

namespace core::state { struct GameState; }
namespace core::effects { struct RuleResult; }

namespace core::effects::actions {

/**
 * @brief Polymorphic base for all game actions.
 */
class ActionBase {
 public:
  virtual ~ActionBase() = default;

  /** @brief Validates if the action is legal in the current state. */
  virtual RuleResult Validate(const state::GameState& state) const = 0;

  /** @brief Applies the action's effects to the game state. */
  virtual void Apply(state::GameState& state) const = 0;

  /** @brief Returns true if the action (and any associated animations) is finished. */
  virtual bool IsComplete(const state::GameState& state) const { return true; }

  /** @brief Human-readable name of the action. */
  virtual std::string name() const = 0;

  /** @brief Returns the ID of the player performing the action, or -1 if game-driven. */
  virtual int GetActorId() const { return static_cast<int>(ActorId::None); }
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_ACTION_BASE_H_
