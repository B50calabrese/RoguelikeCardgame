#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_MAIN_MENU_SCENE_H_
#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_MAIN_MENU_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_MAIN_MENU_SCENE_H_

#include <vector>
#include <string>
#include <functional>
#include <glm/vec2.hpp>

#include "engine/scene/scene.h"

namespace scenes {

class MainMenuScene : public engine::Scene {
 public:
  MainMenuScene() : engine::Scene("MainMenuScene") {}

  void OnAttach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;
  bool OnInput() override;

 private:
  struct Button {
      std::string label;
      glm::vec2 position;
      glm::vec2 size;
      std::function<void()> callback;
  };
  std::vector<Button> buttons_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_MAIN_MENU_SCENE_H_
#endif
