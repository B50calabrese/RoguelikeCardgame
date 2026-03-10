#ifndef INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_
#define INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_

#include <glm/vec2.hpp>
#include <vector>

#include "core/card_data.h"

namespace core::graphics {

/**
 * @brief Utility class to render a hand of cards in an arc.
 */
class HandRenderer {
 public:
  /**
   * @brief Renders a vector of cards in an arc.
   *
   * @param cards The list of cards to render.
   * @param bounds_pos The bottom-left corner of the bounding box.
   * @param bounds_size The width and height of the bounding box.
   * @param arc_angle_degrees The total angular spread of the cards in degrees.
   * @param overlap_factor The amount cards are allowed to overlap (0.0 to 1.0).
   *                       0.0 means no overlap, 1.0 means fully overlapping.
   */
  static void RenderHand(const std::vector<core::CardData>& cards,
                         glm::vec2 bounds_pos, glm::vec2 bounds_size,
                         float arc_angle_degrees, float overlap_factor);
};

}  // namespace core::graphics

#endif  // INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_
