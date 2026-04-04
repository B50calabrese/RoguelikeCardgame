#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_BATTLE_UI_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_BATTLE_UI_H_

#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include "core/state/game_state.h"
#include "core/graphics/ui_button.h"

namespace core::graphics {

class BattleUI {
 public:
  BattleUI();
  void Update(float delta_time, const GameState& state);
  void Render(const GameState& state) const;

 private:
  void RenderBorder() const;
  void RenderManaPool(const state::PlayerState& player, bool is_player) const;

  std::unique_ptr<UIButton> pass_turn_button_;
};

}  // namespace core::graphics

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_BATTLE_UI_H_
