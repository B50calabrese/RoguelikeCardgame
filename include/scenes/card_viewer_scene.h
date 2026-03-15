#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_CARD_VIEWER_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_CARD_VIEWER_SCENE_H_

#include <vector>
#include <string>
#include <functional>
#include <glm/vec2.hpp>

#include "engine/scene/scene.h"
#include "core/card_data.h"

namespace scenes {

class CardViewerScene : public engine::Scene {
 public:
  CardViewerScene() : engine::Scene("CardViewerScene") {}

  void OnAttach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;
  bool OnInput() override;

 private:
  enum class SortOption { kAlphabetical, kCost };

  void SortCards();
  bool IsMouseOver(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& mouse_pos) const;

  std::vector<core::CardData> cards_;
  SortOption current_sort_ = SortOption::kAlphabetical;
  float scroll_offset_ = 0.0f;
  float max_scroll_ = 0.0f;
  bool is_dragging_scrollbar_ = false;
  int hovered_card_index_ = -1;
  int selected_card_index_ = -1;

  // Basic button properties
  struct Button {
      std::string label;
      glm::vec2 position;
      glm::vec2 size;
      std::function<void()> callback;
  };
  std::vector<Button> buttons_;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_CARD_VIEWER_SCENE_H_
