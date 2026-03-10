#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_CARD_RENDERER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_CARD_RENDERER_H_

#include <glm/vec2.hpp>

#include "core/card_data.h"

namespace core::graphics {

/**
 * @brief Utility class to draw game cards.
 */
class CardRenderer {
 public:
  /**
   * @brief Renders a complete card with all its layers.
   */
  static void RenderCard(const core::CardData& data, glm::vec2 position,
                         float scale = 1.0f, float alpha = 1.0f,
                         float rotation = 0.0f);
};

}  // namespace core::graphics

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_CARD_RENDERER_H_
