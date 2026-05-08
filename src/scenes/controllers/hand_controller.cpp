#include "scenes/controllers/hand_controller.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include "core/constants.h"
#include "core/effects/actions/play_card_action.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/visual_blocker.h"
#include "core/game_config.h"
#include "core/graphics/card_renderer.h"
#include "core/graphics/hand_renderer.h"
#include "core/util/math_util.h"
#include "engine/graphics/utils/render_queue.h"
#include "engine/input/input_manager.h"
#include "engine/util/logger.h"
#include "scenes/combat/combat_ui_constants.h"

namespace scenes::controllers {

HandController::HandController(int player_id)
    : player_id_(player_id),
      bounds_pos_(0.0f),
      bounds_size_(0.0f),
      arc_angle_(core::graphics::kDefaultArcAngle),
      hovered_card_index_(std::nullopt),
      held_card_index_(std::nullopt) {}

void HandController::Update(float delta_time_seconds,
                            core::state::GameState& state,
                            combat::HitboxManager* hitbox_manager) {
  const auto& hand_state =
      (player_id_ == state.player->id) ? state.player->hand : state.enemy->hand;

  if (core::effects::VisualBlocker::Get().is_blocking()) {
    AnimateCards(delta_time_seconds);
    return;
  }

  SyncHandWithState(hand_state);
  if (is_interactive_) {
    auto& input = engine::InputManager::Get();
    glm::vec2 mouse_pos = input.mouse_screen_pos();
    bool clicked = input.IsKeyPressed(engine::KeyCode::kMouseLeft);

    hovered_card_index_ = std::nullopt;

    // 1. Handle Held Card
    if (held_card_index_) {
      if (*held_card_index_ >= hand_visuals_.size()) {
        held_card_index_ = std::nullopt;
        is_picking_target_ = false;
        return;
      }
      VisualCard& held_card = hand_visuals_[*held_card_index_];

      if (!is_picking_target_) {
        held_card.target_transform.position = mouse_pos;
        held_card.target_transform.scale = glm::vec2(combat::kCardHeldScale);
        held_card.target_transform.rotation = 0.0f;
      }

      if (clicked) {
        if (is_picking_target_) {
          std::optional<core::effects::Target> selected_target =
              FindTargetAt(state, mouse_pos, hitbox_manager);

          if (selected_target && current_target_filter_.IsValid(
                                     state, player_id_, *selected_target)) {
            core::effects::EffectResolver::Get().QueueAction(
                std::make_shared<core::effects::actions::PlayCardAction>(
                    player_id_, held_card.instance_id,
                    std::vector<core::effects::Target>{*selected_target}));

            // Immediately remove card from visuals to prevent it snapping back
            // to hand before state updates.
            hand_visuals_.erase(hand_visuals_.begin() + *held_card_index_);
            held_card_index_ = std::nullopt;
            is_picking_target_ = false;
            return;
          } else {
            // Cancel play if click was not on a valid target
            is_picking_target_ = false;
            held_card.is_held = false;
            held_card_index_ = std::nullopt;
          }
          return;
        }

        bool in_play_zone = false;
        if (play_zone_) {
          in_play_zone =
              core::util::PointInRect(mouse_pos, {play_zone_->x, play_zone_->y},
                                      {play_zone_->z, play_zone_->w}, false);
        }

        if (in_play_zone) {
          core::CardInstance* inst =
              state.FindCardInstance(held_card.instance_id);
          if (inst) {
            bool needs_target = false;
            for (const auto& effect_def : inst->data->effects) {
              if (effect_def.trigger == core::Trigger::OnPlay &&
                  effect_def.filter.is_required) {
                needs_target = true;
                current_target_filter_ = effect_def.filter;
                break;
              }
            }

            if (needs_target) {
              is_picking_target_ = true;
              // Freeze card at a nice "casting" position
              auto& config = core::GameConfig::Get();
              held_card.target_transform.position = glm::vec2(
                  config.window_width * 0.5f, config.window_height * 0.4f);
              return;
            } else {
              core::effects::EffectResolver::Get().QueueAction(
                  std::make_shared<core::effects::actions::PlayCardAction>(
                      player_id_, held_card.instance_id,
                      std::vector<core::effects::Target>{}));

              // Immediately remove card from visuals to prevent it snapping back
              // to hand before state updates.
              hand_visuals_.erase(hand_visuals_.begin() + *held_card_index_);
              held_card_index_ = std::nullopt;
              return;
            }
          }
        }

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
  UpdateLayout();
  AnimateCards(delta_time_seconds);
}

void HandController::SyncHandWithState(
    const std::vector<std::unique_ptr<core::CardInstance>>& hand_state) {
  auto& config = core::GameConfig::Get();

  // 1. Remove cards that are no longer in hand state
  hand_visuals_.erase(std::remove_if(hand_visuals_.begin(), hand_visuals_.end(),
                                     [&](const VisualCard& vc) {
                                       return std::none_of(
                                           hand_state.begin(), hand_state.end(),
                                           [&](const auto& inst) {
                                             return inst->instance_id ==
                                                    vc.instance_id;
                                           });
                                     }),
                      hand_visuals_.end());

  // 2. Add cards that are in hand state but not in visuals
  for (const auto& inst : hand_state) {
    bool found = std::any_of(hand_visuals_.begin(), hand_visuals_.end(),
                             [&](const VisualCard& vc) {
                               return vc.instance_id == inst->instance_id;
                             });
    if (!found) {
      VisualCard vc;
      vc.data = *inst->data;
      vc.instance_id = inst->instance_id;
      // Start off-screen
      vc.current_transform.position = {config.window_width * 0.5f, -200.0f};
      vc.current_transform.scale = glm::vec2(0.5f);
      vc.current_transform.rotation = 0.0f;
      vc.target_transform = vc.current_transform;
      hand_visuals_.push_back(vc);
    }
  }
}


void HandController::UpdateLayout() {
  std::vector<size_t> in_hand_indices;
  for (size_t i = 0; i < hand_visuals_.size(); ++i) {
    if (!hand_visuals_[i].is_held) {
      in_hand_indices.push_back(i);
    }
  }

  if (in_hand_indices.empty()) return;

  auto layouts = core::graphics::HandRenderer::CalculateHandLayout(
      in_hand_indices.size(), bounds_pos_, bounds_size_, arc_angle_,
      core::graphics::kDefaultOverlapFactor);

  for (size_t i = 0; i < in_hand_indices.size(); ++i) {
    size_t vc_idx = in_hand_indices[i];
    VisualCard& vc = hand_visuals_[vc_idx];

    vc.target_transform = layouts[i];

    // Apply hover zoom
    if (is_interactive_ && hovered_card_index_ &&
        *hovered_card_index_ == vc_idx) {
      vc.target_transform.scale *= combat::kCardHoverScale;
      vc.target_transform.rotation = 0.0f;
      // Slightly lift the hovered card
      // If arc_angle is negative (top hand), we should probably "lift" it
      // downwards? For now, let's just use the same logic, but maybe adjust
      // based on arc_angle.
      float lift = (arc_angle_ < 0.0f) ? -50.0f : 50.0f;
      vc.target_transform.position.y += lift;
    }
  }
}

void HandController::AnimateCards(float delta_time_seconds) {
  bool any_moving = false;

  for (auto& vc : hand_visuals_) {
    if (vc.is_held && !is_picking_target_) {
      vc.current_transform.position = vc.target_transform.position;
      vc.current_transform.scale = vc.target_transform.scale;
      vc.current_transform.rotation = vc.target_transform.rotation;
    } else {
      float lerp_speed =
          vc.is_held ? combat::kHeldCardLerpSpeed : combat::kDefaultLerpSpeed;
      float t = glm::clamp(delta_time_seconds * lerp_speed, 0.0f, 1.0f);

      vc.current_transform.position = glm::mix(vc.current_transform.position,
                                               vc.target_transform.position, t);
      vc.current_transform.scale =
          glm::mix(vc.current_transform.scale, vc.target_transform.scale, t);
      vc.current_transform.rotation = glm::mix(vc.current_transform.rotation,
                                               vc.target_transform.rotation, t);
    }

    if (glm::distance(vc.current_transform.position,
                      vc.target_transform.position) > 1.0f) {
      any_moving = true;
    }
  }

  if (any_moving) {
    core::effects::VisualBlocker::Get().AddBlocker("HandAnimation_" +
                                                   std::to_string(player_id_));
  } else {
    core::effects::VisualBlocker::Get().RemoveBlocker(
        "HandAnimation_" + std::to_string(player_id_));
  }
}

void HandController::DrawTargetingLine() {
  if (is_picking_target_ && held_card_index_) {
    const auto& held_card = hand_visuals_[*held_card_index_];
    glm::vec2 start_pos = held_card.current_transform.position;
    glm::vec2 end_pos = engine::InputManager::Get().mouse_screen_pos();

    auto& queue = engine::graphics::utils::RenderQueue::Default();
    engine::graphics::utils::RenderCommand cmd;
    cmd.shape_type = engine::graphics::utils::ShapeType::kLine;
    cmd.position = start_pos;
    cmd.size = end_pos;
    cmd.color = {0.0f, 1.0f, 1.0f, 1.0f}; // Cyan for spells
    cmd.thickness = 5.0f;
    cmd.z_order = combat::kTargetingLineZ;

    queue.Submit(cmd);
  }
}

std::optional<core::effects::Target> HandController::FindTargetAt(
    const core::state::GameState& state, glm::vec2 mouse_pos,
    combat::HitboxManager* hitbox_manager) {
  auto& config = core::GameConfig::Get();
  float icon_size = config.window_width * 0.05f;
  float icon_top = icon_size;

  glm::vec2 enemy_health_pos = {
      config.window_width * 0.5f,
      config.window_height - icon_top + icon_size * 0.5f};
  glm::vec2 player_health_pos = {config.window_width * 0.5f,
                                 icon_top - icon_size * 0.5f};

  if (core::util::PointInRect(mouse_pos, enemy_health_pos,
                              {icon_size, icon_size}, true)) {
    return core::effects::Target{core::effects::Target::Type::kEnemy, 1};
  } else if (core::util::PointInRect(mouse_pos, player_health_pos,
                                    {icon_size, icon_size}, true)) {
    return core::effects::Target{core::effects::Target::Type::kPlayer, 0};
  }

  // Check creatures on board using HitboxManager if available
  if (hitbox_manager) {
    if (auto hitbox = hitbox_manager->GetHitboxAt(mouse_pos)) {
      return core::effects::Target{core::effects::Target::Type::kCreature,
                                   hitbox->instance_id};
    }
  }

  return std::nullopt;
}

void HandController::Render() {
  DrawTargetingLine();

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
    float z_index = combat::kHandZ + static_cast<float>(i);
    if (is_face_down_) {
      core::graphics::CardRenderer::RenderCardBack(
          vc.current_transform.position, vc.current_transform.scale.x, 1.0f,
          vc.current_transform.rotation, z_index);
    } else {
      core::graphics::CardRenderer::RenderCard(
          vc.data, vc.current_transform.position, vc.current_transform.scale.x,
          1.0f, vc.current_transform.rotation, z_index);
    }
  }

  if (last_to_render && *last_to_render < hand_visuals_.size()) {
    const auto& vc = hand_visuals_[*last_to_render];
    // Render last_to_render on top of everything else in the hand.
    float z_index = combat::kHandZ + 100.0f;
    if (is_face_down_) {
      core::graphics::CardRenderer::RenderCardBack(
          vc.current_transform.position, vc.current_transform.scale.x, 1.0f,
          vc.current_transform.rotation, z_index);
    } else {
      core::graphics::CardRenderer::RenderCard(
          vc.data, vc.current_transform.position, vc.current_transform.scale.x,
          1.0f, vc.current_transform.rotation, z_index);
    }
  }
}

}  // namespace scenes::controllers
