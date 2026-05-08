#include "core/effects/event_bus.h"

#include <gtest/gtest.h>

#include "core/effects/game_event.h"
#include "core/state/combat_state.h"

namespace core::effects {
namespace {

TEST(EventBusTest, PublishSubscribe) {
  CombatState state;
  bool event_received = false;

  EventBus::Get().Subscribe([&](state::CombatState& s, const GameEvent& e) {
    event_received = true;
    EXPECT_EQ(e.type, GameEventType::TurnStarted);
  });

  EventBus::Get().Publish(state, {GameEventType::TurnStarted, 0, -1});
  EXPECT_TRUE(event_received);
}

}  // namespace
}  // namespace core::effects
