# Project Status

## Most Recent Changes
- **Combat Scene UI Refinement**: Overhauled the combat scene layout and interaction.
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
- **Engine Restoration**: Restore or implement the `GaussianBlurEffect` in the `GameEngine` submodule.
- **Trigger Timing**: Fine-tune the `TriggerSystem` for complex effect sequences.

### Feature Development
- **AI Enhancement**: Upgrade `SimpleAI` to evaluate and play cards.
- **Keyword Expansion**: Implement keywords like Taunt, Haste, and Lifesteal.
- **Audio Integration**: Bootstrap an audio system for sound effects and music.
