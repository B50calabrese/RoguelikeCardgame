#include "core/graphics/health_icon.h"
#include "engine/graphics/renderer.h"
#include <string>

namespace core::graphics {

HealthIcon::HealthIcon(const glm::vec2& position, float size, const glm::vec4& color)
    : position_(position), size_(size), color_(color) {}

void HealthIcon::Render(int health) const {
  auto& renderer = engine::graphics::Renderer::Get();

  // Render the colored square centered at position_
  // DrawQuad uses the origin for rotation and positioning relative to size.
  // Origin (0.5, 0.5) centers the quad on position_.
  renderer.DrawQuad(position_, {size_, size_}, color_, 0.0f, {0.5f, 0.5f});

  // Render the health text
  std::string health_text = std::to_string(health);

  // Calculate text position. TextRenderer/Renderer::DrawText is baseline-based.
  // To center it, we'd ideally need text bounds, but for now we'll offset it.
  // Assuming scale 1.0 is roughly 32px high.
  float text_scale = size_ / 64.0f;
  // Very rough centering offset - in a real engine we'd use GetTextBounds
  glm::vec2 text_pos = position_ + glm::vec2(-size_ * 0.25f, -size_ * 0.15f);

  renderer.DrawText("default", health_text, text_pos, 0.0f, text_scale, {0.0f, 0.0f, 0.0f, 1.0f});
}

}  // namespace core::graphics
