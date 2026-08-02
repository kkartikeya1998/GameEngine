import json
from pathlib import Path
from PIL import Image

# Directory where this script lives
SCRIPT_DIR = Path(__file__).resolve().parent

# Go up one level to project root if needed; adjust if your layout is different
PROJECT_ROOT = SCRIPT_DIR.parent

SPRITESHEET_PATH = PROJECT_ROOT / "assets" / "objects" / "SpriteResource_Objects.png"
TILE_W = 32
TILE_H = 32

OUTPUT_JSON_PATH = PROJECT_ROOT / "assets" / "maps" / "objects_metadatav3.json"
TEXTURE_PATH_IN_JSON = "assets/objects/SpriteResource_Objects.png"
TILE_SIZE_IN_JSON = 32
# ---------------- LOGIC ----------------

def build_metadata():
    img_path = Path(SPRITESHEET_PATH)
    with Image.open(img_path) as img:
        sheet_w, sheet_h = img.size

    # Number of objects in grid
    cols = sheet_w // TILE_W
    rows = sheet_h // TILE_H
    total = cols * rows

    if sheet_w % TILE_W != 0 or sheet_h % TILE_H != 0:
        print(
            f"Warning: spritesheet size ({sheet_w}x{sheet_h}) is not an exact "
            f"multiple of object size ({TILE_W}x{TILE_H}). Ignoring remainder."
        )

    render_components = {}
    collision_components = {}
    archetypes = {}

    collision_components["obj_001"] = {
        "offset_x": 0,
        "offset_y": 0,
        "width": TILE_W,
        "height": TILE_H,
    }

    idx = 1  # start at 1 for obj_001
    for row in range(rows):
        for col in range(cols):
            obj_id = f"obj_{idx:03d}"  # obj_001, obj_002, ...

            x_px = col * TILE_W
            y_px = row * TILE_H

            # Render
            render_components[obj_id] = {
                "texture": TEXTURE_PATH_IN_JSON,
                "tile_size": TILE_SIZE_IN_JSON,
                "x": x_px,
                "y": y_px,
                "w": TILE_W,
                "h": TILE_H,
            }

            # Collision: default to full sprite size, no offset
            # collision_components[obj_id] = {
            #     "offset_x": 0,
            #     "offset_y": 0,
            #     "width": TILE_W,
            #     "height": TILE_H,
            # }

            # Archetype
            archetypes[obj_id] = {
                "render": obj_id,
                "collision": "obj_001",
            }

            idx += 1

    return {
        "components": {
            "Render": render_components,
            "Collision": collision_components,
        },
        "archetypes": archetypes,
    }

def main():
    metadata = build_metadata()

    out_path = Path(OUTPUT_JSON_PATH)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    with out_path.open("w", encoding="utf-8") as f:
        json.dump(metadata, f, indent=2)

    # print(f"Wrote metadata for {len(archetypes)} objects to {out_path}")

if __name__ == "__main__":
    main()