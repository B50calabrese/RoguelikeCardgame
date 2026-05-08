# Project Status

## Most Recent Changes
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
    - **Uniform Formatting**: Applied `clang-format` to all header and source files.
    - **Naming Convention Alignment**:
        - Updated all **Enums** to use `kPascalCase` for values (e.g., `CardType::kCreature`).
        - Standardized **Accessors** to use `snake_case` matching the underlying variable (e.g., `position()`, `set_position()`).
        - Renamed **Singleton** accessors to `Get()` for consistency with the engine's existing pattern while maintaining Google Style.
        - Ensured **private class members** use `snake_case_` with a trailing underscore.
    - **Modern C++ Practices**: Correctly utilized `override` for virtual functions and ensured no `using namespace` declarations exist in headers.
- **Combat Scene UI Refinement**: Overhauled the combat scene layout and interaction.
    - **Health Icon Repositioning**: Moved health icons to be centered within the battle UI's top and bottom borders, improving visibility and board space.
    - **Targeting Line Fix**: Resolved the targeting line visibility issue by assigning it a high Z-index (1300) and ensuring it is submitted to the sorted `RenderQueue`.
    - **Mirrored Hand Layout**: Repositioned the enemy hand to be 20px from the top edge, mirroring the player's hand position.
    - **Dynamic Board Stretching**: The board background now automatically stretches to fill the entire vertical space between the player and enemy hands.
    - **Player Zones & Constraints**: Implemented visual quads for player and enemy board zones with 64px borders and 64px padding between them.
    - **Play Zone Enforcement**: Restricted the clickable area for playing cards to the player's visual board zone; cards released outside this area will now return to the hand instead of being played.
- **Standardized Base Card Set**: Implemented a new color-coded ID system and naming convention for cards.
    - White (1-20), Blue (21-40), Black (41-60), Red (61-80), Green (81-100).
- **Submodule Fixes**: Added `z_index` support to the `GameEngine` submodule's `Renderer` class and converted drawing methods to use the sorted `RenderQueue`.
- **Established Project Tracking System**: Created `STATUS.md` and updated `AGENTS.md` with maintenance directives.
- **Dedicated Card Art System**: Centralized card art assets and simplified their management.
    - **Art Directory**: Created `assets/cards/art/` for all card illustrations.
    - **Smart Art Resolution**: Updated `CardRegistry` to automatically resolve simple filenames (e.g., `<Art>knight</Art>`) to `assets/cards/art/knight.png`, while maintaining backward compatibility for full paths.
    - **Placeholder Standard**: Standardized all current cards to use `<Art>placeholder</Art>`, which maps to `assets/cards/art/placeholder.png`.
- **Mage Apprentice Card Art**: Implemented unique art for the five Mage Apprentice cards.
    - **Image Slicing**: Sliced the composite `mage_apprentices.png` asset into five individual portrait files using Python/Pillow.
    - **Standardized Naming**: Individual art files are named to match their XML definition files (e.g., `008_white_mage_apprentice.png`).
    - **XML Updates**: Updated the `<Art>` tags in `008_white`, `028_blue`, `048_black`, `068_red`, and `088_green` mage apprentice XML files.
    - **Asset Cleanup**: Removed the original `mage_apprentices.png` composite file after successful slicing.
- **Iconic Creature Card Art**: Implemented unique art for the five high-level iconic creatures.
    - **New Assets**: Added art for `009_angel_of_justice.png`, `029_sphinx_of_intellect.png`, `049_demon_of_the_damned.png`, `069_dragon_of_the_north.png`, and `089_beast_of_ole.png`.
    - **XML Updates**: Updated the `<Art>` tags in the corresponding card XML files to transition from placeholders to specific iconic art.
- **Non-Creature Spell Functionality**: Implemented the foundational systems for playing and resolving spell cards.
    - **Timed Modifiers**: Enhanced `CardInstance` to support temporary stat modifications (e.g., "until end of turn") alongside permanent ones, using a robust stat recalculation system.
    - **Spell Resolution Pipeline**: Implemented a multi-stage resolution process: `PlayCardAction` -> `SpellVisualAction` -> `ResolveSpellAction`.
    - **Visual Spell Animation**: Added a "floating" state in `CombatScene` where played spells move to the center-left and pause game logic for 1.5 seconds.
    - **Manual Spell Targeting**: Enhanced `HandController` to support manual targeting for spells. Players now drag a cyan line to creatures or players to select targets.
    - **New Effects & Actions**: Added `HealEffect`, `ModifyMaxManaEffect`, and updated `StatModifyEffect` to support durations.
    - **Demo Spell Set**: Created five new demo spells: Holy Light (White), Arcane Intellect (Blue), Weakness (Black), Fireblast (Red), and Growth Spurt (Green).
- **Keyword Abilities (Haste & Blocker)**: Implemented "Haste" and "Blocker" as a hybrid of `Effect` classes and `CardInstance` flags.
    - **Haste Implementation**: Creatures with Haste can attack on the turn they are played, bypassing summoning sickness logic in `PlayCardAction`.
    - **Blocker Implementation**: Enforced targeting rules in `AttackRule`; if a defender has any creatures with Blocker, they must be targeted first.
    - **Visual Indicators**: Added red highlights for Haste and green highlights for Blocker on the board in `CombatScene`.
    - **Keyword Descriptions**: Updated `CardRegistry` to automatically prepend "Haste." and "Blocker." to card descriptions.
    - **Card Set Update**: Assigned Haste and Blocker to 10 existing cards (2 of each color) by updating their XML definitions.
- **Visual Polish & Board Layout Refinement**: Improved card interaction feel and board presentation.
    - **Held Card Snapping**: Modified `HandController` to snap held cards directly to the mouse position. This eliminates the "ghosting" artifact caused by linear interpolation lag during rapid mouse movement.
    - **Play Interaction Fix**: Resolved the "snap-back" glitch where cards would briefly return to the hand layout after being played. The `HandController` now immediately removes played cards from its visual state upon queuing the play action.
    - **Board Spacing & Centering**: Overhauled the board layout logic in `CombatScene` to enforce a fixed 24px gap between cards. Creatures are now centered within their respective board zones and maintain a consistent scale, rather than spreading to fill the entire width.

## Technical Status Report

### Scenes
- **MainMenuScene**: Functional entry point for the application.
- **NewRunScene**: Allows players to select starting colors. Features visual emphasis (3px white outline) on selections.
- **CombatScene**: The primary gameplay loop. Orchestrates `HandController`, `BattleUI`, and board state. Supports debug console commands via `CombatCommandSystem`. Now features restricted play zones and refined layout.
- **CardViewerScene**: Provides a grid view of all registered cards.

### Core Systems
- **RulesEngine**: Singleton managing gameplay validation via pluggable `IRule` objects.
- **EventBus**: Facilitates decoupled communication between systems.
- **EffectResolver**: Manages the execution of game actions. Uses a `VisualBlocker` system for animation synchronization.

### UI & Visuals
- **HandController & HandRenderer**: Manage fluid card animations and hand layout math. `HandController` now supports play-zone hit detection.
- **BattleUI**: Renders the combat interface, including mana pools, 'Pass Turn' button, and the new dynamic board/zone quads.
- **HealthIcon**: Component-based health tracking for player and enemy.
- **TextRenderer**: Singleton for font management and UI text rendering.

### Infrastructure
- **InputManager**: Handles mouse and keyboard input. Synchronized with window height.
- **SceneManager**: Manages scene transitions using a deferred mechanism.
- **Engine Z-Sorting Support**: The layering hierarchy is now strictly enforced: Targeting Line (1300) > Hand Cards (1100+) > UI/Health Icons (1000) > Border (900) > Zone Outlines > Background (-100).

## Recommendations & Next Steps

### Technical Improvements
- **Code Maintenance**: Maintain strict adherence to the new Google C++ Style conventions in all future PRs.
- **Engine Restoration**: Restore or implement the `GaussianBlurEffect` in the `GameEngine` submodule.
- **Trigger Timing**: Fine-tune the `TriggerSystem` for complex effect sequences.

### Feature Development
- **AI Enhancement**: Expand `SimpleAI` to support non-creature spell targeting and more complex tactical decisions.
- **Keyword Expansion**: Implement keywords like Taunt and Lifesteal (Haste and Blocker already implemented).
- **Audio Integration**: Bootstrap an audio system for sound effects and music.
