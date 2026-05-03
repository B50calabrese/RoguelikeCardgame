#include "core/ai/simple_ai.h"

#include <algorithm>
#include <random>

#include "core/effects/actions/creature_attack_action.h"
#include "core/effects/actions/end_turn_action.h"
#include "core/effects/actions/play_card_action.h"
#include "core/effects/effect_resolver.h"

namespace core::ai {

void SimpleAI::Update(float delta_time, GameState& state) {
  if (state.current_turn_player_id != ai_player_id_) {
    wait_timer_ = 0.0f;
    return;
  }

  if (effects::EffectResolver::Get().is_busy()) {
    return;
  }

  wait_timer_ += delta_time;
  if (wait_timer_ < kActionDelay) {
    return;
  }

  PlayerState& ai_player =
      (ai_player_id_ == state.player->id) ? *state.player : *state.enemy;
  PlayerState& opponent =
      (ai_player_id_ == state.player->id) ? *state.enemy : *state.player;

  // 1. Try to play a creature card
  std::vector<CardInstance*> playable_creatures;
  int max_cost = -1;

  for (const auto& card : ai_player.hand) {
    if (card->data->type == CardType::Creature &&
        card->current_cost <= ai_player.mana) {
      if (card->current_cost > max_cost) {
        max_cost = card->current_cost;
        playable_creatures.clear();
        playable_creatures.push_back(card.get());
      } else if (card->current_cost == max_cost) {
        playable_creatures.push_back(card.get());
      }
    }
  }

  if (!playable_creatures.empty()) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, playable_creatures.size() - 1);
    CardInstance* to_play = playable_creatures[dis(gen)];

    effects::EffectResolver::Get().QueueAction(
        std::make_shared<effects::actions::PlayCardAction>(
            ai_player_id_, to_play->instance_id, std::vector<effects::Target>()));
    wait_timer_ = 0.0f;
    return;
  }

  // 2. Try to attack with creatures
  for (auto& creature : ai_player.board) {
    if (creature->can_attack && !creature->has_attacked) {
      effects::Target target;

      // Determine target priority: Blockers > Other Creatures > Player Health
      std::vector<CardInstance*> blockers;
      std::vector<CardInstance*> other_creatures;

      for (const auto& opp_creature : opponent.board) {
        if (opp_creature->is_blocker) {
          blockers.push_back(opp_creature.get());
        } else {
          other_creatures.push_back(opp_creature.get());
        }
      }

      auto lowest_health_cmp = [](CardInstance* a, CardInstance* b) {
        return a->current_health < b->current_health;
      };

      if (!blockers.empty()) {
        auto it = std::min_element(blockers.begin(), blockers.end(),
                                   lowest_health_cmp);
        target.type = effects::Target::Type::kCreature;
        target.id = (*it)->instance_id;
      } else if (!other_creatures.empty()) {
        auto it = std::min_element(other_creatures.begin(), other_creatures.end(),
                                   lowest_health_cmp);
        target.type = effects::Target::Type::kCreature;
        target.id = (*it)->instance_id;
      } else {
        target.type = (opponent.id == 0) ? effects::Target::Type::kPlayer
                                         : effects::Target::Type::kEnemy;
        target.id = -1;  // Conventional ID for player/enemy health
      }

      effects::EffectResolver::Get().QueueAction(
          std::make_shared<effects::actions::CreatureAttackAction>(
              creature->instance_id, target));
      wait_timer_ = 0.0f;
      return;
    }
  }

  // 3. No more actions, end turn
  effects::EffectResolver::Get().QueueAction(
      std::make_shared<effects::actions::EndTurnAction>(ai_player_id_));
  wait_timer_ = 0.0f;
}

void SimpleAI::Reset() { wait_timer_ = 0.0f; }

}  // namespace core::ai
