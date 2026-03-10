#include "core/graphics/hand_renderer.h"

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

#include "core/graphics/card_renderer.h"

namespace core::graphics {

void HandRenderer::RenderHand(const std::vector<core::CardData>& cards,
                              glm::vec2 bounds_pos, glm::vec2 bounds_size,
                              float arc_angle_degrees, float overlap_factor) {
  if (cards.empty()) {
    return;
  }

  const float base_card_width = 200.0f;
  const float base_card_height = 300.0f;
  const size_t card_count = cards.size();

  // 1. Calculate Arc and Scaling
  glm::vec2 bounds_center = bounds_pos + bounds_size * 0.5f;

  if (card_count == 1) {
    float scale = std::min(bounds_size.x / base_card_width, bounds_size.y / base_card_height);
    CardRenderer::RenderCard(cards[0], bounds_center, scale, 1.0f, 0.0f);
    return;
  }

  float arc_angle_abs = std::abs(arc_angle_degrees);
  float arc_radians_abs = glm::radians(arc_angle_abs);

  // We want to fit within bounds_size.x and bounds_size.y.
  // Horizontal space used: card_width * (1 + (n-1)*(1-overlap)) if flat.
  // If curved, it's roughly the chord length + width of a rotated card.
  // Let's simplify: total_width = scale * base_width * (1 + (n-1)*(1-overlap))
  float width_factor = 1.0f + (static_cast<float>(card_count) - 1.0f) * (1.0f - overlap_factor);
  float scale_x = bounds_size.x / (base_card_width * width_factor);

  // Vertical space: sagitta + card_height (roughly).
  // sagitta = radius * (1 - cos(theta/2))
  // total_spread_width = scale * base_card_width * (n-1) * (1-overlap)
  // radius = total_spread_width / (2 * sin(theta/2))
  // sagitta = [scale * base_card_width * (n-1) * (1-overlap) / (2 * sin(theta/2))] * (1 - cos(theta/2))
  // sagitta = scale * base_card_width * (n-1) * (1-overlap) * tan(theta/4) / 2

  float sagitta_factor = 0.0f;
  if (arc_angle_abs > 0.1f) {
    sagitta_factor = (base_card_width * (static_cast<float>(card_count) - 1.0f) * (1.0f - overlap_factor) * std::tan(arc_radians_abs / 4.0f)) / 2.0f;
  }

  float scale_y = bounds_size.y / (base_card_height + sagitta_factor);
  float scale = std::min(scale_x, scale_y);

  float card_width = base_card_width * scale;
  float card_height = base_card_height * scale;

  // 2. Calculate Arc Parameters
  if (arc_angle_abs < 0.1f) {
    // Flat hand
    float total_width = card_width * width_factor;
    float start_x = bounds_center.x - total_width / 2.0f + card_width / 2.0f;
    float step_x = card_width * (1.0f - overlap_factor);
    for (size_t i = 0; i < card_count; ++i) {
      CardRenderer::RenderCard(cards[i], glm::vec2(start_x + i * step_x, bounds_center.y), scale, 1.0f, 0.0f);
    }
    return;
  }

  float arc_radians = glm::radians(arc_angle_degrees);
  float total_spread_width = card_width * (static_cast<float>(card_count) - 1.0f) * (1.0f - overlap_factor);
  float radius = total_spread_width / (2.0f * std::sin(arc_radians / 2.0f));
  float sagitta = radius * (1.0f - std::cos(arc_radians / 2.0f));

  // If arc_angle is positive, it's an upward arc (radius is below the hand).
  // If negative, it's a downward arc (radius is above the hand).
  glm::vec2 arc_center;
  if (arc_angle_degrees > 0.0f) {
    arc_center = bounds_center - glm::vec2(0.0f, radius);
    arc_center.y += (bounds_size.y - (card_height + sagitta)) * 0.5f;
  } else {
    arc_center = bounds_center - glm::vec2(0.0f, radius); // radius is negative
    arc_center.y -= (bounds_size.y - (card_height + std::abs(sagitta))) * 0.5f;
  }

  // 3. Render Cards
  float start_angle = 90.0f + arc_angle_degrees / 2.0f;
  float angle_step = (card_count > 1) ? (arc_angle_degrees / (card_count - 1)) : 0.0f;

  for (size_t i = 0; i < card_count; ++i) {
    float current_angle_deg = start_angle - i * angle_step;
    float current_angle_rad = glm::radians(current_angle_deg);

    glm::vec2 card_pos = arc_center + glm::vec2(std::cos(current_angle_rad) * radius,
                                                std::sin(current_angle_rad) * radius);

    // Rotation: 0 is upright. current_angle_deg is 90 for the center card.
    float card_rotation = current_angle_deg - 90.0f;

    CardRenderer::RenderCard(cards[i], card_pos, scale, 1.0f, card_rotation);
  }
}

}  // namespace core::graphics
