from PIL import Image, ImageDraw, ImageFilter, ImageOps
import random
import math

def create_base_texture(width, height, color, noise_range=15):
    img = Image.new('RGB', (width, height), color)
    draw = ImageDraw.Draw(img)
    pixels = img.load()
    for x in range(width):
        for y in range(height):
            noise = random.randint(-noise_range, noise_range)
            r, g, b = color
            pixels[x, y] = (max(0, min(255, r + noise)),
                            max(0, min(255, g + noise)),
                            max(0, min(255, b + noise)))
    return img

def apply_pattern(img, pattern_type):
    width, height = img.size
    draw = ImageDraw.Draw(img)
    if pattern_type == 'marble': # White
        for _ in range(20):
            x1, y1 = random.randint(0, width), random.randint(0, height)
            x2, y2 = x1 + random.randint(-50, 50), y1 + random.randint(-50, 50)
            draw.line([x1, y1, x2, y2], fill=(220, 220, 200), width=1)
    elif pattern_type == 'water': # Blue
        for _ in range(15):
            x, y = random.randint(0, width), random.randint(0, height)
            r = random.randint(10, 40)
            draw.ellipse([x-r, y-r, x+r, y+r], outline=(150, 180, 255), width=1)
    elif pattern_type == 'iron': # Black
        for _ in range(100):
            x, y = random.randint(0, width), random.randint(0, height)
            draw.point([x, y], fill=(100, 100, 100))
    elif pattern_type == 'rock': # Red
        for _ in range(30):
            x, y = random.randint(0, width), random.randint(0, height)
            r = random.randint(2, 5)
            draw.polygon([x, y, x+r, y+r//2, x+r//2, y+r], fill=(180, 100, 100))
    elif pattern_type == 'wood': # Green
        for y in range(0, height, 10):
            draw.line([0, y, width, y + random.randint(-5, 5)], fill=(120, 160, 120), width=1)
    return img

def create_card_back():
    width, height = 500, 700
    img = create_base_texture(width, height, (90, 50, 20), noise_range=20)
    draw = ImageDraw.Draw(img)

    # Weathered look
    for _ in range(2000):
        x, y = random.randint(0, width), random.randint(0, height)
        draw.point([x, y], fill=(70, 40, 15))

    # Border
    draw.rectangle([0, 0, width, height], outline=(40, 20, 5), width=25)
    draw.rectangle([25, 25, width-25, height-25], outline=(120, 80, 40), width=4)

    # WUBRG Pentagon
    center_x, center_y = width // 2, height // 2
    radius = 100
    colors = [
        (255, 255, 245), # White
        (40, 100, 200),  # Blue
        (30, 30, 30),    # Black
        (180, 30, 30),   # Red
        (30, 150, 30)    # Green
    ]
    for i in range(5):
        angle = math.radians(i * 72 - 90)
        x = center_x + radius * math.cos(angle)
        y = center_y + radius * math.sin(angle)
        # Glow
        for r in range(40, 25, -2):
            alpha = int(255 * (1 - (r-25)/15))
            draw.ellipse([x-r, y-r, x+r, y+r], outline=colors[i])
        draw.ellipse([x-25, y-25, x+25, y+25], fill=colors[i], outline=(0,0,0), width=3)

    img.save('assets/card_back.png')

def create_frame_variant(is_creature, color_name, base_color, pattern):
    width, height = 500, 700
    img = create_base_texture(width, height, base_color)
    img = apply_pattern(img, pattern)
    draw = ImageDraw.Draw(img)

    # Outer black border
    draw.rectangle([0, 0, width, height], outline=(0, 0, 0), width=12)

    # Interior bevel
    draw.rectangle([12, 12, width-12, height-12], outline=(60, 60, 60), width=2)

    # Boxes (Parchment color)
    box_color = (245, 240, 220)

    # Name bar
    draw.rectangle([25, 25, 475, 75], outline=(0, 0, 0), fill=box_color, width=3)

    # Art box (darker neutral)
    draw.rectangle([35, 85, 465, 385], outline=(0, 0, 0), fill=(180, 180, 180), width=3)

    # Type bar
    draw.rectangle([25, 395, 475, 435], outline=(0, 0, 0), fill=box_color, width=3)

    # Text box
    draw.rectangle([35, 445, 465, 645], outline=(0, 0, 0), fill=(250, 250, 240), width=3)

    suffix = "creature" if is_creature else "noncreature"
    filename = f"{color_name}_{suffix}_frame.png"

    if is_creature:
        # P/T box
        draw.rectangle([370, 630, 480, 685], outline=(0, 0, 0), fill=box_color, width=3)
        draw.line([425, 635, 425, 680], fill=(0, 0, 0), width=2)

    img.save(f'assets/{filename}')
    # Also save as generic if requested
    if color_name == "gray":
        gen_name = "creature_frame.png" if is_creature else "non_creature_frame.png"
        img.save(f'assets/{gen_name}')

if __name__ == "__main__":
    create_card_back()
    configs = [
        ("white", (240, 240, 220), "marble"),
        ("blue", (160, 190, 230), "water"),
        ("black", (100, 100, 105), "iron"),
        ("red", (230, 160, 150), "rock"),
        ("green", (160, 210, 160), "wood"),
        ("gray", (180, 180, 180), None)
    ]
    for name, color, pattern in configs:
        create_frame_variant(True, name, color, pattern)
        create_frame_variant(False, name, color, pattern)
