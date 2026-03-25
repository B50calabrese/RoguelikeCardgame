#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_RESOLVER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_RESOLVER_H_

#include <deque>
#include <memory>
#include <vector>
#include <iostream>

#include "core/effects/action.h"
#include "core/game_state.h"
#include "core/effects/rules_engine.h"

namespace core {

/**
 * @brief Singleton for managing the action queue and executing state changes.
 */
class EffectResolver {
 public:
  static EffectResolver& Get() {
    static EffectResolver instance;
    return instance;
  }

  /**
   * @brief Adds an action to the queue.
   */
  void QueueAction(const Action& action) {
    action_queue_.push_back(action);
  }

  /**
   * @brief Processes all pending actions in the queue.
   */
  void ProcessQueue(GameState& state) {
    while (!action_queue_.empty()) {
      Action next = action_queue_.front();
      action_queue_.pop_front();

      // Validate action
      RuleResult rule = RulesEngine::CanPerformAction(state, next);
      if (!rule.success) {
          std::cout << "[EffectResolver] Action failed: " << rule.message << " (fail_open: " << rule.fail_open << ")" << std::endl;
          if (!rule.fail_open) continue;
      }

      // Apply action to state
      ApplyAction(state, next);
    }
  }

 private:
  EffectResolver() = default;

  void ApplyAction(GameState& state, const Action& action) {
    std::visit([&state, this](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, action::PlayCard>) {
          ApplyPlayCard(state, arg);
      } else if constexpr (std::is_same_v<T, action::DealDamage>) {
          ApplyDealDamage(state, arg);
      } else if constexpr (std::is_same_v<T, action::DrawCard>) {
          ApplyDrawCard(state, arg);
      } else if constexpr (std::is_same_v<T, action::KillCreature>) {
          ApplyKillCreature(state, arg);
      } else if constexpr (std::is_same_v<T, action::ModifyStats>) {
          ApplyModifyStats(state, arg);
      }
    }, action);
  }

  void ApplyPlayCard(GameState& state, const action::PlayCard& act) {
    PlayerState& p = (act.player_id == 0) ? *state.player : *state.enemy;

    // Find card in hand
    auto it = std::find_if(p.hand.begin(), p.hand.end(),
        [id = act.card_instance_id](const auto& c) { return c->instance_id == id; });

    if (it != p.hand.end()) {
        std::cout << "[EffectResolver] Playing card: " << (*it)->data->name << std::endl;
        p.mana -= (*it)->current_cost;
        (*it)->location = CardLocation::Board;
        p.board.push_back(std::move(*it));
        p.hand.erase(it);

        // Note: Effects that trigger 'OnPlay' will be handled by the caller/scene
        // who triggers this action, by queuing more actions.
    }
  }

  void ApplyDealDamage(GameState& state, const action::DealDamage& act) {
    if (act.target.type == Target::Type::Player) {
        state.player->health -= act.amount;
        std::cout << "[EffectResolver] Player takes " << act.amount << " damage. Health: " << state.player->health << std::endl;
    } else if (act.target.type == Target::Type::Enemy) {
        state.enemy->health -= act.amount;
        std::cout << "[EffectResolver] Enemy takes " << act.amount << " damage. Health: " << state.enemy->health << std::endl;
    } else if (act.target.type == Target::Type::Creature) {
        CardInstance* target_inst = state.FindCardInstance(act.target.id);
        if (target_inst && target_inst->location == CardLocation::Board) {
            target_inst->current_health -= act.amount;
            std::cout << "[EffectResolver] Creature " << target_inst->data->name << " takes " << act.amount << " damage. Health: " << target_inst->current_health << std::endl;
            if (target_inst->current_health <= 0) {
                QueueAction(action::KillCreature{target_inst->instance_id});
            }
        }
    }
  }

  void ApplyDrawCard(GameState& state, const action::DrawCard& act) {
    PlayerState& p = (act.player_id == 0) ? *state.player : *state.enemy;
    if (p.deck.empty()) {
        std::cout << "[EffectResolver] No more cards in deck for player " << act.player_id << std::endl;
        return;
    }

    for (int i = 0; i < act.amount && !p.deck.empty(); ++i) {
        std::unique_ptr<CardInstance> c = std::move(p.deck.back());
        p.deck.pop_back();
        c->location = CardLocation::Hand;
        std::cout << "[EffectResolver] Player " << act.player_id << " draws " << c->data->name << std::endl;
        p.hand.push_back(std::move(c));
    }
  }

  void ApplyKillCreature(GameState& state, const action::KillCreature& act) {
    // Find the creature on either board
    auto find_and_kill = [&state](PlayerState& p, int id) -> bool {
      auto it = std::find_if(p.board.begin(), p.board.end(),
          [id](const auto& c) { return c->instance_id == id; });
      if (it != p.board.end()) {
          std::cout << "[EffectResolver] Creature " << (*it)->data->name << " dies." << std::endl;
          (*it)->location = CardLocation::Graveyard;
          p.graveyard.push_back(std::move(*it));
          p.board.erase(it);
          return true;
      }
      return false;
    };

    if (!find_and_kill(*state.player, act.card_instance_id)) {
        find_and_kill(*state.enemy, act.card_instance_id);
    }
  }

  void ApplyModifyStats(GameState& state, const action::ModifyStats& act) {
    CardInstance* inst = state.FindCardInstance(act.card_instance_id);
    if (inst && inst->location == CardLocation::Board) {
        inst->current_power += act.power_change;
        inst->current_health += act.health_change;
        inst->max_health += act.health_change;
        std::cout << "[EffectResolver] Modified " << inst->data->name << " stats: P=" << inst->current_power << ", H=" << inst->current_health << std::endl;

        if (inst->current_health <= 0) {
            QueueAction(action::KillCreature{inst->instance_id});
        }
    }
  }

  std::deque<Action> action_queue_;
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_RESOLVER_H_
