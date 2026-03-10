#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_

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
   */
  static void RenderHand(const std::vector<core::CardData>& cards,
                         glm::vec2 bounds_pos, glm::vec2 bounds_size,
                         float arc_angle_degrees, float overlap_factor);
};

}  // namespace core::graphics

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_
