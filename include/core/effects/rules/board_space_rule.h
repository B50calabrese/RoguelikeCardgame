#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_BOARD_SPACE_RULE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_BOARD_SPACE_RULE_H_

#include "core/effects/rule.h"
#include "core/effects/actions/play_card_action.h"
#include "core/state/player_state.h"
#include "core/card_instance.h"

namespace core::effects::rules {

/**
 * @brief Checks if a player has enough space on the board to play a creature.
 */
class BoardSpaceRule : public IRule {
 public:
  RuleResult Validate(const state::GameState& state, const Action& action) const override {
    // Only check board space for PlayCardAction
    auto play_action = std::dynamic_pointer_cast<actions::PlayCardAction>(action);
    if (!play_action) return RuleResult::Success();

    int player_id = play_action->GetActorId();
    int card_id = play_action->card_instance_id();

    const PlayerState& p = (player_id == 0) ? *state.player : *state.enemy;

    // Find the card in hand to check its type
    const CardInstance* inst = nullptr;
    for (const auto& c : p.hand) {
      if (c->instance_id == card_id) {
        inst = c.get();
        break;
      }
    }

    if (!inst) return RuleResult::Failure("Card not in hand");

    // Only creatures occupy board space
    if (inst->data->type == CardType::Creature) {
      if (p.board.size() >= 7) {
        return RuleResult::Failure("Board is full (max 7 creatures)");
      }
    }

    return RuleResult::Success("Sufficient board space");
  }
};

}  // namespace core::effects::rules

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_RULES_BOARD_SPACE_RULE_H_
