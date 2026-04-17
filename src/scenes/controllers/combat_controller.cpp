#include "scenes/controllers/combat_controller.h"
#include "core/effects/visual_blocker.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/creature_attack_action.h"
#include "core/game_config.h"
#include "core/util/math_util.h"
#include "engine/input/input_manager.h"
#include "engine/util/logger.h"

namespace scenes::controllers {

void CombatController::Update(float delta_time, core::state::GameState& state, const std::vector<BoardHitbox>& hitboxes, float icon_top, float icon_size) {
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

void CombatController::HandleInput(core::state::GameState& state, const std::vector<BoardHitbox>& hitboxes, float icon_top, float icon_size) {
    if (core::effects::VisualBlocker::Get().IsBlocking()) return;

    auto& input = engine::InputManager::Get();
    glm::vec2 mouse_pos = input.mouse_screen_pos();

    if (input.IsKeyPressed(engine::KeyCode::kMouseLeft)) {
        if (current_state_ == CombatState::Idle) {
            for (const auto& hitbox : hitboxes) {
                if (!hitbox.is_enemy && core::util::PointInRect(mouse_pos, hitbox.position, hitbox.size, true)) {
                    auto* inst = state.FindCardInstance(hitbox.instance_id);
                    if (inst && inst->can_attack && !inst->has_attacked && state.current_turn_player_id == inst->owner_id) {
                        selected_attacker_id_ = hitbox.instance_id;
                        current_state_ = CombatState::PickingTarget;
                        LOG_INFO("[CombatController] Selected attacker %d", *selected_attacker_id_);
                        return;
                    }
                }
            }
        } else if (current_state_ == CombatState::PickingTarget) {
            bool target_found = false;
            for (const auto& hitbox : hitboxes) {
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

            if (!target_found) {
                auto& config = core::GameConfig::Get();
                glm::vec2 enemy_health_pos = {config.window_width * 0.5f, config.window_height - icon_top + icon_size * 0.5f};
                glm::vec2 icon_size_vec = {icon_size, icon_size};
                if (core::util::PointInRect(mouse_pos, enemy_health_pos, icon_size_vec, true)) {
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

void CombatController::OnCreatureAttacked(core::state::GameState& state, const core::effects::GameEvent& event, const std::vector<BoardHitbox>& hitboxes, float icon_top, float icon_size) {
    AttackAnimation anim;
    anim.attacker_id = event.source_id;
    anim.target.id = event.target_id;

    auto& config = core::GameConfig::Get();
    glm::vec2 target_pos(0.0f);
    if (event.target_id == -1) {
         if (state.current_turn_player_id == state.player->id) {
             target_pos = {config.window_width * 0.5f, config.window_height - icon_top + icon_size * 0.5f};
             anim.target.type = core::effects::Target::Type::Enemy;
         } else {
             target_pos = {config.window_width * 0.5f, icon_top - icon_size * 0.5f};
             anim.target.type = core::effects::Target::Type::Player;
         }
    } else {
        anim.target.type = core::effects::Target::Type::Creature;
        bool found = false;
        for (const auto& hitbox : hitboxes) {
            if (hitbox.instance_id == event.target_id) {
                target_pos = hitbox.position;
                found = true;
                break;
            }
        }
        if (!found) {
            LOG_ERR("[CombatController] Animation target creature %d not found in hitboxes!", event.target_id);
            target_pos = {config.window_width * 0.5f, config.window_height * 0.5f};
        }
    }

    for (const auto& hitbox : hitboxes) {
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

} // namespace scenes::controllers
