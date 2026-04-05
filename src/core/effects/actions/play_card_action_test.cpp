#include <gtest/gtest.h>
#include "core/effects/actions/play_card_action.h"
#include "core/state/game_state.h"
#include "core/card_instance.h"
#include "core/card_data.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/rules_engine.h"

namespace core::effects::actions {
namespace {

using ::testing::Test;

class PlayCardActionTest : public Test {
protected:
    void SetUp() override {
        state = std::make_unique<GameState>();
        EffectResolver::Get().ClearQueue();

        creature_data = std::make_shared<CardData>();
        creature_data->name = "Test Creature";
        creature_data->cost = 2;
        creature_data->type = CardType::Creature;

        spell_data = std::make_shared<CardData>();
        spell_data->name = "Test Spell";
        spell_data->cost = 1;
        spell_data->type = CardType::Spell;
    }

    std::unique_ptr<GameState> state;
    std::shared_ptr<CardData> creature_data;
    std::shared_ptr<CardData> spell_data;
};

TEST_F(PlayCardActionTest, PlayCreatureSuccess) {
    state->player->mana = 5;
    auto instance = std::make_unique<CardInstance>(creature_data.get(), 100, 0);
    instance->location = CardLocation::Hand;
    state->player->hand.push_back(std::move(instance));

    PlayCardAction action(0, 100, {});

    EXPECT_TRUE(action.Validate(*state).success);
    action.Apply(*state);

    EXPECT_EQ(state->player->mana, 3);
    EXPECT_EQ(state->player->hand.size(), 0);
    EXPECT_EQ(state->player->board.size(), 1);
    EXPECT_EQ(state->player->board[0]->instance_id, 100);
    EXPECT_EQ(state->player->board[0]->location, CardLocation::Board);
}

TEST_F(PlayCardActionTest, InsufficientMana) {
    state->player->mana = 1;
    auto instance = std::make_unique<CardInstance>(creature_data.get(), 100, 0);
    state->player->hand.push_back(std::move(instance));

    auto action = std::make_shared<PlayCardAction>(0, 100, std::vector<Target>{});
    EXPECT_FALSE(RulesEngine::Get().ValidateAction(*state, action).success);
}

TEST_F(PlayCardActionTest, NotYourTurn) {
    state->current_turn_player_id = 1;
    auto instance = std::make_unique<CardInstance>(creature_data.get(), 100, 0);
    state->player->hand.push_back(std::move(instance));

    auto action = std::make_shared<PlayCardAction>(0, 100, std::vector<Target>{});
    EXPECT_FALSE(RulesEngine::Get().ValidateAction(*state, action).success);
}

TEST_F(PlayCardActionTest, PlaySpellQueuesResolution) {
    state->player->mana = 5;
    auto instance = std::make_unique<CardInstance>(spell_data.get(), 101, 0);
    instance->location = CardLocation::Hand;
    state->player->hand.push_back(std::move(instance));

    PlayCardAction action(0, 101, {});
    action.Apply(*state);

    EXPECT_EQ(state->player->hand.size(), 0);
    EXPECT_EQ(state->player->stack.size(), 1);
    EXPECT_EQ(state->player->stack[0]->instance_id, 101);
    EXPECT_EQ(EffectResolver::Get().QueueSize(), 1);
}

}  // namespace
}  // namespace core::effects::actions
