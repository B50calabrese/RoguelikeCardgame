#include <gtest/gtest.h>
#include "core/effects/rules_engine.h"
#include "core/effects/actions/play_card_action.h"
#include "core/state/game_state.h"
#include "core/card_instance.h"
#include "core/card_data.h"

namespace core::effects {
namespace {

class BoardSpaceRuleTest : public ::testing::Test {
protected:
    void SetUp() override {
        state = std::make_unique<state::GameState>();
        state->player->mana = 100;
        state->current_turn_player_id = 0;

        creature_data = std::make_shared<CardData>();
        creature_data->name = "Test Creature";
        creature_data->cost = 1;
        creature_data->type = CardType::Creature;

        spell_data = std::make_shared<CardData>();
        spell_data->name = "Test Spell";
        spell_data->cost = 1;
        spell_data->type = CardType::Spell;
    }

    std::unique_ptr<state::GameState> state;
    std::shared_ptr<CardData> creature_data;
    std::shared_ptr<CardData> spell_data;
};

TEST_F(BoardSpaceRuleTest, AllowsCreatureWhenSpaceAvailable) {
    auto instance = std::make_unique<CardInstance>(creature_data.get(), 100, 0);
    state->player->hand.push_back(std::move(instance));

    auto action = std::make_shared<actions::PlayCardAction>(0, 100, std::vector<Target>{});

    RuleResult result = RulesEngine::Get().ValidateAction(*state, action);
    EXPECT_TRUE(result.success);
}

TEST_F(BoardSpaceRuleTest, BlocksCreatureWhenBoardFull) {
    // Fill board with 7 creatures
    for (int i = 0; i < 7; ++i) {
        state->player->board.push_back(std::make_unique<CardInstance>(creature_data.get(), i, 0));
    }

    auto instance = std::make_unique<CardInstance>(creature_data.get(), 100, 0);
    state->player->hand.push_back(std::move(instance));

    auto action = std::make_shared<actions::PlayCardAction>(0, 100, std::vector<Target>{});

    RuleResult result = RulesEngine::Get().ValidateAction(*state, action);
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.message, "Board is full (max 7 creatures)");
}

TEST_F(BoardSpaceRuleTest, AllowsSpellWhenBoardFull) {
    // Fill board with 7 creatures
    for (int i = 0; i < 7; ++i) {
        state->player->board.push_back(std::make_unique<CardInstance>(creature_data.get(), i, 0));
    }

    auto instance = std::make_unique<CardInstance>(spell_data.get(), 100, 0);
    state->player->hand.push_back(std::move(instance));

    auto action = std::make_shared<actions::PlayCardAction>(0, 100, std::vector<Target>{});

    RuleResult result = RulesEngine::Get().ValidateAction(*state, action);
    EXPECT_TRUE(result.success);
}

} // namespace
} // namespace core::effects
