#include <gtest/gtest.h>
#include "core/effects/event_bus.h"
#include "core/effects/game_event.h"
#include "core/state/game_state.h"

namespace core::effects {
namespace {

TEST(EventBusTest, PublishSubscribe) {
    GameState state;
    bool event_received = false;

    EventBus::Get().Subscribe([&](state::GameState& s, const GameEvent& e) {
        event_received = true;
        EXPECT_EQ(e.type, GameEventType::TurnStarted);
    });

    EventBus::Get().Publish(state, {GameEventType::TurnStarted, 0, -1});
    EXPECT_TRUE(event_received);
}

} // namespace
} // namespace core::effects
