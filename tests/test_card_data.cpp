#include <gtest/gtest.h>
#include "core/card_data.h"

namespace core {

TEST(CardDataTest, DefaultInitialization) {
    CardData card;
    card.id = 1;
    card.name = "Test Card";

    EXPECT_EQ(card.id, 1);
    EXPECT_EQ(card.name, "Test Card");
    EXPECT_EQ(card.type, CardType::Spell);
    EXPECT_EQ(card.cost, 0);
    EXPECT_EQ(card.power, 0);
    EXPECT_EQ(card.health, 0);
}

TEST(CardDataTest, CreatureType) {
    CardData card;
    card.type = CardType::Creature;
    card.power = 3;
    card.health = 4;

    EXPECT_EQ(card.type, CardType::Creature);
    EXPECT_EQ(card.power, 3);
    EXPECT_EQ(card.health, 4);
}

} // namespace core
