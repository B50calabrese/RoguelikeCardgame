#include "scenes/new_run_scene.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <memory>
#include <vector>

#include "core/constants.h"
#include "core/character_config.h"
#include "core/game_config.h"
#include "engine/graphics/texture.h"
#include "engine/util/asset_manager.h"
#include "core/util/math_util.h"
#include "engine/graphics/renderer.h"
#include "engine/input/input_manager.h"
#include "engine/scene/scene_manager.h"
#include "scenes/main_menu_scene.h"

namespace scenes {

void NewRunScene::OnAttach() {
  auto& config = core::GameConfig::Get();
  float window_w = config.window_width;
  float window_h = config.window_height;
  float buffer = window_w * kNewRunBufferFactor;
  float drawable_w = window_w - 2.0f * buffer;

  // Initialize characters
  struct CharInitData {
    std::string name;
    std::string texture;
  };
  std::vector<CharInitData> char_inits = {
      {"Wizard", "art/characters/wizard.png"},
      {"Warrior", "art/characters/warrior.png"},
      {"Rogue", "art/characters/rogue.png"}};

  float char_y = window_h * kNewRunCharacterYFactor;
  float char_spacing = drawable_w / (char_inits.size() - 1);

  for (size_t i = 0; i < char_inits.size(); ++i) {
    float x = buffer + i * char_spacing;
    auto tex = engine::util::AssetManager<engine::graphics::Texture>::Get(
        char_inits[i].texture);
    unsigned int tex_id = tex ? tex->renderer_id() : 0;
    characters_.push_back(
        {char_inits[i].name, char_inits[i].texture, tex_id, {x, char_y}});
  }

  // Initialize colors
  struct ColorPair {
    glm::vec4 base;
    glm::vec4 highlight;
  };
  std::vector<ColorPair> gameplay_colors = {
      {core::graphics::kColorWhite, core::graphics::kColorWhiteHighlight},
      {core::graphics::kColorBlue, core::graphics::kColorBlueHighlight},
      {core::graphics::kColorBlack, core::graphics::kColorBlackHighlight},
      {core::graphics::kColorRed, core::graphics::kColorRedHighlight},
      {core::graphics::kColorGreen, core::graphics::kColorGreenHighlight}};

  float color_y = window_h * kNewRunColorYFactor;
  float color_spacing = drawable_w / (gameplay_colors.size() - 1);

  for (size_t i = 0; i < gameplay_colors.size(); ++i) {
    float x = buffer + i * color_spacing;
    colors_.push_back(
        {gameplay_colors[i].base, gameplay_colors[i].highlight, {x, color_y}});
  }

  // Initialize buttons
  float btn_width = 150.0f;
  float btn_height = 50.0f;
  float margin = 20.0f;

  buttons_.push_back(std::make_unique<core::graphics::UIButton>(
      "Back", glm::vec2{margin, window_h - btn_height - margin},
      glm::vec2{btn_width, btn_height}, []() {
        engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
      }));

  start_button_ = std::make_unique<core::graphics::UIButton>(
      "Start Run", glm::vec2{window_w * 0.5f - 100.0f, 50.0f},
      glm::vec2{200.0f, 60.0f}, []() {
        GLFWwindow* window = glfwGetCurrentContext();
        if (window) {
          glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
      });
}

void NewRunScene::OnUpdate(float delta_time_seconds) { HandleInput(); }

void NewRunScene::HandleInput() {
  auto& input = engine::InputManager::Get();
  auto mouse_pos = input.mouse_screen_pos();
  auto& config = core::GameConfig::Get();

  float mx = (mouse_pos.x + 1.0f) * 0.5f * config.window_width;
  float my = (mouse_pos.y + 1.0f) * 0.5f * config.window_height;
  glm::vec2 pixel_mouse_pos = {mx, my};

  bool clicked = input.IsKeyPressed(engine::KeyCode::kMouseLeft);

  // Update fixed buttons
  for (auto& btn : buttons_) {
    btn->Update(pixel_mouse_pos, clicked);
  }

  // Update start button if active
  bool selections_complete =
      selected_character_index_ != -1 && selected_color_indices_.size() == 2;
  if (selections_complete) {
    start_button_->Update(pixel_mouse_pos, clicked);
  }

  if (clicked) {
    // Check characters
    for (int i = 0; i < static_cast<int>(characters_.size()); ++i) {
      if (core::util::PointInRect(pixel_mouse_pos, characters_[i].pos,
                                  kNewRunCharacterSize, true)) {
        selected_character_index_ = i;
        break;
      }
    }

    // Check colors
    for (int i = 0; i < static_cast<int>(colors_.size()); ++i) {
      if (core::util::PointInRect(pixel_mouse_pos, colors_[i].pos,
                                  kNewRunColorSize, true)) {
        auto it = std::find(selected_color_indices_.begin(),
                           selected_color_indices_.end(), i);
        if (it != selected_color_indices_.end()) {
          // Deselect if already selected
          selected_color_indices_.erase(it);
        } else {
          // Select new color
          if (selected_color_indices_.size() == 2) {
            selected_color_indices_.erase(selected_color_indices_.begin());
          }
          selected_color_indices_.push_back(i);
        }
        break;
      }
    }
  }
}

void NewRunScene::OnRender() {
  auto& renderer = engine::graphics::Renderer::Get();
  auto& config = core::GameConfig::Get();

  renderer.DrawText("default", "NEW RUN",
                    {config.window_width * 0.5f - 100.0f,
                     config.window_height * 0.9f},
                    0.0f, 1.0f, {1, 1, 1, 1});

  // Render Characters
  for (int i = 0; i < static_cast<int>(characters_.size()); ++i) {
    if (characters_[i].texture_id != 0) {
      renderer.DrawTexturedQuad(characters_[i].pos, kNewRunCharacterSize,
                                characters_[i].texture_id, 0.0f, {1, 1, 1, 1},
                                {0.5f, 0.5f});
    } else {
      renderer.DrawQuad(characters_[i].pos, kNewRunCharacterSize,
                        {0.5f, 0.5f, 0.5f, 1.0f}, 0.0f, {0.5f, 0.5f});
    }

    if (i == selected_character_index_) {
      float outline_thickness = 4.0f;
      glm::vec2 outline_size =
          kNewRunCharacterSize + glm::vec2(outline_thickness * 2.0f);
      renderer.DrawQuad(characters_[i].pos, outline_size, {1, 1, 1, 1}, 0.0f,
                        {0.5f, 0.5f});
      // Redraw portrait (or fallback) on top
      if (characters_[i].texture_id != 0) {
        renderer.DrawTexturedQuad(characters_[i].pos, kNewRunCharacterSize,
                                  characters_[i].texture_id, 0.0f, {1, 1, 1, 1},
                                  {0.5f, 0.5f});
      } else {
        renderer.DrawQuad(characters_[i].pos, kNewRunCharacterSize,
                          {0.5f, 0.5f, 0.5f, 1.0f}, 0.0f, {0.5f, 0.5f});
      }
    }

    renderer.DrawText("default", characters_[i].name,
                      characters_[i].pos - glm::vec2(40.0f, 80.0f), 0.0f, 0.6f,
                      {1, 1, 1, 1});
  }

  // Render Colors
  for (int i = 0; i < static_cast<int>(colors_.size()); ++i) {
    bool selected = std::find(selected_color_indices_.begin(),
                             selected_color_indices_.end(),
                             i) != selected_color_indices_.end();
    glm::vec4 color = selected ? colors_[i].highlight_color : colors_[i].color;
    renderer.DrawQuad(colors_[i].pos, kNewRunColorSize,
                      color, 0.0f, {0.5f, 0.5f});
  }

  // Render Buttons
  for (const auto& btn : buttons_) {
    btn->Render();
  }

  bool selections_complete =
      selected_character_index_ != -1 && selected_color_indices_.size() == 2;
  if (selections_complete) {
    start_button_->Render();
  }
}

bool NewRunScene::OnInput() { return false; }

}  // namespace scenes
