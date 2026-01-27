#include <application.h>
#include <engine.h>
#include <scene_manager.h>

#include <iostream>

#include "scenes/combat_scene.h"

class DeckBuilderApp : public engine::Application {
 public:
  DeckBuilderApp() = default;  // Add a default constructor

  void OnInit() override {
    // Push the first scene
    engine::SceneManager::Get().SetScene(
        std::make_unique<scenes::CombatScene>());
  }

  void OnUpdate(double deltaTimeSeconds) override {}

  void OnShutdown() override {
    std::cout << "Game cleaning up..." << std::endl;
  }
};

// --- Entry Point ---
int main() {
  engine::EngineConfig engine_config;
  engine_config.window_width = 1600;
  engine_config.window_height = 1200;
  engine::Engine::Init(engine_config);
  DeckBuilderApp app;
  app.Run();
  return 0;
}