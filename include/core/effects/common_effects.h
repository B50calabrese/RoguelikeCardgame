#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_COMMON_EFFECTS_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_COMMON_EFFECTS_H_

#include <string>
#include <vector>

#include "core/effects/effect.h"

namespace core {

/**
 * @brief Simple effect that deals damage to a single target.
 */
class DamageEffect : public Effect {
 public:
  std::vector<Action> GenerateActions(int source_id, const std::vector<Target>& targets, const EffectParams& params) const override {
    std::vector<Action> actions;
    if (targets.empty()) return actions;

    int amount = 0;
    auto it = params.find("amount");
    if (it != params.end()) {
      amount = std::stoi(it->second);
    }

    actions.push_back(action::DealDamage{source_id, targets[0], amount});
    return actions;
  }
};

/**
 * @brief Simple effect that draws cards for a player.
 */
class DrawEffect : public Effect {
 public:
  std::vector<Action> GenerateActions(int source_id, const std::vector<Target>& targets, const EffectParams& params) const override {
    std::vector<Action> actions;
    int amount = 0;
    auto it = params.find("amount");
    if (it != params.end()) {
      amount = std::stoi(it->second);
    }

    // Usually source_id's owner draws, but we might have custom logic later.
    // For now, assume player 0 unless specified.
    actions.push_back(action::DrawCard{0, amount});
    return actions;
  }
};

/**
 * @brief Effect that modifies a creature's power and health.
 */
class StatModifyEffect : public Effect {
 public:
  std::vector<Action> GenerateActions(int source_id, const std::vector<Target>& targets, const EffectParams& params) const override {
    std::vector<Action> actions;
    if (targets.empty()) return actions;

    int power = 0;
    int health = 0;

    auto it_p = params.find("power");
    if (it_p != params.end()) power = std::stoi(it_p->second);

    auto it_h = params.find("health");
    if (it_h != params.end()) health = std::stoi(it_h->second);

    actions.push_back(action::ModifyStats{targets[0].id, power, health});
    return actions;
  }
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_COMMON_EFFECTS_H_
