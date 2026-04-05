#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EVENT_BUS_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EVENT_BUS_H_

#include <vector>
#include <functional>
#include "core/effects/game_event.h"
#include "core/state/game_state.h"

namespace core::effects {

/**
 * @brief Simple synchronous event bus for game events.
 */
class EventBus {
 public:
  static EventBus& Get() {
    static EventBus instance;
    return instance;
  }

  using Listener = std::function<void(state::GameState&, const GameEvent&)>;

  /** @brief Adds a listener for game events. */
  void Subscribe(Listener listener) {
    listeners_.push_back(std::move(listener));
  }

  /** @brief Broadcasts an event to all subscribers. */
  void Publish(state::GameState& state, const GameEvent& event) {
    for (const auto& listener : listeners_) {
        listener(state, event);
    }
  }

 private:
  EventBus() = default;
  std::vector<Listener> listeners_;
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EVENT_BUS_H_
