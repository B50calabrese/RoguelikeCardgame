#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ENGINE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ENGINE_H_

#include <vector>
#include <string>
#include <optional>

#include "core/effects/action.h"
#include "core/game_state.h"
#include "core/effects/target_filter.h"

namespace core {

/**
 * @brief Represents the result of a rule check.
 */
struct RuleResult {
  bool success;
  std::string message;
  bool fail_open;  // If true, the action fails but doesn't necessarily block.
};

/**
 * @brief Singleton for validating gameplay actions against rules.
 */
class RulesEngine {
 public:
  static RuleResult CanPerformAction(const GameState& state, const Action& action) {
    return std::visit([&state](auto&& arg) -> RuleResult {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, action::PlayCard>) {
        return ValidatePlayCard(state, arg);
      } else if constexpr (std::is_same_v<T, action::DealDamage>) {
        return ValidateDealDamage(state, arg);
      } else if constexpr (std::is_same_v<T, action::DrawCard>) {
        return {true, "Draw allowed", false};
      } else if constexpr (std::is_same_v<T, action::KillCreature>) {
        return {true, "Kill allowed", false};
      } else if constexpr (std::is_same_v<T, action::ModifyStats>) {
        return {true, "Modify allowed", false};
      }
      return {false, "Unknown action", false};
    }, action);
  }

  static RuleResult ValidatePlayCard(const GameState& state, const action::PlayCard& act) {
    const PlayerState& p = (act.player_id == 0) ? *state.player : *state.enemy;

    // Find card instance
    CardInstance* inst = nullptr;
    for (const auto& c : p.hand) {
      if (c->instance_id == act.card_instance_id) {
        inst = c.get();
        break;
      }
    }

    if (!inst) return {false, "Card not in hand", false};
    if (p.mana < inst->current_cost) return {false, "Insufficient mana", false};

    // Note: Targeted effects on play would have been checked separately or via GenerateActions
    // For now, simple mana check.
    return {true, "Valid play", false};
  }

  static RuleResult ValidateDealDamage(const GameState& state, const action::DealDamage& act) {
    // Check if target is valid in current state
    if (act.target.type == Target::Type::Player || act.target.type == Target::Type::Enemy) {
        return {true, "Valid target player", false};
    }

    CardInstance* target_inst = state.FindCardInstance(act.target.id);
    if (!target_inst || target_inst->location != CardLocation::Board) {
        return {false, "Target not on board", true}; // Fail open if target disappeared
    }

    return {true, "Valid damage target", false};
  }

  /**
   * @brief Checks if a set of targets meets the requirements of a filter.
   */
  static RuleResult CheckTargeting(const GameState& state, int actor_id, const std::vector<Target>& targets, const TargetFilter& filter) {
    if (filter.is_required && targets.empty()) {
        return {false, "Targets required", false};
    }

    for (const auto& t : targets) {
        if (!filter.IsValid(state, actor_id, t)) {
            return {false, "Invalid target for filter", false};
        }
    }

    return {true, "Targets valid", false};
  }
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_ENGINE_H_
