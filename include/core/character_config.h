#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_CHARACTER_CONFIG_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_CHARACTER_CONFIG_H_

#include <glm/vec4.hpp>

namespace core {

/**
 * @brief Configuration for character placeholders and later for real character assets.
 */
namespace characters {

// Placeholder Character Colors
const glm::vec4 kCharColor1 = {0.0f, 0.8f, 0.8f, 1.0f}; // Cyan
const glm::vec4 kCharColor2 = {0.8f, 0.0f, 0.8f, 1.0f}; // Magenta
const glm::vec4 kCharColor3 = {0.8f, 0.8f, 0.0f, 1.0f}; // Yellow

}  // namespace characters
}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_CHARACTER_CONFIG_H_
