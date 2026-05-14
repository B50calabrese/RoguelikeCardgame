#include "scenes/combat/spell_visual_manager.h"

#include <algorithm>
#include <string>

#include "core/effects/actions/spell_visual_action.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/visual_blocker.h"
#include "core/game_config.h"
#include "core/graphics/card_renderer.h"
#include "scenes/combat/combat_ui_constants.h"

namespace scenes::combat {

void SpellVisualManager::Update(float delta_time,
                                core::state::CombatState& state) {
  // Check for new SpellVisualAction
  auto current_action = core::effects::EffectResolver::Get().current_action();
  if (current_action) {
    auto spell_action =
        std::dynamic_pointer_cast<core::effects::actions::SpellVisualAction>(
            current_action);
    if (spell_action) {
      int inst_id = spell_action->card_instance_id();
      bool already_active = std::any_of(
          active_visuals_.begin(), active_visuals_.end(),
          [inst_id](const auto& v) { return v.instance_id == inst_id; });

      if (!already_active) {
        auto& config = core::GameConfig::Get();
        ActiveSpellVisual visual;
        visual.instance_id = inst_id;
        visual.elapsed_time = 0.0f;
        visual.current_pos = {config.window_width * 0.3f,
                              config.window_height * 0.5f};
        active_visuals_.push_back(visual);
      }
    }
  }

  // Update active visuals
  for (auto it = active_visuals_.begin(); it != active_visuals_.end();) {
    it->elapsed_time += delta_time;

    if (it->elapsed_time >= 1.5f) {
      core::effects::VisualBlocker::Get().RemoveBlocker(
          "SpellVisual_" + std::to_string(it->instance_id));
      it = active_visuals_.erase(it);
    } else {
      ++it;
    }
  }
}

void SpellVisualManager::Render(const core::state::CombatState& state) const {
  for (const auto& visual : active_visuals_) {
    core::CardInstance* inst = state.FindCardInstance(visual.instance_id);
    if (inst) {
      core::graphics::CardRenderer::RenderCard(*inst->data, visual.current_pos,
                                               0.8f, 1.0f, 0.0f,
                                               combat::kHandZ + 500.0f);
    }
  }
}

}  // namespace scenes::combat
