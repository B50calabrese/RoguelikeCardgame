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
 */
class CardRegistry {
 public:
  static CardRegistry& Get();

  /**
   * @brief Loads all card XML files from a directory.
   * @param directory Path to the directory containing XML files.
   * @param fail_closed If true, loading stops on the first error.
   * @return True if all files were loaded (or if fail_closed is false and at
   * least some loaded), false otherwise.
   */
  bool LoadCardsFromDirectory(const std::string& directory,
                              bool fail_closed = false);

  /**
   * @brief Retrieves card data by ID.
   * @param id The card ID.
   * @return Pointer to CardData if found, nullptr otherwise.
   */
  const CardData* GetCardById(int id) const;

  /**
   * @brief Retrieves all registered cards.
   * @return A map of card ID to CardData.
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
  // Keep textures alive
  std::vector<std::shared_ptr<engine::graphics::Texture>> loaded_textures_;
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_CARD_REGISTRY_H_
