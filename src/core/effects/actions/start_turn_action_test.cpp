#include <gtest/gtest.h>
#include "core/effects/actions/start_turn_action.h"
#include "core/state/game_state.h"

namespace core::effects::actions {
namespace {

using ::testing::Test;

class StartTurnActionTest : public Test {
protected:
    void SetUp() override {
        state = std::make_unique<GameState>();
    }

    std::unique_ptr<GameState> state;
};

TEST_F(StartTurnActionTest, IncrementMaxMana) {
    state->player->max_mana = 3;
    state->player->mana = 0;

    StartTurnAction action(state->player->id);
    action.Apply(*state);

    EXPECT_EQ(state->player->max_mana, 4);
    EXPECT_EQ(state->player->mana, 4);
}

TEST_F(StartTurnActionTest, ManaCap) {
    state->player->max_mana = 10;
    state->player->mana = 2;

    StartTurnAction action(state->player->id);
    action.Apply(*state);

    EXPECT_EQ(state->player->max_mana, 10);
    EXPECT_EQ(state->player->mana, 10);
}

TEST_F(StartTurnActionTest, EnemyManaIncrement) {
    state->enemy->max_mana = 5;
    state->enemy->mana = 1;

    StartTurnAction action(state->enemy->id);
    action.Apply(*state);

    EXPECT_EQ(state->enemy->max_mana, 6);
    EXPECT_EQ(state->enemy->mana, 6);
}

}  // namespace
}  // namespace core::effects::actions
