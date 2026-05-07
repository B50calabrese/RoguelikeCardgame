#include "scenes/map_scene.h"

#include <GLFW/glfw3.h>
#include "core/game_config.h"
#include "engine/graphics/renderer.h"
#include "engine/scene/scene_manager.h"
#include "engine/input/input_manager.h"
#include "scenes/main_menu_scene.h"

namespace scenes {

void MapScene::OnAttach() {}

void MapScene::OnUpdate(float delta_time_seconds) {
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEscape)) {
    engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
  }
}

void MapScene::OnRender() {
  auto& renderer = engine::graphics::Renderer::Get();
  auto& config = core::GameConfig::Get();

  renderer.DrawText(
      "default", "MAP SCENE (Placeholder)",
      {config.window_width * 0.5f - 150.0f, config.window_height * 0.5f}, 0.0f,
      1.5f, {1, 1, 1, 1});

  renderer.DrawText(
      "default", "Press ESC to return to Main Menu",
      {config.window_width * 0.5f - 180.0f, config.window_height * 0.4f}, 0.0f,
      1.0f, {0.8f, 0.8f, 0.8f, 1.0f});
}

}  // namespace scenes
