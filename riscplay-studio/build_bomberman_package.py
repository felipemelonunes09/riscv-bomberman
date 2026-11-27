#!/usr/bin/env python3
"""Build the Bomberman loader package with all character sprites."""

from __future__ import annotations

import json
import struct
import sys
import uuid
from pathlib import Path

from asset_manager import convert


ASSET_BASE = 0x100000
ASSET_ROOT = Path(__file__).resolve().parent.parent / "assets"
ASSET_FOLDERS = (
    "walk/left",
    "walk/right",
    "walk/down",
    "walk/up",
    "dead",
)


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit(
            "usage: build_bomberman_package.py FIRMWARE.bin OUTPUT.json.prog"
        )

    firmware_path = Path(sys.argv[1])
    output_path = Path(sys.argv[2])

    specs = []
    for folder in ASSET_FOLDERS:
        files = sorted(
            (ASSET_ROOT / folder).glob("*.png"),
            key=lambda path: int(path.stem),
        )
        for png_path in files:
            specs.append({
                "name": f"{folder.replace('/', '_')}_{int(png_path.stem) - 1}.png",
                "data": png_path.read_bytes(),
            })

    asset_binary, _header, entries = convert(specs, base=ASSET_BASE)
    writes = list(struct.iter_unpack("<II", asset_binary))

    values: list[int] = []
    for index, (address, value) in enumerate(writes):
        expected = ASSET_BASE + index
        if address != expected:
            raise ValueError(
                f"non-contiguous asset block: expected 0x{expected:X}, got 0x{address:X}"
            )
        values.append(value)

    firmware_block = {
        "comp_uuid": str(
            uuid.uuid5(uuid.NAMESPACE_URL, "riscplay-studio/bomberman/firmware")
        ),
        "type": 0,
        "addr": "20000000",
        "addr_mem": "20000000",
        "value": firmware_path.read_bytes().hex(),
    }

    asset_address = f"{ASSET_BASE:X}"
    asset_block = {
        "comp_uuid": str(
            uuid.uuid5(uuid.NAMESPACE_URL, "riscplay-studio/bomberman/assets")
        ),
        "type": 1,
        "addr": asset_address,
        "addr_mem": asset_address,
        "value": json.dumps(values, separators=(",", ":")),
        "transparent_palette_index": 255,
    }

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(
        json.dumps([firmware_block, asset_block], separators=(",", ":")) + "\n",
        encoding="utf-8",
    )

    print(f"Bomberman assets: {len(entries)} sprites, {len(values)} SDRAM words")


if __name__ == "__main__":
    main()
