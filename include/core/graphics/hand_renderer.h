#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_

#include <glm/vec2.hpp>
#include <vector>

#include "core/card_data.h"

namespace core::graphics {

/**
 * @brief Represents the visual transformation for a card in the hand.
 */
struct CardLayout {
  glm::vec2 position;
  float scale;
  float rotation;
};

/**
 * @brief Utility class to render a hand of cards in an arc.
 */
class HandRenderer {
 public:
  /**
   * @brief Calculates the layout for a hand of cards.
   *
   * @param card_count The number of cards in the hand.
   * @param bounds_pos The bottom-left corner of the bounding box.
   * @param bounds_size The width and height of the bounding box.
   * @param arc_angle_degrees The total angular spread of the cards in degrees.
   * @param overlap_factor The amount cards are allowed to overlap (0.0 to 1.0).
   * @return A vector of CardLayout objects, one for each card.
   */
  static std::vector<CardLayout> CalculateHandLayout(
      size_t card_count, glm::vec2 bounds_pos, glm::vec2 bounds_size,
      float arc_angle_degrees, float overlap_factor);

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

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HAND_RENDERER_H_
