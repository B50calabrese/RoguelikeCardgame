#include "core/card_registry.h"

#include <filesystem>
#include <iostream>

#include "engine/graphics/renderer.h"
#include "engine/util/asset_manager.h"
#include "engine/util/logger.h"
#include "pugixml.hpp"

namespace core {

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
      if (!card_node) {
        LOG_ERR("[CardRegistry] No <Card> root in %s",
                entry.path().string().c_str());
        overall_success = false;
        if (fail_closed) return false;
        continue;
      }

      CardData card;
      card.id = card_node.attribute("id").as_int();
      card.name = card_node.child_value("Name");
      card.description = card_node.child_value("Description");
      card.cost = card_node.child("Cost").text().as_int();
      card.power = card_node.child("Power").text().as_int();
      card.health = card_node.child("Health").text().as_int();

      // Parse Effects
      for (pugi::xml_node effect_node = card_node.child("Effect"); effect_node;
           effect_node = effect_node.next_sibling("Effect")) {
        CardEffectDefinition effect_def;
        std::string trigger_str = effect_node.attribute("trigger").as_string();
        if (trigger_str == "OnPlay") {
          effect_def.trigger = Trigger::OnPlay;
        } else if (trigger_str == "OnDeath") {
          effect_def.trigger = Trigger::OnDeath;
        } else if (trigger_str == "AtStartOfTurn") {
          effect_def.trigger = Trigger::AtStartOfTurn;
        } else if (trigger_str == "AtEndOfTurn") {
          effect_def.trigger = Trigger::AtEndOfTurn;
        } else {
          effect_def.trigger = Trigger::Manual;
        }

        effect_def.effect_type = effect_node.attribute("type").as_string();

        // Parse Params
        for (pugi::xml_node param_node = effect_node.child("Param"); param_node;
             param_node = param_node.next_sibling("Param")) {
          effect_def.params[param_node.attribute("name").as_string()] =
              param_node.attribute("value").as_string();
        }

        // Parse Filter
        pugi::xml_node filter_node = effect_node.child("TargetFilter");
        if (filter_node) {
          effect_def.filter.is_required =
              filter_node.attribute("required").as_bool(true);
          effect_def.filter.allow_creature =
              filter_node.attribute("allow_creature").as_bool(true);
          effect_def.filter.allow_player =
              filter_node.attribute("allow_player").as_bool(true);
          effect_def.filter.allow_enemy =
              filter_node.attribute("allow_enemy").as_bool(true);
          effect_def.filter.allow_self =
              filter_node.attribute("allow_self").as_bool(true);

          if (filter_node.attribute("max_cost")) {
            effect_def.filter.max_cost = filter_node.attribute("max_cost").as_int();
          }
          if (filter_node.attribute("min_power")) {
            effect_def.filter.min_power = filter_node.attribute("min_power").as_int();
          }
          // etc... add more stats as needed
        }

        card.effects.push_back(effect_def);
      }

      std::string frame_path = card_node.child_value("Frame");
      std::string art_path = card_node.child_value("Art");

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
        // Even if we fail open, we probably shouldn't register a card with
        // missing textures unless we have a fallback.
        continue;
      }

      card.frame_texture_id = frame_tex->renderer_id();
      card.art_texture_id = art_tex->renderer_id();

      // Keep them alive
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
