#include "scenes/card_viewer_scene.h"

#include <algorithm>
#include "engine/graphics/post_processor.h"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

#include "core/card_registry.h"
#include "core/constants.h"
#include "core/game_config.h"
#include "core/graphics/card_renderer.h"
#include "core/util/math_util.h"
#include "engine/graphics/renderer.h"
#include "engine/input/input_manager.h"
#include "engine/scene/scene_manager.h"
#include "scenes/main_menu_scene.h"

namespace scenes {

void CardViewerScene::OnAttach() {
  const auto& all_cards = core::CardRegistry::Get().GetAllCards();
  for (const auto& [id, card] : all_cards) {
    cards_.push_back(card);
  }
  SortCards();

  auto& config = core::GameConfig::Get();
  float btn_width = 150.0f;
  float btn_height = 50.0f;
  float margin = 20.0f;

  buttons_.push_back(std::make_unique<core::graphics::UIButton>(
      "Sort: Name",
      glm::vec2{margin, config.window_height - btn_height - margin},
      glm::vec2{btn_width, btn_height}, [this]() {
        current_sort_ = SortOption::kAlphabetical;
        SortCards();
      }));

  buttons_.push_back(std::make_unique<core::graphics::UIButton>(
      "Sort: Cost",
      glm::vec2{margin + btn_width + margin,
                config.window_height - btn_height - margin},
      glm::vec2{btn_width, btn_height}, [this]() {
        current_sort_ = SortOption::kCost;
        SortCards();
      }));

  buttons_.push_back(std::make_unique<core::graphics::UIButton>(
      "Back",
      glm::vec2{config.window_width - btn_width - margin,
                config.window_height - btn_height - margin},
      glm::vec2{btn_width, btn_height}, []() {
        engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
      }));
}

void CardViewerScene::SortCards() {
  if (current_sort_ == SortOption::kAlphabetical) {
    std::sort(cards_.begin(), cards_.end(),
              [](const core::CardData& a, const core::CardData& b) {
                return a.name < b.name;
              });
  } else {
    std::sort(cards_.begin(), cards_.end(),
              [](const core::CardData& a, const core::CardData& b) {
                if (a.cost != b.cost) return a.cost < b.cost;
                return a.name < b.name;
              });
  }
}

void CardViewerScene::OnUpdate(float delta_time_seconds) {
  HandleInput(delta_time_seconds);
}

void CardViewerScene::HandleInput(float delta_time_seconds) {
  auto& input = engine::InputManager::Get();
  glm::vec2 pixel_mouse_pos = input.mouse_screen_pos();
  auto& config = core::GameConfig::Get();

  if (selected_card_index_ != -1) {
    if (input.IsKeyPressed(engine::KeyCode::kMouseLeft) ||
        input.IsKeyPressed(engine::KeyCode::kEscape)) {
      selected_card_index_ = -1;
    }
    return;
  }

  // Calculate scroll limits
  int cols = config.card_viewer_columns;
  float card_h = core::graphics::kBaseCardHeight;
  float padding = 40.0f;
  int rows = static_cast<int>((cards_.size() + cols - 1) / cols);
  float total_content_height = rows * (card_h + padding) + 200.0f;
  max_scroll_ = std::max(0.0f, total_content_height - config.window_height);

  // Scrollbar interaction
  float sb_width = 20.0f;
  float sb_x = config.window_width - sb_width - 5.0f;
  float sb_y = 100.0f;
  float sb_height = config.window_height - 200.0f;

  if (input.IsKeyPressed(engine::KeyCode::kMouseLeft)) {
    if (core::util::PointInRect(pixel_mouse_pos, {sb_x, sb_y},
                                {sb_width, sb_height}, false)) {
      is_dragging_scrollbar_ = true;
    }
  }
  if (input.IsKeyReleased(engine::KeyCode::kMouseLeft)) {
    is_dragging_scrollbar_ = false;
  }

  if (is_dragging_scrollbar_) {
    float t = (pixel_mouse_pos.y - sb_y) / sb_height;
    t = 1.0f - glm::clamp(t, 0.0f, 1.0f);
    scroll_offset_ = t * max_scroll_;
  }

  // Update buttons
  bool clicked = input.IsKeyPressed(engine::KeyCode::kMouseLeft);
  if (selected_card_index_ == -1) {
    for (auto& btn : buttons_) {
      btn->Update(pixel_mouse_pos, clicked);
    }
  }

  // Card hover and selection
  if (selected_card_index_ != -1) return;

  hovered_card_index_ = -1;
  float card_w = core::graphics::kBaseCardWidth;
  float start_x =
      (config.window_width - (cols * card_w + (cols - 1) * padding)) * 0.5f;
  float start_y = config.window_height - 150.0f + scroll_offset_;

  int hovered_i = -1;
  glm::vec2 hovered_pos;

  for (int i = 0; i < cards_.size(); ++i) {
    int row = i / cols;
    int col = i % cols;
    glm::vec2 pos = {start_x + col * (card_w + padding) + card_w * 0.5f,
                     start_y - row * (card_h + padding) - card_h * 0.5f};

    if (IsMouseOverCard(pos, {card_w, card_h}, pixel_mouse_pos)) {
      hovered_card_index_ = i;
      break;
    }
  }

  if (clicked && !is_dragging_scrollbar_ && hovered_card_index_ != -1) {
    bool button_clicked = false;
    for (const auto& btn : buttons_) {
      if (core::util::PointInRect(pixel_mouse_pos, btn->position(), btn->size(),
                                  false)) {
        button_clicked = true;
        break;
      }
    }
    if (!button_clicked) {
      selected_card_index_ = hovered_card_index_;
    }
  }

  // Keyboard scrolling
  if (input.IsKeyDown(engine::KeyCode::kUp))
    scroll_offset_ -= 1000.0f * delta_time_seconds;
  if (input.IsKeyDown(engine::KeyCode::kDown))
    scroll_offset_ += 1000.0f * delta_time_seconds;
  if (input.IsKeyPressed(engine::KeyCode::kPageUp))
    scroll_offset_ -= config.window_height * 0.5f;
  if (input.IsKeyPressed(engine::KeyCode::kPageDown))
    scroll_offset_ += config.window_height * 0.5f;
  scroll_offset_ = glm::clamp(scroll_offset_, 0.0f, max_scroll_);
}

bool CardViewerScene::IsMouseOverCard(const glm::vec2& pos,
                                      const glm::vec2& size,
                                      const glm::vec2& mouse_pos) const {
  return core::util::PointInRect(mouse_pos, pos, size, true);
}

void CardViewerScene::OnRender() {
  RenderGrid();
  RenderUI();
  RenderFullscreenOverlay();
}

void CardViewerScene::RenderGrid() {
  auto& config = core::GameConfig::Get();
  int cols = config.card_viewer_columns;
  float card_w = core::graphics::kBaseCardWidth;
  float card_h = core::graphics::kBaseCardHeight;
  float padding = 40.0f;
  float start_x =
      (config.window_width - (cols * card_w + (cols - 1) * padding)) * 0.5f;
  float start_y = config.window_height - 150.0f + scroll_offset_;

  int hovered_i = -1;
  glm::vec2 hovered_pos;

  for (int i = 0; i < (int)cards_.size(); ++i) {
    int row = i / cols;
    int col = i % cols;
    glm::vec2 pos = {start_x + col * (card_w + padding) + card_w * 0.5f,
                     start_y - row * (card_h + padding) - card_h * 0.5f};

    if (pos.y + card_h < 0 || pos.y - card_h > config.window_height) continue;

    if (i == hovered_card_index_ && selected_card_index_ == -1) {
      hovered_i = i;
      hovered_pos = pos;
    } else {
      core::graphics::CardRenderer::RenderCard(cards_[i], pos, 1.0f);
    }
  }

  // Render hovered card last so it's on top
  if (hovered_i != -1) {
    core::graphics::CardRenderer::RenderCard(cards_[hovered_i], hovered_pos,
                                             config.card_viewer_hover_zoom);
  }
}

void CardViewerScene::RenderUI() {
  auto& renderer = engine::graphics::Renderer::Get();
  auto& config = core::GameConfig::Get();

  // Render Scrollbar
  float sb_width = 20.0f;
  float sb_x = config.window_width - sb_width - 5.0f;
  float sb_y = 100.0f;
  float sb_height = config.window_height - 200.0f;
  renderer.DrawRect(sb_x, sb_y, sb_width, sb_height, 0.2f, 0.2f, 0.2f);
  if (max_scroll_ > 0) {
    float handle_height =
        std::max(20.0f, sb_height * (sb_height / (sb_height + max_scroll_)));
    float handle_y = sb_y + (1.0f - (scroll_offset_ / max_scroll_)) *
                                (sb_height - handle_height);
    renderer.DrawRect(sb_x, handle_y, sb_width, handle_height, 0.5f, 0.5f,
                      0.5f);
  }

  // Render Buttons
  for (const auto& btn : buttons_) {
    btn->Render();
  }
}

void CardViewerScene::RenderFullscreenOverlay() {
  if (selected_card_index_ == -1) return;

  auto& renderer = engine::graphics::Renderer::Get();
  auto& config = core::GameConfig::Get();

  renderer.DrawQuad({config.window_width * 0.5f, config.window_height * 0.5f},
                    {config.window_width, config.window_height},
                    {0.0f, 0.0f, 0.0f, 0.9f}, 0.0f, {0.5f, 0.5f});

  core::graphics::CardRenderer::RenderCard(
      cards_[selected_card_index_],
      {config.window_width * 0.5f, config.window_height * 0.5f},
      config.card_viewer_fullscreen_zoom);
}

bool CardViewerScene::OnInput() { return false; }

}  // namespace scenes
