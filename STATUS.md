# Project Status

## Most Recent Changes
- **Established Project Tracking System**: Created `STATUS.md` and updated `AGENTS.md` with maintenance directives to ensure continuous documentation of project progress.
- **Battle Scene Board Support**: Implemented support for rendering and managing creatures on the board within `CombatScene`.
- **Card Rendering Refinement**: Creatures on the board are now centered and spread out using `HandRenderer::CalculateHandLayout` with a 0-degree arc.
- **Rules Enforcement**: Integrated `BoardSpaceRule` into the `RulesEngine` to cap the number of creatures per player at seven.

## Technical Status Report

### Scenes
- **MainMenuScene**: Functional entry point for the application.
- **NewRunScene**: Allows players to select starting colors. Features visual emphasis (3px white outline) on selections.
- **CombatScene**: The primary gameplay loop. Orchestrates `HandController`, `BattleUI`, and board state. Supports debug console commands via `CombatCommandSystem`.
- **CardViewerScene**: Provides a grid view of all registered cards. Includes hover effects and a dark overlay for selected cards. (Note: Gaussian blur effect is currently disabled).

### Core Systems
- **RulesEngine**: Singleton managing gameplay validation via pluggable `IRule` objects (e.g., `ManaRule`, `TurnRule`, `BoardSpaceRule`).
- **EventBus**: Facilitates decoupled communication between systems. Used for triggering effects like Battlecries and Deathrattles.
- **EffectResolver**: Manages the execution of game actions. Uses a `VisualBlocker` system to ensure animations complete before state changes proceed.
- **CardRegistry & EffectRegistry**: Handle the loading of XML card data and registration of dynamic effects (Damage, Draw, Buff).

### UI & Visuals
- **HandController & HandRenderer**: Manage fluid card animations (lerping) and hand layout math.
- **BattleUI**: Renders the combat interface, including mana pools, the 'Pass Turn' button, and borders.
- **HealthIcon**: Component-based health tracking for player and enemy, positioned dynamically.
- **TextRenderer**: Singleton for font management and UI text rendering.

### Infrastructure
- **InputManager**: Handles mouse and keyboard input. Includes synchronization with window height to prevent vertical offsets.
- **SceneManager**: Manages scene transitions using a deferred mechanism to prevent crashes during input callbacks.
- **CombatCommandSystem**: Decouples debug and cheat commands from the main scene logic.

## Recommendations & Next Steps

### Technical Improvements
- **Engine Restoration**: Restore or implement the `GaussianBlurEffect` in the `GameEngine` submodule to enable intended UI effects in `CardViewerScene`.
- **Trigger Timing**: Fine-tune the `TriggerSystem` to ensure complex sequences (like multiple simultaneous Deathrattles) resolve in an intuitive order.
- **Memory Safety**: Continue audit of `EventBus` subscriptions to ensure no capturing of transient state that could lead to dangling pointers.

### Feature Development
- **AI Enhancement**: Upgrade `SimpleAI` from a basic timer-based pass to a system that can evaluate and play cards from its hand.
- **Keyword Expansion**: Implement more advanced card keywords (e.g., Taunt, Haste, Lifesteal).
- **Audio Integration**: Bootstrap an audio system for sound effects (card plays, attacks) and background music.
- **Persistence**: Implement a save/load system for run progress.
