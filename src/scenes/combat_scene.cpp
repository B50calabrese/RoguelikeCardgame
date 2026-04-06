#include "scenes/combat_scene.h"

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include "core/card_registry.h"
#include "core/card_instance.h"
#include "core/constants.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/start_turn_action.h"
#include "core/ai/simple_ai.h"
#include "scenes/controllers/hand_controller.h"
#include "core/game_config.h"
#include "core/graphics/card_renderer.h"
#include "core/graphics/hand_renderer.h"
#include "core/effects/actions/play_card_action.h"
#include "core/util/math_util.h"
#include "engine/input/input_manager.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/console.h"
#include "engine/util/logger.h"
#include "scenes/combat_command_system.h"
#include "scenes/main_menu_scene.h"

namespace scenes {

void CombatScene::OnAttach() {
  LOG_INFO("[CombatScene] Attached");

  // Load cards from assets
  bool success =
      core::CardRegistry::Get().LoadCardsFromDirectory("cards", false);
  if (!success) {
    LOG_ERR("[CombatScene] Failed to load some cards.");
  }

  CombatCommandSystem::Register(game_state_);

  const auto& all_cards = core::CardRegistry::Get().GetAllCards();
  if (all_cards.empty()) {
    LOG_ERR("[CombatScene] No cards loaded!");
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

  // Populate hand for demo
  int starting_hand_size = config.starting_hand_size;
  auto it = all_cards.begin();
  for (int i = 0; i < starting_hand_size && it != all_cards.end(); ++i, ++it) {
    auto p_inst = std::make_unique<core::CardInstance>(&it->second, game_state_.next_instance_id++, game_state_.player->id);
    p_inst->location = core::CardLocation::Hand;
    game_state_.player->hand.push_back(std::move(p_inst));

    auto e_inst = std::make_unique<core::CardInstance>(&it->second, game_state_.next_instance_id++, game_state_.enemy->id);
    e_inst->location = core::CardLocation::Hand;
    game_state_.enemy->hand.push_back(std::move(e_inst));
  }

  // Configure hands
  kBorderThickness = config.window_width * 0.05f;
  kIconSize = config.window_width * 0.1f;
  kIconTop = kBorderThickness + kIconSize;
  kEnemyIconBottom = config.window_height - kBorderThickness - kIconSize;

  kHandBoundsSize = {static_cast<float>(config.window_width) * 0.8f,
                     static_cast<float>(config.window_height) * 0.4f};
  kPlayerHandPos = {
      (static_cast<float>(config.window_width) - kHandBoundsSize.x) * 0.5f,
      kIconTop + 20.0f};
  kEnemyHandPos = {
      (static_cast<float>(config.window_width) - kHandBoundsSize.x) * 0.5f,
      kEnemyIconBottom - kHandBoundsSize.y - 20.0f};

  player_hand_ = std::make_unique<controllers::HandController>(game_state_.player->id);
  player_hand_->SetBounds(kPlayerHandPos, kHandBoundsSize);
  player_hand_->SetArcAngle(core::graphics::kDefaultArcAngle);
  player_hand_->SetInteractive(true);
  player_hand_->SetFaceDown(false);

  enemy_hand_ = std::make_unique<controllers::HandController>(game_state_.enemy->id);
  enemy_hand_->SetBounds(kEnemyHandPos, kHandBoundsSize);
  enemy_hand_->SetArcAngle(-core::graphics::kDefaultArcAngle);
  enemy_hand_->SetInteractive(false);
  enemy_hand_->SetFaceDown(true);
}

void CombatScene::OnUpdate(float delta_time_seconds) {
  engine::util::Console::Get().Update();

  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEscape)) {
    engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
    return;
  }

  // Process game logic
  core::effects::EffectResolver::Get().ProcessQueue(game_state_);
  enemy_ai_->Update(delta_time_seconds, game_state_);
  battle_ui_.Update(delta_time_seconds, game_state_);

  player_hand_->Update(delta_time_seconds, game_state_);
  enemy_hand_->Update(delta_time_seconds, game_state_);
}

void CombatScene::OnRender() {
  battle_ui_.Render(game_state_);
  player_hand_->Render();
  enemy_hand_->Render();
  engine::util::Console::Get().Render();
}

}  // namespace scenes
