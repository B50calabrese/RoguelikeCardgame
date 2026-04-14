#include "core/card_registry.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "engine/graphics/renderer.h"
#include "engine/util/asset_manager.h"
#include "engine/util/logger.h"
#include "pugixml.hpp"
#include "core/effects/effect.h"
#include "core/effects/effect_registry.h"

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

CreatureType StringToCreatureType(const std::string& str) {
  if (str == "Human") return CreatureType::Human;
  if (str == "Soldier") return CreatureType::Soldier;
  if (str == "Knight") return CreatureType::Knight;
  if (str == "Cleric") return CreatureType::Cleric;
  if (str == "Elf") return CreatureType::Elf;
  if (str == "Warrior") return CreatureType::Warrior;
  if (str == "Dwarf") return CreatureType::Dwarf;
  if (str == "Elemental") return CreatureType::Elemental;
  if (str == "Cat") return CreatureType::Cat;
  if (str == "Wizard") return CreatureType::Wizard;
  if (str == "Angel") return CreatureType::Angel;
  if (str == "Avatar") return CreatureType::Avatar;
  if (str == "Merfolk") return CreatureType::Merfolk;
  if (str == "Rogue") return CreatureType::Rogue;
  if (str == "Sphinx") return CreatureType::Sphinx;
  if (str == "Zombie") return CreatureType::Zombie;
  if (str == "Vampire") return CreatureType::Vampire;
  if (str == "Demon") return CreatureType::Demon;
  if (str == "Placeholder") return CreatureType::Placeholder;
  if (str == "Spell") return CreatureType::Placeholder; // Map Spell subtype to Placeholder for now
  return CreatureType::None;
}

std::string CreatureTypeToString(CreatureType type) {
  switch (type) {
    case CreatureType::Human: return "Human";
    case CreatureType::Soldier: return "Soldier";
    case CreatureType::Knight: return "Knight";
    case CreatureType::Cleric: return "Cleric";
    case CreatureType::Elf: return "Elf";
    case CreatureType::Warrior: return "Warrior";
    case CreatureType::Dwarf: return "Dwarf";
    case CreatureType::Elemental: return "Elemental";
    case CreatureType::Cat: return "Cat";
    case CreatureType::Wizard: return "Wizard";
    case CreatureType::Angel: return "Angel";
    case CreatureType::Avatar: return "Avatar";
    case CreatureType::Merfolk: return "Merfolk";
    case CreatureType::Rogue: return "Rogue";
    case CreatureType::Sphinx: return "Sphinx";
    case CreatureType::Zombie: return "Zombie";
    case CreatureType::Vampire: return "Vampire";
    case CreatureType::Demon: return "Demon";
    case CreatureType::Placeholder: return "Placeholder";
    default: return "";
  }
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

      // Parse Subtypes and generate TypeLine
      for (pugi::xml_node subtype_node = card_node.child("Subtype"); !subtype_node.empty();
           subtype_node = subtype_node.next_sibling("Subtype")) {
        card.subtypes.push_back(StringToCreatureType(subtype_node.text().as_string()));
      }

      // Backward compatibility for <Subtypes> (comma separated)
      if (card.subtypes.empty()) {
        std::string subtypes_str = card_node.child("Subtypes").text().as_string();
        if (!subtypes_str.empty()) {
          std::stringstream ss(subtypes_str);
          std::string subtype;
          while (std::getline(ss, subtype, ',')) {
            size_t first = subtype.find_first_not_of(' ');
            if (std::string::npos != first) {
              size_t last = subtype.find_last_not_of(' ');
              card.subtypes.push_back(StringToCreatureType(subtype.substr(first, (last - first + 1))));
            }
          }
        }
      }

      if (!card.subtypes.empty()) {
        for (size_t i = 0; i < card.subtypes.size(); ++i) {
          card.type_line += CreatureTypeToString(card.subtypes[i]);
          if (i < card.subtypes.size() - 1) {
            card.type_line += " ";
          }
        }
      } else {
        card.type_line = card_node.child("TypeLine").text().as_string();
      }

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

      card.description = GenerateDescription(card.type, card.effects);

      std::string frame_path = card_node.child("Frame").text().as_string();
      if (frame_path.empty()) {
        // Auto-determine frame path based on color and type
        std::string color_str = "colorless";
        switch (card.color) {
          case CardColor::White: color_str = "white"; break;
          case CardColor::Blue: color_str = "blue"; break;
          case CardColor::Black: color_str = "black"; break;
          case CardColor::Red: color_str = "red"; break;
          case CardColor::Green: color_str = "green"; break;
          default: color_str = "gray"; break;
        }
        std::string type_suffix = (card.type == CardType::Creature) ? "creature" : "noncreature";
        frame_path = color_str + "_" + type_suffix + "_frame.png";
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

std::string CardRegistry::GenerateDescription(
    CardType type, const std::vector<CardEffectDefinition>& effects) const {
  std::string description = "";
  for (const auto& effect_def : effects) {
    auto effect =
        effects::EffectRegistry::Get().CreateEffect(effect_def.effect_type);
    if (effect) {
      std::string effect_desc = effect->GetDescription(effect_def.params);
      std::string prefix = "";

      if (type == CardType::Creature) {
        switch (effect_def.trigger) {
          case Trigger::OnPlay: prefix = "Battlecry: "; break;
          case Trigger::OnDeath: prefix = "Deathrattle: "; break;
          case Trigger::AtStartOfTurn: prefix = "At the start of your turn, "; break;
          case Trigger::AtEndOfTurn: prefix = "At the end of your turn, "; break;
          default: break;
        }
      } else if (type == CardType::Spell) {
        // Usually no prefix for spells unless they are end of turn etc
        if (effect_def.trigger == Trigger::AtEndOfTurn) {
          prefix = "At the end of your turn, ";
        } else if (effect_def.trigger == Trigger::AtStartOfTurn) {
          prefix = "At the start of your turn, ";
        }
      }

      if (!description.empty()) {
        description += " ";
      }
      description += prefix + effect_desc;
    }
  }
  return description;
}

}  // namespace core
