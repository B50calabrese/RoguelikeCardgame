#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_NEW_RUN_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_NEW_RUN_SCENE_H_

#include <vector>
#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "engine/scene/scene.h"
#include "core/graphics/ui_button.h"

namespace scenes {

struct CharacterData {
  glm::vec4 color;
  glm::vec2 pos;
};

struct ColorData {
  glm::vec4 color;
  glm::vec4 highlight_color;
  glm::vec2 pos;
};

class NewRunScene : public engine::Scene {
 public:
  NewRunScene() : engine::Scene("NewRunScene") {}
  ~NewRunScene() override = default;

  void OnAttach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;
  bool OnInput() override;

 private:
  void HandleInput();

  // UI Constants
  const float kBufferFactor = 0.1f;
  const float kCharacterYFactor = 0.75f;
  const float kColorYFactor = 0.25f;
  const glm::vec2 kCharacterSize = {120.0f, 120.0f};
  const glm::vec2 kColorSize = {80.0f, 80.0f};

  std::vector<CharacterData> characters_;
  std::vector<ColorData> colors_;
  std::vector<std::unique_ptr<core::graphics::UIButton>> buttons_;

  int selected_character_index_ = -1;
  std::vector<int> selected_color_indices_;

  std::unique_ptr<core::graphics::UIButton> start_button_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_NEW_RUN_SCENE_H_
