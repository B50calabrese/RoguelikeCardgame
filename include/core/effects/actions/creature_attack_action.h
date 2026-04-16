#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_CREATURE_ATTACK_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_CREATURE_ATTACK_ACTION_H_

#include "core/effects/actions/action_base.h"
#include "core/effects/target.h"

namespace core::effects::actions {

/**
 * @brief Action for a creature attacking a target.
 */
class CreatureAttackAction : public ActionBase {
 public:
  CreatureAttackAction(int attacker_id, Target target)
      : attacker_id_(attacker_id), target_(target) {}

  RuleResult Validate(const state::GameState& state) const override;
  void Apply(state::GameState& state) const override;
  bool IsComplete(const state::GameState& state) const override;

  int attacker_id() const { return attacker_id_; }
  Target target() const { return target_; }

  std::string name() const override { return "CreatureAttack"; }

 private:
  int attacker_id_;
  Target target_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_CREATURE_ATTACK_ACTION_H_
