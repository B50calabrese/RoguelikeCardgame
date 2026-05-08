#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_END_TURN_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_END_TURN_ACTION_H_

#include <string>

#include "core/effects/actions/action_base.h"

namespace core::effects::actions {

class EndTurnAction : public ActionBase {
 public:
  explicit EndTurnAction(int player_id) : player_id_(player_id) {}

  RuleResult Validate(const state::CombatState& state) const override;
  void Apply(state::CombatState& state) const override;
  std::string name() const override { return "EndTurnAction"; }

 private:
  int player_id_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_END_TURN_ACTION_H_
