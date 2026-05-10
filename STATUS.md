# Project Status

## Most Recent Changes
- **Scene Stitching & Flow**: Fully integrated the game's core loop: `NewRunScene` -> `MapScene` -> `CombatScene`.
    - **Persistence**: Player health and deck now persist across scenes via the global `core::GameState` singleton.
    - **Win/Loss Transitions**:
        - **Victory**: Defeating the enemy in `CombatScene` (enemy health <= 0) saves the player's current health back to `GameState` and returns them to the `MapScene`, which generates a new set of choices.
        - **Defeat**: If the player's health reaches 0, the program gracefully closes via `glfwSetWindowShouldClose`.
    - **Run Initialization**: `NewRunScene` now correctly sets the player's starting health based on their chosen character (Warrior: 35, Mage: 25, Rogue: 30) before entering the map.
- **Combat Initialization Overhaul**: `CombatScene` now pulls its initial state directly from the persistent `GameState`.
    - Added `GameSetup::SetupDeckFromIds` to reconstruct the combat deck from the ID list stored in the run state.
- **Map Scene Navigation (MVP)**: Implemented the "run navigation" system in `MapScene`.
    - **Scenario Choices**: The scene generates 2-5 randomized cards representing different encounter types: Battle (Red), Random Event (Purple), Shop (Gold), and Treasure (Cyan).
    - **Battle Transition**: Clicking a "Battle" choice card now correctly initiates a combat encounter.
- **Game State Architecture Refactor**: Separated persistent run-level state from encounter-specific combat state.
    - **CombatState**: Tracks individual combat encounters (health, mana, cards in zones).
    - **Run-Level GameState**: Tracks progress across an entire run, including chosen character, selected color identity, current deck, and current/max health.
- **Simple AI Implementation**: Implemented a "simple" AI logic for the enemy with attack priorities (Blockers > Other Creatures > Player Health).
- **Established Project Tracking System**: Created `STATUS.md` and updated `AGENTS.md` with maintenance directives.

## Technical Status Report

### Scenes
- **MainMenuScene**: Functional entry point for the application.
- **NewRunScene**: Allows players to select starting colors and character. Properly initializes persistent run state (health, deck, colors).
- **MapScene**: Functional run navigation scene with randomized scenario choices and UI animations. Successfully transitions to Combat.
- **CombatScene**: Integrated with persistent state. Handles encounter logic and redirects back to Map or exits based on outcome.
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
- **CardRenderer**: Central utility for rendering cards, used by `MapScene` and `CombatScene`.

### Infrastructure
- **InputManager**: Handles mouse and keyboard input.
- **SceneManager**: Manages scene transitions using a deferred mechanism.
- **Engine Z-Sorting Support**: Layering hierarchy strictly enforced via `RenderQueue`.

## Recommendations & Next Steps

### Technical Improvements
- **Map State Persistence**: Extend `GameState` to track the player's current position on the map and visited nodes if moving to a graph-based map.
- **Engine Linker Resolution**: Standardize the search paths for GLFW and OpenGL in the build system to improve sandbox compatibility.

### Feature Development
- **Non-Battle Scenarios**: Implement the specific scenes/logic for Random Event, Shop, and Treasure choices in the `MapScene`.
- **Node-Based Map**: Transition from simple scenario choices to a persistent node-based map graph.
- **Character Perks**: Introduce unique starting stats or passive abilities for the Warrior, Mage, and Rogue archetypes.
- **AI Enhancement**: Expand `SimpleAI` to support non-creature spell targeting and more complex tactical decisions.
- **Audio Integration**: Bootstrap an audio system for sound effects and music.
