#include <gtest/gtest.h>
#include "core/effects/actions/deal_damage_action.h"
#include "core/state/game_state.h"
#include "core/card_instance.h"
#include "core/card_data.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/target.h"

using namespace core;
using namespace core::effects;
using namespace core::effects::actions;

class DealDamageActionTest : public ::testing::Test {
protected:
    void SetUp() override {
        state = std::make_unique<GameState>();
    }

    std::unique_ptr<GameState> state;
};

TEST_F(DealDamageActionTest, DamagePlayer) {
    int initial_health = state->player->health;
    int damage_amount = 5;
    Target target;
    target.type = Target::Type::Player;

    DealDamageAction action(0, target, damage_amount);
    action.Apply(*state);

    EXPECT_EQ(state->player->health, initial_health - damage_amount);
}

TEST_F(DealDamageActionTest, DamageEnemy) {
    int initial_health = state->enemy->health;
    int damage_amount = 10;
    Target target;
    target.type = Target::Type::Enemy;

    DealDamageAction action(0, target, damage_amount);
    action.Apply(*state);

    EXPECT_EQ(state->enemy->health, initial_health - damage_amount);
}

TEST_F(DealDamageActionTest, DamageCreature) {
    // Setup a creature on the board
    auto card_data = std::make_shared<CardData>();
    card_data->name = "Test Creature";
    card_data->health = 5;
    card_data->type = CardType::Creature;

    auto instance = std::make_unique<CardInstance>(card_data.get(), 1, 0);
    instance->current_health = 5;
    instance->location = CardLocation::Board;

    // We need to keep a pointer to it to check its health later
    CardInstance* instance_ptr = instance.get();
    state->player->board.push_back(std::move(instance));

    int damage_amount = 3;
    Target target;
    target.type = Target::Type::Creature;
    target.id = 1;

    DealDamageAction action(0, target, damage_amount);
    action.Apply(*state);

    EXPECT_EQ(instance_ptr->current_health, 2);
}

TEST_F(DealDamageActionTest, CreatureDies) {
    // Setup a creature on the board
    auto card_data = std::make_shared<CardData>();
    card_data->name = "Test Creature";
    card_data->health = 5;
    card_data->type = CardType::Creature;

    auto instance = std::make_unique<CardInstance>(card_data.get(), 1, 0);
    instance->current_health = 5;
    instance->location = CardLocation::Board;

    CardInstance* instance_ptr = instance.get();
    state->player->board.push_back(std::move(instance));

    int damage_amount = 5;
    Target target;
    target.type = Target::Type::Creature;
    target.id = 1;

    DealDamageAction action(0, target, damage_amount);
    action.Apply(*state);

    EXPECT_EQ(instance_ptr->current_health, 0);
}
