#include <gtest/gtest.h>
#include "core/effects/actions/modify_stats_action.h"
#include "core/state/game_state.h"
#include "core/card_instance.h"
#include "core/card_data.h"
#include "core/effects/effect_resolver.h"

namespace core::effects::actions {
namespace {

using ::testing::Test;

class ModifyStatsActionTest : public Test {
protected:
    void SetUp() override {
        state = std::make_unique<GameState>();
        EffectResolver::Get().ClearQueue();
        card_data = std::make_shared<CardData>();
        card_data->name = "Test Creature";
        card_data->power = 2;
        card_data->health = 2;
        card_data->type = CardType::Creature;
    }

    std::unique_ptr<GameState> state;
    std::shared_ptr<CardData> card_data;
};

TEST_F(ModifyStatsActionTest, ModifyPowerAndHealth) {
    auto instance = std::make_unique<CardInstance>(card_data.get(), 100, 0);
    instance->location = CardLocation::Board;
    state->player->board.push_back(std::move(instance));

    ModifyStatsAction action(100, 3, 1); // +3 Power, +1 Health
    action.Apply(*state);

    CardInstance* inst = state->FindCardInstance(100);
    EXPECT_EQ(inst->current_power, 5);
    EXPECT_EQ(inst->current_health, 3);
    EXPECT_EQ(inst->max_health, 3);
}

TEST_F(ModifyStatsActionTest, KillOnHealthZero) {
    auto instance = std::make_unique<CardInstance>(card_data.get(), 101, 0);
    instance->location = CardLocation::Board;
    state->player->board.push_back(std::move(instance));

    ModifyStatsAction action(101, 0, -2); // -2 Health
    action.Apply(*state);

    // Should have queued a KillCreatureAction
    EXPECT_EQ(EffectResolver::Get().QueueSize(), 1);
}

TEST_F(ModifyStatsActionTest, NoChangeIfNotFound) {
    ModifyStatsAction action(999, 10, 10);
    action.Apply(*state);
    // Should not crash and nothing in queue
    EXPECT_EQ(EffectResolver::Get().QueueSize(), 0);
}

}  // namespace
}  // namespace core::effects::actions
