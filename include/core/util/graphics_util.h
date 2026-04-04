#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_GRAPHICS_UTIL_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_GRAPHICS_UTIL_H_

#include <glm/vec4.hpp>
#include "core/card_data.h"
#include "core/constants.h"

namespace core::util {

inline glm::vec4 GetColorVector(core::CardColor color) {
  switch (color) {
    case core::CardColor::White: return core::graphics::kColorWhite;
    case core::CardColor::Blue:  return core::graphics::kColorBlue;
    case core::CardColor::Black: return core::graphics::kColorBlack;
    case core::CardColor::Red:   return core::graphics::kColorRed;
    case core::CardColor::Green: return core::graphics::kColorGreen;
    case core::CardColor::Colorless:
    default:
      return glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
  }
}

}  // namespace core::util

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_UTIL_GRAPHICS_UTIL_H_
