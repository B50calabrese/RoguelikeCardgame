#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_DEAL_DAMAGE_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_DEAL_DAMAGE_ACTION_H_

#include <iostream>

#include "core/effects/actions/action_base.h"
#include "core/effects/target.h"
#include "core/effects/rule_result.h"
#include "core/state/game_state.h"

namespace core::effects::actions {

class DealDamageAction : public ActionBase {
 public:
  DealDamageAction(int source_id, Target target, int amount)
      : source_id_(source_id), target_(target), amount_(amount) {}

  RuleResult Validate(const GameState& state) const override {
    if (target_.type == Target::Type::Player || target_.type == Target::Type::Enemy) {
        return {true, "Valid target player", false};
    }

    CardInstance* target_inst = state.FindCardInstance(target_.id);
    if (!target_inst || target_inst->location != CardLocation::Board) {
        return {false, "Target not on board", true}; // Fail open
    }

    return {true, "Valid damage target", false};
  }

  void Apply(GameState& state) const override;

  std::string name() const override { return "DealDamage"; }

 private:
  int source_id_;
  Target target_;
  int amount_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_DEAL_DAMAGE_ACTION_H_
