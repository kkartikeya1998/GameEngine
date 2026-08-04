import json
import random
from pathlib import Path


# ---------------- CONFIGURATION ----------------

MAP_JSON_PATH = Path("assets/tmp/apple_woods.json")  # adjust to map file

TILE_SIZE = 48  # map tile size in pixels (must match tileset metadata)
NUM_OBJECTS_TO_PLACE = 25
RANDOM_SEED = 12345

# ---------------- LOGIC ----------------

def load_map(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as f:
        return json.load(f)

def generate_map_objects(map_data: dict, num_objects: int, rng: random.Random) -> list[dict]:
    width = map_data["width"]
    height = map_data["height"]
    tiles = map_data["tiles"]

    # Debug: how many tiles do we have, and how many are considered ground?
    # You can remove these prints later.
    # print(f"Map size: {width} x {height}, total tiles: {len(tiles)}")

    placeable_indices = [
        i for i, t in enumerate(tiles)
        if "Ground" in t  # matches "steam_caves_Ground_..."
    ]

    # Debug:
    # print(f"Placeable (Ground) tiles: {len(placeable_indices)}")

    if len(placeable_indices) < num_objects:
        # Fallback: allow anything that’s not clearly a wall or water
        placeable_indices = [
            i for i, t in enumerate(tiles)
            if "Wall" not in t
        ]

    chosen_indices = rng.sample(placeable_indices, min(num_objects, len(placeable_indices)))

    objects = []
    for i, idx in enumerate(chosen_indices):
        col = idx % width
        row = idx // width

        obj_id = f"obj_{i + 1:03d}"  # obj_001, obj_002, ...

        # Use the SAME tile size as your engine/tileset metadata
        origin_x = col * TILE_SIZE
        origin_y = row * TILE_SIZE

        objects.append({
            "type": obj_id,
            "origin": {
                "x": origin_x,
                "y": origin_y,
            },
        })

    return objects

def main():
    rng = random.Random(RANDOM_SEED)
    map_data = load_map(MAP_JSON_PATH)

    new_objects = generate_map_objects(map_data, NUM_OBJECTS_TO_PLACE, rng)

    # Print only the map_objects array as JSON
    print(json.dumps(new_objects, indent=2))

if __name__ == "__main__":
    main()