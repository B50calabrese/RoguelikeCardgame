#include "scenes/combat_scene.h"

#include <algorithm>
#include <random>
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
#include "core/effects/actions/spell_visual_action.h"
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
#include "engine/graphics/utils/render_queue.h"
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

  const auto& all_cards = core::CardRegistry::Get().all_cards();
  if (all_cards.empty()) {
    LOG_ERR("[CombatScene] No cards loaded!");
    return;
  }

  // Setup initial state
  game_state_.player->colors = {core::CardColor::White, core::CardColor::Blue};
  game_state_.enemy->colors = {core::CardColor::Red, core::CardColor::Black};
  game_state_.current_turn_player_id = game_state_.player->id;

  game_state_.player->mana = 1;
  game_state_.player->max_mana = 1;
  game_state_.enemy->mana = 0;
  game_state_.enemy->max_mana = 0;

  enemy_ai_ = std::make_unique<core::ai::SimpleAI>(game_state_.enemy->id);

  // Initialize decks: first 10 cards of each color
  auto setup_deck = [&](core::PlayerState& p) {
    for (auto color : p.colors) {
      int base_id = 0;
      switch (color) {
        case core::CardColor::White: base_id = 0; break;
        case core::CardColor::Blue: base_id = 20; break;
        case core::CardColor::Black: base_id = 40; break;
        case core::CardColor::Red: base_id = 60; break;
        case core::CardColor::Green: base_id = 80; break;
        default: break;
      }
      for (int i = 1; i <= 10; ++i) {
        const core::CardData* data = core::CardRegistry::Get().GetCardById(base_id + i);
        if (data) {
          auto inst = std::make_unique<core::CardInstance>(
              data, game_state_.next_instance_id++, p.id);
          inst->location = core::CardLocation::Deck;
          p.deck.push_back(std::move(inst));
        }
      }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(p.deck.begin(), p.deck.end(), g);

    // Initial draw
    for (int i = 0; i < 5 && !p.deck.empty(); ++i) {
      auto inst = std::move(p.deck.back());
      p.deck.pop_back();
      inst->location = core::CardLocation::Hand;
      p.hand.push_back(std::move(inst));
    }
  };

  setup_deck(*game_state_.player);
  setup_deck(*game_state_.enemy);

  // Trigger first turn
  core::effects::EffectResolver::Get().QueueAction(
      std::make_shared<core::effects::actions::StartTurnAction>(
          game_state_.player->id));

  auto& config = core::GameConfig::Get();

  // Configure hands
  kBorderThickness = config.window_width * 0.05f;
  kIconSize = kBorderThickness;
  kIconTop = kBorderThickness;
  kEnemyIconBottom = config.window_height - kBorderThickness;

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
  float board_x =
      (static_cast<float>(config.window_width) - board_width) * 0.5f;

  // Zones within the board
  float zone_width = board_width - 2.0f * combat::kZoneBorder;
  float zone_height =
      (board_height - 2.0f * combat::kZoneBorder - combat::kZonePadding) * 0.5f;

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
  UpdateSpellVisuals(delta_time_seconds);
  enemy_ai_->Update(delta_time_seconds, game_state_);
  battle_ui_.Update(delta_time_seconds, game_state_);

  player_hand_->Update(delta_time_seconds, game_state_,
                       &combat_controller_->hitbox_manager());
  enemy_hand_->Update(delta_time_seconds, game_state_);

  combat_controller_->Update(delta_time_seconds, game_state_, kIconTop,
                             kIconSize);
  combat_controller_->HandleInput(game_state_, kIconTop, kIconSize);
}

void CombatScene::OnRender() {
  battle_ui_.Render(game_state_, player_zone_rect_, enemy_zone_rect_);

  // Update board hitboxes and render creatures
  combat_controller_->hitbox_manager().Clear();
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

    combat_controller_->hitbox_manager().AddHitbox({inst_id, pos, size, false});

    // Render highlight if it can attack
    if (game_state_.current_turn_player_id == game_state_.player->id &&
        game_state_.player->board[i]->can_attack &&
        !game_state_.player->board[i]->has_attacked) {
      engine::graphics::PrimitiveRenderer::SubmitQuad(
          pos, size * 1.1f, glm::vec4(1.0f, 1.0f, 0.0f, 0.5f), 0.0f,
          {0.5f, 0.5f});
    }

    // Render Haste highlight (red)
    if (game_state_.player->board[i]->has_haste) {
      engine::graphics::PrimitiveRenderer::SubmitQuad(
          pos, size * 1.05f, glm::vec4(1.0f, 0.0f, 0.0f, 0.4f), 0.0f,
          {0.5f, 0.5f});
    }

    // Render Blocker highlight (green)
    if (game_state_.player->board[i]->is_blocker) {
      engine::graphics::PrimitiveRenderer::SubmitQuad(
          pos, size * 1.05f, glm::vec4(0.0f, 1.0f, 0.0f, 0.4f), 0.0f,
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
      game_state_.enemy->board.size(), {enemy_zone_rect_.x, enemy_zone_rect_.y},
      {enemy_zone_rect_.z, enemy_zone_rect_.w}, 0.0f, 0.2f);
  for (size_t i = 0; i < game_state_.enemy->board.size(); ++i) {
    int inst_id = game_state_.enemy->board[i]->instance_id;
    glm::vec2 pos = combat_controller_->animator().GetAnimatedPosition(
        inst_id, enemy_board_layouts[i].position);
    float scale =
        enemy_board_layouts[i].scale.x * combat::kBoardCardScaleMultiplier;
    glm::vec2 size =
        glm::vec2(card_base_width * scale, card_base_height * scale);

    combat_controller_->hitbox_manager().AddHitbox({inst_id, pos, size, true});

    // Render Haste highlight (red)
    if (game_state_.enemy->board[i]->has_haste) {
      engine::graphics::PrimitiveRenderer::SubmitQuad(
          pos, size * 1.05f, glm::vec4(1.0f, 0.0f, 0.0f, 0.4f), 0.0f,
          {0.5f, 0.5f});
    }

    // Render Blocker highlight (green)
    if (game_state_.enemy->board[i]->is_blocker) {
      engine::graphics::PrimitiveRenderer::SubmitQuad(
          pos, size * 1.05f, glm::vec4(0.0f, 1.0f, 0.0f, 0.4f), 0.0f,
          {0.5f, 0.5f});
    }

    core::graphics::CardRenderer::RenderCard(
        *game_state_.enemy->board[i]->data, pos, scale, 1.0f,
        enemy_board_layouts[i].rotation, 0.0f);
  }

  player_hand_->Render();
  enemy_hand_->Render();

  RenderSpellVisuals();

  DrawTargetingLine();

  engine::graphics::utils::RenderQueue::Default().Flush();
  engine::util::Console::Get().Render();
}

void CombatScene::DrawTargetingLine() {
  if (combat_controller_->current_state() == CombatState::PickingTarget &&
      combat_controller_->selected_attacker_id()) {
    glm::vec2 start_pos;
    if (auto hitbox = combat_controller_->hitbox_manager().GetHitboxFor(
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

void CombatScene::UpdateSpellVisuals(float delta_time_seconds) {
  // Check for new SpellVisualAction
  auto current_action = core::effects::EffectResolver::Get().current_action();
  if (current_action) {
    auto spell_action =
        std::dynamic_pointer_cast<core::effects::actions::SpellVisualAction>(
            current_action);
    if (spell_action) {
      int inst_id = spell_action->card_instance_id();
      bool already_active = std::any_of(
          active_spell_visuals_.begin(), active_spell_visuals_.end(),
          [inst_id](const auto& v) { return v.instance_id == inst_id; });

      if (!already_active) {
        auto& config = core::GameConfig::Get();
        ActiveSpellVisual visual;
        visual.instance_id = inst_id;
        visual.elapsed_time = 0.0f;
        // Start from a reasonable middle position or hand?
        // Let's just pop it in the middle for now.
        visual.current_pos = {config.window_width * 0.3f, config.window_height * 0.5f};
        active_spell_visuals_.push_back(visual);
      }
    }
  }

  // Update active visuals
  for (auto it = active_spell_visuals_.begin();
       it != active_spell_visuals_.end();) {
    it->elapsed_time += delta_time_seconds;

    if (it->elapsed_time >= 1.5f) {
      core::effects::VisualBlocker::Get().RemoveBlocker(
          "SpellVisual_" + std::to_string(it->instance_id));
      it = active_spell_visuals_.erase(it);
    } else {
      ++it;
    }
  }
}

void CombatScene::RenderSpellVisuals() {
  for (const auto& visual : active_spell_visuals_) {
    core::CardInstance* inst = game_state_.FindCardInstance(visual.instance_id);
    if (inst) {
      // Float to the left of center
      core::graphics::CardRenderer::RenderCard(
          *inst->data, visual.current_pos, 0.8f, 1.0f, 0.0f, combat::kHandZ + 500.0f);
    }
  }
}

}  // namespace scenes
