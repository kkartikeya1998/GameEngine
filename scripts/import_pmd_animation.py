import xml.etree.ElementTree as ET
import json, sys
from pathlib import Path

NAME_MAP = {
    "Walk": "walk", "Idle": "idle", "Attack": "attack",
    "Sleep": "sleep", "Hurt": "hurt", "Faint": "faint",
}
NON_LOOPING = {"attack", "hurt", "faint"}
DURATION_UNIT = 1.0

def convert(species_folder: Path, species_id: str):
    tree = ET.parse(species_folder / "AnimData.xml")
    clips = {}
    for anim in tree.findall(".//Anim"):
        raw_name = anim.findtext("Name")
        if raw_name not in NAME_MAP:
            continue  # skip anims you don't use yet (Rotate, Charge, etc.)
        clip_name = NAME_MAP[raw_name]

        copy_of = anim.findtext("CopyOf")
        if copy_of:
            continue  # resolved in a second pass below

        frame_w = int(anim.findtext("FrameWidth"))
        frame_h = int(anim.findtext("FrameHeight"))
        durations = [int(d.text) * DURATION_UNIT for d in anim.findall("Durations/Duration")]

        anim_png = species_folder / f"{raw_name}-Anim.png"
        clips[clip_name] = {
            "texturePath": str(anim_png),
            "frameWidth": frame_w,
            "frameHeight": frame_h,
            "frameCount": len(durations),
            "frameDurations": durations,
            "loop": clip_name not in NON_LOOPING,
        }

    # second pass: resolve CopyOf aliases (e.g. SpAttack -> Shoot)
    for anim in tree.findall(".//Anim"):
        raw_name = anim.findtext("Name")
        copy_of = anim.findtext("CopyOf")
        if raw_name in NAME_MAP and copy_of and NAME_MAP.get(copy_of):
            target = NAME_MAP[raw_name]
            source = NAME_MAP[copy_of]
            if source in clips:
                clips[target] = clips[source]

    return {species_id: clips}

if __name__ == "__main__":
    folder = Path(sys.argv[1])   # e.g. assets/characters/pokemon/species_1
    species_id = sys.argv[2]     # e.g. "species_1"
    out = convert(folder, species_id)
    print(json.dumps(out, indent=2))