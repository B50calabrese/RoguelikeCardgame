#include "scenes/combat_commander.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "core/card_registry.h"
#include "core/card_instance.h"
#include "core/effects/effect_resolver.h"
#include "core/effects/actions/play_card_action.h"
#include "engine/util/console.h"

namespace scenes {

void CombatCommander::Register(core::GameState& state) {
  auto& console = engine::util::Console::Get();
  console.RegisterCommand("add_card", [&state](const std::vector<std::string>& args) {
    AddCardCommand(state, args);
  });
  console.RegisterCommand("remove_card", [&state](const std::vector<std::string>& args) {
    RemoveCardCommand(state, args);
  });
  console.RegisterCommand("list_cards", [](const std::vector<std::string>& args) {
    ListCardsCommand(args);
  });
  console.RegisterCommand("set_health", [&state](const std::vector<std::string>& args) {
    SetHealthCommand(state, args);
  });
  console.RegisterCommand("draw", [&state](const std::vector<std::string>& args) {
    DrawCardCommand(state, args);
  });
  console.RegisterCommand("set_mana", [&state](const std::vector<std::string>& args) {
    SetManaCommand(state, args);
  });
  console.RegisterCommand("set_turn", [&state](const std::vector<std::string>& args) {
    SetTurnCommand(state, args);
  });
  console.RegisterCommand("play_card", [&state](const std::vector<std::string>& args) {
    PlayCardCommand(state, args);
  });
}

void CombatCommander::AddCardCommand(core::GameState& state, const std::vector<std::string>& args) {
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

  core::PlayerState* player = (target_player == "player") ? state.player.get() : state.enemy.get();
  int owner_id = (target_player == "player") ? 0 : 1;

  auto instance = std::make_unique<core::CardInstance>(data, state.next_instance_id++, owner_id);

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

void CombatCommander::RemoveCardCommand(core::GameState& state, const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.empty()) {
    console.Log("Usage: remove_card <instance_id>");
    return;
  }

  int instance_id = std::stoi(args[0]);

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

  if (remove_from_player(*state.player) || remove_from_player(*state.enemy)) {
    console.Log("Removed card instance " + std::to_string(instance_id));
  } else {
    console.Log("Error: Card instance " + std::to_string(instance_id) + " not found.");
  }
}

void CombatCommander::ListCardsCommand(const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  const auto& all_cards = core::CardRegistry::Get().GetAllCards();
  console.Log("Available Cards:");
  for (const auto& [id, data] : all_cards) {
    console.Log("  ID: " + std::to_string(id) + " - " + data.name);
  }
}

void CombatCommander::SetHealthCommand(core::GameState& state, const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.size() < 2) {
    console.Log("Usage: set_health <player|enemy> <amount>");
    return;
  }
  std::string target = args[0];
  int amount = std::stoi(args[1]);
  if (target == "player") {
    state.player->health = amount;
    console.Log("Player health set to " + std::to_string(amount));
  } else {
    state.enemy->health = amount;
    console.Log("Enemy health set to " + std::to_string(amount));
  }
}

void CombatCommander::DrawCardCommand(core::GameState& state, const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.empty()) {
    console.Log("Usage: draw <player|enemy>");
    return;
  }
  std::string target = args[0];
  core::PlayerState* player = (target == "player") ? state.player.get() : state.enemy.get();
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

void CombatCommander::SetManaCommand(core::GameState& state, const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.size() < 2) {
    console.Log("Usage: set_mana <player|enemy> <amount>");
    return;
  }
  std::string target = args[0];
  int amount = std::stoi(args[1]);
  if (target == "player") {
    state.player->mana = amount;
    console.Log("Player mana set to " + std::to_string(amount));
  } else {
    state.enemy->mana = amount;
    console.Log("Enemy mana set to " + std::to_string(amount));
  }
}

void CombatCommander::SetTurnCommand(core::GameState& state, const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.empty()) {
    console.Log("Usage: set_turn <player|enemy>");
    return;
  }
  std::string target = args[0];
  state.current_turn_player_id = (target == "player") ? 0 : 1;
  console.Log("Current turn set to " + target);
}

void CombatCommander::PlayCardCommand(core::GameState& state, const std::vector<std::string>& args) {
  auto& console = engine::util::Console::Get();
  if (args.size() < 2) {
    console.Log("Usage: play_card <player|enemy> <instance_id>");
    return;
  }
  int player_id = (args[0] == "player") ? 0 : 1;
  int instance_id = std::stoi(args[1]);

  auto action = std::make_shared<core::effects::actions::PlayCardAction>(player_id, instance_id, std::vector<core::effects::Target>());
  core::effects::EffectResolver::Get().QueueAction(action);
  core::effects::EffectResolver::Get().ProcessQueue(state);
}

}  // namespace scenes
