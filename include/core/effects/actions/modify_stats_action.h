#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_MODIFY_STATS_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_MODIFY_STATS_ACTION_H_

#include <iostream>

#include "core/effects/actions/action_base.h"
#include "core/effects/rule_result.h"
#include "core/state/game_state.h"

namespace core::effects::actions {

class ModifyStatsAction : public ActionBase {
 public:
  ModifyStatsAction(int card_instance_id, int power_change, int health_change, bool is_permanent = true)
      : card_instance_id_(card_instance_id), power_change_(power_change), health_change_(health_change), is_permanent_(is_permanent) {}

  RuleResult Validate(const GameState& state) const override {
    return {true, "Modify allowed", false};
  }

  void Apply(GameState& state) const override;

  std::string name() const override { return "ModifyStats"; }

 private:
  int card_instance_id_;
  int power_change_;
  int health_change_;
  bool is_permanent_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_MODIFY_STATS_ACTION_H_
