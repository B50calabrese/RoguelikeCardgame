#include <iostream>
#include <memory>

#include "engine/core/application.h"
#include "engine/core/engine.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/logger.h"
#include "engine/util/console.h"
#include "engine/graphics/text_renderer.h"
#include "scenes/battle_test_scene.h"

using namespace engine;

class BattleTestApp : public Application {
 public:
  void OnInit() override {
    // Initialize text rendering (required for console and cards)
    graphics::TextRenderer::Get().Init();
    graphics::TextRenderer::Get().LoadFont("arial", "arial.ttf", 24);
    graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 18);

    // Set the initial scene
    SceneManager::Get().SetScene(std::make_unique<scenes::BattleTestScene>());

    // Configure console
    util::Console::Get().SetToggleKey(KeyCode::kTilde);
    LOG_INFO("Battle Test Demo Initialized. Press ~ to open console.");
  }

  void OnShutdown() override {
    LOG_INFO("Battle Test Demo Shutting Down.");
  }

  void OnUpdate(double delta_time_seconds) override {
    // Application-level update if needed
  }
};

int main(int argc, char* argv[]) {
  EngineConfig config;
  config.window_width = 1280;
  config.window_height = 720;
  config.window_title = "Mage Tower - Battle Test Demo";
  config.asset_path = ENGINE_ASSETS_PATH;

  Engine::Init(config);

  BattleTestApp app;
  app.Run();

  Engine::Shutdown();

  return 0;
}
