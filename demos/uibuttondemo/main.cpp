#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/util/logger.h>
#include <engine/input/input_manager.h>
#include <engine/graphics/renderer.h>
#include <engine/scene/scene_manager.h>

#include "core/graphics/ui_button.h"
#include <vector>
#include <memory>

class UIButtonDemo : public engine::Application {
 public:
  void OnInit() override {
    LOG_INFO("UI Button Demo Initialized");

    buttons_.push_back(std::make_unique<core::graphics::UIButton>(
        "Button 1", glm::vec2{100, 400}, glm::vec2{200, 50},
        []() { LOG_INFO("Button 1 Clicked!"); }));

    buttons_.push_back(std::make_unique<core::graphics::UIButton>(
        "Button 2", glm::vec2{100, 300}, glm::vec2{200, 50},
        []() { LOG_INFO("Button 2 Clicked!"); }));

    buttons_.push_back(std::make_unique<core::graphics::UIButton>(
        "Quit", glm::vec2{100, 200}, glm::vec2{200, 50},
        [this]() {
            LOG_INFO("Quit Clicked!");
        }));
  }

  void OnUpdate(double delta_time_seconds) override {
    auto& input = engine::InputManager::Get();
    auto mouse_pos = input.mouse_screen_pos();

    // Normalize to pixel coordinates for 800x600 window
    float mx = (mouse_pos.x + 1.0f) * 0.5f * 800.0f;
    float my = (mouse_pos.y + 1.0f) * 0.5f * 600.0f;
    glm::vec2 pixel_mouse_pos = {mx, my};

    bool clicked = input.IsKeyPressed(engine::KeyCode::KC_MOUSE_LEFT);
    for (auto& btn : buttons_) {
      btn->Update(pixel_mouse_pos, clicked);
    }

    // Render in OnUpdate because Application::Run calls OnUpdate every frame
    // and there is no OnRender in Application.
    auto& renderer = engine::graphics::Renderer::Get();
    for (const auto& btn : buttons_) {
      btn->Render();
    }
  }

  void OnShutdown() override {}

 private:
  std::vector<std::unique_ptr<core::graphics::UIButton>> buttons_;
};

int main() {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_width = 800;
  engine_config.window_height = 600;
  engine_config.window_title = "UI Button Demo";

  engine::Engine::Init(engine_config);

  UIButtonDemo app;
  app.Run();

  return 0;
}
