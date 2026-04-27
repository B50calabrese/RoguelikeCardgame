#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_BATTLE_UI_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_BATTLE_UI_H_

#include <glm/vec2.hpp>
#include <memory>
#include <vector>

#include "core/graphics/health_icon.h"
#include "core/graphics/ui_button.h"
#include "core/state/game_state.h"

namespace core::graphics {

class BattleUI {
 public:
  BattleUI();
  void Update(float delta_time, const GameState& state);
  void Render(const GameState& state, const glm::vec4& player_zone,
              const glm::vec4& enemy_zone) const;

 private:
  void RenderBorder() const;
  void RenderManaPool(const state::PlayerState& player, bool is_player) const;

  std::unique_ptr<UIButton> pass_turn_button_;
  std::unique_ptr<HealthIcon> player_health_icon_;
  std::unique_ptr<HealthIcon> enemy_health_icon_;
};

}  // namespace core::graphics

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_BATTLE_UI_H_
