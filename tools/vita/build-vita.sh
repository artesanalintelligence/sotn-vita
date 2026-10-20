#!/usr/bin/env bash
# Builds sotn.vpk and the ux0:data/sotn folder for the PS Vita from your own
# Castlevania: Symphony of the Night (USA, SLUS-00067) disc image, with Docker.
# Linux and macOS host wrapper of tools/vita/build_from_rom.sh.
#
# Usage: tools/vita/build-vita.sh [options] <disc.cue> [output dir]
#   --image NAME        Docker image tag (default: sotn-vita-kit)
#   --skip-image-build  reuse the existing image
#   --jobs N            parallel compile jobs in the container
#   --clean             redo every step
# The .bin files must be next to the .cue. Output defaults to build/vita-out.
set -euo pipefail

REPO=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
IMAGE=sotn-vita-kit
BUILD_IMAGE=1
EXTRA=()
POSITIONAL=()

die() {
    printf 'ERROR: %s\n' "$*" >&2
    exit 1
}
usage() {
    sed -n '2,/^set -euo/{/^#/s/^# \{0,1\}//p}' "${BASH_SOURCE[0]}"
    exit "${1:-0}"
}

while [ $# -gt 0 ]; do
    case $1 in
    --image)
        IMAGE=${2:?--image needs a value}
        shift 2
        ;;
    --skip-image-build)
        BUILD_IMAGE=0
        shift
        ;;
    -j | --jobs)
        EXTRA+=(--jobs "${2:?--jobs needs a value}")
        shift 2
        ;;
    --clean)
        EXTRA+=(--clean)
        shift
        ;;
    -h | --help) usage 0 ;;
    -*) die "unknown option: $1" ;;
    *)
        POSITIONAL+=("$1")
        shift
        ;;
    esac
done
[ ${#POSITIONAL[@]} -ge 1 ] && [ ${#POSITIONAL[@]} -le 2 ] || usage 1

command -v docker >/dev/null || die "docker not found, install Docker Engine or Docker Desktop"
docker info >/dev/null 2>&1 || die "cannot talk to the Docker daemon (is it running? is your user in the docker group?)"

CUE=${POSITIONAL[0]}
[ -f "$CUE" ] || die "cue sheet not found: $CUE"
ROM_DIR=$(cd "$(dirname "$CUE")" && pwd)
CUE_NAME=$(basename "$CUE")
case $ROM_DIR/ in
"$REPO"/disks/*) ;;
"$REPO"/*) echo "WARNING: the disc image is inside the repository; keep it outside (or in disks/) so it is never committed." >&2 ;;
esac
OUT=${POSITIONAL[1]:-$REPO/build/vita-out}
mkdir -p "$OUT"
OUT=$(cd "$OUT" && pwd)

if [ "$BUILD_IMAGE" = 1 ]; then
    echo "==> Building the Docker image $IMAGE (first time: 5-10 minutes)"
    docker build -f "$REPO/tools/vita/docker/Dockerfile" -t "$IMAGE" "$REPO"
fi

echo "==> Building the Vita port from $CUE_NAME"
docker run --rm \
    -e HOST_UID="$(id -u)" -e HOST_GID="$(id -g)" \
    -v "$ROM_DIR:/rom:ro" -v "$OUT:/out" \
    "$IMAGE" ${EXTRA[@]+"${EXTRA[@]}"} "/rom/$CUE_NAME" /out

cat <<EOF

Output: $OUT
  sotn.vpk           the application
  ux0/data/sotn/     game data (your disc), goes to ux0:data/sotn

PS Vita (HENkaku/Enso + VitaShell):
  1. Copy sotn.vpk to the Vita (USB or FTP in VitaShell) and install it.
  2. Copy the folder ux0/data/sotn to ux0:data/sotn on the Vita.
Vita3K:
  1. File > Install .vpk > sotn.vpk
  2. Copy ux0/data/sotn into the emulator's ux0/data/ folder
     (Linux default: ~/.local/share/Vita3K/Vita3K/ux0/data/sotn).
EOF
