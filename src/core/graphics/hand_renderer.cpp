#include "core/graphics/hand_renderer.h"

#include <algorithm>
#include <cmath>

#include <glm/glm.hpp>

#include "core/constants.h"
#include "core/graphics/card_renderer.h"

namespace core::graphics {

void HandRenderer::RenderHand(const std::vector<core::CardData>& cards,
                              glm::vec2 bounds_pos, glm::vec2 bounds_size,
                              float arc_angle_degrees, float overlap_factor) {
  if (cards.empty()) {
    return;
  }

  const size_t card_count = cards.size();

  // 1. Calculate Arc and Scaling
  glm::vec2 bounds_center = bounds_pos + bounds_size * 0.5f;

  if (card_count == 1) {
    float scale = std::min(bounds_size.x / kBaseCardWidth,
                           bounds_size.y / kBaseCardHeight);
    CardRenderer::RenderCard(cards[0], bounds_center, scale, 1.0f, 0.0f);
    return;
  }

  float arc_angle_abs = std::abs(arc_angle_degrees);
  float arc_radians_abs = glm::radians(arc_angle_abs);

  float width_factor =
      1.0f + (static_cast<float>(card_count) - 1.0f) * (1.0f - overlap_factor);
  float scale_x = bounds_size.x / (kBaseCardWidth * width_factor);

  float sagitta_factor = 0.0f;
  if (arc_angle_abs > 0.1f) {
    sagitta_factor = (kBaseCardWidth * (static_cast<float>(card_count) - 1.0f) *
                      (1.0f - overlap_factor) *
                      std::tan(arc_radians_abs / 4.0f)) /
                     2.0f;
  }

  float scale_y = bounds_size.y / (kBaseCardHeight + sagitta_factor);
  float scale = std::min(scale_x, scale_y);

  float card_width = kBaseCardWidth * scale;
  float card_height = kBaseCardHeight * scale;

  // 2. Calculate Arc Parameters
  if (arc_angle_abs < 0.1f) {
    // Flat hand
    float total_width = card_width * width_factor;
    float start_x = bounds_center.x - total_width / 2.0f + card_width / 2.0f;
    float step_x = card_width * (1.0f - overlap_factor);
    for (size_t i = 0; i < card_count; ++i) {
      CardRenderer::RenderCard(
          cards[i], glm::vec2(start_x + i * step_x, bounds_center.y), scale,
          1.0f, 0.0f);
    }
    return;
  }

  float arc_radians = glm::radians(arc_angle_degrees);
  float total_spread_width = card_width *
                             (static_cast<float>(card_count) - 1.0f) *
                             (1.0f - overlap_factor);
  float radius = total_spread_width / (2.0f * std::sin(arc_radians / 2.0f));
  float sagitta = radius * (1.0f - std::cos(arc_radians / 2.0f));

  glm::vec2 arc_center;
  if (arc_angle_degrees > 0.0f) {
    arc_center = bounds_center - glm::vec2(0.0f, radius);
    arc_center.y += (bounds_size.y - (card_height + sagitta)) * 0.5f;
  } else {
    arc_center = bounds_center - glm::vec2(0.0f, radius);  // radius is negative
    arc_center.y -=
        (bounds_size.y - (card_height + std::abs(sagitta))) * 0.5f;
  }

  // 3. Render Cards
  float start_angle = 90.0f + arc_angle_degrees / 2.0f;
  float angle_step =
      (card_count > 1) ? (arc_angle_degrees / (card_count - 1)) : 0.0f;

  for (size_t i = 0; i < card_count; ++i) {
    float current_angle_deg = start_angle - i * angle_step;
    float current_angle_rad = glm::radians(current_angle_deg);

    glm::vec2 card_pos =
        arc_center +
        glm::vec2(std::cos(current_angle_rad) * radius,
                  std::sin(current_angle_rad) * radius);

    float card_rotation = current_angle_deg - 90.0f;

    CardRenderer::RenderCard(cards[i], card_pos, scale, 1.0f, card_rotation);
  }
}

}  // namespace core::graphics
