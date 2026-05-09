#include "scenes/map_scene.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <iostream>
#include <random>

#include "core/constants.h"
#include "core/game_config.h"
#include "core/graphics/card_renderer.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/texture.h"
#include "engine/graphics/utils/render_queue.h"
#include "engine/input/input_manager.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/asset_manager.h"
#include "scenes/main_menu_scene.h"

namespace scenes {

void MapScene::OnAttach() { GenerateChoices(); }

void MapScene::GenerateChoices() {
  choices_.clear();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> num_choices_dist(2, 5);
  std::uniform_int_distribution<> scenario_dist(0, 3);

  int num_choices = num_choices_dist(gen);

  auto frame_tex = engine::util::AssetManager<engine::graphics::Texture>::Get(
      "gray_noncreature_frame.png");
  auto art_tex = engine::util::AssetManager<engine::graphics::Texture>::Get(
      "cards/art/placeholder.png");

  auto& config = core::GameConfig::Get();
  float total_width =
      (num_choices * core::graphics::kBaseCardWidth * kCardScale) +
      ((num_choices - 1) * kCardSpacing);
  float start_x = (config.window_width - total_width) / 2.0f +
                  (core::graphics::kBaseCardWidth * kCardScale / 2.0f);
  float center_y = config.window_height / 2.0f;

  for (int i = 0; i < num_choices; ++i) {
    ChoiceCard card;
    card.type = static_cast<ScenarioType>(scenario_dist(gen));

    switch (card.type) {
      case ScenarioType::Battle:
        card.name = "BATTLE";
        card.color = {0.7f, 0.1f, 0.1f, 1.0f};
        break;
      case ScenarioType::RandomEvent:
        card.name = "RANDOM EVENT";
        card.color = {0.5f, 0.0f, 0.5f, 1.0f};
        break;
      case ScenarioType::Shop:
        card.name = "SHOP";
        card.color = {1.0f, 0.84f, 0.0f, 1.0f};
        break;
      case ScenarioType::Treasure:
        card.name = "TREASURE";
        card.color = {0.0f, 1.0f, 1.0f, 1.0f};
        break;
    }

    card.position = {start_x + i * (core::graphics::kBaseCardWidth * kCardScale +
                                    kCardSpacing),
                     center_y};

    // Setup dummy CardData for CardRenderer
    card.dummy_data.name = card.name;
    card.dummy_data.type = core::CardType::Spell;
    card.dummy_data.description = "";
    card.dummy_data.cost = 0;
    if (frame_tex) card.dummy_data.frame_texture_id = frame_tex->renderer_id();
    if (art_tex) card.dummy_data.art_texture_id = art_tex->renderer_id();

    choices_.push_back(card);
  }
  is_fading_in_ = true;
  fade_timer_ = fade_duration_;
}

bool MapScene::IsMouseOverCard(const ChoiceCard& card,
                               glm::vec2 mouse_pos) const {
  float half_width = (core::graphics::kBaseCardWidth * kCardScale) / 2.0f;
  float half_height = (core::graphics::kBaseCardHeight * kCardScale) / 2.0f;

  return mouse_pos.x >= card.position.x - half_width &&
         mouse_pos.x <= card.position.x + half_width &&
         mouse_pos.y >= card.position.y - half_height &&
         mouse_pos.y <= card.position.y + half_height;
}

void MapScene::OnUpdate(float delta_time_seconds) {
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEscape)) {
    engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
    return;
  }

  // Handle fading
  if (is_fading_out_) {
    fade_timer_ -= delta_time_seconds;
    float alpha = std::max(0.0f, fade_timer_ / fade_duration_);
    for (auto& choice : choices_) {
      choice.alpha = alpha;
    }

    if (fade_timer_ <= 0.0f) {
      is_fading_out_ = false;
      GenerateChoices();
    }
    return;
  }

  if (is_fading_in_) {
    fade_timer_ -= delta_time_seconds;
    float alpha = 1.0f - std::max(0.0f, fade_timer_ / fade_duration_);
    for (auto& choice : choices_) {
      choice.alpha = alpha;
    }

    if (fade_timer_ <= 0.0f) {
      is_fading_in_ = false;
    }
  }

  // Interaction
  auto& input = engine::InputManager::Get();
  glm::vec2 mouse_pos = input.mouse_screen_pos();

  for (auto& choice : choices_) {
    choice.is_hovered = IsMouseOverCard(choice, mouse_pos);

    if (choice.is_hovered) {
      choice.shake_timer += delta_time_seconds;

      if (input.IsKeyPressed(engine::KeyCode::kMouseLeft)) {
        std::cout << "Selected Scenario: " << choice.name << std::endl;
        is_fading_out_ = true;
        fade_timer_ = fade_duration_;
        break;
      }
    } else {
      choice.shake_timer = 0.0f;
    }
  }
}

void MapScene::OnRender() {
  auto& renderer = engine::graphics::Renderer::Get();
  auto& config = core::GameConfig::Get();
  auto& queue = engine::graphics::utils::RenderQueue::Default();

  // 1. Draw Top Bar
  engine::graphics::utils::RenderCommand top_bar_cmd;
  top_bar_cmd.z_order = 1000.0f;
  top_bar_cmd.position = {config.window_width * 0.5f,
                          config.window_height - kTopBarHeight * 0.5f};
  top_bar_cmd.size = {config.window_width, kTopBarHeight};
  top_bar_cmd.color = {0.2f, 0.2f, 0.2f, 1.0f};
  top_bar_cmd.origin = {0.5f, 0.5f};
  queue.Submit(top_bar_cmd);

  // 2. Draw Choice Cards
  for (const auto& choice : choices_) {
    glm::vec2 render_pos = choice.position;

    // Apply shake effect if hovered
    if (choice.is_hovered) {
      render_pos.x +=
          std::sin(choice.shake_timer * kShakeSpeed) * kShakeIntensity;
      render_pos.y +=
          std::cos(choice.shake_timer * kShakeSpeed * 0.7f) * kShakeIntensity;
    }

    // Draw the card frame and text using CardRenderer
    core::graphics::CardRenderer::RenderCard(choice.dummy_data, render_pos,
                                             kCardScale, choice.alpha, 0.0f,
                                             500.0f);

    // Draw the colored square in the art area
    glm::vec2 art_pos = render_pos + core::graphics::kCardArtOffset * kCardScale;
    glm::vec2 art_size = core::graphics::kCardArtSize * kCardScale;

    engine::graphics::utils::RenderCommand art_bg_cmd;
    art_bg_cmd.z_order = 499.0f;  // Slightly behind the frame
    art_bg_cmd.position = art_pos;
    art_bg_cmd.size = art_size;
    art_bg_cmd.color = {choice.color.r, choice.color.g, choice.color.b,
                        choice.color.a * choice.alpha};
    art_bg_cmd.origin = {0.5f, 0.5f};
    queue.Submit(art_bg_cmd);
  }
}

}  // namespace scenes
