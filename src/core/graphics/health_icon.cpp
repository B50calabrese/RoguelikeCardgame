#include "core/graphics/health_icon.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/utils/render_queue.h"
#include <string>

namespace core::graphics {

HealthIcon::HealthIcon(const glm::vec2& position, float size, const glm::vec4& color)
    : position_(position), size_(size), color_(color) {}

void HealthIcon::Render(int health) const {
  auto& renderer = engine::graphics::Renderer::Get();

  // Use a high Z-index for UI to ensure it's on top of game elements.
  const float ui_z = 1000.0f;

  // Render the colored square centered at position_
  engine::graphics::utils::RenderCommand cmd;
  cmd.z_order = ui_z;
  cmd.position = position_;
  cmd.size = {size_, size_};
  cmd.color = color_;
  cmd.origin = {0.5f, 0.5f};
  engine::graphics::utils::RenderQueue::Default().Submit(cmd);

  // Render the health text
  std::string health_text = std::to_string(health);

  // Calculate text position. TextRenderer/Renderer::DrawText is baseline-based.
  // To center it, we'd ideally need text bounds, but for now we'll offset it.
  // Assuming scale 1.0 is roughly 32px high.
  float text_scale = size_ / 64.0f;
  // Very rough centering offset - in a real engine we'd use GetTextBounds
  glm::vec2 text_pos = position_ + glm::vec2(-size_ * 0.25f, -size_ * 0.15f);

  renderer.DrawText("default", health_text, text_pos, 0.0f, text_scale,
                    {0.0f, 0.0f, 0.0f, 1.0f}, ui_z + 0.1f);
}

}  // namespace core::graphics
