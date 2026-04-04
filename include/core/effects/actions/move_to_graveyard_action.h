#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_MOVE_TO_GRAVEYARD_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_MOVE_TO_GRAVEYARD_ACTION_H_

#include <algorithm>
#include "core/effects/actions/action_base.h"
#include "core/effects/rule_result.h"
#include "core/state/game_state.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

class MoveToGraveyardAction : public ActionBase {
 public:
  MoveToGraveyardAction(int card_instance_id) : card_instance_id_(card_instance_id) {}

  RuleResult Validate(const state::GameState& state) const override {
    return {true, "Move to graveyard allowed", false};
  }

  void Apply(state::GameState& state) const override {
    auto find_and_move = [this](PlayerState& p, int id) -> bool {
      auto it = std::find_if(p.limbo.begin(), p.limbo.end(),
          [id](const auto& c) { return c->instance_id == id; });
      if (it != p.limbo.end()) {
          LOG_INFO("[EffectResolver] Moving card %s from Limbo to Graveyard.", (*it)->data->name.c_str());
          (*it)->location = CardLocation::Graveyard;
          p.graveyard.push_back(std::move(*it));
          p.limbo.erase(it);
          return true;
      }
      return false;
    };

    if (!find_and_move(*state.player, card_instance_id_)) {
        find_and_move(*state.enemy, card_instance_id_);
    }
  }

  std::string name() const override { return "MoveToGraveyard"; }

 private:
  int card_instance_id_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_MOVE_TO_GRAVEYARD_ACTION_H_
