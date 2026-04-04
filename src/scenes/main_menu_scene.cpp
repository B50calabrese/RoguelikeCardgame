#include "scenes/main_menu_scene.h"

#include <memory>

#include "core/game_config.h"
#include "engine/graphics/renderer.h"
#include "engine/input/input_manager.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/logger.h"
#include "scenes/new_run_scene.h"
#include "scenes/card_viewer_scene.h"
#include "scenes/effect_demo_scene.h"

namespace scenes {

void MainMenuScene::OnAttach() {
  auto& config = core::GameConfig::Get();
  float btn_width = 300.0f;
  float btn_height = 60.0f;
  float center_x = config.window_width * 0.5f;
  float start_y = config.window_height * 0.7f;

  buttons_.push_back(std::make_unique<core::graphics::UIButton>(
      "New Run", glm::vec2{center_x - btn_width * 0.5f, start_y},
      glm::vec2{btn_width, btn_height}, []() {
        engine::SceneManager::Get().SetScene(std::make_unique<NewRunScene>());
      }));

  buttons_.push_back(std::make_unique<core::graphics::UIButton>(
      "Effect Demo", glm::vec2{center_x - btn_width * 0.5f, start_y - 80.0f},
      glm::vec2{btn_width, btn_height}, []() {
        engine::SceneManager::Get().SetScene(std::make_unique<EffectDemoScene>());
      }));

  buttons_.push_back(std::make_unique<core::graphics::UIButton>(
      "Card Viewer", glm::vec2{center_x - btn_width * 0.5f, start_y - 160.0f},
      glm::vec2{btn_width, btn_height}, []() {
        engine::SceneManager::Get().SetScene(std::make_unique<CardViewerScene>());
      }));

  buttons_.push_back(std::make_unique<core::graphics::UIButton>(
      "Exit", glm::vec2{center_x - btn_width * 0.5f, start_y - 240.0f},
      glm::vec2{btn_width, btn_height}, []() {
        LOG_INFO("Exit requested");
      }));
}

void MainMenuScene::OnUpdate(float delta_time_seconds) {
  HandleInput();
}

void MainMenuScene::HandleInput() {
  auto& input = engine::InputManager::Get();
  glm::vec2 pixel_mouse_pos = input.mouse_screen_pos();

  bool clicked = input.IsKeyPressed(engine::KeyCode::kMouseLeft);
  for (auto& btn : buttons_) {
    btn->Update(pixel_mouse_pos, clicked);
  }
}

void MainMenuScene::OnRender() {
  auto& renderer = engine::graphics::Renderer::Get();
  auto& config = core::GameConfig::Get();

  renderer.DrawText("default", "DECK BUILDER GAME",
                    {config.window_width * 0.5f - 200.0f,
                     config.window_height * 0.8f},
                    0.0f, 1.0f, {1, 1, 1, 1});

  for (const auto& btn : buttons_) {
    btn->Render();
  }
}

bool MainMenuScene::OnInput() { return false; }

}  // namespace scenes
