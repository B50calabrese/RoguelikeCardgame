#include <memory>

#include "core/game_config.h"
#include "engine/core/application.h"
#include "engine/core/engine.h"
#include "engine/graphics/text_renderer.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/logger.h"
#include "scenes/main_menu_scene.h"

class DeckBuilderApp : public engine::Application {
 public:
  DeckBuilderApp() = default;

  void OnInit() override {
    // Initialize text rendering
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("arial", "arial.ttf", 24);
    engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 18);

    // Push the first scene
    engine::SceneManager::Get().SetScene(
        std::make_unique<scenes::MainMenuScene>());
  }

  void OnUpdate(double delta_time_seconds) override {}

  void OnShutdown() override {
    LOG_INFO("Game cleaning up...");
  }
};

// --- Entry Point ---
int main() {
  auto& config = core::GameConfig::Get();
  config.asset_path = ENGINE_ASSETS_PATH;
  config.window_width = 1600;
  config.window_height = 1200;

  engine::EngineConfig engine_config;
  engine_config.asset_path = config.asset_path;
  engine_config.window_width = config.window_width;
  engine_config.window_height = config.window_height;

  engine::Engine::Init(engine_config);
  DeckBuilderApp app;
  app.Run();
  return 0;
}
