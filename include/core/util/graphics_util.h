#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_GRAPHICS_UTIL_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_GRAPHICS_UTIL_H_

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include "core/enums.h"
#include "engine/ecs/components/transform.h"

namespace core::util {

/**
 * @brief Utility functions for graphics and layout.
 */
class GraphicsUtil {
 public:
  /**
   * @brief Calculates transforms for a set of cards to be laid out in a zone.
   *
   * @param count Number of cards to lay out.
   * @param zone_pos Bottom-left position of the zone.
   * @param zone_size Size of the zone.
   * @param card_base_size Base dimensions of a single card.
   * @param gap Gap between cards in pixels.
   * @param max_scale_multiplier Maximum scale relative to the base size.
   * @return Vector of transforms for each card.
   */
  static std::vector<engine::ecs::components::Transform> CalculateBoardLayout(
      size_t count, glm::vec2 zone_pos, glm::vec2 zone_size,
      glm::vec2 card_base_size, float gap, float max_scale_multiplier);

  /**
   * @brief Returns the RGBA color vector for a given card color.
   */
  static glm::vec4 GetColorVector(CardColor color);
};

}  // namespace core::util

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_GRAPHICS_UTIL_H_
