from enum import IntEnum
import random

class Direction(IntEnum):
    TL = 0
    T  = 1
    TR = 2
    R  = 3
    BR = 4
    B  = 5
    BL = 6
    L  = 7

MATCHES = {
    Direction.TL: [(Direction.TL, Direction.BR)],
    Direction.T: [
        (Direction.TL, Direction.BL),
        (Direction.T, Direction.B),
        (Direction.TR, Direction.BR),
    ],
    Direction.TR: [(Direction.TR, Direction.BL)],
    Direction.R: [
        (Direction.TR, Direction.TL),
        (Direction.R, Direction.L),
        (Direction.BR, Direction.BL),
    ],
    Direction.BR: [(Direction.BR, Direction.TL)],
    Direction.B: [
        (Direction.BL, Direction.TL),
        (Direction.B, Direction.T),
        (Direction.BR, Direction.TR),
    ],
    Direction.BL: [(Direction.BL, Direction.TR)],
    Direction.L: [
        (Direction.TL, Direction.TR),
        (Direction.L, Direction.R),
        (Direction.BL, Direction.BR),
    ],}

OPPOSITE = {
    Direction.TL: Direction.BR, Direction.T:  Direction.B,
    Direction.TR: Direction.BL, Direction.R:  Direction.L,
    Direction.BR: Direction.TL, Direction.B:  Direction.T,
    Direction.BL: Direction.TR, Direction.L:  Direction.R,
}

OFFSETS = {
    Direction.TL: (-1, -1), Direction.T:  (-1,  0), Direction.TR: (-1,  1),
    Direction.R:  ( 0,  1), Direction.BR: ( 1,  1), Direction.B:  ( 1,  0),
    Direction.BL: ( 1, -1), Direction.L:  ( 0, -1),
}

def canonical(mask: int) -> int:
    """Remove invalid corners."""

    # cardinals
    t = mask & (1 << Direction.T)
    r = mask & (1 << Direction.R)
    b = mask & (1 << Direction.B)
    l = mask & (1 << Direction.L)

    # remove invalid corners
    if not (t and l):
        mask &= ~(1 << Direction.TL)

    if not (t and r):
        mask &= ~(1 << Direction.TR)

    if not (b and r):
        mask &= ~(1 << Direction.BR)

    if not (b and l):
        mask &= ~(1 << Direction.BL)

    return mask

def print_bitmask(mask: int):
    def bit(i):
        return "1" if mask & (1 << i) else "."
    
    print(f"{bit(Direction.TL)} {bit(Direction.T)} {bit(Direction.TR)}")
    print(f"{bit(Direction.L)} x {bit(Direction.R)}")
    print(f"{bit(Direction.BL)} {bit(Direction.B)} {bit(Direction.BR)}")

def print_blob(blob_masks):
    for i, mask in enumerate(blob_masks):
        print(f"Tile {i:2d} : 0x{mask:02X}")
        print_bitmask(mask)
        print()

def is_compatible(blobA, blobB, direction):
    bit = lambda m, i: bool(m & (1 << i))
    return all(
        bit(blobA, a) == bit(blobB, b)
        for a, b in MATCHES[direction]
    )

def find_compatible(blob_masks):
    compatible = {}

    for blob1 in blob_masks:
        compatible[blob1] = {}

        for direction in Direction:
            compatible[blob1][direction] = []

            for blob2 in blob_masks:
                if is_compatible(blob1, blob2, direction):
                    compatible[blob1][direction].append(blob2)

    return compatible

def print_compatible(compatible):
    for blob, directions in compatible.items():
        print(f"Blob 0x{blob:02X}")
        # print_bitmask(blob)

        for direction, neighbors in directions.items():
            print(f"  {direction.name:2s}: ", end="")
            
            if neighbors:
                print(" ".join(f"0x{n:02X}" for n in neighbors))
                # for n in neighbors:
                #     print_bitmask(n)
            else:
                print("-")
        
        print()



def generate_grid(grid, blob_masks, compatible, rng=random):
    m = len(grid)
    n = len(grid[0]) if m else 0
    result = [[0] * n for _ in range(m)]
    all_masks = set(blob_masks)
 
    def present(r, c):
        return 0 <= r < m and 0 <= c < n and grid[r][c]
 
    for r in range(m):
        for c in range(n):
            if not grid[r][c]:
                result[r][c] = 0
                continue
 
            forced_off = 0
            for direction, (dr, dc) in OFFSETS.items():
                if not present(r + dr, c + dc):
                    forced_off |= (1 << direction)
            candidates = {mk for mk in all_masks if mk != 0 and mk & forced_off == 0}
 
            for direction in (Direction.TL, Direction.T, Direction.TR, Direction.L):
                dr, dc = OFFSETS[direction]
                nr, nc = r + dr, c + dc
                if not (0 <= nr < m and 0 <= nc < n):
                    continue
                neighbor_mask = result[nr][nc]
                candidates &= set(compatible[neighbor_mask][OPPOSITE[direction]])
 
            if not candidates:
                raise ValueError(f"No compatible tile for cell ({r}, {c})")
 
            result[r][c] = rng.choice(sorted(candidates))
 
    return result
 
def print_grid(result):
    for row in result:
        print(" ".join(f"0x{v:02X}" for v in row))
 
 
blob_masks = []
for mask in range(256):
    c = canonical(mask)
    if c not in blob_masks:
        blob_masks.append(c)

assert len(blob_masks) == 47
print_blob(blob_masks)

compatible = find_compatible(blob_masks)
grid = [
    [False, True,  True,  True,  True,  True],
    [False, False, True,  True,  True,  True],
    [False, False, False, True,  True,  True],
    [False, False, True,  True,  True,  True],
    [False, True,  True,  True,  True,  True],
    [True,  True,  True,  True,  True,  True],
]
 
random.seed(42)
result = generate_grid(grid, blob_masks, compatible)
print_grid(result)

def bitmask_lines(mask):
    def bit(i):
        return "1" if mask & (1 << i) else "0"
    return [
        f"{bit(Direction.TL)} {bit(Direction.T)} {bit(Direction.TR)}",
        f"{bit(Direction.L)} x {bit(Direction.R)}",
        f"{bit(Direction.BL)} {bit(Direction.B)} {bit(Direction.BR)}",
    ]
 
def print_grid_bitmask(result, cell_gap="   "):
    for row in result:
        cell_lines = [bitmask_lines(mask) for mask in row]
        for line_idx in range(3):
            print(cell_gap.join(cell_lines[c][line_idx] for c in range(len(row))))
        print()
 
print_grid_bitmask(result)
 