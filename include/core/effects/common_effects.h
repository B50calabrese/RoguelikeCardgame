#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_COMMON_EFFECTS_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_COMMON_EFFECTS_H_

#include <string>
#include <vector>

#include "core/effects/effect.h"
#include "core/effects/actions/deal_damage_action.h"
#include "core/effects/actions/draw_card_action.h"
#include "core/effects/actions/modify_stats_action.h"

namespace core::effects {

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

    actions.push_back(std::make_shared<actions::DealDamageAction>(source_id, targets[0], amount));
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

    // Default to player 0 for now
    actions.push_back(std::make_shared<actions::DrawCardAction>(0, amount));
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

    actions.push_back(std::make_shared<actions::ModifyStatsAction>(targets[0].id, power, health));
    return actions;
  }
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_COMMON_EFFECTS_H_
