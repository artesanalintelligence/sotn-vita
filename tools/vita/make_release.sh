#!/bin/bash
# Builds the distributable PS Vita package: an executable without any byte
# taken from the game disc, plus the manifest the game uses to restore them
# from the user's own disc at startup. See tools/vita/RELEASE.md.
#
# Usage: tools/vita/make_release.sh [extra cmake args...]
# Environment: VITASDK (required), BUILD_DIR (default build_release),
#              DISC_DIR (default disks/us, the extracted USA disc)
# Output: $BUILD_DIR/sotn_release.vpk
set -euo pipefail

ROOT=$(cd "$(dirname "$0")/../.." && pwd)
BUILD=${BUILD_DIR:-$ROOT/build_release}
DISC=${DISC_DIR:-$ROOT/disks/us}
MKBLANK="python3 $ROOT/tools/vita/mkblank.py"
: "${VITASDK:?set VITASDK to the VitaSDK install directory}"
export PATH=$VITASDK/bin:$PATH

if [ ! -f "$DISC/SLUS_000.67" ]; then
    echo "$DISC: extracted disc not found (make extract_disk)" >&2
    exit 1
fi

echo "== build"
mkdir -p "$BUILD"
cmake -S "$ROOT" -B "$BUILD" -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake" \
    -DCMAKE_BUILD_TYPE=Release -DENABLE_ASAN=OFF "$@" > "$BUILD/cmake.log" 2>&1 ||
    { tail -n 40 "$BUILD/cmake.log"; exit 1; }
cmake --build "$BUILD" > "$BUILD/build.log" 2>&1 ||
    { grep -E "error|undefined reference" "$BUILD/build.log" | head -n 40
      tail -n 20 "$BUILD/build.log"; exit 1; }

echo "== blank the disc data"
$MKBLANK blank --root "$ROOT" --disc "$DISC" \
    --elf "$BUILD/sotn" --map "$BUILD/sotn.map" \
    --out-elf "$BUILD/sotn_release" --manifest "$BUILD/sotn_assets.bin" \
    --report "$BUILD/mkblank_report.txt"

echo "== check the restore"
$MKBLANK verify --disc "$DISC" --elf "$BUILD/sotn" \
    --blank-elf "$BUILD/sotn_release" --manifest "$BUILD/sotn_assets.bin"

echo "== package"
REL="$BUILD/release"
rm -rf "$REL"
mkdir -p "$REL"
vita-elf-create "$BUILD/sotn_release" "$REL/sotn.velf" > /dev/null
vita-make-fself -c -s "$REL/sotn.velf" "$REL/eboot.bin" > /dev/null
# same title, version and name as the development package
cp "$BUILD/sotn.vpk_param.sfo" "$REL/param.sfo"
vita-pack-vpk -s "$REL/param.sfo" -b "$REL/eboot.bin" \
    -a "$BUILD/sotn_assets.bin=sotn_assets.bin" "$BUILD/sotn_release.vpk" \
    > /dev/null

echo "== scan the package for disc data"
$MKBLANK scan --strict --disc "$DISC" --elf "$BUILD/sotn" \
    --map "$BUILD/sotn.map" "$REL/eboot.bin"

ls -l "$BUILD/sotn_release.vpk"
