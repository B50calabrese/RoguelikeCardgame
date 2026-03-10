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
  const float base_width = 200.0f;
  const float base_height = 300.0f;
  const float card_width = base_width * scale;
  const float card_height = base_height * scale;
  const float rotation_radians = glm::radians(rotation);
  const glm::vec2 rotation_vec =
      glm::vec2(std::cos(rotation_radians), std::sin(rotation_radians));
  const glm::vec4 tint = glm::vec4(1.0f, 1.0f, 1.0f, alpha);
  const glm::vec4 text_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

  // Render the Main Frame
  engine::graphics::Renderer::Get().DrawTexturedQuad(
      position, glm::vec2(card_width, card_height), data.frame_texture_id,
      rotation, tint, glm::vec2(0.5f, 0.5f));

  // Card Name
  // Offset is relative to the bottom-left of the card in base units.
  // We convert it to be relative to the center pivot.
  glm::vec2 card_name_offset =
      glm::vec2(20.0f - base_width / 2.0f, 264.0f - base_height / 2.0f);
  glm::vec2 card_name_transformed_offset =
      position +
      CalculateTransformedOffset(card_name_offset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", data.name, card_name_transformed_offset, rotation, 1.0f * scale,
      text_color);
}
}  // namespace core::graphics