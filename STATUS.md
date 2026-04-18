# Project Status

## Most Recent Changes
- **Standardized Base Card Set**: Implemented a new color-coded ID system and naming convention for cards.
    - White: IDs 1-20 (1-10 are new creatures: Lowly Squire, White Knight, etc.; 11-20 are placeholder spells).
    - Blue: IDs 21-40 (21-30 are new creatures: Merfolk Fledgling, Blue Knight, etc.; 31-40 are placeholder spells).
    - Black: IDs 41-60 (41-50 are new creatures: Brittle Husk, Black Knight, etc.; 51-60 are placeholder spells).
    - Red: IDs 61-80 (61-70 are placeholders).
    - Green: IDs 81-100 (81-90 are new creatures: Elvish Recruit, Green Knight, etc.; 91-100 are placeholders).
- **Submodule Fixes**: Added `z_index` support to the `GameEngine` submodule's `Renderer` class and converted drawing methods to use the sorted `RenderQueue` to resolve pre-existing build errors and ensure correct UI layering.
- **Established Project Tracking System**: Created `STATUS.md` and updated `AGENTS.md` with maintenance directives to ensure continuous documentation of project progress.
- **Battle Scene Board Support**: Implemented support for rendering and managing creatures on the board within `CombatScene`.
- **Card Rendering Refinement**: Creatures on the board are now centered and spread out using `HandRenderer::CalculateHandLayout` with a 0-degree arc.
- **Rules Enforcement**: Integrated `BoardSpaceRule` into the `RulesEngine` to cap the number of creatures per player at seven.
- **Creature Combat Implementation**:
    - Implemented `AttackRule` and `CreatureAttackAction` to handle combat logic.
    - Added UI support for initiating attacks (targeting line, highlights).
    - Integrated combat animations (lunge) synchronized with resolution via `VisualBlocker`.
    - Added new events (`CreatureAttacked`, `CreatureTookDamage`, etc.) for ability triggers.

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
- **Card Naming Convention**: Standardized card XML filenames as `XXX_name_with_underscores.xml` (e.g., `001_lowly_squire.xml`) for better organization and easier replacement.
- **Creature Type Expansion**: Added support for new creature types: `Merfolk`, `Rogue`, `Sphinx`, `Zombie`, `Vampire`, `Demon`, `Goblin`, `Minotaur`, and `Dragon` in the core engine.
- **Red Base Creature Set**: Implemented 10 new red creature cards (IDs 61-70), replacing placeholders.
- **Engine Z-Sorting Support**: Updated the `GameEngine` submodule to support `z_index` in `Renderer` and `PrimitiveRenderer` methods, enabling proper 2D layer sorting via depth buffer.
- **Creature Type Expansion**: Added support for new creature types: `Merfolk`, `Rogue`, `Sphinx`, `Zombie`, `Vampire`, `Demon`, `Druid`, `Archer`, `Treefolk`, and `Beast` in the core engine.

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
