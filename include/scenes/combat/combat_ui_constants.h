#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMBAT_UI_CONSTANTS_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMBAT_UI_CONSTANTS_H_

#include <glm/vec4.hpp>

namespace scenes::combat {

// Layout Percentages
constexpr float kBoardWidthPercent = 0.9f;
constexpr float kHandWidthPercent = 0.8f;
constexpr float kHandHeightPercent = 0.25f;

// Positioning Offsets
constexpr float kHandEdgeOffset = 20.0f;
constexpr float kZonePadding = 64.0f;
constexpr float kZoneBorder = 64.0f;

// Scaling
constexpr float kBoardCardScaleMultiplier = 0.5f;
constexpr float kCardHoverScale = 1.2f;
constexpr float kCardHeldScale = 1.3f;

// Animation
constexpr float kDefaultLerpSpeed = 10.0f;
constexpr float kHeldCardLerpSpeed = 25.0f;

// Z-Indices
constexpr float kBackgroundZ = -100.0f;
constexpr float kBoardOutlineZ = -50.0f;
constexpr float kBorderZ = 900.0f;
constexpr float kUIZ = 1000.0f;
constexpr float kHandZ = 1100.0f;
constexpr float kTargetingLineZ = 1300.0f;

// Colors
const glm::vec4 kBoardBackgroundColor = {0.54f, 0.47f, 0.36f,
                                         1.0f};  // Light table-like brown
const glm::vec4 kBoardOutlineColor = {
    0.45f, 0.35f, 0.25f, 0.5f};  // Slightly darker brown for outlines

}  // namespace scenes::combat

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMBAT_UI_CONSTANTS_H_
