#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TRIGGER_SYSTEM_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TRIGGER_SYSTEM_H_

#include <memory>
#include <vector>
#include "core/effects/game_event.h"
#include "core/effects/trigger.h"
#include "core/state/game_state.h"
#include "core/effects/effect_registry.h"

namespace core::effects {

class EffectResolver;

/**
 * @brief System for handling triggered abilities in the game.
 */
class TriggerSystem {
 public:
  static TriggerSystem& Get() {
    static TriggerSystem instance;
    return instance;
  }

  /**
   * @brief Handles a game event by checking for any triggered abilities.
   */
  void HandleEvent(state::GameState& state, const GameEvent& event) {
    // Determine which Trigger type this event maps to
    Trigger trigger_to_check;
    switch (event.type) {
      case GameEventType::CardPlayed: trigger_to_check = Trigger::OnPlay; break;
      case GameEventType::CreatureDied: trigger_to_check = Trigger::OnDeath; break;
      case GameEventType::TurnStarted: trigger_to_check = Trigger::AtStartOfTurn; break;
      case GameEventType::TurnEnded: trigger_to_check = Trigger::AtEndOfTurn; break;
      default: return; // Not a triggered event type
    }

    // Identify the card instance that is triggering (the source)
    CardInstance* source_inst = state.FindCardInstance(event.source_id);

    // Check all effects on the card for the matching trigger.
    if (source_inst) {
      for (const auto& effect_def : source_inst->data->effects) {
        if (effect_def.trigger == trigger_to_check) {
          // If the effect requires targets, this simple system might need human input or AI
          // For now, we only automatically trigger non-targeted effects or effects where targets are provided.
          if (effect_def.filter.is_required && event.targets.empty()) continue;

          auto effect = EffectRegistry::Get().CreateEffect(effect_def.effect_type);
          if (effect) {
              auto actions = effect->GenerateActions(source_inst->instance_id, event.actor_id, event.targets, effect_def.params);
            QueueActions(actions);
          }
        }
      }
    }

    // Also check OTHER cards on the board for triggers (e.g. "Whenever a creature dies...")
    auto check_others = [&](const std::vector<std::unique_ptr<CardInstance>>& board) {
      for (const auto& inst : board) {
        if (source_inst && inst->instance_id == source_inst->instance_id) continue;
        for (const auto& effect_def : inst->data->effects) {
          if (effect_def.trigger == trigger_to_check && !effect_def.filter.is_required) {
            auto effect = EffectRegistry::Get().CreateEffect(effect_def.effect_type);
            if (effect) {
              auto actions = effect->GenerateActions(inst->instance_id, event.actor_id, {}, effect_def.params);
              QueueActions(actions);
            }
          }
        }
      }
    };

    check_others(state.player->board);
    check_others(state.enemy->board);
  }

 private:
  TriggerSystem() = default;
  void QueueActions(const std::vector<Action>& actions);
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TRIGGER_SYSTEM_H_
