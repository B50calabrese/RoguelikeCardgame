#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_HEAL_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_HEAL_ACTION_H_

#include "core/effects/actions/action_base.h"
#include "core/effects/rule_result.h"
#include "core/effects/target.h"
#include "core/state/game_state.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

class HealAction : public ActionBase {
 public:
  HealAction(Target target, int amount) : target_(target), amount_(amount) {}

  RuleResult Validate(const GameState& state) const override {
    if (target_.type == Target::Type::kPlayer ||
        target_.type == Target::Type::kEnemy) {
      return {true, "Valid target player", false};
    }

    CardInstance* target_inst = state.FindCardInstance(target_.id);
    if (!target_inst || target_inst->location != CardLocation::Board) {
      return {false, "Target not on board", false};
    }

    return {true, "Valid heal target", false};
  }

  void Apply(GameState& state) const override {
    if (target_.type == Target::Type::kPlayer) {
      state.player->health =
          std::min(state.player->max_health, state.player->health + amount_);
      LOG_INFO("[EffectResolver] Player healed for %d. New health: %d", amount_,
               state.player->health);
    } else if (target_.type == Target::Type::kEnemy) {
      state.enemy->health =
          std::min(state.enemy->max_health, state.enemy->health + amount_);
      LOG_INFO("[EffectResolver] Enemy healed for %d. New health: %d", amount_,
               state.enemy->health);
    } else if (target_.type == Target::Type::kCreature) {
      CardInstance* target_inst = state.FindCardInstance(target_.id);
      if (target_inst) {
        target_inst->current_health =
            std::min(target_inst->max_health, target_inst->current_health + amount_);
        LOG_INFO("[EffectResolver] Creature %s healed for %d. New health: %d",
                 target_inst->data->name.c_str(), amount_,
                 target_inst->current_health);
      }
    }
  }

  std::string name() const override { return "Heal"; }

 private:
  Target target_;
  int amount_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_HEAL_ACTION_H_
