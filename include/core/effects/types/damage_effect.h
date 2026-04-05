#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_DAMAGE_EFFECT_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_DAMAGE_EFFECT_H_

#include <string>
#include <vector>

#include "core/effects/effect.h"
#include "core/effects/actions/deal_damage_action.h"

namespace core::effects::types {

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

  std::string GetDescription(const EffectParams& params) const override {
    int amount = 0;
    auto it = params.find("amount");
    if (it != params.end()) {
      amount = std::stoi(it->second);
    }
    return "Deal " + std::to_string(amount) + " damage.";
  }
};

}  // namespace core::effects::types

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_DAMAGE_EFFECT_H_
