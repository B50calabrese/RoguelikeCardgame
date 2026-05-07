#include "core/state/combat_state.h"

#include <gtest/gtest.h>

#include "core/card_data.h"
#include "core/card_instance.h"

namespace core::state {
namespace {

class CombatStateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    state = std::make_unique<CombatState>();
    card_data = std::make_shared<CardData>();
    card_data->name = "Test Card";
  }

  std::unique_ptr<CombatState> state;
  std::shared_ptr<CardData> card_data;
};

TEST_F(CombatStateTest, FindCardInHand) {
  auto instance = std::make_unique<CardInstance>(card_data.get(), 100, 0);
  state->player->hand.push_back(std::move(instance));

  CardInstance* found = state->FindCardInstance(100);
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->instance_id, 100);
}

TEST_F(CombatStateTest, FindCardInBoard) {
  auto instance = std::make_unique<CardInstance>(card_data.get(), 101, 0);
  state->player->board.push_back(std::move(instance));

  CardInstance* found = state->FindCardInstance(101);
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->instance_id, 101);
}

TEST_F(CombatStateTest, FindCardInDeck) {
  auto instance = std::make_unique<CardInstance>(card_data.get(), 102, 0);
  state->player->deck.push_back(std::move(instance));

  CardInstance* found = state->FindCardInstance(102);
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->instance_id, 102);
}

TEST_F(CombatStateTest, FindCardInGraveyard) {
  auto instance = std::make_unique<CardInstance>(card_data.get(), 103, 0);
  state->player->graveyard.push_back(std::move(instance));

  CardInstance* found = state->FindCardInstance(103);
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->instance_id, 103);
}

TEST_F(CombatStateTest, FindCardInStack) {
  auto instance = std::make_unique<CardInstance>(card_data.get(), 104, 0);
  state->player->stack.push_back(std::move(instance));

  CardInstance* found = state->FindCardInstance(104);
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->instance_id, 104);
}

TEST_F(CombatStateTest, FindCardInEnemyZones) {
  auto instance = std::make_unique<CardInstance>(card_data.get(), 200, 1);
  state->enemy->hand.push_back(std::move(instance));

  CardInstance* found = state->FindCardInstance(200);
  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->instance_id, 200);
}

TEST_F(CombatStateTest, CardNotFound) {
  CardInstance* found = state->FindCardInstance(999);
  EXPECT_EQ(found, nullptr);
}

}  // namespace
}  // namespace core::state
