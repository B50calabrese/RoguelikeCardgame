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

const glm::vec2 kCardNameOffset = {15.0f - kBaseCardWidth / 2.0f,
                                   278.0f - kBaseCardHeight / 2.0f};
const glm::vec2 kCardCostOffset = {175.0f - kBaseCardWidth / 2.0f,
                                   278.0f - kBaseCardHeight / 2.0f};

const glm::vec2 kArtBoxOffset = {100.0f - kBaseCardWidth / 2.0f,
                                 200.0f - kBaseCardHeight / 2.0f};
const glm::vec2 kArtBoxSize = {184.0f, 120.0f};

const glm::vec2 kTypeLineOffset = {15.0f - kBaseCardWidth / 2.0f,
                                   125.0f - kBaseCardHeight / 2.0f};

const glm::vec2 kDescriptionBoxOffset = {100.0f - kBaseCardWidth / 2.0f,
                                          59.0f - kBaseCardHeight / 2.0f};
const glm::vec2 kDescriptionBoxSize = {184.0f, 102.0f};

const glm::vec2 kPowerOffset = {145.0f - kBaseCardWidth / 2.0f,
                                22.0f - kBaseCardHeight / 2.0f};
const glm::vec2 kHealthOffset = {176.0f - kBaseCardWidth / 2.0f,
                                 22.0f - kBaseCardHeight / 2.0f};

const glm::vec4 kDefaultCardTint = {1.0f, 1.0f, 1.0f, 1.0f};
const glm::vec4 kDefaultTextColor = {0.0f, 0.0f, 0.0f, 1.0f};

constexpr float kDefaultArcAngle = 30.0f;
constexpr float kDefaultOverlapFactor = 0.3f;

// Gameplay Colors (Vintage/Classic MTG Palette)
const glm::vec4 kColorWhite = {0.92f, 0.90f, 0.85f, 1.0f}; // Parchment/Off-white
const glm::vec4 kColorBlue = {0.40f, 0.60f, 0.85f, 1.0f};  // Classic Blue
const glm::vec4 kColorBlack = {0.35f, 0.35f, 0.35f, 1.0f}; // Deep Charcoal
const glm::vec4 kColorRed = {0.80f, 0.40f, 0.30f, 1.0f};   // Brick/Classic Red
const glm::vec4 kColorGreen = {0.35f, 0.55f, 0.35f, 1.0f}; // Sage/Nature Green

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
