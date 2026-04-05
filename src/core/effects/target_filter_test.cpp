#include <gtest/gtest.h>
#include "core/effects/target_filter.h"
#include "core/state/game_state.h"
#include "core/card_instance.h"
#include "core/card_data.h"

namespace core::effects {
namespace {

class TargetFilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        state = std::make_unique<state::GameState>();
        card_data = std::make_shared<CardData>();
    }

    std::unique_ptr<state::GameState> state;
    std::shared_ptr<CardData> card_data;
};

TEST_F(TargetFilterTest, AllowPlayer) {
    TargetFilter filter;
    filter.allow_player = true;
    filter.allow_enemy = true;
    filter.allow_self = true;

    Target self_target;
    self_target.type = Target::Type::Player;
    self_target.id = 0;

    Target enemy_target;
    enemy_target.type = Target::Type::Enemy;
    enemy_target.id = 1;

    EXPECT_TRUE(filter.IsValid(*state, 0, self_target));
    EXPECT_TRUE(filter.IsValid(*state, 0, enemy_target));

    filter.allow_self = false;
    EXPECT_FALSE(filter.IsValid(*state, 0, self_target));
    EXPECT_TRUE(filter.IsValid(*state, 0, enemy_target));

    filter.allow_self = true;
    filter.allow_enemy = false;
    EXPECT_TRUE(filter.IsValid(*state, 0, self_target));
    EXPECT_FALSE(filter.IsValid(*state, 0, enemy_target));
}

TEST_F(TargetFilterTest, AllowCreature) {
    TargetFilter filter;
    filter.allow_creature = true;
    filter.allow_enemy = true;
    filter.allow_self = true;

    auto instance = std::make_unique<CardInstance>(card_data.get(), 100, 0);
    instance->location = CardLocation::Board;
    state->player->board.push_back(std::move(instance));

    Target target;
    target.type = Target::Type::Creature;
    target.id = 100;

    EXPECT_TRUE(filter.IsValid(*state, 0, target));

    filter.allow_self = false;
    EXPECT_FALSE(filter.IsValid(*state, 0, target));

    filter.allow_self = true;
    filter.allow_creature = false;
    EXPECT_FALSE(filter.IsValid(*state, 0, target));
}

TEST_F(TargetFilterTest, StatsConstraints) {
    TargetFilter filter;
    filter.allow_creature = true;
    filter.min_power = 5;
    filter.max_health = 10;

    card_data->power = 4;
    card_data->health = 5;
    auto inst1 = std::make_unique<CardInstance>(card_data.get(), 1, 0);
    inst1->location = CardLocation::Board;
    state->player->board.push_back(std::move(inst1));

    card_data->power = 6;
    card_data->health = 11;
    auto inst2 = std::make_unique<CardInstance>(card_data.get(), 2, 0);
    inst2->location = CardLocation::Board;
    state->player->board.push_back(std::move(inst2));

    card_data->power = 6;
    card_data->health = 5;
    auto inst3 = std::make_unique<CardInstance>(card_data.get(), 3, 0);
    inst3->location = CardLocation::Board;
    state->player->board.push_back(std::move(inst3));

    Target t1{Target::Type::Creature, 1};
    Target t2{Target::Type::Creature, 2};
    Target t3{Target::Type::Creature, 3};

    EXPECT_FALSE(filter.IsValid(*state, 0, t1)); // Power too low
    EXPECT_FALSE(filter.IsValid(*state, 0, t2)); // Health too high
    EXPECT_TRUE(filter.IsValid(*state, 0, t3));  // Just right
}

TEST_F(TargetFilterTest, CreatureInHandIsInvalid) {
    TargetFilter filter;
    filter.allow_creature = true;

    auto instance = std::make_unique<CardInstance>(card_data.get(), 100, 0);
    instance->location = CardLocation::Hand;
    state->player->hand.push_back(std::move(instance));

    Target target;
    target.type = Target::Type::Creature;
    target.id = 100;

    EXPECT_FALSE(filter.IsValid(*state, 0, target));
}

}  // namespace
}  // namespace core::effects
