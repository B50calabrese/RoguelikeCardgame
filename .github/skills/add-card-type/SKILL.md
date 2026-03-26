# Skill: Adding a New Card Type

This skill outlines the steps to add a new card type (e.g., `Equipment`, `Artifact`) to the Mage Tower game.

## Prerequisites
- Basic understanding of the `core::CardData` structure.
- Familiarity with the `core::CardRegistry` XML parsing logic.

## Steps

### 1. Update the `CardType` Enum
In `include/core/card_data.h`, add the new type to the `CardType` enum.

```cpp
enum class CardType {
  Creature,
  Spell,
  Equipment // New type
};
```

### 2. Update the XML Parser
In `src/core/card_registry.cpp`, update the `StringToCardType` helper function to handle the new XML string.

```cpp
CardType StringToCardType(const std::string& str) {
  if (str == "Creature") return CardType::Creature;
  if (str == "Spell") return CardType::Spell;
  if (str == "Equipment") return CardType::Equipment; // Add this line
  return CardType::Spell; // Default
}
```

### 3. Update Rendering Logic (Optional)
If the new card type requires special rendering (e.g., unique stats or frame elements), update `CardRenderer::RenderCard` in `src/core/graphics/card_renderer.cpp`.

```cpp
void CardRenderer::RenderCard(...) {
  // ... frame rendering ...

  if (data.type == CardType::Equipment) {
    // Render equipment-specific elements (e.g., Durability)
  }
}
```

### 4. Update Card XMLs
Now you can use the new type in your card XML files located in `assets/cards/`.

```xml
<Card id="123">
    <Name>Magic Sword</Name>
    <Type>Equipment</Type>
    <Cost>3</Cost>
    <!-- ... -->
</Card>
```
