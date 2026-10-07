#!/usr/bin/env python3
"""Merge per-package compile_commands.json into the workspace root."""
from __future__ import annotations

import json
from pathlib import Path

WS = Path(__file__).resolve().parents[1]
BUILD = WS / "build"
OUT = WS / "compile_commands.json"


def main() -> None:
    entries: list[object] = []
    for path in sorted(BUILD.glob("*/compile_commands.json")):
        try:
            data = json.loads(path.read_text())
        except (OSError, json.JSONDecodeError):
            continue
        if isinstance(data, list):
            entries.extend(data)
    OUT.write_text(json.dumps(entries, indent=2))
    print(f"Wrote {len(entries)} entries to {OUT}")


if __name__ == "__main__":
    main()
