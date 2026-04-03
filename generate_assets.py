import random
import math
from PIL import Image, ImageDraw, ImageFilter, ImageEnhance

def generate_texture(width, height, texture_type):
    img = Image.new('L', (width, height), color=200)
    pixels = img.load()

    if texture_type == "marble": # White
        for _ in range(60):
            x = random.randint(0, width)
            y = random.randint(0, height)
            draw = ImageDraw.Draw(img)
            draw.line([x, y, x + random.randint(-40, 40), y + random.randint(-40, 40)], fill=random.randint(220, 255), width=1)
        img = img.filter(ImageFilter.GaussianBlur(radius=1))

    elif texture_type == "parchment": # Blue
        for x in range(width):
            for y in range(height):
                noise = random.randint(-10, 10)
                pixels[x, y] = max(0, min(255, 215 + noise))
        img = img.filter(ImageFilter.MedianFilter(size=3))

    elif texture_type == "stone": # Black
        for x in range(width):
            for y in range(height):
                noise = random.randint(-40, 10)
                pixels[x, y] = max(0, min(255, 120 + noise))
        img = img.filter(ImageFilter.UnsharpMask(radius=2, percent=150, threshold=3))

    elif texture_type == "fire": # Red
        for x in range(width):
            for y in range(height):
                v = int(abs(math.sin(x/5) * math.cos(y/5) * 100))
                pixels[x, y] = max(0, min(255, 180 + v))

    elif texture_type == "nature": # Green
        for x in range(width):
            grain = random.randint(-15, 15)
            for y in range(height):
                pixels[x, y] = max(0, min(255, 190 + grain + random.randint(-5, 5)))

    return img

def create_card_frame_classic(texture_type, is_creature=True):
    width, height = 200, 300
    img_bg = generate_texture(width, height, texture_type)
    img = Image.new('RGB', (width, height))
    img.paste(Image.merge("RGB", (img_bg, img_bg, img_bg)), (0,0))
    draw = ImageDraw.Draw(img)

    # Thick black outer border (MTG classic)
    draw.rectangle([0, 0, width-1, height-1], outline=(15, 15, 15), width=6)
    # Inner border line
    draw.rectangle([6, 6, width-7, height-7], outline=(100, 100, 100), width=1)

    # Art Box
    draw.rectangle([12, 40, 188, 160], fill=(0,0,0), outline=(10, 10, 10), width=2)

    # Name Bar
    draw.rectangle([12, 10, 188, 34], fill=(235, 230, 220), outline=(0, 0, 0), width=1)

    # Type Line Bar
    draw.rectangle([12, 166, 188, 186], fill=(235, 230, 220), outline=(0, 0, 0), width=1)

    # Description Box
    draw.rectangle([12, 192, 188, 288], fill=(245, 245, 240), outline=(0, 0, 0), width=1)

    if is_creature:
        # Stats Box
        draw.rectangle([135, 263, 188, 288], fill=(255, 255, 255), outline=(0, 0, 0), width=1)
        draw.line([161, 263, 161, 288], fill=(0, 0, 0), width=1)

    return img

# Save assets
create_card_frame_classic("marble").save('assets/frame_white.png')
create_card_frame_classic("parchment").save('assets/frame_blue.png')
create_card_frame_classic("stone").save('assets/frame_black.png')
create_card_frame_classic("fire").save('assets/frame_red.png')
create_card_frame_classic("nature").save('assets/frame_green.png')
create_card_frame_classic("marble", False).save('assets/frame_colorless.png')

# Placeholders (using standard icons for now as finding high res art failed)
def create_icon_placeholder(color, style):
    width, height = 172, 116
    img = Image.new('RGB', (width, height), color=(20, 20, 20))
    draw = ImageDraw.Draw(img)
    cx, cy = width // 2, height // 2
    if style == "sword":
        draw.line([cx-40, cy+40, cx+40, cy-40], fill=color, width=8)
        draw.line([cx-30, cy+10, cx-10, cy+30], fill=color, width=8)
    elif style == "shield":
        draw.polygon([cx-30, cy-40, cx+30, cy-40, cx+40, cy, cx, cy+50, cx-40, cy], fill=color, outline=(255,255,255))
    elif style == "fire":
        draw.pieslice([cx-30, cy-40, cx+30, cy+40], 180, 360, fill=color)
    return img

create_icon_placeholder((169, 169, 169), "sword").save('assets/sword.png')
create_icon_placeholder((100, 149, 237), "shield").save('assets/shield.png')
create_icon_placeholder((255, 69, 0), "fire").save('assets/flame.png')
