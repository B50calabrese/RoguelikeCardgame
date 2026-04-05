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
    while (true) {
      // 1. If we have a current action, check if it's done.
      if (current_action_) {
        if (current_action_->IsComplete(state)) {
          current_action_ = nullptr;
        } else {
          // Still waiting on current action (e.g. animations)
          return;
        }
      }

      // 2. Process next action in queue
      if (action_queue_.empty()) {
        return;
      }

      current_action_ = action_queue_.front();
      action_queue_.pop_front();

      RuleResult rule = current_action_->Validate(state);
      if (!rule.success) {
          LOG_INFO("[EffectResolver] %s failed: %s (fail_open: %d)",
                   current_action_->name().c_str(), rule.message.c_str(), rule.fail_open);
          if (!rule.fail_open) {
            current_action_ = nullptr;
            continue;
          }
      }

      current_action_->Apply(state);

      // If it's already complete after Apply, clear it so we can potentially
      // process the next one in the same frame.
      if (current_action_->IsComplete(state)) {
        current_action_ = nullptr;
      } else {
        return;
      }
    }
  }

  /** @brief Returns true if there are pending or active actions. */
  bool IsBusy() const { return !action_queue_.empty() || current_action_ != nullptr; }

  /** @brief Clears the action queue. Useful for tests. */
  void ClearQueue() {
    action_queue_.clear();
    current_action_ = nullptr;
  }

  /** @brief Returns the number of actions in the queue. */
  size_t QueueSize() const { return action_queue_.size(); }

 private:
  EffectResolver() = default;
  std::deque<Action> action_queue_;
  Action current_action_ = nullptr;
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_RESOLVER_H_
