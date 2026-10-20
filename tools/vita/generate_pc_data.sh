#!/usr/bin/env bash
# SPDX-License-Identifier: AGPL-3.0-or-later
# Regenerates the PC-only data files of the PC/Vita port that contain bytes
# from the game disc (raw overlay data, sprite frames, palettes, weapon data).
# They are not committed: each one is expanded from its template under
# tools/vita/pcdata/ using the user's own disc files. See tools/vita/GENERATED.md.
#
# Run from anywhere once the disc has been extracted to disks/us (the usual
# `make extract_disk`); it is idempotent and only rewrites files that change.
# The full PC build also needs `make extract_assets build_assets` for the
# src/**/gen/ headers that sotn-assets produces.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$ROOT"

if [ ! -d disks/us ]; then
    echo "generate_pc_data: disks/us not found, extract the US disc first" >&2
    exit 1
fi

PYTHON=python3
if [ -x .venv/bin/python3 ]; then
    PYTHON=.venv/bin/python3
fi

"$PYTHON" tools/vita/pcdata.py --root . generate-all tools/vita/pcdata
