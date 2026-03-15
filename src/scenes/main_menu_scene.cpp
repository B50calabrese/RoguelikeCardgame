#include "scenes/main_menu_scene.h"

#include <iostream>

#include "core/game_config.h"
#include "engine/graphics/renderer.h"
#include "engine/input/input_manager.h"
#include "engine/scene/scene_manager.h"
#include "scenes/combat_scene.h"
#include "scenes/card_viewer_scene.h"

namespace scenes {

void MainMenuScene::OnAttach() {
  auto& config = core::GameConfig::Get();
  float btn_width = 300.0f;
  float btn_height = 60.0f;
  float center_x = config.window_width * 0.5f;
  float start_y = config.window_height * 0.6f;

  buttons_.push_back({"Start Combat", {center_x - btn_width * 0.5f, start_y}, {btn_width, btn_height}, []() {
    engine::SceneManager::Get().SetScene(std::make_unique<CombatScene>());
  }});

  buttons_.push_back({"Card Viewer", {center_x - btn_width * 0.5f, start_y - 100.0f}, {btn_width, btn_height}, []() {
    engine::SceneManager::Get().SetScene(std::make_unique<CardViewerScene>());
  }});

  buttons_.push_back({"Exit", {center_x - btn_width * 0.5f, start_y - 200.0f}, {btn_width, btn_height}, []() {
    // Exit logic? For now just log.
    std::cout << "Exit requested" << std::endl;
  }});
}

void MainMenuScene::OnUpdate(float delta_time_seconds) {
  auto& input = engine::InputManager::Get();
  auto mouse_pos = input.mouse_screen_pos();
  auto& config = core::GameConfig::Get();

  float mx = (mouse_pos.x + 1.0f) * 0.5f * config.window_width;
  float my = (mouse_pos.y + 1.0f) * 0.5f * config.window_height;
  glm::vec2 pixel_mouse_pos = {mx, my};

  if (input.IsKeyPressed(engine::KeyCode::KC_MOUSE_LEFT)) {
    for (const auto& btn : buttons_) {
      if (pixel_mouse_pos.x >= btn.position.x && pixel_mouse_pos.x <= btn.position.x + btn.size.x &&
          pixel_mouse_pos.y >= btn.position.y && pixel_mouse_pos.y <= btn.position.y + btn.size.y) {
        btn.callback();
        return;
      }
    }
  }
}

void MainMenuScene::OnRender() {
  auto& renderer = engine::graphics::Renderer::Get();
  auto& config = core::GameConfig::Get();

  renderer.DrawText("default", "DECK BUILDER GAME", {config.window_width * 0.5f - 200.0f, config.window_height * 0.8f}, 0.0f, 1.0f, {1,1,1,1});

  for (const auto& btn : buttons_) {
    renderer.DrawRect(btn.position.x, btn.position.y, btn.size.x, btn.size.y, 0.3f, 0.3f, 0.3f);
    renderer.DrawText("default", btn.label, btn.position + glm::vec2(20.0f, 20.0f), 0.0f, 0.6f, {1,1,1,1});
  }
}

bool MainMenuScene::OnInput() {
  return false;
}

}  // namespace scenes
