#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_MATH_UTIL_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_MATH_UTIL_H_

#include <glm/vec2.hpp>

namespace core::util {

/**
 * @brief Checks if a point is within a rectangle.
 * @param point The point to check.
 * @param rect_pos The position of the rectangle.
 * @param rect_size The dimensions of the rectangle.
 * @param centered If true, rect_pos is treated as the center of the rectangle.
 *                 If false, rect_pos is treated as the bottom-left corner.
 * @return True if the point is inside the rectangle, false otherwise.
 */
inline bool PointInRect(glm::vec2 point, glm::vec2 rect_pos, glm::vec2 rect_size,
                        bool centered = false) {
  if (centered) {
    return point.x >= rect_pos.x - rect_size.x * 0.5f &&
           point.x <= rect_pos.x + rect_size.x * 0.5f &&
           point.y >= rect_pos.y - rect_size.y * 0.5f &&
           point.y <= rect_pos.y + rect_size.y * 0.5f;
  }
  return point.x >= rect_pos.x && point.x <= rect_pos.x + rect_size.x &&
         point.y >= rect_pos.y && point.y <= rect_pos.y + rect_size.y;
}

}  // namespace core::util

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_MATH_UTIL_H_
