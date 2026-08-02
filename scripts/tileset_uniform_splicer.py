from PIL import Image
from pathlib import Path
from collections import defaultdict

# --------------------------------------------------------------------
# One entry per group of 3 columns.
# Repeating a name causes all those groups to be accumulated together.
# --------------------------------------------------------------------
groups = [
    "Legend",
    "Walls",
    "Walls",
    "Walls",
    "Ground",
    "Ground",
    "Ground",
    "Ground",
    "Water",
]

# --------------------------------------------------------------------
# Legend mappings for each row.
# None means ignore that legend for this row.
# --------------------------------------------------------------------
legend_map = [
    [10, 42, 34],
    [12, 46, 36],
    [4, 20, 15],
    [7, 16, 21],
    [6, 0, None],
    [3, None, 14],
    [None, 5, None],
    [2, 25, 13],
    [None, 1, None],
    [None, 24, None],
    [8, None, 22],
    [None, 17, None],
    [None, 28, None],
    [39, None, 32],
    [None, 43, None],
    [41, 33, None],
    [44, 45, None],
    [9, 23, None],
    [11, 35, None],
    [37, 29, None],
    [18, 19, None],
    [30, 38, None],
    [27, 26, None],
    [40, 31, None],
]


def crop_tiles(image_path, tile_width, tile_height,
               groups,
               legend_map,
               out_dir=None,
               print_tiles=False):

    img = Image.open(image_path)
    img_width, img_height = img.size

    tiles = []

    # legend_id -> terrain -> list of replacement tiles
    legend_tiles = defaultdict(lambda: defaultdict(list))

    if out_dir is not None:
        Path(out_dir).mkdir(parents=True, exist_ok=True)

    rows = img_height // tile_height
    cols = img_width // tile_width

    for row in range(rows):

        # Skip rows not described in the legend table
        if row >= len(legend_map):
            break

        for col in range(cols):

            left = col * tile_width
            top = row * tile_height

            tile = img.crop((left, top, left + tile_width, top + tile_height))

            idx = len(tiles)
            tiles.append(tile)

            if out_dir is not None:
                tile.save(Path(out_dir) / f"tile_{idx}.png")

            if print_tiles:
                print(f"Tile {idx}: row={row}, col={col}")

            # Which 3-column group is this?
            group_idx = col // 3

            if group_idx >= len(groups):
                continue

            group_name = groups[group_idx]

            # Skip the legend graphics themselves
            if group_name == "Legend":
                continue

            # Which legend within the group? (L1/L2/L3)
            legend_col = col % 3

            legend_id = legend_map[row][legend_col]

            # Ignore missing legends
            if legend_id is None:
                continue

            legend_tiles[legend_id][group_name].append({
                "index": idx,
                "image": tile,
            })

    return tiles, legend_tiles


tiles, legend_tiles = crop_tiles(
    "../assets/tmp/apple_woods.png",
    tile_width=24,
    tile_height=24,
    groups=groups,
    legend_map=legend_map,
    out_dir="tiles",
    print_tiles=True,
)

# --------------------------------------------------------------------
# Print result
# --------------------------------------------------------------------
for legend_id in sorted(legend_tiles.keys()):
    print(f"\nLegend {legend_id}")

    for terrain, tiles in legend_tiles[legend_id].items():
        print(f"  {terrain:8}: {[t['index'] for t in tiles]}")