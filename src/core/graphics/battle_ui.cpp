#include "core/graphics/battle_ui.h"
#include "core/game_config.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/end_turn_action.h"
#include "core/util/graphics_util.h"
#include "core/util/math_util.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/primitive_renderer.h"
#include "engine/graphics/utils/render_queue.h"
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

  float icon_size = config.window_width * 0.1f;
  glm::vec2 player_pos = {config.window_width * 0.5f, border_thickness + icon_size * 0.5f};
  glm::vec2 enemy_pos = {config.window_width * 0.5f, config.window_height - border_thickness - icon_size * 0.5f};

  player_health_icon_ = std::make_unique<HealthIcon>(player_pos, icon_size, glm::vec4(0.0f, 0.8f, 0.0f, 1.0f));
  enemy_health_icon_ = std::make_unique<HealthIcon>(enemy_pos, icon_size, glm::vec4(0.8f, 0.0f, 0.0f, 1.0f));
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

  if (player_health_icon_) {
    player_health_icon_->Render(state.player->health);
  }
  if (enemy_health_icon_) {
    enemy_health_icon_->Render(state.enemy->health);
  }
}

void BattleUI::RenderBorder() const {
  auto& config = core::GameConfig::Get();
  float t = config.window_width * 0.05f;
  glm::vec4 border_color = {0.3f, 0.3f, 0.3f, 1.0f};

  // Border should be below other UI elements but potentially above cards if
  // they fly out. Let's put it at a high Z but lower than buttons/health.
  const float border_z = 900.0f;

  auto& queue = engine::graphics::utils::RenderQueue::Default();

  // Top
  engine::graphics::utils::RenderCommand top;
  top.z_order = border_z;
  top.position = {0, config.window_height - t};
  top.size = {config.window_width, t};
  top.color = border_color;
  queue.Submit(top);

  // Bottom
  engine::graphics::utils::RenderCommand bottom;
  bottom.z_order = border_z;
  bottom.position = {0, 0};
  bottom.size = {config.window_width, t};
  bottom.color = border_color;
  queue.Submit(bottom);

  // Left
  engine::graphics::utils::RenderCommand left;
  left.z_order = border_z;
  left.position = {0, 0};
  left.size = {t, config.window_height};
  left.color = border_color;
  queue.Submit(left);

  // Right
  engine::graphics::utils::RenderCommand right;
  right.z_order = border_z;
  right.position = {config.window_width - t, 0};
  right.size = {t, config.window_height};
  right.color = border_color;
  queue.Submit(right);
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

  const float ui_z = 1000.0f;

  for (int i = 0; i < player.max_mana; ++i) {
    glm::vec2 offset = is_player ? glm::vec2(- (i + 1) * radius * 2.5f, 0.0f) : glm::vec2((i + 1) * radius * 2.5f, 0.0f);
    glm::vec2 pos = base_pos + offset;

    glm::vec4 c1 = color1;
    glm::vec4 c2 = color2;

    if (i >= player.mana) {
      c1.a = 0.3f;
      c2.a = 0.3f;
    }

    engine::graphics::utils::RenderCommand cmd;
    cmd.z_order = ui_z;
    cmd.shape_type = engine::graphics::utils::ShapeType::kCircle;
    cmd.position = pos;
    cmd.size = glm::vec2(radius); // For circles, size.x is radius
    cmd.color = c1;
    cmd.color2 = c2;
    cmd.gradient_type = 1; // Linear
    engine::graphics::utils::RenderQueue::Default().Submit(cmd);
  }

  // Draw mana text
  std::string mana_text = std::to_string(player.mana) + "/" + std::to_string(player.max_mana);
  glm::vec2 text_pos = base_pos + (is_player ? glm::vec2(-radius, radius * 1.5f) : glm::vec2(-radius, -radius * 2.5f));
  engine::graphics::Renderer::Get().DrawText("default", mana_text, text_pos, 0.0f, 0.5f, {1.0f, 1.0f, 1.0f, 1.0f}, ui_z + 0.1f);
}

}  // namespace core::graphics
