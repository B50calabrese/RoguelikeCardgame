#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_UI_BUTTON_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_UI_BUTTON_H_

#include <string>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace core::graphics {

/**
 * @brief A simple, reusable UI button.
 */
class UIButton {
 public:
  UIButton(const std::string& label, glm::vec2 position, glm::vec2 size,
           std::function<void()> callback);

  /**
   * @brief Updates the button state (hover, click).
   * @param mouse_pos Current mouse position in pixel coordinates.
   * @param is_clicked True if the mouse was clicked this frame.
   */
  void Update(glm::vec2 mouse_pos, bool is_clicked);

  /**
   * @brief Renders the button.
   */
  void Render() const;

  // Getters
  const glm::vec2& position() const { return position_; }
  const glm::vec2& size() const { return size_; }

 private:
  bool IsMouseOver(glm::vec2 mouse_pos) const;

  std::string label_;
  glm::vec2 position_;
  glm::vec2 size_;
  std::function<void()> callback_;

  bool is_hovered_ = false;
  glm::vec4 base_color_ = {0.3f, 0.3f, 0.3f, 1.0f};
  glm::vec4 hover_color_ = {0.4f, 0.4f, 0.4f, 1.0f};
  glm::vec4 text_color_ = {1.0f, 1.0f, 1.0f, 1.0f};
};

}  // namespace core::graphics

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_UI_BUTTON_H_
