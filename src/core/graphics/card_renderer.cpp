#include "core/graphics/card_renderer.h"

#include <cmath>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "core/constants.h"
#include "engine/graphics/utils/render_queue.h"
#include "engine/graphics/renderer.h"

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
  const float card_width = kBaseCardWidth * scale;
  const float card_height = kBaseCardHeight * scale;
  const float rotation_radians = glm::radians(rotation);
  const glm::vec2 rotation_vec =
      glm::vec2(std::cos(rotation_radians), std::sin(rotation_radians));
  const glm::vec4 tint = glm::vec4(1.0f, 1.0f, 1.0f, alpha);

  // Render the Main Frame
  engine::graphics::Renderer::Get().DrawTexturedQuad(
      position, glm::vec2(card_width, card_height), data.frame_texture_id,
      rotation, tint, glm::vec2(0.5f, 0.5f));

  // Card Name
  glm::vec2 card_name_transformed_offset =
      position +
      CalculateTransformedOffset(kCardNameOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", data.name, card_name_transformed_offset, rotation, 1.0f * scale,
      kDefaultTextColor);

  // Render Stats for Creatures
  if (data.type == CardType::Creature) {
    glm::vec2 power_transformed_offset =
        position +
        CalculateTransformedOffset(kPowerOffset, scale, rotation_vec);
    engine::graphics::Renderer::Get().DrawText(
        "arial", std::to_string(data.power), power_transformed_offset, rotation,
        0.8f * scale, kDefaultTextColor);

    glm::vec2 health_transformed_offset =
        position +
        CalculateTransformedOffset(kHealthOffset, scale, rotation_vec);
    engine::graphics::Renderer::Get().DrawText(
        "arial", std::to_string(data.health), health_transformed_offset,
        rotation, 0.8f * scale, kDefaultTextColor);
  }
}
}  // namespace core::graphics
