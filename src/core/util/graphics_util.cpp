#include "core/util/graphics_util.h"

#include <algorithm>

#include "core/constants.h"

namespace core::util {

std::vector<engine::ecs::components::Transform>
GraphicsUtil::CalculateBoardLayout(size_t count, glm::vec2 zone_pos,
                                   glm::vec2 zone_size,
                                   glm::vec2 card_base_size, float gap,
                                   float max_scale_multiplier) {
  std::vector<engine::ecs::components::Transform> layouts(count);
  if (count == 0) return layouts;

  float max_width = zone_size.x - (count > 1 ? (count - 1) * gap : 0.0f);
  float scale_x = max_width / (count * card_base_size.x);
  float scale_y = zone_size.y / card_base_size.y;
  float scale = std::min({scale_x, scale_y, max_scale_multiplier});

  float card_width = scale * card_base_size.x;
  float total_width =
      count * card_width + (count > 1 ? (count - 1) * gap : 0.0f);
  float start_x =
      zone_pos.x + (zone_size.x - total_width) * 0.5f + card_width * 0.5f;
  float center_y = zone_pos.y + zone_size.y * 0.5f;

  for (size_t i = 0; i < count; ++i) {
    layouts[i].position = {start_x + i * (card_width + gap), center_y};
    layouts[i].scale = glm::vec2(scale);
    layouts[i].rotation = 0.0f;
  }
  return layouts;
}

glm::vec4 GraphicsUtil::GetColorVector(CardColor color) {
  switch (color) {
    case CardColor::White:
      return graphics::kColorWhite;
    case CardColor::Blue:
      return graphics::kColorBlue;
    case CardColor::Black:
      return graphics::kColorBlack;
    case CardColor::Red:
      return graphics::kColorRed;
    case CardColor::Green:
      return graphics::kColorGreen;
    default:
      return {0.5f, 0.5f, 0.5f, 1.0f};
  }
}

}  // namespace core::util
