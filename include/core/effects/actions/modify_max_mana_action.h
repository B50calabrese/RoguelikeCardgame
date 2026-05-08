#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_MODIFY_MAX_MANA_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_MODIFY_MAX_MANA_ACTION_H_

#include "core/effects/actions/action_base.h"
#include "core/effects/rule_result.h"
#include "core/state/combat_state.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

class ModifyMaxManaAction : public ActionBase {
 public:
  ModifyMaxManaAction(int player_id, int amount)
      : player_id_(player_id), amount_(amount) {}

  RuleResult Validate(const CombatState& state) const override {
    return {true, "OK", false};
  }

  void Apply(CombatState& state) const override {
    int target_player_id = player_id_;
    if (target_player_id == -1) {
      target_player_id = state.current_turn_player_id;
    }

    PlayerState& p =
        (target_player_id == state.player->id) ? *state.player : *state.enemy;
    p.max_mana += amount_;
    LOG_INFO("[EffectResolver] Player %d max mana modified by %d. New max: %d",
             target_player_id, amount_, p.max_mana);
  }

  std::string name() const override { return "ModifyMaxMana"; }

 private:
  int player_id_;
  int amount_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_MODIFY_MAX_MANA_ACTION_H_
