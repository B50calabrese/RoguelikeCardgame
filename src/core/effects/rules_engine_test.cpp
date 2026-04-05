#include <gtest/gtest.h>
#include "core/effects/rules_engine.h"
#include "core/effects/actions/play_card_action.h"
#include "core/state/game_state.h"
#include "core/card_instance.h"
#include "core/card_data.h"

namespace core::effects {
namespace {

class RulesEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        state = std::make_unique<state::GameState>();

        card_data = std::make_shared<CardData>();
        card_data->name = "Test Card";
        card_data->cost = 5;
        card_data->type = CardType::Creature;
    }

    std::unique_ptr<state::GameState> state;
    std::shared_ptr<CardData> card_data;
};

TEST_F(RulesEngineTest, ManaRuleCheck) {
    state->player->mana = 3;
    auto instance = std::make_unique<CardInstance>(card_data.get(), 100, 0);
    state->player->hand.push_back(std::move(instance));

    auto action = std::make_shared<actions::PlayCardAction>(0, 100, std::vector<Target>{});

    RuleResult result = RulesEngine::Get().ValidateAction(*state, action);
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.message, "Insufficient mana");

    state->player->mana = 10;
    result = RulesEngine::Get().ValidateAction(*state, action);
    EXPECT_TRUE(result.success);
}

TEST_F(RulesEngineTest, TurnRuleCheck) {
    state->player->mana = 10;
    state->current_turn_player_id = 1; // Enemy turn

    auto instance = std::make_unique<CardInstance>(card_data.get(), 100, 0);
    state->player->hand.push_back(std::move(instance));

    auto action = std::make_shared<actions::PlayCardAction>(0, 100, std::vector<Target>{});

    RuleResult result = RulesEngine::Get().ValidateAction(*state, action);
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.message, "Not your turn");
}

} // namespace
} // namespace core::effects
