#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_RESOLVE_SPELL_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_RESOLVE_SPELL_ACTION_H_

#include <vector>
#include "core/effects/actions/action_base.h"
#include "core/effects/target.h"
#include "core/effects/rule_result.h"
#include "core/effects/effect_registry.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/move_to_graveyard_action.h"
#include "core/state/game_state.h"
#include "engine/util/logger.h"

namespace core::effects::actions {

class ResolveSpellAction : public ActionBase {
 public:
  ResolveSpellAction(int player_id, int card_instance_id, std::vector<Target> targets)
      : player_id_(player_id), card_instance_id_(card_instance_id), targets_(std::move(targets)) {}

  RuleResult Validate(const state::GameState& state) const override {
    return {true, "Valid resolution", false};
  }

  void Apply(state::GameState& state) const override {
    CardInstance* inst = state.FindCardInstance(card_instance_id_);
    if (!inst || inst->location != CardLocation::Limbo) {
        LOG_ERR("[EffectResolver] Spell %d not found in limbo for resolution.", card_instance_id_);
        return;
    }

    LOG_INFO("[EffectResolver] Resolving spell: %s", inst->data->name.c_str());

    // NOTE: In a full UI implementation, we'd wait for user targeting if targets_ is empty
    // but the spell requires targets. For now, we assume targets_ contains the needed targets
    // or we use the first available legal target if needed (placeholder logic).

    for (const auto& effect_def : inst->data->effects) {
      auto effect = EffectRegistry::Get().CreateEffect(effect_def.effect_type);
      if (effect) {
          // Generate actions for this effect.
          // TODO: Improve targeting logic. For now, we pass the targets_ provided to PlayCardAction.
          auto actions = effect->GenerateActions(card_instance_id_, targets_, effect_def.params);
          for (auto& action : actions) {
              EffectResolver::Get().QueueAction(action);
          }
      }
    }

    // After all effects are queued, queue the move to graveyard.
    EffectResolver::Get().QueueAction(std::make_shared<MoveToGraveyardAction>(card_instance_id_));
  }

  std::string name() const override { return "ResolveSpell"; }

 private:
  int player_id_;
  int card_instance_id_;
  std::vector<Target> targets_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_RESOLVE_SPELL_ACTION_H_
