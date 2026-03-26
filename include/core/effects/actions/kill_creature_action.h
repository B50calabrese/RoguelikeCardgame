#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_KILL_CREATURE_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_KILL_CREATURE_ACTION_H_

#include <algorithm>

#include "core/effects/actions/action_base.h"
#include "core/effects/rule_result.h"
#include "core/state/game_state.h"
#include "core/card_data.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

class KillCreatureAction : public ActionBase {
 public:
  KillCreatureAction(int card_instance_id) : card_instance_id_(card_instance_id) {}

  RuleResult Validate(const state::GameState& state) const override {
    return {true, "Kill allowed", false};
  }

  void Apply(state::GameState& state) const override {
    auto find_and_kill = [this](PlayerState& p, int id) -> bool {
      auto it = std::find_if(p.board.begin(), p.board.end(),
          [id](const auto& c) { return c->instance_id == id; });
      if (it != p.board.end()) {
          LOG_INFO("[EffectResolver] Creature %s dies.", (*it)->data->name.c_str());

          // Logic: Transition Board -> Graveyard
          (*it)->location = CardLocation::Graveyard;
          p.graveyard.push_back(std::move(*it));
          p.board.erase(it);

          // Note: In a full implementation, we would trigger an "OnDeath" event here
          // that could queue further actions for cards on the board that care about death.
          return true;
      }
      return false;
    };

    if (!find_and_kill(*state.player, card_instance_id_)) {
        find_and_kill(*state.enemy, card_instance_id_);
    }
  }

  std::string name() const override { return "KillCreature"; }

 private:
  int card_instance_id_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_KILL_CREATURE_ACTION_H_
