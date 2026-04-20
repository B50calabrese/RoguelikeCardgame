#include "core/graphics/card_renderer.h"

#include <cmath>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "core/constants.h"
#include "engine/graphics/utils/render_queue.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/texture.h"
#include "engine/util/asset_manager.h"

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
                              float scale, float alpha, float rotation,
                              float z_index) {
  const float card_width = kBaseCardWidth * scale;
  const float card_height = kBaseCardHeight * scale;
  const float rotation_radians = glm::radians(rotation);
  const glm::vec2 rotation_vec =
      glm::vec2(std::cos(rotation_radians), std::sin(rotation_radians));
  const glm::vec4 tint = glm::vec4(1.0f, 1.0f, 1.0f, alpha);

  // Use a slight Z-offset for text so it's always above the card it belongs to
  // but still respects the overall card's Z-order.
  const float text_z_offset = 0.01f;

  auto& queue = engine::graphics::utils::RenderQueue::Default();

  // 1. Render the Main Frame (Tinted)
  // We use the color-specific textures, but we can also apply a light tint
  // if needed. For now, white tint.
  engine::graphics::utils::RenderCommand frame_cmd;
  frame_cmd.z_order = z_index;
  frame_cmd.texture_id = data.frame_texture_id;
  frame_cmd.position = position;
  frame_cmd.size = glm::vec2(card_width, card_height);
  frame_cmd.rotation = rotation;
  frame_cmd.color = tint;
  frame_cmd.origin = glm::vec2(0.5f, 0.5f);
  queue.Submit(frame_cmd);

  // 2. Render Art
  glm::vec2 art_transformed_offset =
      position +
      CalculateTransformedOffset(kCardArtOffset, scale, rotation_vec);
  engine::graphics::utils::RenderCommand art_cmd;
  art_cmd.z_order = z_index;
  art_cmd.texture_id = data.art_texture_id;
  art_cmd.position = art_transformed_offset;
  art_cmd.size = kCardArtSize * scale;
  art_cmd.rotation = rotation;
  art_cmd.color = tint;
  art_cmd.origin = glm::vec2(0.5f, 0.5f);
  queue.Submit(art_cmd);

  // 3. Card Name
  glm::vec2 card_name_transformed_offset =
      position +
      CalculateTransformedOffset(kCardNameOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", data.name, card_name_transformed_offset, rotation, 0.8f * scale,
      kDefaultTextColor, z_index + text_z_offset);

  // 4. Card Cost (Top Right)
  glm::vec2 cost_transformed_offset =
      position +
      CalculateTransformedOffset(kCostOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", std::to_string(data.cost), cost_transformed_offset, rotation,
      0.8f * scale, kDefaultTextColor, z_index + text_z_offset);

  // 5. Type Line
  glm::vec2 type_transformed_offset =
      position +
      CalculateTransformedOffset(kCardTypeOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", data.type_line, type_transformed_offset, rotation, 0.6f * scale,
      kDefaultTextColor, z_index + text_z_offset);

  // 6. Description
  glm::vec2 desc_transformed_offset =
      position +
      CalculateTransformedOffset(kCardDescOffset, scale, rotation_vec);
  engine::graphics::Renderer::Get().DrawText(
      "arial", data.description, desc_transformed_offset, rotation,
      0.5f * scale, kDefaultTextColor, z_index + text_z_offset);

  // 7. Render Stats for Creatures (Bottom Right)
  if (data.type == CardType::Creature) {
    glm::vec2 power_transformed_offset =
        position +
        CalculateTransformedOffset(kPowerOffset, scale, rotation_vec);
    engine::graphics::Renderer::Get().DrawText(
        "arial", std::to_string(data.power), power_transformed_offset, rotation,
        0.7f * scale, kDefaultTextColor, z_index + text_z_offset);

    glm::vec2 health_transformed_offset =
        position +
        CalculateTransformedOffset(kHealthOffset, scale, rotation_vec);
    engine::graphics::Renderer::Get().DrawText(
        "arial", std::to_string(data.health), health_transformed_offset,
        rotation, 0.7f * scale, kDefaultTextColor, z_index + text_z_offset);
  }
}

void CardRenderer::RenderCardBack(glm::vec2 position, float scale, float alpha,
                                  float rotation, float z_index) {
  const float card_width = kBaseCardWidth * scale;
  const float card_height = kBaseCardHeight * scale;
  const glm::vec4 tint = glm::vec4(1.0f, 1.0f, 1.0f, alpha);

  auto back_tex =
      engine::util::AssetManager<engine::graphics::Texture>::Get("card_back.png");

  if (back_tex) {
    engine::graphics::utils::RenderCommand cmd;
    cmd.z_order = z_index;
    cmd.texture_id = back_tex->renderer_id();
    cmd.position = position;
    cmd.size = glm::vec2(card_width, card_height);
    cmd.rotation = rotation;
    cmd.color = tint;
    cmd.origin = glm::vec2(0.5f, 0.5f);
    engine::graphics::utils::RenderQueue::Default().Submit(cmd);
  }
}
}  // namespace core::graphics
