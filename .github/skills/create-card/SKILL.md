---
name: create-card
description: Instructions for creating and registering a new card in the game.
---

# Creating a New Card

Use this skill when a user asks you to add a new card to the game.

## Steps to Create a Card

### 1. Define the Card XML
All card definitions are stored in `assets/cards/` as XML files. Each card should have its own file (e.g., `my_awesome_card.xml`).

#### XML Format:
```xml
<Card id="unique_integer_id">
    <Name>Card Name</Name>
    <Description>Detailed card effects and abilities.</Description>
    <Cost>Mana cost</Cost>
    <Power>Attack or damage power</Power>
    <Health>Defense or health points (0 for non-creatures)</Health>
    <Frame>frame_texture_filename.png</Frame>
    <Art>art_texture_filename.png</Art>
</Card>
```
- **id**: Ensure the ID is unique across all cards.
- **Frame**: Common frames are `non_creature_frame.png` or `creature_frame.png`.
- **Art**: The filename of the unique illustration in `assets/textures/`.

### 2. Verify Assets
Ensure the `Frame` and `Art` textures exist in the `assets/textures/` directory. If they don't, you must notify the user or use existing placeholders like `card_back.png`.

### 3. Load the Card
The `core::CardRegistry` automatically loads all XML files from `assets/cards/` during initialization (typically in the `OnInit` method of a Scene or the Main application).
- If you add a card while the game is running, a restart or a re-call to `CardRegistry::Get().LoadCardsFromDirectory("assets/cards/")` is required.

### 4. Verification
After creating the XML:
1. Run a build (if possible) to ensure no XML parsing errors.
2. Check `assets/cards/` to confirm the file exists.
3. If adding a card with a new ID, verify no other card in `assets/cards/` uses that same ID using `grep`.

## Code Reference
- Data Structure: `include/core/card_data.h`
- Loading Logic: `src/core/card_registry.cpp`
