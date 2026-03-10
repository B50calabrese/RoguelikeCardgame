#include <iostream>
#include <memory>

#include "core/game_config.h"
#include "engine/core/application.h"
#include "engine/core/engine.h"
#include "engine/scene/scene_manager.h"
#include "scenes/combat_scene.h"

class DeckBuilderApp : public engine::Application {
 public:
  DeckBuilderApp() = default;

  void OnInit() override {
    // Push the first scene
    engine::SceneManager::Get().SetScene(
        std::make_unique<scenes::CombatScene>());
  }

  void OnUpdate(double delta_time_seconds) override {}

  void OnShutdown() override {
    std::cout << "Game cleaning up..." << std::endl;
  }
};

// --- Entry Point ---
int main() {
  auto& config = core::GameConfig::Get();
  config.SetAssetPath(ENGINE_ASSETS_PATH);
  config.SetWindowWidth(1600);
  config.SetWindowHeight(1200);

  engine::EngineConfig engine_config;
  engine_config.asset_path = config.GetAssetPath();
  engine_config.window_width = config.GetWindowWidth();
  engine_config.window_height = config.GetWindowHeight();

  engine::Engine::Init(engine_config);
  DeckBuilderApp app;
  app.Run();
  return 0;
}
