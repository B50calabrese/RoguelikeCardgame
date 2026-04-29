#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_SPELL_VISUAL_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_SPELL_VISUAL_ACTION_H_

#include "core/effects/actions/action_base.h"
#include "core/effects/rule_result.h"
#include "core/effects/visual_blocker.h"
#include "core/state/game_state.h"
#include <string>

namespace core::effects::actions {

class SpellVisualAction : public ActionBase {
 public:
  explicit SpellVisualAction(int card_instance_id)
      : card_instance_id_(card_instance_id) {}

  RuleResult Validate(const state::GameState& state) const override {
    return RuleResult::Success();
  }

  void Apply(state::GameState& state) const override {
    VisualBlocker::Get().AddBlocker("SpellVisual_" + std::to_string(card_instance_id_));
  }

  bool IsComplete(const state::GameState& state) const override {
    return !VisualBlocker::Get().is_blocking();
  }

  std::string name() const override { return "SpellVisual"; }
  int card_instance_id() const { return card_instance_id_; }

 private:
  int card_instance_id_;
};

}  // namespace core::effects::actions

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTIONS_SPELL_VISUAL_ACTION_H_
