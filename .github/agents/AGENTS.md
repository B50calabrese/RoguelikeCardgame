---
name: mage_tower_agent
description: Expert developer for the Mage Tower rogue-like deck builder game.
---

# Mage Tower: Rogue-like Deck Builder

## Overview
Mage Tower is a strategic rogue-like deck building game inspired by Hearthstone, Magic: The Gathering (MTG), and Slay the Spire.
- **Board & Mana**: Adopts a board setup and mana system similar to Hearthstone.
- **Card Design**: Utilizes a "color pie" philosophy for card mechanics and themes, drawing from MTG.
- **Progression**: Features the rogue-like progression and deck-building mechanics of Slay the Spire.

## Architecture
The game is built on a custom C++ game engine, organized into the following core components:

### Core Components
- **CardRegistry (`core::CardRegistry`)**: A singleton that manages all card definitions. Cards are loaded from XML files located in `assets/cards/`.
- **CardData (`core::CardData`)**: Represents the static data for a card (cost, power, health, etc.).
- **GameConfig (`core::GameConfig`)**: Singleton for managing game-wide settings and preferences.
- **Scenes**: The game uses a scene-based architecture (e.g., `MainMenuScene`, `CombatScene`, `CardViewerScene`) managed by the `engine::scene::SceneManager`.

### Graphics & UI
- **CardRenderer**: Specialized utility for rendering cards with support for hovering and scaling.
- **HandRenderer**: Utility to render a collection of cards in an arc, fitting them within a defined bounding box.
- **UIButton**: Reusable UI component for clickable elements.

### Third-Party Libraries
- **pugixml**: Used for parsing card definitions from XML.
- **GLM**: Mathematics library for vectors and matrices.

## Game Engine
The game uses a custom engine located in the `GameEngine/` submodule.
- **Include Path**: `GameEngine/include/`
- **Namespaces**: Primarily under `engine::` (e.g., `engine::graphics`, `engine::scene`, `engine::input`).
- **Engine Documentation**: Refer to the source headers in `GameEngine/include/engine/` for detailed API information.

## Code Specifications
- **Style Guide**: Adhere strictly to the **Google C++ Style Guide**.
- **Naming Conventions**:
  - `PascalCase` for functions, classes, and types.
  - `snake_case` for variables and parameters.
  - `snake_case_` for private/protected class members.
  - `kCamelCase` for constants.
- **Header Guards**: Use the format `DECK_BUILDER_GAME_<PATH>_<FILE>_H_`.
- **Include Order**: Related header, C system headers, C++ system headers, other library headers (GLM, pugixml), project headers.

## Scripting & Extensibility
### Lua Scripting for Effects and Actions
The game engine supports Lua scripting via the `engine::util::ScriptManager`. This can be leveraged to move card effects and game actions from C++ into Lua scripts, allowing for faster iteration and easier modding.

#### High-Level Proposal
1.  **Lua Effect Wrapper**: Create a C++ class `LuaEffect` that implements the `core::effects::Effect` interface. This class would hold a reference to a Lua function or table.
2.  **Registry Binding**: Bind `core::effects::EffectRegistry` to Lua so that new effect types can be registered directly from Lua scripts.
3.  **Action Binding**: Bind the `Action` system and `GameState` to Lua using `sol2` (which the engine already uses). This allows Lua scripts to inspect the game state and return a list of actions.
4.  **Scripted Cards**: Update the XML schema to allow specifying a `.lua` file for an effect instead of a built-in type.

#### Example Implementation
**C++ Binding (in `EffectRegistry` or a dedicated binder):**
```cpp
void BindEffectsToLua(sol::state& lua) {
  lua.new_usertype<core::effects::Target>("Target",
    "type", &core::effects::Target::type,
    "id", &core::effects::Target::id
  );

  // Bind other necessary types (GameState, Action, etc.)
}
```

**Lua Effect Definition (`assets/scripts/effects/burn.lua`):**
```lua
-- Define a new effect in Lua
function GenerateBurnActions(source_id, targets, params)
    local actions = {}
    local damage_amount = tonumber(params["amount"]) or 0

    for _, target in ipairs(targets) do
        -- Assuming a 'CreateDamageAction' helper is bound to Lua
        table.insert(actions, CreateDamageAction(source_id, target, damage_amount))
    end

    return actions
end
```

### Self-Evolution & Knowledge Management
- **Updating AGENTS.md**: When significant architectural changes or new core components are added, update this file to reflect the current state of the game.
- **Adding Skills**: If a recurring task is identified (e.g., adding a new type of game entity), create a new skill in `.github/skills/<skill-name>/SKILL.md`.
- **Skill Maintenance**: Keep existing skills updated as the underlying APIs or requirements evolve.

### General Guidelines
- **Verify before Submit**: Always use the `pre_commit_instructions` tool and ensure all verification steps pass.
- **Edit Source, Not Artifacts**: Never modify files in `build/` or `dist/` directories.
- **Proactive Testing**: Write and run tests for any new logic added to `core` or `scenes`.
