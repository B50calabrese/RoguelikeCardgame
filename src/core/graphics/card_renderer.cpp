#include "core/graphics/card_renderer.h"

#include <engine/graphics/render_queue.h>
#include <engine/graphics/renderer.h>

#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace core::graphics {
namespace {

glm::vec2 CalculateTransformedOffset(glm::vec2 local_offset, float scale,
                                     glm::vec2 rotation_vec) {
  glm::vec2 scaled = local_offset * scale;
  return glm::vec2(scaled.x * rotation_vec.x - scaled.y * rotation_vec.y,
                   scaled.x * rotation_vec.y + scaled.y * rotation_vec.x);
}

}  // namespace

void CardRenderer::RenderCard(const core::CardData& data, glm::vec2 position,
                              float scale, float alpha, float rotation) {
  // Layout constants
  const float card_width = 200.0f * scale;
  const float card_height = 300.0f * scale;
  const float rotation_radians = glm::radians(rotation);
  const glm::vec2 rotation_vec =
      glm::vec2(std::cos(rotation_radians), std::sin(rotation_radians));
  const glm::vec2 offset = glm::vec2(card_width / 2.0f, card_height / 2.0f);
  const float white[4] = {1.0f, 1.0f, 1.0f, alpha};
  const glm::vec4 text_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

  // Render the Main Frame
  engine::graphics::Renderer::Get().DrawTexturedQuad(
      position, glm::vec2(card_width, card_height), data.frame_texture_id,
      rotation);

  // Card Name
  glm::vec2 card_name_offset = glm::vec2(20.0f, 264.0f);
  glm::vec2 card_name_transformed_offset =
      position +
      CalculateTransformedOffset(card_name_offset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", data.name, card_name_transformed_offset, rotation, 1.0f * scale,
      text_color);
}
}  // namespace core::graphics