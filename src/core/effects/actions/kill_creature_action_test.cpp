#include <gtest/gtest.h>
#include "core/effects/actions/kill_creature_action.h"
#include "core/state/game_state.h"
#include "core/card_instance.h"
#include "core/card_data.h"

namespace core::effects::actions {
namespace {

using ::testing::Test;

class KillCreatureActionTest : public Test {
protected:
    void SetUp() override {
        state = std::make_unique<GameState>();
        card_data = std::make_shared<CardData>();
        card_data->name = "Test Creature";
    }

    std::unique_ptr<GameState> state;
    std::shared_ptr<CardData> card_data;
};

TEST_F(KillCreatureActionTest, KillFromPlayerBoard) {
    auto instance = std::make_unique<CardInstance>(card_data.get(), 100, 0);
    instance->location = CardLocation::Board;
    state->player->board.push_back(std::move(instance));

    KillCreatureAction action(100);
    action.Apply(*state);

    EXPECT_EQ(state->player->board.size(), 0);
    EXPECT_EQ(state->player->graveyard.size(), 1);
    EXPECT_EQ(state->player->graveyard[0]->instance_id, 100);
    EXPECT_EQ(state->player->graveyard[0]->location, CardLocation::Graveyard);
}

TEST_F(KillCreatureActionTest, KillFromEnemyBoard) {
    auto instance = std::make_unique<CardInstance>(card_data.get(), 200, 1);
    instance->location = CardLocation::Board;
    state->enemy->board.push_back(std::move(instance));

    KillCreatureAction action(200);
    action.Apply(*state);

    EXPECT_EQ(state->enemy->board.size(), 0);
    EXPECT_EQ(state->enemy->graveyard.size(), 1);
}

TEST_F(KillCreatureActionTest, NotFound) {
    KillCreatureAction action(999);
    action.Apply(*state); // Should not crash
    EXPECT_EQ(state->player->graveyard.size(), 0);
}

}  // namespace
}  // namespace core::effects::actions
