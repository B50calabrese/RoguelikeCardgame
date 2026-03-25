#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_DRAW_CARD_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_DRAW_CARD_ACTION_H_

#include <iostream>

#include "core/effects/actions/action_base.h"
#include "core/effects/rule_result.h"
#include "core/state/game_state.h"

namespace core::effects::actions {

class DrawCardAction : public ActionBase {
 public:
  DrawCardAction(int player_id, int amount)
      : player_id_(player_id), amount_(amount) {}

  RuleResult Validate(const GameState& state) const override {
    return {true, "Draw allowed", false};
  }

  void Apply(GameState& state) const override {
    PlayerState& p = (player_id_ == 0) ? *state.player : *state.enemy;
    if (p.deck.empty()) {
        std::cout << "[EffectResolver] No more cards in deck for player " << player_id_ << std::endl;
        return;
    }

    for (int i = 0; i < amount_ && !p.deck.empty(); ++i) {
        std::unique_ptr<CardInstance> c = std::move(p.deck.back());
        p.deck.pop_back();
        c->location = CardLocation::Hand;
        std::cout << "[EffectResolver] Player " << player_id_ << " draws " << c->data->name << std::endl;
        p.hand.push_back(std::move(c));
    }
  }

  std::string name() const override { return "DrawCard"; }

 private:
  int player_id_;
  int amount_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_DRAW_CARD_ACTION_H_
