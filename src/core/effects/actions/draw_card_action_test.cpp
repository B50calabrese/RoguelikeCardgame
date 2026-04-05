#include <gtest/gtest.h>
#include "core/effects/actions/draw_card_action.h"
#include "core/state/game_state.h"
#include "core/card_instance.h"
#include "core/card_data.h"

namespace core::effects::actions {
namespace {

using ::testing::Test;

class DrawCardActionTest : public Test {
protected:
    void SetUp() override {
        state = std::make_unique<GameState>();
        card_data = std::make_shared<CardData>();
        card_data->name = "Test Card";
    }

    std::unique_ptr<GameState> state;
    std::shared_ptr<CardData> card_data;
};

TEST_F(DrawCardActionTest, DrawSuccess) {
    auto instance = std::make_unique<CardInstance>(card_data.get(), 100, 0);
    state->player->deck.push_back(std::move(instance));

    EXPECT_EQ(state->player->hand.size(), 0);
    EXPECT_EQ(state->player->deck.size(), 1);

    DrawCardAction action(0, 1);
    action.Apply(*state);

    EXPECT_EQ(state->player->hand.size(), 1);
    EXPECT_EQ(state->player->deck.size(), 0);
    EXPECT_EQ(state->player->hand[0]->instance_id, 100);
}

TEST_F(DrawCardActionTest, DrawMultiple) {
    state->player->deck.push_back(std::make_unique<CardInstance>(card_data.get(), 101, 0));
    state->player->deck.push_back(std::make_unique<CardInstance>(card_data.get(), 102, 0));
    state->player->deck.push_back(std::make_unique<CardInstance>(card_data.get(), 103, 0));

    DrawCardAction action(0, 2);
    action.Apply(*state);

    EXPECT_EQ(state->player->hand.size(), 2);
    EXPECT_EQ(state->player->deck.size(), 1);
}

TEST_F(DrawCardActionTest, EmptyDeck) {
    DrawCardAction action(0, 1);
    action.Apply(*state); // Should not crash
    EXPECT_EQ(state->player->hand.size(), 0);
}

}  // namespace
}  // namespace core::effects::actions
