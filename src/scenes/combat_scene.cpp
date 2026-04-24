#include "scenes/combat_scene.h"

#include <algorithm>
#include "engine/graphics/utils/render_queue.h"
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include "core/ai/simple_ai.h"
#include "core/card_instance.h"
#include "core/card_registry.h"
#include "core/constants.h"
#include "core/effects/actions/creature_attack_action.h"
#include "core/effects/actions/play_card_action.h"
#include "core/effects/actions/start_turn_action.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/event_bus.h"
#include "core/effects/rules_engine.h"
#include "core/effects/visual_blocker.h"
#include "core/game_config.h"
#include "core/graphics/card_renderer.h"
#include "core/graphics/hand_renderer.h"
#include "core/util/math_util.h"
#include "engine/graphics/primitive_renderer.h"
#include "engine/graphics/renderer.h"
#include "engine/input/input_manager.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/console.h"
#include "engine/util/logger.h"
#include "scenes/combat_command_system.h"
#include "scenes/controllers/hand_controller.h"
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
      std::make_shared<core::effects::actions::StartTurnAction>(
          game_state_.player->id));

  auto& config = core::GameConfig::Get();

  // Populate hand for demo
  int starting_hand_size = config.starting_hand_size;
  auto it = all_cards.begin();
  for (int i = 0; i < starting_hand_size && it != all_cards.end(); ++i, ++it) {
    auto p_inst = std::make_unique<core::CardInstance>(
        &it->second, game_state_.next_instance_id++, game_state_.player->id);
    p_inst->location = core::CardLocation::Hand;
    game_state_.player->hand.push_back(std::move(p_inst));

    auto e_inst = std::make_unique<core::CardInstance>(
        &it->second, game_state_.next_instance_id++, game_state_.enemy->id);
    e_inst->location = core::CardLocation::Hand;
    game_state_.enemy->hand.push_back(std::move(e_inst));
  }

  // Configure hands
  kBorderThickness = config.window_width * 0.05f;
  kIconSize = config.window_width * 0.1f;
  kIconTop = kBorderThickness + kIconSize;
  kEnemyIconBottom = config.window_height - kBorderThickness - kIconSize;

  kHandBoundsSize = {
      static_cast<float>(config.window_width) * combat::kHandWidthPercent,
      static_cast<float>(config.window_height) * combat::kHandHeightPercent};

  // Hand position for player: bottom of card is 20px from bottom.
  // Pivot of hand is center of the bounds.
  // HandRenderer::CalculateHandLayout puts the hand around the center of the
  // bounds.
  kPlayerHandPos = {
      (static_cast<float>(config.window_width) - kHandBoundsSize.x) * 0.5f,
      combat::kHandEdgeOffset};

  // For mirror, enemy hand top of card is 20px from top.
  kEnemyHandPos = {
      (static_cast<float>(config.window_width) - kHandBoundsSize.x) * 0.5f,
      static_cast<float>(config.window_height) - kHandBoundsSize.y -
          combat::kHandEdgeOffset};

  // Configure boards
  float board_top = kEnemyHandPos.y;
  float board_bottom = kPlayerHandPos.y + kHandBoundsSize.y;
  float board_height = board_top - board_bottom;
  float board_width =
      static_cast<float>(config.window_width) * combat::kBoardWidthPercent;

  kBoardBoundsSize = {board_width, board_height};
  float board_x = (static_cast<float>(config.window_width) - board_width) * 0.5f;

  // Zones within the board
  float zone_width = board_width - 2.0f * combat::kZoneBorder;
  float zone_height = (board_height - 2.0f * combat::kZoneBorder -
                       combat::kZonePadding) * 0.5f;

  enemy_zone_rect_ = {board_x + combat::kZoneBorder,
                      board_top - combat::kZoneBorder - zone_height, zone_width,
                      zone_height};

  player_zone_rect_ = {board_x + combat::kZoneBorder,
                       board_bottom + combat::kZoneBorder, zone_width,
                       zone_height};

  // Card layout positions (center of zones)
  kEnemyBoardPos = {enemy_zone_rect_.x, enemy_zone_rect_.y};
  kPlayerBoardPos = {player_zone_rect_.x, player_zone_rect_.y};
  kBoardBoundsSize = {zone_width, zone_height};

  player_hand_ =
      std::make_unique<controllers::HandController>(game_state_.player->id);
  player_hand_->SetBounds(kPlayerHandPos, kHandBoundsSize);
  player_hand_->SetArcAngle(core::graphics::kDefaultArcAngle);
  player_hand_->SetInteractive(true);
  player_hand_->SetFaceDown(false);
  player_hand_->SetPlayZone(player_zone_rect_);

  enemy_hand_ =
      std::make_unique<controllers::HandController>(game_state_.enemy->id);
  enemy_hand_->SetBounds(kEnemyHandPos, kHandBoundsSize);
  enemy_hand_->SetArcAngle(-core::graphics::kDefaultArcAngle);
  enemy_hand_->SetInteractive(false);
  enemy_hand_->SetFaceDown(true);

  combat_controller_ = std::make_unique<controllers::CombatController>();

  core::effects::EventBus::Get().Subscribe(
      [this](core::state::GameState& state,
             const core::effects::GameEvent& event) {
        if (event.type == core::effects::GameEventType::CreatureAttacked) {
          this->combat_controller_->OnCreatureAttacked(
              state, event, this->kIconTop, this->kIconSize);
        }
      });
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

  combat_controller_->Update(delta_time_seconds, game_state_, kIconTop,
                             kIconSize);
  combat_controller_->HandleInput(game_state_, kIconTop, kIconSize);
}

void CombatScene::OnRender() {
  battle_ui_.Render(game_state_, player_zone_rect_, enemy_zone_rect_);

  // Update board hitboxes and render creatures
  combat_controller_->hitboxes().Clear();
  auto& config = core::GameConfig::Get();
  float card_base_width = core::graphics::kBaseCardWidth;
  float card_base_height = core::graphics::kBaseCardHeight;

  auto player_board_layouts = core::graphics::HandRenderer::CalculateHandLayout(
      game_state_.player->board.size(), kPlayerBoardPos, kBoardBoundsSize, 0.0f,
      0.2f);
  for (size_t i = 0; i < game_state_.player->board.size(); ++i) {
    int inst_id = game_state_.player->board[i]->instance_id;
    glm::vec2 pos = combat_controller_->animator().GetAnimatedPosition(
        inst_id, player_board_layouts[i].position);
    float scale =
        player_board_layouts[i].scale.x * combat::kBoardCardScaleMultiplier;
    glm::vec2 size =
        glm::vec2(card_base_width * scale, card_base_height * scale);

    combat_controller_->hitboxes().AddHitbox({inst_id, pos, size, false});

    // Render highlight if it can attack
    if (game_state_.current_turn_player_id == game_state_.player->id &&
        game_state_.player->board[i]->can_attack &&
        !game_state_.player->board[i]->has_attacked) {
      engine::graphics::PrimitiveRenderer::SubmitQuad(
          pos, size * 1.1f, glm::vec4(1.0f, 1.0f, 0.0f, 0.5f), 0.0f,
          {0.5f, 0.5f});
    }

    // Render selection highlight
    if (combat_controller_->selected_attacker_id() == inst_id) {
      engine::graphics::PrimitiveRenderer::SubmitQuad(
          pos, size * 1.15f, glm::vec4(0.0f, 1.0f, 0.0f, 0.7f), 0.0f,
          {0.5f, 0.5f});
    }

    core::graphics::CardRenderer::RenderCard(
        *game_state_.player->board[i]->data, pos, scale, 1.0f,
        player_board_layouts[i].rotation, 0.0f);
  }

  auto enemy_board_layouts = core::graphics::HandRenderer::CalculateHandLayout(
      game_state_.enemy->board.size(),
      {enemy_zone_rect_.x, enemy_zone_rect_.y},
      {enemy_zone_rect_.z, enemy_zone_rect_.w}, 0.0f, 0.2f);
  for (size_t i = 0; i < game_state_.enemy->board.size(); ++i) {
    int inst_id = game_state_.enemy->board[i]->instance_id;
    glm::vec2 pos = combat_controller_->animator().GetAnimatedPosition(
        inst_id, enemy_board_layouts[i].position);
    float scale =
        enemy_board_layouts[i].scale.x * combat::kBoardCardScaleMultiplier;
    glm::vec2 size =
        glm::vec2(card_base_width * scale, card_base_height * scale);

    combat_controller_->hitboxes().AddHitbox({inst_id, pos, size, true});

    core::graphics::CardRenderer::RenderCard(*game_state_.enemy->board[i]->data,
                                             pos, scale, 1.0f,
                                             enemy_board_layouts[i].rotation,
                                             0.0f);
  }

  player_hand_->Render();
  enemy_hand_->Render();

  DrawTargetingLine();

  engine::graphics::utils::RenderQueue::Default().Flush();
  engine::util::Console::Get().Render();
}

void CombatScene::DrawTargetingLine() {
  if (combat_controller_->current_state() == CombatState::PickingTarget &&
      combat_controller_->selected_attacker_id()) {
    glm::vec2 start_pos;
    if (auto hitbox = combat_controller_->hitboxes().GetHitboxFor(
            *combat_controller_->selected_attacker_id())) {
      start_pos = hitbox->position;
    } else {
      return;
    }
    glm::vec2 end_pos = engine::InputManager::Get().mouse_screen_pos();

    auto& queue = engine::graphics::utils::RenderQueue::Default();
    engine::graphics::utils::RenderCommand cmd;
    cmd.shape_type = engine::graphics::utils::ShapeType::kLine;
    cmd.position = start_pos;
    cmd.size = end_pos;  // For lines, size is used as end point in some systems
    cmd.color = {1.0f, 1.0f, 1.0f, 1.0f};
    cmd.thickness = 5.0f;
    cmd.z_order = combat::kTargetingLineZ;

    queue.Submit(cmd);
  }
}

}  // namespace scenes
