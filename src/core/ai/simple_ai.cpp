#include "core/ai/simple_ai.h"

#include <algorithm>
#include <random>

#include "core/effects/actions/creature_attack_action.h"
#include "core/effects/actions/end_turn_action.h"
#include "core/effects/actions/play_card_action.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/rules_engine.h"

namespace core::ai {

void SimpleAI::Update(float delta_time, CombatState& state) {
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

  PlayerState& ai_player = state.GetPlayerById(ai_player_id_);
  PlayerState& opponent = state.GetOpponentOf(ai_player_id_);

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
            ai_player_id_, to_play->instance_id,
            std::vector<effects::Target>()));
    wait_timer_ = 0.0f;
    return;
  }

  // 2. Try to attack with creatures
  for (auto& creature : ai_player.board) {
    if (creature->can_attack && !creature->has_attacked) {
      effects::Target target;

      // Determine target priority: Blockers > Other Creatures > Player Health
      std::vector<effects::Target> possible_targets;

      // 1. Check for blockers
      for (const auto& opp_creature : opponent.board) {
        if (opp_creature->is_blocker) {
          possible_targets.push_back(
              {effects::Target::Type::kCreature, opp_creature->instance_id});
        }
      }

      // 2. If no blockers, check for other creatures
      if (possible_targets.empty()) {
        for (const auto& opp_creature : opponent.board) {
          possible_targets.push_back(
              {effects::Target::Type::kCreature, opp_creature->instance_id});
        }
      }

      // 3. If no creatures, target opponent's health
      if (possible_targets.empty()) {
        target.type = (opponent.id == 0) ? effects::Target::Type::kPlayer
                                         : effects::Target::Type::kEnemy;
        target.id = -1;
      } else {
        // Find valid targets according to rules
        std::vector<effects::Target> valid_targets;
        for (const auto& t : possible_targets) {
          auto attack_action =
              std::make_shared<effects::actions::CreatureAttackAction>(
                  creature->instance_id, t);
          if (effects::RulesEngine::Get()
                  .ValidateAction(state, attack_action)
                  .success) {
            valid_targets.push_back(t);
          }
        }

        if (valid_targets.empty()) continue;

        // Among valid targets, pick one with lowest health if they are
        // creatures
        auto lowest_health_it = std::min_element(
            valid_targets.begin(), valid_targets.end(),
            [&state](const effects::Target& a, const effects::Target& b) {
              if (a.type == effects::Target::Type::kCreature &&
                  b.type == effects::Target::Type::kCreature) {
                return state.FindCardInstance(a.id)->current_health <
                       state.FindCardInstance(b.id)->current_health;
              }
              return false;
            });
        target = *lowest_health_it;
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
