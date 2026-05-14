# Project Status

## Most Recent Changes
- **Refactoring & Modularity**: significantly improved codebase architecture and modularity.
    - **Naming Collision Resolution**: Renamed the `scenes::CombatState` enum to `scenes::CombatUIState` to eliminate confusion with the core `core::state::CombatState` structure.
    - **Modular Effect Registration**: Moved game effect registration from `main.cpp` to a dedicated `core::effects::EffectInitializer`, centralizing game-logic bootstrapping.
    - **UI & Layout Decoupling**: Extracted board layout calculation logic from `CombatScene` into a reusable `core::util::GraphicsUtil` class.
    - **Spell Visuals Refactor**: Modularized spell visual effect management into a dedicated `scenes::combat::SpellVisualManager` class.
- **Cleanup**:
    - Removed redundant card loading in `CombatScene` as it is already handled globally.
    - Eliminated dead code, including unused hitbox cache members in `CombatScene`.
    - Centralized color vector mapping in `GraphicsUtil`.
- **Scene Stitching & Flow**: Fully integrated the game's core loop: `NewRunScene` -> `MapScene` -> `CombatScene`.
    - **Persistence**: Player health and deck now persist across scenes via the global `core::GameState` singleton.
- **Combat Initialization Overhaul**: `CombatScene` now pulls its initial state directly from the persistent `GameState`.
- **Map Scene Navigation (MVP)**: Implemented the "run navigation" system in `MapScene` with randomized scenario choices.

## Technical Status Report

### Scenes
- **MainMenuScene**: Functional entry point for the application.
- **NewRunScene**: Allows players to select starting colors and character. Properly initializes persistent run state.
- **MapScene**: Functional run navigation scene with randomized scenario choices.
- **CombatScene**: Integrated with persistent state. Handles encounter logic and modularized UI/Visual systems.
- **CardViewerScene**: Provides a grid view of all registered cards.

### Core Systems
- **GameState & CombatState**: Two-tier state management for persistent run data and temporary combat data.
- **Effect Initializer**: Centralized registry for all game-defined effects.
- **SpellVisualManager**: Independent system for managing spell play animations and visual blockers.
- **RulesEngine**: Singleton managing gameplay validation via pluggable `IRule` objects.
- **EventBus**: Facilitates decoupled communication between systems.
- **EffectResolver**: Manages the execution of game actions.

### UI & Visuals
- **GraphicsUtil**: Utility class for layout math and color mapping.
- **HandController & HandRenderer**: Manage fluid card animations and hand layout math.
- **BattleUI**: Renders the combat interface, including mana pools and board zones.
- **CardRenderer**: Central utility for rendering cards.

### Infrastructure
- **InputManager**: Handles mouse and keyboard input.
- **SceneManager**: Manages scene transitions using a deferred mechanism.
- **Engine Z-Sorting Support**: Layering hierarchy strictly enforced via `RenderQueue`.

## Recommendations & Next Steps

### Technical Improvements
- **Automated Testing Expansion**: Increase unit test coverage for the newly created `GraphicsUtil` and `SpellVisualManager`.
- **Map State Persistence**: Extend `GameState` to track the player's current position on the map.
- **Engine Linker Resolution**: Standardize the search paths for GLFW and OpenGL in the build system to improve sandbox compatibility.

### Feature Development
- **Non-Battle Scenarios**: Implement the specific scenes/logic for Random Event, Shop, and Treasure choices in the `MapScene`.
- **Node-Based Map**: Transition from simple scenario choices to a persistent node-based map graph.
- **Character Perks**: Introduce unique starting stats or passive abilities for the Warrior, Mage, and Rogue archetypes.
- **AI Enhancement**: Expand `SimpleAI` to support non-creature spell targeting and more complex tactical decisions.
