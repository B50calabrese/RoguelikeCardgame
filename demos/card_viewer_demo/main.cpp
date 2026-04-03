#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include <engine/graphics/text_renderer.h>
#include "core/game_config.h"
#include "core/card_registry.h"
#include "scenes/card_viewer_scene.h"

class CardViewerDemo : public engine::Application {
 public:
  void OnInit() override {
    LOG_INFO("Card Viewer Demo Initialized");

    engine::graphics::TextRenderer::Get().LoadFont("arial", "arial.ttf", 24);
    engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);

    // Load cards from the assets/cards directory
    if (core::CardRegistry::Get().LoadCardsFromDirectory("cards", false)) {
      LOG_INFO("Successfully loaded cards from assets/cards/");
    } else {
      LOG_WARN("Some cards failed to load, or directory not found.");
    }

    engine::SceneManager::Get().SetScene(std::make_unique<scenes::CardViewerScene>());
  }

  void OnUpdate(double deltaTimeSeconds) override {}
  void OnShutdown() override {}
};

int main() {
  auto& config = core::GameConfig::Get();
  config.asset_path = ENGINE_ASSETS_PATH;
  config.window_width = 1600;
  config.window_height = 1200;

  engine::EngineConfig engine_config;
  engine_config.asset_path = config.asset_path;
  engine_config.window_width = config.window_width;
  engine_config.window_height = config.window_height;
  engine_config.window_title = "Card Viewer Demo";

  engine::Engine::Init(engine_config);

  CardViewerDemo app;
  app.Run();

  return 0;
}
