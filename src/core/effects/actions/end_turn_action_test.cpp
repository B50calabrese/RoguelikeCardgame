#include <gtest/gtest.h>
#include "core/effects/actions/end_turn_action.h"
#include "core/state/game_state.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/start_turn_action.h"

namespace core::effects::actions {
namespace {

using ::testing::Test;

class EndTurnActionTest : public Test {
protected:
    void SetUp() override {
        state = std::make_unique<GameState>();
        EffectResolver::Get().ClearQueue();
    }

    std::unique_ptr<GameState> state;
};

TEST_F(EndTurnActionTest, SwapsPlayers) {
    state->current_turn_player_id = 0;
    auto action = std::make_shared<EndTurnAction>(0);

    action->Apply(*state);

    EXPECT_EQ(state->current_turn_player_id, 1);
}

TEST_F(EndTurnActionTest, QueuesStartTurnAction) {
    state->current_turn_player_id = 0;
    auto action = std::make_shared<EndTurnAction>(0);

    action->Apply(*state);

    // Verify next action is queued in EffectResolver
    EXPECT_EQ(EffectResolver::Get().QueueSize(), 1);

    // Process it to verify it works as expected (it should be a StartTurnAction for player 1)
    EffectResolver::Get().ProcessQueue(*state);

    // Initial max_mana is 1, StartTurnAction should increment it to 2.
    EXPECT_EQ(state->enemy->max_mana, 2);
    EXPECT_EQ(state->enemy->mana, 2);
}

TEST_F(EndTurnActionTest, Validation) {
    state->current_turn_player_id = 0;
    EndTurnAction action_wrong_player(1); // Not player 1's turn

    auto result = action_wrong_player.Validate(*state);
    EXPECT_FALSE(result.success);

    EndTurnAction action_right_player(0);
    result = action_right_player.Validate(*state);
    EXPECT_TRUE(result.success);
}

}  // namespace
}  // namespace core::effects::actions
