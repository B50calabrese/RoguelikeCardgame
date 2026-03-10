#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/graphics/text_renderer.h>
#include <engine/graphics/texture.h>
#include <engine/input/input_manager.h>

#include <memory>

#include "core/card_data.h"
#include "core/graphics/card_renderer.h"

class CardRendererApp : public engine::Application {
 public:
  CardRendererApp() = default;  // Add a default constructor

  void OnInit() override {
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("arial", "arial.ttf", 16);
    texture = engine::graphics::Texture::Load("creature_frame.png");
    card_data = {.id = 1,
                 .name = "test",
                 .description = "description",
                 .power = 1,
                 .health = 1,
                 .frame_texture_id = texture->id(),
                 .art_texture_id = 1};
  }

  void OnUpdate(double deltaTimeSeconds) override {
    core::graphics::CardRenderer::RenderCard(card_data, position, scale,
                                             /*alpha=*/1.0f, rotation);

    // Scaling
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_UP)) {
      scale += 0.1f;
    } else if (engine::InputManager::Get().IsKeyPressed(
                   engine::KeyCode::KC_DOWN)) {
      scale -= 0.1f;
    }

    // Rotation
    if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_LEFT)) {
      rotation += 0.5f;
    } else if (engine::InputManager::Get().IsKeyDown(
                   engine::KeyCode::KC_RIGHT)) {
      rotation -= 0.5f;
    }

    // Position
    if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_W)) {
      position.y += 5.0f;
    } else if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_S)) {
      position.y -= 5.0f;
    }

    if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_A)) {
      position.x -= 5.0f;
    } else if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_D)) {
      position.x += 5.0f;
    }
  }

  void OnShutdown() override {}

 private:
  std::shared_ptr<engine::graphics::Texture> texture;
  core::CardData card_data;
  float scale = 1.0f;
  float rotation = 0.0f;
  glm::vec2 position = glm::vec2(800.0f, 600.0f);
};

// --- Entry Point ---
int main() {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_width = 1600;
  engine_config.window_height = 1200;
  engine::Engine::Init(engine_config);
  CardRendererApp app;
  app.Run();
  return 0;
}