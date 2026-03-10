#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/graphics/text_renderer.h>
#include <engine/graphics/texture.h>
#include <engine/input/input_manager.h>

#include <memory>
#include <vector>

#include "core/card_data.h"
#include "core/graphics/hand_renderer.h"

class HandRendererApp : public engine::Application {
 public:
  HandRendererApp() = default;

  void OnInit() override {
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("arial", "arial.ttf", 16);
    texture = engine::graphics::Texture::Load("creature_frame.png");

    for (int i = 0; i < 7; ++i) {
      cards.push_back({
          .id = i,
          .name = "Card " + std::to_string(i),
          .description = "A demo card",
          .power = i,
          .health = i,
          .frame_texture_id = texture->id(),
          .art_texture_id = 1
      });
    }
  }

  void OnUpdate(double deltaTimeSeconds) override {
    // Background UI for visualization
    engine::graphics::Renderer::Get().DrawRect(bounds_pos.x, bounds_pos.y, bounds_size.x, bounds_size.y, 0.2f, 0.2f, 0.2f);

    core::graphics::HandRenderer::RenderHand(cards, bounds_pos, bounds_size, arc_angle, overlap);

    // Controls
    if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_UP)) {
      arc_angle += 10.0f * (float)deltaTimeSeconds;
    } else if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_DOWN)) {
      arc_angle -= 10.0f * (float)deltaTimeSeconds;
    }

    if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_RIGHT)) {
      overlap = std::min(1.0f, overlap + 0.1f * (float)deltaTimeSeconds);
    } else if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_LEFT)) {
      overlap = std::max(0.0f, overlap - 0.1f * (float)deltaTimeSeconds);
    }

    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_EQUALS)) { // Plus key
       cards.push_back({
          .id = (int)cards.size(),
          .name = "Card " + std::to_string(cards.size()),
          .frame_texture_id = texture->id(),
      });
    } else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_MINUS)) {
      if (!cards.empty()) cards.pop_back();
    }
  }

  void OnShutdown() override {}

 private:
  std::shared_ptr<engine::graphics::Texture> texture;
  std::vector<core::CardData> cards;

  glm::vec2 bounds_pos = glm::vec2(400.0f, 100.0f);
  glm::vec2 bounds_size = glm::vec2(800.0f, 400.0f);
  float arc_angle = 30.0f;
  float overlap = 0.3f;
};

int main() {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_width = 1600;
  engine_config.window_height = 1200;
  engine::Engine::Init(engine_config);
  HandRendererApp app;
  app.Run();
  return 0;
}
