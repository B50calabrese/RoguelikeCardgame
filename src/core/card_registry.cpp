#include "core/card_registry.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "engine/graphics/renderer.h"
#include "engine/util/asset_manager.h"
#include "engine/util/logger.h"
#include "pugixml.hpp"
#include "core/effects/effect.h"

namespace core {

namespace {

Trigger StringToTrigger(const std::string& str) {
  if (str == "OnPlay") return Trigger::OnPlay;
  if (str == "OnDeath") return Trigger::OnDeath;
  if (str == "AtStartOfTurn") return Trigger::AtStartOfTurn;
  if (str == "AtEndOfTurn") return Trigger::AtEndOfTurn;
  return Trigger::Manual;
}

CardType StringToCardType(const std::string& str) {
  if (str == "Creature") return CardType::Creature;
  if (str == "Spell") return CardType::Spell;
  return CardType::Spell;
}

CardColor StringToCardColor(const std::string& str) {
  if (str == "White") return CardColor::White;
  if (str == "Blue") return CardColor::Blue;
  if (str == "Black") return CardColor::Black;
  if (str == "Red") return CardColor::Red;
  if (str == "Green") return CardColor::Green;
  return CardColor::Colorless;
}

void ParseTargetFilter(pugi::xml_node filter_node, effects::TargetFilter& filter) {
  if (filter_node.empty()) return;
  filter.is_required = filter_node.attribute("required").as_bool(true);
  filter.allow_creature = filter_node.attribute("allow_creature").as_bool(true);
  filter.allow_player = filter_node.attribute("allow_player").as_bool(true);
  filter.allow_enemy = filter_node.attribute("allow_enemy").as_bool(true);
  filter.allow_self = filter_node.attribute("allow_self").as_bool(true);

  if (filter_node.attribute("max_cost")) filter.max_cost = filter_node.attribute("max_cost").as_int();
  if (filter_node.attribute("min_cost")) filter.min_cost = filter_node.attribute("min_cost").as_int();
  if (filter_node.attribute("max_power")) filter.max_power = filter_node.attribute("max_power").as_int();
  if (filter_node.attribute("min_power")) filter.min_power = filter_node.attribute("min_power").as_int();
  if (filter_node.attribute("max_health")) filter.max_health = filter_node.attribute("max_health").as_int();
  if (filter_node.attribute("min_health")) filter.min_health = filter_node.attribute("min_health").as_int();
}

}  // namespace

CardRegistry& CardRegistry::Get() {
  static CardRegistry instance;
  return instance;
}

bool CardRegistry::LoadCardsFromDirectory(const std::string& directory,
                                          bool fail_closed) {
  std::string full_path =
      engine::graphics::Renderer::Get().ResolveAssetPath(directory);

  if (!std::filesystem::exists(full_path) ||
      !std::filesystem::is_directory(full_path)) {
    LOG_ERR("[CardRegistry] Directory does not exist: %s", full_path.c_str());
    return false;
  }

  bool overall_success = true;

  for (const auto& entry : std::filesystem::directory_iterator(full_path)) {
    if (entry.path().extension() == ".xml") {
      pugi::xml_document doc;
      pugi::xml_parse_result result = doc.load_file(entry.path().c_str());

      if (!result) {
        LOG_ERR("[CardRegistry] Failed to parse XML %s: %s",
                entry.path().string().c_str(), result.description());
        overall_success = false;
        if (fail_closed) return false;
        continue;
      }

      pugi::xml_node card_node = doc.child("Card");
      if (card_node.empty()) {
        LOG_ERR("[CardRegistry] No <Card> root in %s",
                entry.path().string().c_str());
        overall_success = false;
        if (fail_closed) return false;
        continue;
      }

      CardData card;
      card.id = card_node.attribute("id").as_int();
      card.name = card_node.child("Name").text().as_string();
      card.description = card_node.child("Description").text().as_string();
      card.type = StringToCardType(card_node.child("Type").text().as_string());
      card.color = StringToCardColor(card_node.child("Color").text().as_string());
      card.cost = card_node.child("Cost").text().as_int();
      card.power = card_node.child("Power").text().as_int();
      card.health = card_node.child("Health").text().as_int();

      // Parse Effects
      for (pugi::xml_node effect_node = card_node.child("Effect"); !effect_node.empty();
           effect_node = effect_node.next_sibling("Effect")) {
        CardEffectDefinition effect_def;
        effect_def.trigger = StringToTrigger(effect_node.attribute("trigger").as_string());
        effect_def.effect_type = effect_node.attribute("type").as_string();

        // Parse Params
        for (pugi::xml_node param_node = effect_node.child("Param"); !param_node.empty();
             param_node = param_node.next_sibling("Param")) {
          effect_def.params[param_node.attribute("name").as_string()] =
              param_node.attribute("value").as_string();
        }

        // Parse Filter
        ParseTargetFilter(effect_node.child("TargetFilter"), effect_def.filter);

        card.effects.push_back(effect_def);
      }

      std::string frame_path = card_node.child("Frame").text().as_string();
      // If frame path is missing or empty, use color-based default
      if (frame_path.empty()) {
        switch (card.color) {
          case CardColor::White: frame_path = "frame_white.png"; break;
          case CardColor::Blue: frame_path = "frame_blue.png"; break;
          case CardColor::Black: frame_path = "frame_black.png"; break;
          case CardColor::Red: frame_path = "frame_red.png"; break;
          case CardColor::Green: frame_path = "frame_green.png"; break;
          default: frame_path = "frame_colorless.png"; break;
        }
      }

      std::string art_path = card_node.child("Art").text().as_string();

      auto frame_tex =
          engine::util::AssetManager<engine::graphics::Texture>::Get(
              frame_path);
      auto art_tex =
          engine::util::AssetManager<engine::graphics::Texture>::Get(art_path);

      if (!frame_tex || !art_tex) {
        LOG_ERR("[CardRegistry] Failed to load textures for card %d in %s",
                card.id, entry.path().string().c_str());
        overall_success = false;
        if (fail_closed) return false;
        continue;
      }

      card.frame_texture_id = frame_tex->renderer_id();
      card.art_texture_id = art_tex->renderer_id();

      loaded_textures_.push_back(frame_tex);
      loaded_textures_.push_back(art_tex);

      cards_[card.id] = card;
      LOG_INFO("[CardRegistry] Loaded card: %s (ID: %d)", card.name.c_str(),
               card.id);
    }
  }

  return overall_success;
}

const CardData* CardRegistry::GetCardById(int id) const {
  auto it = cards_.find(id);
  if (it != cards_.end()) {
    return &it->second;
  }
  return nullptr;
}

}  // namespace core
