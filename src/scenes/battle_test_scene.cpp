#include "scenes/battle_test_scene.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <glm/vec2.hpp>

#include "core/card_registry.h"
#include "core/card_instance.h"
#include "core/constants.h"
#include "core/game_config.h"
#include "core/graphics/hand_renderer.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/logger.h"
#include "engine/util/console.h"
#include "engine/input/input_manager.h"

namespace scenes {

BattleTestScene::BattleTestScene() : engine::Scene("BattleTestScene") {}

void BattleTestScene::OnAttach() {
  LOG_INFO("[BattleTestScene] Attached");

  // Ensure cards are loaded
  core::CardRegistry::Get().LoadCardsFromDirectory("cards", false);

  RegisterCommands();
}

void BattleTestScene::OnDetach() {
  LOG_INFO("[BattleTestScene] Detached");
}

void BattleTestScene::RegisterCommands() {
  auto& console = engine::util::Console::Get();
  console.RegisterCommand("add_card", [this](const std::vector<std::string>& args) {
    this->AddCardCommand(args);
  });
  console.RegisterCommand("remove_card", [this](const std::vector<std::string>& args) {
    this->RemoveCardCommand(args);
  });
  console.RegisterCommand("list_cards", [this](const std::vector<std::string>& args) {
    this->ListCardsCommand(args);
  });
  console.RegisterCommand("set_health", [this](const std::vector<std::string>& args) {
    this->SetHealthCommand(args);
  });
  console.RegisterCommand("draw", [this](const std::vector<std::string>& args) {
    this->DrawCardCommand(args);
  });
}

void BattleTestScene::AddCardCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.size() < 3) {
    console.Log("Usage: add_card <player|enemy> <card_id> <hand|deck>");
    return;
  }

  std::string target_player = args[0];
  int card_id = std::stoi(args[1]);
  std::string target_zone = args[2];

  const core::CardData* data = core::CardRegistry::Get().GetCardById(card_id);
  if (!data) {
    console.Log("Error: Card ID " + std::to_string(card_id) + " not found.");
    return;
  }

  core::PlayerState* player = (target_player == "player") ? game_state_.player.get() : game_state_.enemy.get();
  int owner_id = (target_player == "player") ? 0 : 1;

  auto instance = std::make_unique<core::CardInstance>(data, game_state_.next_instance_id++, owner_id);

  if (target_zone == "hand") {
    instance->location = core::CardLocation::Hand;
    player->hand.push_back(std::move(instance));
    console.Log("Added " + data->name + " to " + target_player + "'s hand.");
  } else {
    instance->location = core::CardLocation::Deck;
    player->deck.push_back(std::move(instance));
    console.Log("Added " + data->name + " to " + target_player + "'s deck.");
  }
}

void BattleTestScene::RemoveCardCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.empty()) {
    console.Log("Usage: remove_card <instance_id>");
    return;
  }

  int instance_id = std::stoi(args[0]);
  bool found = false;

  auto remove_from_player = [&](core::PlayerState& p) {
    auto it = std::find_if(p.hand.begin(), p.hand.end(),
                           [instance_id](const auto& c) { return c->instance_id == instance_id; });
    if (it != p.hand.end()) {
      p.hand.erase(it);
      return true;
    }
    it = std::find_if(p.deck.begin(), p.deck.end(),
                      [instance_id](const auto& c) { return c->instance_id == instance_id; });
    if (it != p.deck.end()) {
      p.deck.erase(it);
      return true;
    }
    return false;
  };

  if (remove_from_player(*game_state_.player) || remove_from_player(*game_state_.enemy)) {
    console.Log("Removed card instance " + std::to_string(instance_id));
  } else {
    console.Log("Error: Card instance " + std::to_string(instance_id) + " not found.");
  }
}

void BattleTestScene::ListCardsCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  const auto& all_cards = core::CardRegistry::Get().GetAllCards();
  console.Log("Available Cards:");
  for (const auto& [id, data] : all_cards) {
    console.Log("  ID: " + std::to_string(id) + " - " + data.name);
  }
}

void BattleTestScene::SetHealthCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.size() < 2) {
    console.Log("Usage: set_health <player|enemy> <amount>");
    return;
  }
  std::string target = args[0];
  int amount = std::stoi(args[1]);
  if (target == "player") {
    game_state_.player->health = amount;
    console.Log("Player health set to " + std::to_string(amount));
  } else {
    game_state_.enemy->health = amount;
    console.Log("Enemy health set to " + std::to_string(amount));
  }
}

void BattleTestScene::DrawCardCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.empty()) {
    console.Log("Usage: draw <player|enemy>");
    return;
  }
  std::string target = args[0];
  core::PlayerState* player = (target == "player") ? game_state_.player.get() : game_state_.enemy.get();
  if (player->deck.empty()) {
    console.Log("Error: " + target + "'s deck is empty.");
    return;
  }
  auto card = std::move(player->deck.back());
  player->deck.pop_back();
  card->location = core::CardLocation::Hand;
  player->hand.push_back(std::move(card));
  console.Log(target + " drew a card.");
}

void BattleTestScene::OnUpdate(float delta_time_seconds) {
  engine::util::Console::Get().Update();

  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEscape)) {
    // For now, allow escaping back to menu if we're in the main app
    // But since this is a demo, it might just exit or stay
  }
}

void BattleTestScene::OnRender() {
  auto& config = core::GameConfig::Get();

  // Render Player's Hand
  std::vector<core::CardData> player_hand_data;
  for (const auto& inst : game_state_.player->hand) {
    player_hand_data.push_back(*(inst->data));
  }

  glm::vec2 p_bounds_size = {static_cast<float>(config.window_width) * 0.6f,
                             static_cast<float>(config.window_height) * 0.3f};
  glm::vec2 p_bounds_pos = {
      (static_cast<float>(config.window_width) - p_bounds_size.x) * 0.5f,
      20.0f
  };
  core::graphics::HandRenderer::RenderHand(player_hand_data, p_bounds_pos, p_bounds_size,
                                           core::graphics::kDefaultArcAngle, core::graphics::kDefaultOverlapFactor);

  // Render Enemy's Hand (Top of screen, flipped)
  std::vector<core::CardData> enemy_hand_data;
  for (const auto& inst : game_state_.enemy->hand) {
    enemy_hand_data.push_back(*(inst->data));
  }

  glm::vec2 e_bounds_size = {static_cast<float>(config.window_width) * 0.6f,
                             static_cast<float>(config.window_height) * 0.3f};
  glm::vec2 e_bounds_pos = {
      (static_cast<float>(config.window_width) - e_bounds_size.x) * 0.5f,
      static_cast<float>(config.window_height) - e_bounds_size.y - 20.0f
  };
  // Note: HandRenderer currently doesn't support flipped rendering easily,
  // but we can at least show the cards at the top for now.
  core::graphics::HandRenderer::RenderHand(enemy_hand_data, e_bounds_pos, e_bounds_size,
                                           -core::graphics::kDefaultArcAngle, core::graphics::kDefaultOverlapFactor);

  engine::util::Console::Get().Render();
}

}  // namespace scenes
