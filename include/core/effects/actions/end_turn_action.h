#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_END_TURN_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_END_TURN_ACTION_H_

#include "core/effects/actions/action_base.h"
#include <string>

namespace core::effects::actions {

class EndTurnAction : public ActionBase {
 public:
  explicit EndTurnAction(int player_id) : player_id_(player_id) {}

  RuleResult Validate(const state::GameState& state) const override;
  void Apply(state::GameState& state) const override;
  std::string name() const override { return "EndTurnAction"; }

 private:
  int player_id_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_END_TURN_ACTION_H_
