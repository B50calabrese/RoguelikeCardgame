#include "scenes/combat_scene.h"

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include "core/card_registry.h"
#include "core/constants.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/start_turn_action.h"
#include "core/ai/simple_ai.h"
#include "core/game_config.h"
#include "core/graphics/card_renderer.h"
#include "core/graphics/hand_renderer.h"
#include "core/util/math_util.h"
#include "engine/input/input_manager.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/logger.h"
#include "scenes/main_menu_scene.h"

namespace scenes {

namespace {
constexpr float kCardHoverScale = 1.2f;
constexpr float kCardHeldScale = 1.3f;
constexpr float kLerpSpeed = 10.0f;
}  // namespace

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

  // Setup initial state
  game_state_.player->colors = {core::CardColor::White, core::CardColor::Blue};
  game_state_.enemy->colors = {core::CardColor::Red, core::CardColor::Black};
  game_state_.current_turn_player_id = game_state_.player->id;

  enemy_ai_ = std::make_unique<core::ai::SimpleAI>(game_state_.enemy->id);

  // Trigger first turn
  core::effects::EffectResolver::Get().QueueAction(
      std::make_shared<core::effects::actions::StartTurnAction>(game_state_.player->id));

  auto& config = core::GameConfig::Get();

  // Fill hand based on starting hand size from config
  int starting_hand_size = core::GameConfig::Get().starting_hand_size;
  auto it = all_cards.begin();
  for (int i = 0; i < starting_hand_size && it != all_cards.end(); ++i, ++it) {
    VisualCard vc;
    vc.data = it->second;
    // Start off-screen
    vc.current_transform.position = {config.window_width * 0.5f, -200.0f};
    vc.current_transform.scale = glm::vec2(0.5f);
    vc.current_transform.rotation = 0.0f;
    vc.target_transform = vc.current_transform;
    hand_visuals_.push_back(vc);
  }

  // If we have fewer unique cards than starting hand size, duplicate some for
  // demo
  while (hand_visuals_.size() < static_cast<size_t>(starting_hand_size) &&
         !hand_visuals_.empty()) {
    VisualCard vc = hand_visuals_[hand_visuals_.size() % all_cards.size()];
    hand_visuals_.push_back(vc);
  }
}

void CombatScene::OnUpdate(float delta_time_seconds) {
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEscape)) {
    engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
    return;
  }

  // Process game logic
  core::effects::EffectResolver::Get().ProcessQueue(game_state_);
  enemy_ai_->Update(delta_time_seconds, game_state_);
  battle_ui_.Update(delta_time_seconds, game_state_);

  HandleCardInteraction(delta_time_seconds);
  UpdateHandLayout();
  AnimateCards(delta_time_seconds);
}

void CombatScene::HandleCardInteraction(float delta_time_seconds) {
  auto& input = engine::InputManager::Get();
  glm::vec2 mouse_pos = input.mouse_screen_pos();
  bool clicked = input.IsKeyPressed(engine::KeyCode::kMouseLeft);

  hovered_card_index_ = std::nullopt;

  // 1. Handle Held Card
  if (held_card_index_) {
    VisualCard& held_card = hand_visuals_[*held_card_index_];
    held_card.target_transform.position = mouse_pos;
    held_card.target_transform.scale = glm::vec2(kCardHeldScale);
    held_card.target_transform.rotation = 0.0f;

    if (clicked) {
      // For now, playing just returns it to hand visuals
      // In the future, this would be:
      // core::effects::EffectResolver::Get().QueueAction(std::make_shared<core::effects::actions::PlayCardAction>(...));
      held_card.is_held = false;
      held_card_index_ = std::nullopt;
    }
    return;
  }

  // 2. Hover detection (top-to-bottom)
  for (int i = static_cast<int>(hand_visuals_.size()) - 1; i >= 0; --i) {
    const auto& vc = hand_visuals_[i];
    glm::vec2 size =
        core::graphics::kBaseCardSize * vc.current_transform.scale.x;
    if (core::util::PointInRect(mouse_pos, vc.current_transform.position, size,
                                true)) {
      hovered_card_index_ = i;
      break;
    }
  }

  // 3. Handle Click to Hold
  if (clicked && hovered_card_index_) {
    held_card_index_ = hovered_card_index_;
    hand_visuals_[*held_card_index_].is_held = true;
  }
}

void CombatScene::UpdateHandLayout() {
  auto& config = core::GameConfig::Get();
  float border_thickness = config.window_width * 0.05f;
  float icon_size = config.window_width * 0.1f;

  glm::vec2 bounds_size = {static_cast<float>(config.window_width) * 0.8f,
                           static_cast<float>(config.window_height) * 0.4f};

  // Position hand above the player's health icon.
  // The icon is centered at border_thickness + icon_size * 0.5f.
  // So its top is at border_thickness + icon_size.
  float icon_top = border_thickness + icon_size;

  glm::vec2 bounds_pos = {
      (static_cast<float>(config.window_width) - bounds_size.x) * 0.5f,
      icon_top + 20.0f  // 20px padding above the icon
  };

  // Only calculate layout for cards that are NOT being held
  std::vector<size_t> in_hand_indices;
  for (size_t i = 0; i < hand_visuals_.size(); ++i) {
    if (!hand_visuals_[i].is_held) {
      in_hand_indices.push_back(i);
    }
  }

  if (in_hand_indices.empty()) return;

  auto layouts = core::graphics::HandRenderer::CalculateHandLayout(
      in_hand_indices.size(), bounds_pos, bounds_size,
      core::graphics::kDefaultArcAngle, core::graphics::kDefaultOverlapFactor);

  for (size_t i = 0; i < in_hand_indices.size(); ++i) {
    size_t vc_idx = in_hand_indices[i];
    VisualCard& vc = hand_visuals_[vc_idx];

    vc.target_transform = layouts[i];

    // Apply hover zoom
    if (hovered_card_index_ && *hovered_card_index_ == vc_idx) {
      vc.target_transform.scale *= kCardHoverScale;
      vc.target_transform.rotation = 0.0f;
      // Slightly lift the hovered card
      vc.target_transform.position.y += 50.0f;
    }
  }
}

void CombatScene::AnimateCards(float delta_time_seconds) {
  float t = glm::clamp(delta_time_seconds * kLerpSpeed, 0.0f, 1.0f);

  for (auto& vc : hand_visuals_) {
    vc.current_transform.position =
        glm::mix(vc.current_transform.position, vc.target_transform.position, t);
    vc.current_transform.scale =
        glm::mix(vc.current_transform.scale, vc.target_transform.scale, t);
    vc.current_transform.rotation = glm::mix(
        vc.current_transform.rotation, vc.target_transform.rotation, t);
  }
}

void CombatScene::OnRender() {
  // Render battle UI
  battle_ui_.Render(game_state_);

  // Render cards. Hovered and Held cards should be rendered last.
  std::optional<size_t> last_to_render = std::nullopt;
  if (held_card_index_) {
    last_to_render = held_card_index_;
  } else if (hovered_card_index_) {
    last_to_render = hovered_card_index_;
  }

  for (size_t i = 0; i < hand_visuals_.size(); ++i) {
    if (last_to_render && i == *last_to_render) continue;
    const auto& vc = hand_visuals_[i];
    core::graphics::CardRenderer::RenderCard(
        vc.data, vc.current_transform.position, vc.current_transform.scale.x,
        1.0f, vc.current_transform.rotation);
  }

  if (last_to_render) {
    const auto& vc = hand_visuals_[*last_to_render];
    core::graphics::CardRenderer::RenderCard(
        vc.data, vc.current_transform.position, vc.current_transform.scale.x,
        1.0f, vc.current_transform.rotation);
  }
}

}  // namespace scenes
