#include "scenes/combat_scene.h"

#include <algorithm>
#include <vector>

#include <glm/vec2.hpp>

#include "core/card_registry.h"
#include "core/constants.h"
#include "core/game_config.h"
#include "core/graphics/hand_renderer.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/logger.h"
#include "scenes/main_menu_scene.h"
#include "engine/input/input_manager.h"

namespace scenes {

void CombatScene::OnAttach() {
  LOG_INFO("[CombatScene] Attached");

  // Load cards from assets
  bool success =
      core::CardRegistry::Get().LoadCardsFromDirectory("cards", false);
  if (!success) {
    LOG_ERR("[CombatScene] Failed to load some cards.");
  }

  const auto& all_cards = core::CardRegistry::Get().GetAllCards();
  if (all_cards.empty()) {
    LOG_ERR("[CombatScene] No cards loaded to display in hand!");
    return;
  }

  // Fill hand based on starting hand size from config
  int starting_hand_size = core::GameConfig::Get().starting_hand_size;
  auto it = all_cards.begin();
  for (int i = 0; i < starting_hand_size && it != all_cards.end(); ++i, ++it) {
    hand_.push_back(it->second);
  }

  // If we have fewer unique cards than starting hand size, duplicate some for
  // demo
  while (hand_.size() < static_cast<size_t>(starting_hand_size) &&
         !hand_.empty()) {
    hand_.push_back(hand_[hand_.size() % all_cards.size()]);
  }
}

void CombatScene::OnUpdate(float delta_time_seconds) {
  // Game logic (AI, Card movement) goes here
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEscape)) {
    engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
  }
}

void CombatScene::OnRender() {
  // Render the hand at the bottom of the screen
  auto& config = core::GameConfig::Get();
  glm::vec2 bounds_size = {static_cast<float>(config.window_width) * 0.8f,
                           static_cast<float>(config.window_height) * 0.4f};
  glm::vec2 bounds_pos = {
      (static_cast<float>(config.window_width) - bounds_size.x) * 0.5f,
      20.0f  // Padding from bottom
  };

  core::graphics::HandRenderer::RenderHand(
      hand_, bounds_pos, bounds_size, core::graphics::kDefaultArcAngle,
      core::graphics::kDefaultOverlapFactor);
}

}  // namespace scenes
