#include "scenes/effect_demo_scene.h"

#include <algorithm>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "core/card_registry.h"
#include "core/constants.h"
#include "core/game_config.h"
#include "core/effects/effect_registry.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/rules_engine.h"
#include "core/effects/types/damage_effect.h"
#include "core/effects/types/draw_effect.h"
#include "core/effects/types/stat_modify_effect.h"
#include "core/effects/actions/play_card_action.h"
#include "core/graphics/card_renderer.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/logger.h"
#include "scenes/main_menu_scene.h"
#include "engine/input/input_manager.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/text_renderer.h"

namespace scenes {

void EffectDemoScene::OnAttach() {
    LOG_INFO("[EffectDemoScene] Attached");

    // Register some effects for the demo
    core::effects::EffectRegistry::Get().RegisterEffect("Damage", []() { return std::make_unique<core::effects::types::DamageEffect>(); });
    core::effects::EffectRegistry::Get().RegisterEffect("Draw", []() { return std::make_unique<core::effects::types::DrawEffect>(); });
    core::effects::EffectRegistry::Get().RegisterEffect("Buff", []() { return std::make_unique<core::effects::types::StatModifyEffect>(); });

    // Load cards
    core::CardRegistry::Get().LoadCardsFromDirectory("assets/cards", false);

    InitializeGameState();
}

void EffectDemoScene::InitializeGameState() {
    const auto& all_cards = core::CardRegistry::Get().GetAllCards();
    if (all_cards.empty()) return;

    // Give player some cards in hand and deck
    auto it = all_cards.begin();
    for (int i = 0; i < 5 && it != all_cards.end(); ++i, ++it) {
        auto inst = std::make_unique<core::CardInstance>(&it->second, state_.next_instance_id++, 0);
        inst->location = core::CardLocation::Hand;
        state_.player->hand.push_back(std::move(inst));
    }

    // Put some in deck
    for (int i = 0; i < 10 && it != all_cards.end(); ++i, ++it) {
        auto inst = std::make_unique<core::CardInstance>(&it->second, state_.next_instance_id++, 0);
        inst->location = core::CardLocation::Deck;
        state_.player->deck.push_back(std::move(inst));
    }

    // Give enemy a creature on board for testing
    if (!all_cards.empty()) {
        auto enemy_inst = std::make_unique<core::CardInstance>(&all_cards.begin()->second, state_.next_instance_id++, 1);
        enemy_inst->location = core::CardLocation::Board;
        state_.enemy->board.push_back(std::move(enemy_inst));
    }
}

void EffectDemoScene::OnUpdate(float delta_time_seconds) {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_ESCAPE)) {
        engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
        return;
    }

    HandleInput();
    core::effects::EffectResolver::Get().ProcessQueue(state_);
}

void EffectDemoScene::HandleInput() {
    auto& input = engine::InputManager::Get();

    // Simple index-based card selection for demo (keys 1-9)
    for (int i = 0; i < 9; ++i) {
        if (input.IsKeyPressed(static_cast<engine::KeyCode>(static_cast<int>(engine::KeyCode::KC_1) + i))) {
            if (i < state_.player->hand.size()) {
                SelectCard(i);
            }
        }
    }

    // Simple target selection (P for Player, E for Enemy, T for first enemy creature)
    if (input.IsKeyPressed(engine::KeyCode::KC_P)) {
        SelectTarget({core::effects::Target::Type::Player, 0});
    }
    if (input.IsKeyPressed(engine::KeyCode::KC_E)) {
        SelectTarget({core::effects::Target::Type::Enemy, 1});
    }
    if (input.IsKeyPressed(engine::KeyCode::KC_T)) {
        if (!state_.enemy->board.empty()) {
            SelectTarget({core::effects::Target::Type::Creature, state_.enemy->board[0]->instance_id});
        }
    }
}

void EffectDemoScene::SelectCard(int index) {
    selected_hand_card_idx_ = index;
    core::CardInstance* inst = state_.player->hand[index].get();

    // Check if card can be played
    core::effects::RuleResult rule = core::effects::RulesEngine::CanPerformAction(state_, std::make_shared<core::effects::actions::PlayCardAction>(0, inst->instance_id, std::vector<core::effects::Target>{}));
    if (!rule.success) {
        status_message_ = "Cannot play card: " + rule.message;
        selected_hand_card_idx_ = -1;
        return;
    }

    // Find first 'OnPlay' effect that needs targeting
    pending_effect_def_ = nullptr;
    for (const auto& effect_def : inst->data->effects) {
        if (effect_def.trigger == core::effects::Trigger::OnPlay && effect_def.filter.is_required) {
            pending_effect_def_ = &effect_def;
            pending_source_id_ = inst->instance_id;
            selected_targets_.clear();
            status_message_ = "Select target for " + effect_def.effect_type + " (P/E/T)";
            return;
        }
    }

    // If no targeting needed, just play it
    core::effects::EffectResolver::Get().QueueAction(std::make_shared<core::effects::actions::PlayCardAction>(0, inst->instance_id, std::vector<core::effects::Target>{}));

    // Also trigger non-targeted OnPlay effects
    for (const auto& effect_def : inst->data->effects) {
        if (effect_def.trigger == core::effects::Trigger::OnPlay && !effect_def.filter.is_required) {
            auto effect = core::effects::EffectRegistry::Get().CreateEffect(effect_def.effect_type);
            if (effect) {
                auto actions = effect->GenerateActions(inst->instance_id, {}, effect_def.params);
                for (const auto& a : actions) core::effects::EffectResolver::Get().QueueAction(a);
            }
        }
    }

    status_message_ = "Played " + inst->data->name;
    selected_hand_card_idx_ = -1;
}

void EffectDemoScene::SelectTarget(const core::effects::Target& target) {
    if (!pending_effect_def_) return;

    // Validate target
    if (!pending_effect_def_->filter.IsValid(state_, 0, target)) {
        status_message_ = "Invalid target for " + pending_effect_def_->effect_type;
        return;
    }

    selected_targets_.push_back(target);

    // For now assume all effects need only 1 target
    auto effect = core::effects::EffectRegistry::Get().CreateEffect(pending_effect_def_->effect_type);
    if (effect) {
        // Queue the play card first
        core::effects::EffectResolver::Get().QueueAction(std::make_shared<core::effects::actions::PlayCardAction>(0, pending_source_id_, selected_targets_));

        // Then the effect
        auto actions = effect->GenerateActions(pending_source_id_, selected_targets_, pending_effect_def_->params);
        for (const auto& a : actions) core::effects::EffectResolver::Get().QueueAction(a);

        status_message_ = "Used " + pending_effect_def_->effect_type + " on target";
    }

    pending_effect_def_ = nullptr;
    selected_hand_card_idx_ = -1;
}

void EffectDemoScene::OnRender() {
    auto& config = core::GameConfig::Get();
    auto& renderer = engine::graphics::Renderer::Get();

    // Render status message
    renderer.DrawText("arial", status_message_, {20.0f, config.window_height - 40.0f}, 0.0f, 1.0f, {1, 1, 1, 1});

    // Render Hand
    float x_start = 150.0f;
    for (size_t i = 0; i < state_.player->hand.size(); ++i) {
        RenderCardInHand(*state_.player->hand[i], x_start + i * 180.0f, 150.0f);
    }

    // Render Player Board
    for (size_t i = 0; i < state_.player->board.size(); ++i) {
        RenderCreatureOnBoard(*state_.player->board[i], 300.0f + i * 220.0f, 400.0f);
    }

    // Render Enemy Board
    for (size_t i = 0; i < state_.enemy->board.size(); ++i) {
        RenderCreatureOnBoard(*state_.enemy->board[i], 300.0f + i * 220.0f, config.window_height - 300.0f);
    }

    RenderPlayerStats();
}

void EffectDemoScene::RenderCardInHand(const core::CardInstance& card, float x, float y) {
    float scale = 0.5f;
    core::graphics::CardRenderer::RenderCard(*card.data, {x, y}, scale);
}

void EffectDemoScene::RenderCreatureOnBoard(const core::CardInstance& card, float x, float y) {
    float scale = 0.6f;
    core::graphics::CardRenderer::RenderCard(*card.data, {x, y}, scale);

    // Render current health/power overlay
    std::string stats = std::to_string(card.current_power) + " / " + std::to_string(card.current_health);
    engine::graphics::Renderer::Get().DrawText("arial", stats, {x - 30.0f, y - 80.0f}, 0.0f, 0.8f, {1, 1, 0, 1});
}

void EffectDemoScene::RenderPlayerStats() {
    auto& config = core::GameConfig::Get();
    auto& renderer = engine::graphics::Renderer::Get();
    std::string p_stats = "Player Health: " + std::to_string(state_.player->health) + " Mana: " + std::to_string(state_.player->mana);
    std::string e_stats = "Enemy Health: " + std::to_string(state_.enemy->health);

    renderer.DrawText("arial", p_stats, {20.0f, 20.0f}, 0.0f, 0.8f, {0, 1, 0, 1});
    renderer.DrawText("arial", e_stats, {20.0f, config.window_height - 80.0f}, 0.0f, 0.8f, {1, 0, 0, 1});
}

} // namespace scenes
