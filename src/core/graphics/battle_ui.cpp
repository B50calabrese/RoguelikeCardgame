#include "core/graphics/battle_ui.h"
#include "core/game_config.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/end_turn_action.h"
#include "core/util/graphics_util.h"
#include "core/util/math_util.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/primitive_renderer.h"
#include "engine/input/input_manager.h"

namespace core::graphics {

BattleUI::BattleUI() {
  auto& config = core::GameConfig::Get();
  float border_thickness = config.window_width * 0.05f;
  glm::vec2 button_size = {border_thickness * 0.8f, 100.0f};
  glm::vec2 button_pos = {border_thickness * 0.1f, (config.window_height - button_size.y) * 0.5f};

  pass_turn_button_ = std::make_unique<UIButton>("Pass Turn", button_pos, button_size, []() {
    // This will be updated in the scene to use the actual player ID
  });
}

void BattleUI::Update(float delta_time, const GameState& state) {
  auto& input = engine::InputManager::Get();
  glm::vec2 mouse_pos = input.mouse_screen_pos();
  bool clicked = input.IsKeyPressed(engine::KeyCode::kMouseLeft);

  bool is_player_turn = (state.current_turn_player_id == state.player->id);

  if (is_player_turn) {
    pass_turn_button_->set_base_color({0.1f, 0.6f, 0.1f, 1.0f}); // Green
    pass_turn_button_->set_hover_color({0.2f, 0.8f, 0.2f, 1.0f});

    // We need to re-bind the callback to use the current player ID
    // but since the UI is simple, we can just check if it's clicked here
    pass_turn_button_->Update(mouse_pos, clicked);

    // If it was clicked, we need a way to trigger the end turn.
    // For now, let's just use the callback we set in constructor if we could,
    // but the UIButton doesn't let us change it easily.
    // Let's modify UIButton slightly or just handle it here.
    if (core::util::PointInRect(mouse_pos, pass_turn_button_->position(), pass_turn_button_->size(), false) && clicked) {
        effects::EffectResolver::Get().QueueAction(
            std::make_shared<effects::actions::EndTurnAction>(state.player->id));
    }
  } else {
    pass_turn_button_->set_base_color({0.6f, 0.1f, 0.1f, 1.0f}); // Red
    pass_turn_button_->set_hover_color({0.6f, 0.1f, 0.1f, 1.0f});
    pass_turn_button_->Update(mouse_pos, false); // Don't allow clicking
  }
}

void BattleUI::Render(const GameState& state) const {
  RenderBorder();
  RenderManaPool(*state.player, true);
  RenderManaPool(*state.enemy, false);
  pass_turn_button_->Render();
}

void BattleUI::RenderBorder() const {
  auto& config = core::GameConfig::Get();
  auto& renderer = engine::graphics::Renderer::Get();
  float t = config.window_width * 0.05f;
  glm::vec4 border_color = {0.3f, 0.3f, 0.3f, 1.0f};

  // Top
  renderer.DrawRect(0, config.window_height - t, config.window_width, t, border_color.r, border_color.g, border_color.b);
  // Bottom
  renderer.DrawRect(0, 0, config.window_width, t, border_color.r, border_color.g, border_color.b);
  // Left
  renderer.DrawRect(0, 0, t, config.window_height, border_color.r, border_color.g, border_color.b);
  // Right
  renderer.DrawRect(config.window_width - t, 0, t, config.window_height, border_color.r, border_color.g, border_color.b);
}

void BattleUI::RenderManaPool(const state::PlayerState& player, bool is_player) const {
  auto& config = core::GameConfig::Get();
  float border_thickness = config.window_width * 0.05f;
  float radius = border_thickness * 0.3f;

  glm::vec2 base_pos;
  if (is_player) {
    base_pos = {config.window_width - border_thickness * 0.5f, border_thickness * 0.5f};
  } else {
    base_pos = {border_thickness * 0.5f, config.window_height - border_thickness * 0.5f};
  }

  glm::vec4 color1 = {0.5f, 0.5f, 0.5f, 1.0f};
  glm::vec4 color2 = {0.5f, 0.5f, 0.5f, 1.0f};

  if (!player.colors.empty()) {
    color1 = util::GetColorVector(player.colors[0]);
    color2 = (player.colors.size() > 1) ? util::GetColorVector(player.colors[1]) : color1;
  }

  for (int i = 0; i < player.max_mana; ++i) {
    glm::vec2 offset = is_player ? glm::vec2(- (i + 1) * radius * 2.5f, 0.0f) : glm::vec2((i + 1) * radius * 2.5f, 0.0f);
    glm::vec2 pos = base_pos + offset;

    glm::vec4 c1 = color1;
    glm::vec4 c2 = color2;

    if (i >= player.mana) {
      c1.a = 0.3f;
      c2.a = 0.3f;
    }

    engine::graphics::PrimitiveRenderer::SubmitCircle(pos, radius, c1, 0.0f, c2, 1); // 1 = Linear Gradient
  }

  // Draw mana text
  std::string mana_text = std::to_string(player.mana) + "/" + std::to_string(player.max_mana);
  glm::vec2 text_pos = base_pos + (is_player ? glm::vec2(-radius, radius * 1.5f) : glm::vec2(-radius, -radius * 2.5f));
  engine::graphics::Renderer::Get().DrawText("default", mana_text, text_pos, 0.0f, 0.5f, {1.0f, 1.0f, 1.0f, 1.0f});
}

}  // namespace core::graphics
