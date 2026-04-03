#include "core/graphics/card_renderer.h"

#include <cmath>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "core/constants.h"
#include "engine/graphics/utils/render_queue.h"
#include "engine/graphics/renderer.h"

namespace core::graphics {
namespace {

glm::vec4 GetColorForCard(CardColor color) {
  switch (color) {
    case CardColor::White:
      return core::graphics::kColorWhite;
    case CardColor::Blue:
      return core::graphics::kColorBlue;
    case CardColor::Black:
      return core::graphics::kColorBlack;
    case CardColor::Red:
      return core::graphics::kColorRed;
    case CardColor::Green:
      return core::graphics::kColorGreen;
    case CardColor::Colorless:
    default:
      return glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
  }
}

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

  // Card Color Tinting
  glm::vec4 color_tint = GetColorForCard(data.color);
  color_tint.a *= alpha;

  // Render the Main Frame (Tinted)
  engine::graphics::Renderer::Get().DrawTexturedQuad(
      position, glm::vec2(card_width, card_height), data.frame_texture_id,
      rotation, color_tint, glm::vec2(0.5f, 0.5f));

  // Render the Art
  glm::vec2 art_transformed_offset =
      position + CalculateTransformedOffset(kArtBoxOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawTexturedQuad(
      art_transformed_offset, kArtBoxSize * scale, data.art_texture_id, rotation,
      glm::vec4(1.0f, 1.0f, 1.0f, alpha), glm::vec2(0.5f, 0.5f));

  const glm::vec4 text_color = glm::vec4(kDefaultTextColor.r, kDefaultTextColor.g,
                                         kDefaultTextColor.b, alpha);

  // Card Name
  glm::vec2 card_name_transformed_offset =
      position +
      CalculateTransformedOffset(kCardNameOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", data.name, card_name_transformed_offset, rotation, 0.8f * scale,
      text_color);

  // Card Cost
  glm::vec2 card_cost_transformed_offset =
      position +
      CalculateTransformedOffset(kCardCostOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", std::to_string(data.cost), card_cost_transformed_offset,
      rotation, 0.8f * scale, text_color);

  // Type Line
  std::string type_str =
      (data.type == CardType::Creature) ? "Creature" : "Spell";
  glm::vec2 type_transformed_offset =
      position +
      CalculateTransformedOffset(kTypeLineOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", type_str, type_transformed_offset, rotation, 0.6f * scale,
      text_color);

  // Description (Center aligned in its box)
  // Note: For simplicity, we assume one line or handle multiple lines if
  // TextRenderer supports it.
  glm::vec2 desc_transformed_offset =
      position +
      CalculateTransformedOffset(kDescriptionBoxOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", data.description, desc_transformed_offset, rotation,
      0.5f * scale, text_color);

  // Render Stats for Creatures
  if (data.type == CardType::Creature) {
    glm::vec2 power_transformed_offset =
        position +
        CalculateTransformedOffset(kPowerOffset, scale, rotation_vec);
    engine::graphics::Renderer::Get().DrawText(
        "arial", std::to_string(data.power), power_transformed_offset, rotation,
        0.8f * scale, text_color);

    glm::vec2 health_transformed_offset =
        position +
        CalculateTransformedOffset(kHealthOffset, scale, rotation_vec);
    engine::graphics::Renderer::Get().DrawText(
        "arial", std::to_string(data.health), health_transformed_offset,
        rotation, 0.8f * scale, text_color);
  }
}
}  // namespace core::graphics
