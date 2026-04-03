from PIL import Image, ImageDraw, ImageFont

def create_card_frame(is_creature=True):
    width, height = 200, 300
    # Grayscale image (white background)
    img = Image.new('L', (width, height), color=255)
    draw = ImageDraw.Draw(img)

    # Main border
    draw.rectangle([2, 2, width-2, height-2], outline=0, width=3)

    # Name & Cost Bar
    draw.rectangle([8, 8, 192, 35], outline=0, width=2)

    # Art Box (approx 40% height)
    # y=40 to y=160
    draw.rectangle([8, 40, 192, 160], outline=0, width=2)

    # Type Line Bar
    draw.rectangle([8, 165, 192, 185], outline=0, width=2)

    # Description Box
    draw.rectangle([8, 190, 192, 292], outline=0, width=2)

    if is_creature:
        # Stats Box (Power/Health) - Overlays description box corner usually
        draw.rectangle([130, 265, 192, 292], fill=255, outline=0, width=2)
        # Divider for P/H
        draw.line([161, 265, 161, 292], fill=0, width=2)

    return img

def create_card_back():
    width, height = 200, 300
    # Brown color (RGB)
    img = Image.new('RGB', (width, height), color=(139, 69, 19))
    draw = ImageDraw.Draw(img)

    # Outer border
    draw.rectangle([0, 0, width-1, height-1], outline=(50, 20, 0), width=10)
    # Inner decorative border
    draw.rectangle([15, 15, width-15, height-15], outline=(210, 180, 140), width=2)

    # Five color spots in a circle (WUBRG)
    center_x, center_y = width // 2, height // 2
    radius = 50
    colors = [
        (255, 255, 240), # White (Off-white)
        (30, 144, 255),  # Blue (Dodger Blue)
        (20, 20, 20),    # Black
        (220, 20, 60),   # Red (Crimson)
        (34, 139, 34)    # Green (Forest Green)
    ]
    import math
    for i in range(5):
        angle = math.radians(i * 72 - 90)
        x = center_x + radius * math.cos(angle)
        y = center_y + radius * math.sin(angle)
        r_spot = 12
        # Shadow
        draw.ellipse([x-r_spot+2, y-r_spot+2, x+r_spot+2, y+r_spot+2], fill=(0,0,0,100))
        draw.ellipse([x-r_spot, y-r_spot, x+r_spot, y+r_spot], fill=colors[i], outline=(0,0,0), width=1)

    return img

def create_placeholder_art(color, symbol_type):
    width, height = 184, 120 # Fits inside the 8,40,192,160 box (192-8=184, 160-40=120)
    img = Image.new('RGB', (width, height), color=(245, 245, 245))
    draw = ImageDraw.Draw(img)

    cx, cy = width // 2, height // 2

    if symbol_type == "sword":
        draw.line([cx-40, cy+40, cx+40, cy-40], fill=color, width=10)
        draw.line([cx-30, cy+10, cx-10, cy+30], fill=color, width=10)
    elif symbol_type == "shield":
        draw.polygon([cx-30, cy-40, cx+30, cy-40, cx+40, cy, cx, cy+50, cx-40, cy], fill=color, outline=(0,0,0))
    elif symbol_type == "flame":
        draw.pieslice([cx-30, cy-40, cx+30, cy+40], 180, 360, fill=color)
        draw.pieslice([cx-20, cy-50, cx+40, cy+20], 210, 330, fill=color)

    return img

# Save assets
create_card_frame(True).save('assets/creature_frame.png')
create_card_frame(False).save('assets/non_creature_frame.png')
create_card_back().save('assets/card_back.png')

create_placeholder_art((169, 169, 169), "sword").save('assets/sword.png')
create_placeholder_art((100, 149, 237), "shield").save('assets/shield.png')
create_placeholder_art((255, 69, 0), "flame").save('assets/flame.png')
