#include "core/graphics/ui_button.h"

#include "core/util/math_util.h"
#include "engine/graphics/renderer.h"

namespace core::graphics {

UIButton::UIButton(const std::string& label, glm::vec2 position, glm::vec2 size,
                   std::function<void()> callback)
    : label_(label), position_(position), size_(size), callback_(callback) {}

void UIButton::Update(glm::vec2 mouse_pos, bool is_clicked) {
  is_hovered_ = core::util::PointInRect(mouse_pos, position_, size_, false);
  if (is_hovered_ && is_clicked) {
    callback_();
  }
}

void UIButton::Render() const {
  auto& renderer = engine::graphics::Renderer::Get();
  glm::vec4 color = is_hovered_ ? hover_color_ : base_color_;

  // Use a high Z-index for UI to ensure it's on top of game elements.
  const float ui_z = 1000.0f;

  renderer.DrawRect(position_.x, position_.y, size_.x, size_.y, color.r, color.g,
                    color.b, ui_z);

  // Center text roughly (this engine doesn't seem to have text bounds yet)
  glm::vec2 text_pos = position_ + glm::vec2(10.0f, size_.y * 0.35f);
  renderer.DrawText("default", label_, text_pos, 0.0f, 0.6f, text_color_,
                    ui_z + 0.1f);
}

bool UIButton::IsMouseOver(glm::vec2 mouse_pos) const {
  return core::util::PointInRect(mouse_pos, position_, size_, false);
}

}  // namespace core::graphics
