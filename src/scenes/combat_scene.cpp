#include "scenes/combat_scene.h"

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/vec2.hpp>

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

  // Configure boards
  kBoardBoundsSize = {static_cast<float>(config.window_width) * kBoardWidthPercent,
                      static_cast<float>(config.window_height) * kBoardHeightPercent};
  kPlayerBoardPos = {
      (static_cast<float>(config.window_width) - kBoardBoundsSize.x) * 0.5f,
      static_cast<float>(config.window_height) * kPlayerBoardYPercent};
  kEnemyBoardPos = {
      (static_cast<float>(config.window_width) - kBoardBoundsSize.x) * 0.5f,
      static_cast<float>(config.window_height) * kEnemyBoardYPercent};

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

  core::effects::EventBus::Get().Subscribe(
      [this](core::state::GameState& state, const core::effects::GameEvent& event) {
        if (event.type == core::effects::GameEventType::CreatureAttacked) {
          this->OnCreatureAttacked(state, event);
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

  UpdateAnimations(delta_time_seconds);
  HandleInput();
}

void CombatScene::OnRender() {
  battle_ui_.Render(game_state_);

  // Update board hitboxes and render creatures
  board_hitboxes_.clear();
  auto& config = core::GameConfig::Get();
  float card_base_width = core::graphics::kBaseCardWidth;
  float card_base_height = core::graphics::kBaseCardHeight;

  auto player_board_layouts = core::graphics::HandRenderer::CalculateHandLayout(
      game_state_.player->board.size(), kPlayerBoardPos, kBoardBoundsSize, 0.0f,
      0.2f);
  for (size_t i = 0; i < game_state_.player->board.size(); ++i) {
    int inst_id = game_state_.player->board[i]->instance_id;
    glm::vec2 pos = player_board_layouts[i].position;
    float scale = player_board_layouts[i].scale.x;

    // Handle animation position
    if (active_animation_ && active_animation_->attacker_id == inst_id) {
        float t = active_animation_->elapsed_time / active_animation_->duration;
        // Simple easing: move to target then back
        // 0 to 0.5: move to target
        // 0.5 to 1.0: move back
        if (active_animation_->moving_to_target) {
            // Accelerate towards target: t^2
            float local_t = active_animation_->elapsed_time / (active_animation_->duration * 0.5f);
            pos = glm::mix(active_animation_->start_pos, active_animation_->target_pos, local_t * local_t);
        } else {
            // Constant rate back: linear
            float local_t = (active_animation_->elapsed_time - active_animation_->duration * 0.5f) / (active_animation_->duration * 0.5f);
            pos = glm::mix(active_animation_->target_pos, active_animation_->start_pos, local_t);
        }
    }

    board_hitboxes_.push_back({inst_id, pos, glm::vec2(card_base_width * scale, card_base_height * scale), false});

    // Render highlight if it can attack
    if (game_state_.current_turn_player_id == game_state_.player->id &&
        game_state_.player->board[i]->can_attack && !game_state_.player->board[i]->has_attacked) {
        engine::graphics::PrimitiveRenderer::SubmitQuad(pos, board_hitboxes_.back().size * 1.1f, glm::vec4(1.0f, 1.0f, 0.0f, 0.5f), 0.0f, {0.5f, 0.5f});
    }

    // Render selection highlight
    if (selected_attacker_id_ == inst_id) {
        engine::graphics::PrimitiveRenderer::SubmitQuad(pos, board_hitboxes_.back().size * 1.15f, glm::vec4(0.0f, 1.0f, 0.0f, 0.7f), 0.0f, {0.5f, 0.5f});
    }

    core::graphics::CardRenderer::RenderCard(
        *game_state_.player->board[i]->data, pos,
        scale, 1.0f, player_board_layouts[i].rotation);
  }

  auto enemy_board_layouts = core::graphics::HandRenderer::CalculateHandLayout(
      game_state_.enemy->board.size(), kEnemyBoardPos, kBoardBoundsSize, 0.0f,
      0.2f);
  for (size_t i = 0; i < game_state_.enemy->board.size(); ++i) {
    int inst_id = game_state_.enemy->board[i]->instance_id;
    glm::vec2 pos = enemy_board_layouts[i].position;
    float scale = enemy_board_layouts[i].scale.x;

    // Handle animation position for enemy (if they attack)
    if (active_animation_ && active_animation_->attacker_id == inst_id) {
         if (active_animation_->moving_to_target) {
            float local_t = active_animation_->elapsed_time / (active_animation_->duration * 0.5f);
            pos = glm::mix(active_animation_->start_pos, active_animation_->target_pos, local_t * local_t);
        } else {
            float local_t = (active_animation_->elapsed_time - active_animation_->duration * 0.5f) / (active_animation_->duration * 0.5f);
            pos = glm::mix(active_animation_->target_pos, active_animation_->start_pos, local_t);
        }
    }

    board_hitboxes_.push_back({inst_id, pos, glm::vec2(card_base_width * scale, card_base_height * scale), true});

    core::graphics::CardRenderer::RenderCard(
        *game_state_.enemy->board[i]->data, pos,
        scale, 1.0f, enemy_board_layouts[i].rotation);
  }

  DrawTargetingLine();

  player_hand_->Render();
  enemy_hand_->Render();
  engine::util::Console::Get().Render();
}

void CombatScene::HandleInput() {
    if (core::effects::VisualBlocker::Get().IsBlocking()) return;

    auto& input = engine::InputManager::Get();
    glm::vec2 mouse_pos = input.mouse_screen_pos();

    if (input.IsKeyPressed(engine::KeyCode::kMouseLeft)) {
        if (current_state_ == CombatState::Idle) {
            // Check for clicking own creature
            for (const auto& hitbox : board_hitboxes_) {
                if (!hitbox.is_enemy && core::util::PointInRect(mouse_pos, hitbox.position, hitbox.size, true)) {
                    // Check if it can attack
                    auto* inst = game_state_.FindCardInstance(hitbox.instance_id);
                    if (inst && inst->can_attack && !inst->has_attacked && game_state_.current_turn_player_id == inst->owner_id) {
                        selected_attacker_id_ = hitbox.instance_id;
                        current_state_ = CombatState::PickingTarget;
                        LOG_INFO("[CombatScene] Selected attacker %d", *selected_attacker_id_);
                        return;
                    }
                }
            }
        } else if (current_state_ == CombatState::PickingTarget) {
            bool target_found = false;
            // Check for clicking enemy creature
            for (const auto& hitbox : board_hitboxes_) {
                if (hitbox.is_enemy && core::util::PointInRect(mouse_pos, hitbox.position, hitbox.size, true)) {
                    core::effects::Target target;
                    target.type = core::effects::Target::Type::Creature;
                    target.id = hitbox.instance_id;

                    core::effects::EffectResolver::Get().QueueAction(
                        std::make_shared<core::effects::actions::CreatureAttackAction>(*selected_attacker_id_, target));

                    target_found = true;
                    break;
                }
            }

            // Check for clicking enemy health icon
            if (!target_found) {
                // Approximate health icon position (top center)
                auto& config = core::GameConfig::Get();
                glm::vec2 enemy_health_pos = {config.window_width * 0.5f, config.window_height - kIconTop + kIconSize * 0.5f};
                glm::vec2 icon_size = {kIconSize, kIconSize};
                if (core::util::PointInRect(mouse_pos, enemy_health_pos, icon_size, true)) {
                    core::effects::Target target;
                    target.type = core::effects::Target::Type::Enemy;
                    target.id = -1;

                    core::effects::EffectResolver::Get().QueueAction(
                        std::make_shared<core::effects::actions::CreatureAttackAction>(*selected_attacker_id_, target));

                    target_found = true;
                }
            }

            current_state_ = CombatState::Idle;
            selected_attacker_id_.reset();
        }
    }
}

void CombatScene::UpdateAnimations(float delta_time) {
    if (active_animation_) {
        active_animation_->elapsed_time += delta_time;
        if (active_animation_->moving_to_target && active_animation_->elapsed_time >= active_animation_->duration * 0.5f) {
            active_animation_->moving_to_target = false;
        }

        if (active_animation_->elapsed_time >= active_animation_->duration) {
            core::effects::VisualBlocker::Get().RemoveBlocker("CreatureAttack_" + std::to_string(active_animation_->attacker_id));
            active_animation_.reset();
            current_state_ = CombatState::Idle;
        }
    }
}

void CombatScene::DrawTargetingLine() {
    if (current_state_ == CombatState::PickingTarget && selected_attacker_id_) {
        glm::vec2 start_pos;
        for (const auto& hitbox : board_hitboxes_) {
            if (hitbox.instance_id == *selected_attacker_id_) {
                start_pos = hitbox.position;
                break;
            }
        }
        glm::vec2 end_pos = engine::InputManager::Get().mouse_screen_pos();
        engine::graphics::PrimitiveRenderer::SubmitLine(start_pos, end_pos, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 3.0f);
    }
}

void CombatScene::OnCreatureAttacked(core::state::GameState& state, const core::effects::GameEvent& event) {
    AttackAnimation anim;
    anim.attacker_id = event.source_id;
    anim.target.id = event.target_id;

    // Determine target position
    glm::vec2 target_pos(0.0f);
    if (event.target_id == -1) { // Player or Enemy target
         auto& config = core::GameConfig::Get();
         if (state.current_turn_player_id == state.player->id) {
             // Attacking Enemy
             target_pos = {config.window_width * 0.5f, config.window_height - kIconTop + kIconSize * 0.5f};
             anim.target.type = core::effects::Target::Type::Enemy;
         } else {
             // Attacking Player
             target_pos = {config.window_width * 0.5f, kIconTop - kIconSize * 0.5f};
             anim.target.type = core::effects::Target::Type::Player;
         }
    } else {
        // Attacking creature
        anim.target.type = core::effects::Target::Type::Creature;
        bool found = false;
        for (const auto& hitbox : board_hitboxes_) {
            if (hitbox.instance_id == event.target_id) {
                target_pos = hitbox.position;
                found = true;
                break;
            }
        }
        if (!found) {
            LOG_ERR("[CombatScene] Animation target creature %d not found in hitboxes!", event.target_id);
            // Fallback to center of screen or something safe
            target_pos = {core::GameConfig::Get().window_width * 0.5f, core::GameConfig::Get().window_height * 0.5f};
        }
    }

    // Determine start position
    for (const auto& hitbox : board_hitboxes_) {
        if (hitbox.instance_id == event.source_id) {
            anim.start_pos = hitbox.position;
            break;
        }
    }

    anim.target_pos = target_pos;
    anim.duration = 1.0f;
    anim.elapsed_time = 0.0f;
    anim.moving_to_target = true;

    active_animation_ = anim;
    current_state_ = CombatState::AnimatingAttack;
}

}  // namespace scenes

}  // namespace scenes
