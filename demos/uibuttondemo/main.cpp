#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/util/logger.h>
#include <engine/input/input_manager.h>
#include <engine/graphics/renderer.h>
#include <engine/scene/scene_manager.h>
#include <engine/graphics/text_renderer.h>

#include "core/graphics/ui_button.h"
#include <vector>
#include <memory>

class UIButtonDemo : public engine::Application {
 public:
  void OnInit() override {
    LOG_INFO("UI Button Demo Initialized");

    engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);

    float window_width = 800.0f;
    float window_height = 600.0f;
    float btn_width = 200.0f;
    float btn_height = 50.0f;
    float spacing = 20.0f;
    float total_height = (btn_height * 3) + (spacing * 2);

    float start_x = (window_width - btn_width) * 0.5f;
    float start_y = (window_height - total_height) * 0.5f;

    buttons_.push_back(std::make_unique<core::graphics::UIButton>(
        "Print: Hello", glm::vec2{start_x, start_y + 2 * (btn_height + spacing)},
        glm::vec2{btn_width, btn_height},
        []() { LOG_INFO("Hello"); }));

    buttons_.push_back(std::make_unique<core::graphics::UIButton>(
        "Print: World", glm::vec2{start_x, start_y + 1 * (btn_height + spacing)},
        glm::vec2{btn_width, btn_height},
        []() { LOG_INFO("World"); }));

    buttons_.push_back(std::make_unique<core::graphics::UIButton>(
        "Print: Goodbye", glm::vec2{start_x, start_y},
        glm::vec2{btn_width, btn_height},
        []() { LOG_INFO("Goodbye"); }));
  }

  void OnUpdate(double delta_time_seconds) override {
    auto& input = engine::InputManager::Get();
    glm::vec2 pixel_mouse_pos = input.mouse_screen_pos();

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
