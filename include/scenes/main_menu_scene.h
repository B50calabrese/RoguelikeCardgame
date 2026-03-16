#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_MAIN_MENU_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_MAIN_MENU_SCENE_H_

#include <vector>
#include <string>
#include <memory>

#include "engine/scene/scene.h"
#include "core/graphics/ui_button.h"

namespace scenes {

class MainMenuScene : public engine::Scene {
 public:
  MainMenuScene() : engine::Scene("MainMenuScene") {}

  void OnAttach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;
  bool OnInput() override;

 private:
  void HandleInput();

  std::vector<std::unique_ptr<core::graphics::UIButton>> buttons_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_MAIN_MENU_SCENE_H_
