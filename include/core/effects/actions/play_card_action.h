#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_PLAY_CARD_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_PLAY_CARD_ACTION_H_

#include <vector>
#include <algorithm>
#include <iostream>

#include "core/effects/actions/action_base.h"
#include "core/effects/target.h"
#include "core/effects/rule_result.h"
#include "core/state/game_state.h"
#include "core/card_data.h"

namespace core::effects::actions {

class PlayCardAction : public ActionBase {
 public:
  PlayCardAction(int player_id, int card_instance_id, std::vector<Target> targets)
      : player_id_(player_id), card_instance_id_(card_instance_id), targets_(std::move(targets)) {}

  RuleResult Validate(const state::GameState& state) const override {
    const PlayerState& p = (player_id_ == 0) ? *state.player : *state.enemy;

    CardInstance* inst = nullptr;
    for (const auto& c : p.hand) {
      if (c->instance_id == card_instance_id_) {
        inst = c.get();
        break;
      }
    }

    if (!inst) return {false, "Card not in hand", false};
    if (p.mana < inst->current_cost) return {false, "Insufficient mana", false};

    return {true, "Valid play", false};
  }

  void Apply(state::GameState& state) const override {
    PlayerState& p = (player_id_ == 0) ? *state.player : *state.enemy;

    auto it = std::find_if(p.hand.begin(), p.hand.end(),
        [id = card_instance_id_](const auto& c) { return c->instance_id == id; });

    if (it != p.hand.end()) {
        std::cout << "[EffectResolver] Playing card: " << (*it)->data->name << std::endl;
        p.mana -= (*it)->current_cost;
        (*it)->location = CardLocation::Board;
        p.board.push_back(std::move(*it));
        p.hand.erase(it);
    }
  }

  std::string name() const override { return "PlayCard"; }

 private:
  int player_id_;
  int card_instance_id_;
  std::vector<Target> targets_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_PLAY_CARD_ACTION_H_
