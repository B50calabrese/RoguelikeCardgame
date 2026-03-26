#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_RESOLVER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_RESOLVER_H_

#include <deque>
#include <memory>
#include <vector>

#include "core/effects/action.h"
#include "core/state/game_state.h"
#include "core/effects/rule_result.h"
#include "engine/util/logger.h"

namespace core::effects {

/**
 * @brief Singleton for managing the action queue and executing state changes.
 */
class EffectResolver {
 public:
  static EffectResolver& Get() {
    static EffectResolver instance;
    return instance;
  }

  /** @brief Adds an action to the queue. */
  void QueueAction(const Action& action) {
    action_queue_.push_back(action);
  }

  /** @brief Processes all pending actions in the queue. */
  void ProcessQueue(GameState& state) {
    while (!action_queue_.empty()) {
      Action next = action_queue_.front();
      action_queue_.pop_front();

      RuleResult rule = next->Validate(state);
      if (!rule.success) {
          LOG_INFO("[EffectResolver] %s failed: %s (fail_open: %d)", next->name().c_str(), rule.message.c_str(), rule.fail_open);
          if (!rule.fail_open) continue;
      }

      next->Apply(state);
    }
  }

 private:
  EffectResolver() = default;
  std::deque<Action> action_queue_;
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_RESOLVER_H_
