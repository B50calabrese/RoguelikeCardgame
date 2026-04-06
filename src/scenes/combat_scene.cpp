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
#include "core/controllers/hand_controller.h"
#include "core/game_config.h"
#include "core/graphics/card_renderer.h"
#include "core/graphics/hand_renderer.h"
#include "core/effects/actions/play_card_action.h"
#include "core/util/math_util.h"
#include "engine/input/input_manager.h"
#include "engine/scene/scene_manager.h"
#include "engine/util/console.h"
#include "engine/util/logger.h"
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

  RegisterCommands();

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
  border_thickness_ = config.window_width * 0.05f;
  icon_size_ = config.window_width * 0.1f;
  icon_top_ = border_thickness_ + icon_size_;
  enemy_icon_bottom_ = config.window_height - border_thickness_ - icon_size_;

  hand_bounds_size_ = {static_cast<float>(config.window_width) * 0.8f,
                       static_cast<float>(config.window_height) * 0.4f};
  player_hand_pos_ = {
      (static_cast<float>(config.window_width) - hand_bounds_size_.x) * 0.5f,
      icon_top_ + 20.0f};
  enemy_hand_pos_ = {
      (static_cast<float>(config.window_width) - hand_bounds_size_.x) * 0.5f,
      enemy_icon_bottom_ - hand_bounds_size_.y - 20.0f};

  player_hand_ = std::make_unique<core::controllers::HandController>(game_state_.player->id);
  player_hand_->SetBounds(player_hand_pos_, hand_bounds_size_);
  player_hand_->SetArcAngle(core::graphics::kDefaultArcAngle);
  player_hand_->SetInteractive(true);
  player_hand_->SetFaceDown(false);

  enemy_hand_ = std::make_unique<core::controllers::HandController>(game_state_.enemy->id);
  enemy_hand_->SetBounds(enemy_hand_pos_, hand_bounds_size_);
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

void CombatScene::RegisterCommands() {
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
  console.RegisterCommand("set_mana", [this](const std::vector<std::string>& args) {
    this->SetManaCommand(args);
  });
  console.RegisterCommand("set_turn", [this](const std::vector<std::string>& args) {
    this->SetTurnCommand(args);
  });
  console.RegisterCommand("play_card", [this](const std::vector<std::string>& args) {
    this->PlayCardCommand(args);
  });
}

void CombatScene::AddCardCommand(const std::vector<std::string>& args) {
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

void CombatScene::RemoveCardCommand(const std::vector<std::string>& args) {
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

void CombatScene::ListCardsCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  const auto& all_cards = core::CardRegistry::Get().GetAllCards();
  console.Log("Available Cards:");
  for (const auto& [id, data] : all_cards) {
    console.Log("  ID: " + std::to_string(id) + " - " + data.name);
  }
}

void CombatScene::SetHealthCommand(const std::vector<std::string>& args) {
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

void CombatScene::DrawCardCommand(const std::vector<std::string>& args) {
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

void CombatScene::SetManaCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.size() < 2) {
    console.Log("Usage: set_mana <player|enemy> <amount>");
    return;
  }
  std::string target = args[0];
  int amount = std::stoi(args[1]);
  if (target == "player") {
    game_state_.player->mana = amount;
    console.Log("Player mana set to " + std::to_string(amount));
  } else {
    game_state_.enemy->mana = amount;
    console.Log("Enemy mana set to " + std::to_string(amount));
  }
}

void CombatScene::SetTurnCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.empty()) {
    console.Log("Usage: set_turn <player|enemy>");
    return;
  }
  std::string target = args[0];
  game_state_.current_turn_player_id = (target == "player") ? 0 : 1;
  console.Log("Current turn set to " + target);
}

void CombatScene::PlayCardCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.size() < 2) {
    console.Log("Usage: play_card <player|enemy> <instance_id>");
    return;
  }
  int player_id = (args[0] == "player") ? 0 : 1;
  int instance_id = std::stoi(args[1]);

  auto action = std::make_shared<core::effects::actions::PlayCardAction>(player_id, instance_id, std::vector<core::effects::Target>());
  core::effects::EffectResolver::Get().QueueAction(action);
  core::effects::EffectResolver::Get().ProcessQueue(game_state_);
}

}  // namespace scenes
