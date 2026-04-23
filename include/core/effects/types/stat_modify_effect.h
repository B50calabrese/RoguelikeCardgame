#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_STAT_MODIFY_EFFECT_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_STAT_MODIFY_EFFECT_H_

#include <string>
#include <vector>

#include "core/effects/effect.h"
#include "core/effects/actions/modify_stats_action.h"

namespace core::effects::types {

/**
 * @brief Effect that modifies a creature's power and health.
 */
class StatModifyEffect : public Effect {
 public:
  std::vector<Action> GenerateActions(int source_id, int actor_id, const std::vector<Target>& targets, const EffectParams& params) const override {
    std::vector<Action> actions;
    if (targets.empty()) return actions;

    int power = 0;
    int health = 0;

    auto it_p = params.find("power");
    if (it_p != params.end()) power = std::stoi(it_p->second);

    auto it_h = params.find("health");
    if (it_h != params.end()) health = std::stoi(it_h->second);

    bool permanent = true;
    auto it_perm = params.find("duration");
    if (it_perm != params.end() && it_perm->second == "UntilEndOfTurn") {
      permanent = false;
    }

    actions.push_back(std::make_shared<actions::ModifyStatsAction>(targets[0].id, power, health));
    return actions;
  }

  std::string GetDescription(const EffectParams& params) const override {
    int power = 0;
    int health = 0;

    auto it_p = params.find("power");
    if (it_p != params.end()) power = std::stoi(it_p->second);

    auto it_h = params.find("health");
    if (it_h != params.end()) health = std::stoi(it_h->second);

    std::string p_str = (power >= 0 ? "+" : "") + std::to_string(power);
    std::string h_str = (health >= 0 ? "+" : "") + std::to_string(health);

    return "Give a creature " + p_str + "/" + h_str + ".";
  }
};

}  // namespace core::effects::types

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_STAT_MODIFY_EFFECT_H_
