#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_CONSTANTS_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_CONSTANTS_H_

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace core {

// Graphics Constants
namespace graphics {
constexpr float kBaseCardWidth = 200.0f;
constexpr float kBaseCardHeight = 300.0f;
const glm::vec2 kBaseCardSize = {kBaseCardWidth, kBaseCardHeight};

const glm::vec2 kCardNameOffset = {20.0f - kBaseCardWidth / 2.0f,
                                   264.0f - kBaseCardHeight / 2.0f};

const glm::vec2 kPowerOffset = {140.0f - kBaseCardWidth / 2.0f,
                                25.0f - kBaseCardHeight / 2.0f};
const glm::vec2 kHealthOffset = {172.0f - kBaseCardWidth / 2.0f,
                                 25.0f - kBaseCardHeight / 2.0f};

const glm::vec4 kDefaultCardTint = {1.0f, 1.0f, 1.0f, 1.0f};
const glm::vec4 kDefaultTextColor = {0.0f, 0.0f, 0.0f, 1.0f};

constexpr float kDefaultArcAngle = 30.0f;
constexpr float kDefaultOverlapFactor = 0.3f;

// Gameplay Colors
const glm::vec4 kColorWhite = {0.8f, 0.8f, 0.7f, 1.0f};
const glm::vec4 kColorBlue = {0.0f, 0.4f, 0.7f, 1.0f};
const glm::vec4 kColorBlack = {0.2f, 0.2f, 0.2f, 1.0f};
const glm::vec4 kColorRed = {0.7f, 0.1f, 0.1f, 1.0f};
const glm::vec4 kColorGreen = {0.1f, 0.5f, 0.1f, 1.0f};

const glm::vec4 kColorWhiteHighlight = {1.0f, 1.0f, 0.9f, 1.0f};
const glm::vec4 kColorBlueHighlight = {0.0f, 0.6f, 1.0f, 1.0f};
const glm::vec4 kColorBlackHighlight = {0.4f, 0.4f, 0.4f, 1.0f};
const glm::vec4 kColorRedHighlight = {1.0f, 0.2f, 0.2f, 1.0f};
const glm::vec4 kColorGreenHighlight = {0.2f, 0.8f, 0.2f, 1.0f};

}  // namespace graphics

// Gameplay Constants
namespace gameplay {
constexpr int kDefaultStartingHandSize = 5;
constexpr int kDefaultStartingDeckSize = 10;
}  // namespace gameplay

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_CONSTANTS_H_
