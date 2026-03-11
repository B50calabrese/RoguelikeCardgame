#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_

#include <glm/vec2.hpp>
#include <vector>

#include "core/card_data.h"

namespace core::graphics {

/**
 * @brief Utility class to render a hand of cards in an arc.
 *
 * Automatically calculates positions, scales, and rotations for multiple
 * CardData objects to fit within a specific bounding area on screen.
 */
class HandRenderer {
 public:
  /**
   * @brief Renders a vector of cards in an arc within a bounding box.
   *
   * This method calculates the layout for the given list of cards,
   * fitting them into the provided rectangular area on screen. It adjusts
   * the card size (scaling) and positioning (arch shape) based on the
   * number of cards and their overlap setting.
   *
   * @param cards The list of CardData objects to render.
   * @param bounds_pos The bottom-left corner of the bounding box area.
   * @param bounds_size The width and height of the bounding box area.
   * @param arc_angle_degrees The total angular spread of the cards in degrees.
   *                          Positive values arc upwards, negative arc downwards.
   * @param overlap_factor The amount cards are allowed to overlap (0.0 to 1.0).
   *                       0.0 means no overlap, 1.0 means fully overlapping.
   */
  static void RenderHand(const std::vector<core::CardData>& cards,
                         glm::vec2 bounds_pos, glm::vec2 bounds_size,
                         float arc_angle_degrees, float overlap_factor);
};

}  // namespace core::graphics

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_
