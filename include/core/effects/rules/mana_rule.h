#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_MANA_RULE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_MANA_RULE_H_

#include "core/effects/rule.h"
#include "core/effects/actions/play_card_action.h"
#include "core/state/player_state.h"
#include "core/card_instance.h"

namespace core::effects::rules {

/**
 * @brief Checks if a player has enough mana to play a card.
 */
class ManaRule : public IRule {
 public:
  RuleResult Validate(const state::GameState& state, const Action& action) const override {
    // Only check mana for PlayCardAction
    auto play_action = std::dynamic_pointer_cast<actions::PlayCardAction>(action);
    if (!play_action) return {true, "", false};

    int player_id = play_action->GetActorId();
    int card_id = play_action->card_instance_id();

    const PlayerState& p = (player_id == 0) ? *state.player : *state.enemy;

    // We need a way to find the card in hand to check its cost
    const CardInstance* inst = nullptr;
    for (const auto& c : p.hand) {
      if (c->instance_id == card_id) {
        inst = c.get();
        break;
      }
    }

    if (!inst) return {false, "Card not in hand", false};
    if (p.mana < inst->current_cost) return {false, "Insufficient mana", false};

    return {true, "Enough mana", false};
  }
};

}  // namespace core::effects::rules

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_MANA_RULE_H_
