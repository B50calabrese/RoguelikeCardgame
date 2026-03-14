#include <iostream>
#include <string>

#include <GLFW/glfw3.h>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/util/logger.h>

#include "core/card_registry.h"

class CardRegistryDemo : public engine::Application {
 public:
  void OnInit() override {
    LOG_INFO("Card Registry Demo Initialized");

    // Load cards from the assets/cards directory
    if (core::CardRegistry::Get().LoadCardsFromDirectory("cards/", false)) {
      LOG_INFO("Successfully loaded cards from assets/cards/");
    } else {
      LOG_WARN("Some cards failed to load, or directory not found.");
    }

    std::cout << "\n--- Card Registry CLI Demo ---" << std::endl;
    std::cout << "Available IDs: ";
    for (const auto& [id, card] : core::CardRegistry::Get().cards()) {
      std::cout << id << " ";
    }
    std::cout << "\nEnter a card ID to view details (or 'q' to quit): "
              << std::endl;

    std::string input;
    while (true) {
      std::cout << "> ";
      if (!(std::cin >> input) || input == "q") {
        break;
      }

      try {
        int id = std::stoi(input);
        const core::CardData* card = core::CardRegistry::Get().GetCardById(id);
        if (card) {
          std::cout << "Card Found:" << std::endl;
          std::cout << "  ID:          " << card->id << std::endl;
          std::cout << "  Name:        " << card->name << std::endl;
          std::cout << "  Description: " << card->description << std::endl;
          std::cout << "  Power:       " << card->power << std::endl;
          std::cout << "  Health:      " << card->health << std::endl;
          std::cout << "  Frame Tex:   " << card->frame_texture_id << std::endl;
          std::cout << "  Art Tex:     " << card->art_texture_id << std::endl;
        } else {
          std::cout << "Card with ID " << id << " not found." << std::endl;
        }
      } catch (const std::invalid_argument&) {
        std::cout << "Invalid input. Please enter a numeric ID or 'q' to quit."
                  << std::endl;
      }
    }

    // Since this is a CLI demo intended to run once and exit in this
    // environment, we can request the window to close.
    glfwSetWindowShouldClose(engine::Engine::window().native_handle(),
                             GLFW_TRUE);
  }

  void OnUpdate(double deltaTimeSeconds) override {}
  void OnShutdown() override {}
};

int main() {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_width = 800;
  engine_config.window_height = 600;

  // We need to initialize the engine to have the Renderer (and AssetManager)
  // working. Note: This will create a window, which might not be ideal for a
  // pure CLI demo, but the engine's systems depend on it.
  engine::Engine::Init(engine_config);

  CardRegistryDemo app;
  app.Run();

  return 0;
}
