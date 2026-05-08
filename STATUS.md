# Project Status

## Most Recent Changes
- **Game State Architecture Refactor**: Separated persistent run-level state from encounter-specific combat state.
    - **CombatState**: Renamed the original `GameState` to `CombatState` to better reflect its role in tracking individual combat encounters (health, mana, cards in zones).
    - **Run-Level GameState**: Introduced a new `core::GameState` singleton to track progress across an entire run, including chosen character, selected color identity, and the player's current deck.
- **Run Initialization Flow**: Properly implemented the transition from character/color selection to the main game loop.
    - **Character Types**: Added a `CharacterType` enum with Warrior, Mage, and Rogue archetypes.
    - **Dynamic Deck Building**: The `NewRunScene` now populates the player's starting deck based on their color choices, using the first 10 cards from each selected color (e.g., IDs 1-10 for White).
    - **Run Setup**: Clicking "Start Run" now resets the run state, saves player selections, and initializes the starting deck.
- **Map Scene Implementation**: Added a placeholder `MapScene` as the destination after starting a new run.
    - **Scene Transition**: Replaced the "Close Window" behavior of the Start Run button with a proper transition to the map.
    - **Navigation**: Included basic navigation back to the Main Menu from the Map Scene for testing.
- **Simple AI Implementation**: Implemented a "simple" AI logic for the enemy.
    - **Decision Making**: The AI now plays the highest-cost creature card it can afford each turn, picking randomly among ties.
    - **Combat Logic**: Follows a strict attack priority (Blockers > Other Creatures > Player Health) and targets the lowest health creature when attacking creatures. It now leverages the `RulesEngine` for target validation.
    - **Turn Choreography**: Actions are executed with a 1-second delay between them for better readability, and the AI correctly waits for the action queue to clear.
- **Combat Deck Initialization**: Updated the `CombatScene` to use more structured deck rules via a new `GameSetup` utility.
    - **Deck Composition**: Decks are now 20 cards each, consisting of the first 10 cards of the player's and enemy's respective color pairs (e.g., IDs 1-10 and 21-30 for White/Blue).
    - **Starting Conditions**: Implemented deck shuffling and a 5-card starting hand draw.
    - **Mana Balancing**: Set starting mana to 1 for the player and 0 for the enemy (with the enemy gaining their first mana at the start of their first turn).
- **GameState Enhancements**: Added `GetPlayerById` and `GetOpponentOf` helper methods to `GameState` to simplify player state access.
- **Google C++ Style Compliance**: Performed a comprehensive refactor of the `include/` and `src/` directories to adhere to the Google C++ Style Guide.
- **Combat Scene UI Refinement**: Overhauled the combat scene layout and interaction.
- **Standardized Base Card Set**: Implemented a new color-coded ID system and naming convention for cards.
- **Submodule Fixes**: Added `z_index` support to the `GameEngine` submodule's `Renderer` class and converted drawing methods to use the sorted `RenderQueue`.
- **Established Project Tracking System**: Created `STATUS.md` and updated `AGENTS.md` with maintenance directives.
- **Dedicated Card Art System**: Centralized card art assets and simplified their management.
- **Mage Apprentice Card Art**: Implemented unique art for the five Mage Apprentice cards.
- **Iconic Creature Card Art**: Implemented unique art for the five high-level iconic creatures.
- **Non-Creature Spell Functionality**: Implemented the foundational systems for playing and resolving spell cards.
- **Keyword Abilities (Haste & Blocker)**: Implemented "Haste" and "Blocker" as a hybrid of `Effect` classes and `CardInstance` flags.

## Technical Status Report

### Scenes
- **MainMenuScene**: Functional entry point for the application.
- **NewRunScene**: Allows players to select starting colors and character. Now properly initializes the run state.
- **MapScene**: A placeholder scene that serves as the main hub after starting a run.
- **CombatScene**: The primary gameplay loop. Orchestrates `HandController`, `BattleUI`, and board state. Currently uses `CombatState` for encounter logic.
- **CardViewerScene**: Provides a grid view of all registered cards.

### Core Systems
- **GameState & CombatState**: Two-tier state management for persistent run data and temporary combat data.
- **RulesEngine**: Singleton managing gameplay validation via pluggable `IRule` objects.
- **EventBus**: Facilitates decoupled communication between systems.
- **EffectResolver**: Manages the execution of game actions. Uses a `VisualBlocker` system for animation synchronization.

### UI & Visuals
- **HandController & HandRenderer**: Manage fluid card animations and hand layout math.
- **BattleUI**: Renders the combat interface, including mana pools and board zones.
- **HealthIcon**: Component-based health tracking for player and enemy.
- **TextRenderer**: Singleton for font management and UI text rendering.

### Infrastructure
- **InputManager**: Handles mouse and keyboard input.
- **SceneManager**: Manages scene transitions using a deferred mechanism.
- **Engine Z-Sorting Support**: The layering hierarchy is strictly enforced across all rendering systems.

## Recommendations & Next Steps

### Technical Improvements
- **Combat Initialization Integration**: Update `CombatScene` to pull player deck and color data from the new `GameState` singleton instead of hardcoding them.
- **Map Functionality**: Replace the placeholder `MapScene` with a node-based map system for run progression.
- **Engine Linker Resolution**: Standardize the search paths for GLFW and OpenGL in the build system to improve sandbox compatibility.

### Feature Development
- **Character Perks**: Introduce unique starting stats or passive abilities for the Warrior, Mage, and Rogue archetypes.
- **AI Enhancement**: Expand `SimpleAI` to support non-creature spell targeting and more complex tactical decisions.
- **Audio Integration**: Bootstrap an audio system for sound effects and music.
