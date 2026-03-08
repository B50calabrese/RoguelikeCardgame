#ifndef INCLUDE_CORE_GRAPHICS_CARD_RENDERER_H_
#define INCLUDE_CORE_GRAPHICS_CARD_RENDERER_H_

#include <glm/vec2.hpp>

#include "core/card_data.h"

namespace core::graphics {

/**
 * @brief Utility class to draw game cards.
 * This class acts as a bridge between high-level CardData and the low-level
 * PrimitiveRenderer. It handles the layout logic (where the art goes, where
 * the text goes) relative to a central pivot point.
 */
class CardRenderer {
 public:
  /**
   * @brief Renders a complete card with all its layers.
   * @param data The card definition (textures, costs, name).
   * @param position The world-space center point of the card.
   * @param scale Overall size multiplier.
   * @param alpha Transparency (useful for fading or graveyard effects).
   * @param rotation Rotation in degrees around the center pivot.
   */
  static void RenderCard(const core::CardData& data, glm::vec2 position,
                         float scale = 1.0f, float alpha = 1.0f,
                         float rotation = 0.0f);
};

}  // namespace core::graphics

#endif  // INCLUDE_CORE_GRAPHICS_CARD_RENDERER_H_