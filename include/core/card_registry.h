#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_CARD_REGISTRY_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_CARD_REGISTRY_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/card_data.h"
#include "engine/graphics/texture.h"

namespace core {

/**
 * @brief Singleton registry that manages all cards in the game.
 *
 * The CardRegistry is responsible for loading card definitions from XML files,
 * caching textures associated with those cards, and providing a central
 * access point for retrieving card metadata.
 */
class CardRegistry {
 public:
  /**
   * @brief Returns the singleton instance of the CardRegistry.
   * @return A reference to the static CardRegistry instance.
   */
  static CardRegistry& Get();

  /**
   * @brief Loads all card XML files from a directory.
   *
   * Iterates through the specified directory, parsing each XML file into
   * a CardData object. It also pre-loads and caches the textures
   * referenced by each card.
   *
   * @param directory Path to the directory containing XML files, relative to
   * the asset root.
   * @param fail_closed If true, the loading process stops immediately if any
   * card fails to load correctly. If false, it will continue to load other
   * cards and return success if at least some were loaded.
   * @return True if all files were loaded correctly (or if fail_closed is false
   * and at least some loaded), false otherwise.
   */
  bool LoadCardsFromDirectory(const std::string& directory,
                              bool fail_closed = false);

  /**
   * @brief Retrieves card data by its unique ID.
   * @param id The unique integer identifier of the card.
   * @return A pointer to the CardData object if found, or nullptr if no card
   * with the given ID exists in the registry.
   */
  const CardData* GetCardById(int id) const;

  /**
   * @brief Retrieves all registered cards in the game.
   * @return A constant reference to the internal map of card IDs to CardData
   * objects.
   */
  const std::unordered_map<int, CardData>& GetAllCards() const {
    return cards_;
  }

 private:
  CardRegistry() = default;
  ~CardRegistry() = default;
  CardRegistry(const CardRegistry&) = delete;
  CardRegistry& operator=(const CardRegistry&) = delete;

  std::unordered_map<int, CardData> cards_;
  // Keep textures alive as they are referenced by texture IDs in CardData.
  std::vector<std::shared_ptr<engine::graphics::Texture>> loaded_textures_;
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_CARD_REGISTRY_H_
