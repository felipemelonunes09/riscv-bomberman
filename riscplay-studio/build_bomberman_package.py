#!/usr/bin/env python3
"""Build the Bomberman loader package with its first player sprite."""

from __future__ import annotations

import json
import struct
import sys
import uuid
from pathlib import Path

from asset_manager import convert


ASSET_BASE = 0x100000


def main() -> None:
    if len(sys.argv) != 4:
        raise SystemExit(
            "usage: build_bomberman_package.py FIRMWARE.bin PLAYER_IDLE.png OUTPUT.json.prog"
        )

    firmware_path = Path(sys.argv[1])
    png_path = Path(sys.argv[2])
    output_path = Path(sys.argv[3])

    asset_binary, _header, entries = convert(
        [{"name": "player_idle.png", "data": png_path.read_bytes()}],
        base=ASSET_BASE,
    )
    entry = entries[0]
    writes = list(struct.iter_unpack("<II", asset_binary))

    # A loader type-1 block is one starting SDRAM word address followed by
    # contiguous values. The converter emits (address, value) pairs, so verify
    # contiguity before dropping the repeated addresses.
    values: list[int] = []
    for index, (address, value) in enumerate(writes):
        expected = ASSET_BASE + index
        if address != expected:
            raise ValueError(
                f"non-contiguous asset block: expected 0x{expected:X}, got 0x{address:X}"
            )
        values.append(value)

    if entry["palette_addr"] != ASSET_BASE or entry["pixels_addr"] != ASSET_BASE + 256:
        raise ValueError("generated addresses no longer match examples/bomberman/assets.c")

    pixel_count = entry["width"] * entry["height"]
    indices = [
        (values[256 + (index // 4)] >> (24 - 8 * (index % 4))) & 0xFF
        for index in range(pixel_count)
    ]
    pixels = [
        indices[index : index + entry["width"]]
        for index in range(0, pixel_count, entry["width"])
    ]
    palette = [
        {"r": (value >> 16) & 0xFF, "g": (value >> 8) & 0xFF, "b": value & 0xFF}
        for value in values[:256]
    ]

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
            uuid.uuid5(uuid.NAMESPACE_URL, "riscplay-studio/bomberman/player-idle")
        ),
        "type": 1,
        "addr": asset_address,
        "addr_mem": asset_address,
        "value": json.dumps(values, separators=(",", ":")),
        "pixels": json.dumps(pixels, separators=(",", ":")),
        "palette": json.dumps(palette, separators=(",", ":")),
        "transparent_palette_index": entry["transparent_index"],
    }

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(
        json.dumps([firmware_block, asset_block], separators=(",", ":")) + "\n",
        encoding="utf-8",
    )

    print(
        "Asset player idle: "
        f"palette=0x{entry['palette_addr']:X}, "
        f"pixels=0x{entry['pixels_addr']:X}, "
        f"{entry['width']}x{entry['height']}, "
        f"{entry['words']} palavras"
    )


if __name__ == "__main__":
    main()
