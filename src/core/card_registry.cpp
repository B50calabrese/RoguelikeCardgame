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
      card.power = card_node.child("Power").text().as_int();
      card.health = card_node.child("Health").text().as_int();

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

      card.frame_texture_id = frame_tex->id();
      card.art_texture_id = art_tex->id();

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
