#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_DRAW_EFFECT_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_DRAW_EFFECT_H_

#include <string>
#include <vector>

#include "core/effects/effect.h"
#include "core/effects/actions/draw_card_action.h"

namespace core::effects::types {

/**
 * @brief Simple effect that draws cards for a player.
 */
class DrawEffect : public Effect {
 public:
  std::vector<Action> GenerateActions(int source_id, int actor_id, const std::vector<Target>& targets, const EffectParams& params) const override {
    std::vector<Action> actions;
    int amount = 0;
    auto it = params.find("amount");
    if (it != params.end()) {
      amount = std::stoi(it->second);
    }

    actions.push_back(std::make_shared<actions::DrawCardAction>(actor_id, amount));
    return actions;
  }

  std::string GetDescription(const EffectParams& params) const override {
    int amount = 0;
    auto it = params.find("amount");
    if (it != params.end()) {
      amount = std::stoi(it->second);
    }
    return "Draw " + std::to_string(amount) + (amount == 1 ? " card." : " cards.");
  }
};

}  // namespace core::effects::types

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_DRAW_EFFECT_H_
