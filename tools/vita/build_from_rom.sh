#!/usr/bin/env bash
# Builds the PS Vita port of Castlevania: Symphony of the Night from a disc
# image supplied by the user and assembles everything that goes on the Vita.
# Nothing derived from the disc is part of the repository: the game files,
# the assets and the generated C data all come from the image passed here.
#
# Usage: tools/vita/build_from_rom.sh [--jobs N] [--clean] <disc.cue> <output dir>
#
# The .cue must sit next to the .bin files it references. Only the USA
# release (SLUS-00067) is supported: Track 1 MODE2/2352 plus the audio track.
#
# Output:
#   <out>/sotn.vpk                                 install with VitaShell
#   <out>/ux0/data/sotn/disks/us/...               extracted game files
#   <out>/ux0/data/sotn/disks/sotn.us.cue          cue sheet + the .bin files
#                                                  it references (CD music)
#
# Re-running is safe: finished steps are cached in build/vita_kit/ and are
# redone only when the disc or their inputs change (--clean redoes all).
#
# Requirements (all present in tools/vita/docker/Dockerfile): bash, make, go,
# cargo, cmake, ninja, python3 with venv, rsync and a VitaSDK with libvita2d.
set -euo pipefail

REPO=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
KIT_DIR=$REPO/build/vita_kit
BUILD_DIR=${SOTN_VITA_BUILD_DIR:-$REPO/build_vita}
JOBS=$(nproc 2>/dev/null || echo 4)
CLEAN=0

# SHA-1 of the tracks of a known-good dump of SLUS-00067, in cue order. A
# different hash is only a warning: the extracted game files are verified
# against config/check.us.sha afterwards, which is what actually matters.
KNOWN_TRACK_SHA1=(
    f967119e006695a59a6442237f9fc7c7811cf7bf # Track 1, MODE2/2352 data
    1cb96a57f7363a2a43e79683f1a79c62bb6190dc # Track 2, audio
)

if [ -t 1 ]; then
    C_ERR=$'\e[1;31m' C_WARN=$'\e[1;33m' C_OK=$'\e[1;32m' C_STEP=$'\e[1;36m' C_OFF=$'\e[0m'
else
    C_ERR="" C_WARN="" C_OK="" C_STEP="" C_OFF=""
fi
info() { printf '%s\n' "$*"; }
warn() { printf '%sWARNING:%s %s\n' "$C_WARN" "$C_OFF" "$*" >&2; }
die() {
    printf '%sERROR:%s %s\n' "$C_ERR" "$C_OFF" "$*" >&2
    printf '##ERROR %s\n' "$*"
    exit 1
}

usage() {
    sed -n '2,/^set -euo/{/^#/s/^# \{0,1\}//p}' "${BASH_SOURCE[0]}"
    exit "${1:-0}"
}

POSITIONAL=()
while [ $# -gt 0 ]; do
    case $1 in
    -j | --jobs)
        [ $# -ge 2 ] || die "--jobs needs a value"
        JOBS=$2
        shift 2
        ;;
    --clean)
        CLEAN=1
        shift
        ;;
    -h | --help) usage 0 ;;
    -*) die "unknown option: $1 (see --help)" ;;
    *)
        POSITIONAL+=("$1")
        shift
        ;;
    esac
done
[ ${#POSITIONAL[@]} -eq 2 ] || usage 1
CUE_ARG=${POSITIONAL[0]}
OUT_ARG=${POSITIONAL[1]}

STEP=0
STEPS=9
# step <id> <title>. Besides the human readable line, every step prints a
# machine readable marker for front ends: "##STEP <n>/<total> <id>". The run
# ends with "##DONE <output dir>" or "##ERROR <message>".
step() {
    STEP=$((STEP + 1))
    STEP_START=$SECONDS
    printf '##STEP %d/%d %s\n' "$STEP" "$STEPS" "$1"
    printf '%s[%d/%d] %s%s\n' "$C_STEP" "$STEP" "$STEPS" "$2" "$C_OFF"
}
step_done() { printf '      done (%ds)\n' $((SECONDS - STEP_START)); }

# Runs a command with its output in $LOG_DIR/<name>.log, shows the end of the
# log and stops when it fails.
run_logged() {
    local name=$1
    shift
    local log=$LOG_DIR/$name.log
    if ! "$@" >"$log" 2>&1; then
        printf '%s--- last lines of %s ---%s\n' "$C_ERR" "$log" "$C_OFF" >&2
        tail -n 40 "$log" >&2
        die "step '$name' failed (command: $*). Full log: $log"
    fi
}

# Step cache: a stamp holds the key of the inputs the step last ran with
stamp_ok() { [ "$CLEAN" = 0 ] && [ -f "$KIT_DIR/$1.stamp" ] && [ "$(cat "$KIT_DIR/$1.stamp")" = "$2" ]; }
stamp_set() { printf '%s' "$2" >"$KIT_DIR/$1.stamp"; }
stamp_clear() { rm -f "$KIT_DIR/$1.stamp"; }
hash_files() { sort -z | xargs -0 -r sha1sum | sha1sum | cut -c1-40; }

# ---------------------------------------------------------------------------
step tools "Checking the build tools"
missing=()
for tool in make go cargo cmake ninja python3 rsync sha1sum realpath; do
    command -v "$tool" >/dev/null 2>&1 || missing+=("$tool")
done
[ ${#missing[@]} -eq 0 ] || die "missing tools: ${missing[*]}. Use the Docker image (tools/vita/docker/Dockerfile) or install them."
python3 -c 'import venv' 2>/dev/null || die "python3 venv module missing (apt install python3-venv)"
if [ -z "${VITASDK:-}" ]; then
    for candidate in /usr/local/vitasdk "$HOME/vitasdk"; do
        [ -f "$candidate/share/vita.toolchain.cmake" ] && VITASDK=$candidate && break
    done
fi
[ -n "${VITASDK:-}" ] && [ -f "$VITASDK/share/vita.toolchain.cmake" ] ||
    die "VitaSDK not found: set VITASDK (see https://vitasdk.org)"
[ -f "$VITASDK/arm-vita-eabi/lib/libvita2d.a" ] ||
    die "libvita2d is not installed in $VITASDK: run 'vdpm install libvita2d'"
[ -f "$VITASDK/arm-vita-eabi/lib/libdl.a" ] ||
    die "libdl is not installed in $VITASDK: run 'vdpm install vita-rss-libdl'"
export VITASDK PATH=$VITASDK/bin:$HOME/.cargo/bin:$PATH
export VERSION=us
mkdir -p "$OUT_ARG" 2>/dev/null || die "cannot create the output folder $OUT_ARG"
OUT=$(realpath "$OUT_ARG")
[ -w "$OUT" ] || die "the output folder $OUT is not writable"
# logs go with the output so they survive a throwaway container
LOG_DIR=$OUT/logs
mkdir -p "$LOG_DIR" "$KIT_DIR"
# in Docker the output is written as root: hand it back to the host user
fix_owner() {
    if [ "$(id -u)" = 0 ] && [ -n "${HOST_UID:-}" ]; then
        chown -R "$HOST_UID:${HOST_GID:-$HOST_UID}" "$OUT" 2>/dev/null || true
    fi
}
trap fix_owner EXIT
if [ "$CLEAN" = 1 ]; then
    rm -f "$KIT_DIR"/*.stamp
    rm -rf "$BUILD_DIR"
fi
info "      repo $REPO, VitaSDK $VITASDK, $JOBS jobs"
step_done

# ---------------------------------------------------------------------------
step disc-check "Checking the disc image"
[ -f "$CUE_ARG" ] || die "cue sheet not found: $CUE_ARG"
CUE=$(realpath "$CUE_ARG")
CUE_DIR=$(dirname "$CUE")
cue_files=()
cue_tracks=()
while IFS= read -r line || [ -n "$line" ]; do
    line=${line%$'\r'}
    if [[ $line =~ ^[[:space:]]*FILE[[:space:]]+\"([^\"]+)\" ]]; then
        cue_files+=("${BASH_REMATCH[1]}")
    elif [[ $line =~ ^[[:space:]]*FILE[[:space:]]+([^[:space:]]+) ]]; then
        cue_files+=("${BASH_REMATCH[1]}")
    elif [[ $line =~ ^[[:space:]]*TRACK[[:space:]]+[0-9]+[[:space:]]+([^[:space:]]+) ]]; then
        cue_tracks+=("${BASH_REMATCH[1]^^}")
    fi
done <"$CUE"
[ ${#cue_files[@]} -gt 0 ] || die "$CUE has no FILE entries, is it a cue sheet?"
[ "${cue_tracks[0]:-}" = "MODE2/2352" ] ||
    die "the first track is '${cue_tracks[0]:-none}', expected MODE2/2352. Dump the disc as raw bin/cue (2352-byte sectors)."
has_audio=0
for t in "${cue_tracks[@]}"; do [ "$t" = AUDIO ] && has_audio=1; done
[ "$has_audio" = 1 ] || warn "the cue has no audio track: the game will run without CD music"
[ ${#cue_files[@]} -eq ${#cue_tracks[@]} ] ||
    warn "the cue lists ${#cue_tracks[@]} tracks in ${#cue_files[@]} files; only one file per track (Redump style) has been tested"

# Resolves each FILE entry next to the cue, ignoring case like Windows does
cue_sources=()
for f in "${cue_files[@]}"; do
    src=$CUE_DIR/$f
    if [ ! -f "$src" ]; then
        src=""
        for candidate in "$CUE_DIR"/*; do
            if [ "${candidate,,}" = "${CUE_DIR,,}/${f,,}" ]; then
                src=$candidate
                break
            fi
        done
        [ -n "$src" ] || die "'$f' is referenced by $(basename "$CUE") but is not in $CUE_DIR"
    fi
    cue_sources+=("$src")
done
size=$(stat -c %s "${cue_sources[0]}")
[ $((size % 2352)) -eq 0 ] && [ "$size" -gt $((100 * 1024 * 1024)) ] ||
    die "$(basename "${cue_sources[0]}") ($size bytes) is not a raw 2352-byte sector image of SOTN"

cue_sha1=()
all_known=1
for i in "${!cue_sources[@]}"; do
    sum=$(sha1sum "${cue_sources[$i]}" | cut -c1-40)
    cue_sha1+=("$sum")
    if [ "$sum" = "${KNOWN_TRACK_SHA1[$i]:-}" ]; then
        info "      ok  $sum  ${cue_files[$i]}"
    else
        info "      ??  $sum  ${cue_files[$i]}"
        all_known=0
    fi
done
[ "$all_known" = 1 ] ||
    warn "the image does not match the reference dump of SLUS-00067; continuing, the extracted files are verified next"
DISC_ID=$(printf '%s\n' "${cue_sha1[@]}" | sha1sum | cut -c1-40)
step_done

# ---------------------------------------------------------------------------
step disc-stage "Copying the disc into disks/"
mkdir -p "$REPO/disks"
for i in "${!cue_sources[@]}"; do
    src=$(realpath "${cue_sources[$i]}")
    dst=$REPO/disks/${cue_files[$i]}
    # a real copy, not a link: sotn-disk opens the image read-write and the
    # source is often a read-only mount
    if [ "$(realpath -m "$dst")" = "$src" ] && [ ! -L "$dst" ] && [ -w "$dst" ]; then
        continue
    fi
    if [ -f "$dst" ] && [ ! -L "$dst" ] && [ "$(stat -c %s "$dst")" = "$(stat -c %s "$src")" ] &&
        [ "$(sha1sum "$dst" | cut -c1-40)" = "${cue_sha1[$i]}" ]; then
        continue
    fi
    rm -f "$dst"
    cp "$src" "$dst"
    chmod u+w "$dst"
done
if [ "$(realpath -m "$REPO/disks/sotn.us.cue")" != "$CUE" ]; then
    cp "$CUE" "$REPO/disks/sotn.us.cue"
fi
step_done

# ---------------------------------------------------------------------------
step disc-extract "Extracting the game files (disks/us)"
cd "$REPO"
if stamp_ok extract "$DISC_ID" && [ -f disks/us/SLUS_000.67 ]; then
    info "      cached"
else
    stamp_clear extract
    rm -rf disks/us
    run_logged extract make extract_disk_us
    stamp_set extract "$DISC_ID"
fi
[ -f disks/us/SLUS_000.67 ] ||
    die "SLUS_000.67 is not on the disc: this is not the USA release of SOTN (SLUS-00067)"
for pair in "main.exe:SLUS_000.67" "DRA.BIN:DRA.BIN" "RIC.BIN:BIN/RIC.BIN"; do
    ref=$(awk -v f="build/us/${pair%%:*}" '$2 == f { print tolower($1) }' config/check.us.sha)
    got=$(sha1sum "disks/us/${pair#*:}" | cut -c1-40)
    if [ -n "$ref" ] && [ "$ref" != "$got" ]; then
        stamp_clear extract
        die "disks/us/${pair#*:} does not match the USA release (sha1 $got, expected $ref). Only SLUS-00067 is supported."
    fi
done
info "      SLUS_000.67, DRA.BIN and RIC.BIN match the USA release"
step_done

# ---------------------------------------------------------------------------
step assets "Extracting and building the assets"
run_logged python-deps make python-dependencies
key=$DISC_ID-$({
    printf '%s\0' config/assets.us.yaml tools/requirements-python.txt
    find tools/sotn-assets -type f -name '*.go' -print0
} | hash_files)
if stamp_ok assets "$key" && [ -d assets ]; then
    info "      cached"
else
    stamp_clear assets
    run_logged assets make extract_assets build_assets
    stamp_set assets "$key"
fi
step_done

# ---------------------------------------------------------------------------
step splat "Splitting the game binaries (asm/us)"
key=$DISC_ID-$({
    printf '%s\0' tools/requirements-python.txt
    find config -maxdepth 1 -type f -name '*us*' -print0
} | hash_files)
if stamp_ok splat "$key" && [ -d asm/us ]; then
    info "      cached"
else
    stamp_clear splat
    : >"$LOG_DIR/splat.log"
    failed=()
    for cfg in config/splat.us.*.yaml; do
        if ! .venv/bin/splat split "$cfg" >>"$LOG_DIR/splat.log" 2>&1; then
            failed+=("$cfg")
        fi
    done
    if [ ${#failed[@]} -gt 0 ]; then
        tail -n 20 "$LOG_DIR/splat.log" >&2
        die "splat failed on: ${failed[*]}. Full log: $LOG_DIR/splat.log"
    fi
    stamp_set splat "$key"
fi
step_done

# ---------------------------------------------------------------------------
step pc-data "Generating the disc-derived C data"
if [ -f tools/vita/generate_pc_data.sh ]; then
    run_logged pc-data bash tools/vita/generate_pc_data.sh
else
    info "      tools/vita/generate_pc_data.sh not present, skipped"
fi
step_done

# ---------------------------------------------------------------------------
step compile "Compiling the Vita build ($BUILD_DIR)"
if [ -f "$BUILD_DIR/CMakeCache.txt" ] &&
    ! grep -q "vita.toolchain.cmake" "$BUILD_DIR/CMakeCache.txt"; then
    die "$BUILD_DIR holds a non-Vita CMake build; remove it or set SOTN_VITA_BUILD_DIR"
fi
run_logged cmake-configure cmake -S "$REPO" -B "$BUILD_DIR" -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake" \
    -DCMAKE_BUILD_TYPE=Release -DENABLE_ASAN=OFF
if ! cmake --build "$BUILD_DIR" -j "$JOBS" >"$LOG_DIR/cmake-build.log" 2>&1; then
    grep -E "error:|undefined reference|multiple definition|FAILED:" "$LOG_DIR/cmake-build.log" | head -n 40 >&2 || true
    tail -n 15 "$LOG_DIR/cmake-build.log" >&2
    die "the Vita build failed. Full log: $LOG_DIR/cmake-build.log"
fi
[ -f "$BUILD_DIR/sotn.vpk" ] || die "the build finished without $BUILD_DIR/sotn.vpk"
step_done

# ---------------------------------------------------------------------------
step output "Assembling the output in $OUT"
DATA=$OUT/ux0/data/sotn
mkdir -p "$DATA/disks/us"
cp "$BUILD_DIR/sotn.vpk" "$OUT/sotn.vpk"
# Same file set as the on-device extraction (psyz src/vita/vita_disc.c): no
# FMVs (*.STR), no XA/ folder, no XA music stream (SD/XA_STR1., 426 MB) and
# no CD-DA entry (SD/CD1.DA). The music streams from the .bin through the
# cue. XA_STR1. also ends with a dot, a name Windows cannot handle. No
# owner/permission copies: the output is often a Windows or macOS mount.
rsync -rt --delete --delete-excluded \
    --exclude '*.STR' --exclude '/XA/' --exclude '/SD/XA_STR1.' --exclude '/SD/CD1.DA' \
    "$REPO/disks/us/" "$DATA/disks/us/"
bad_names=$(find "$DATA/disks/us" -name '*.' -o -name '* ' | head -n 5)
[ -z "$bad_names" ] || warn "file names Windows cannot copy: $bad_names"
cp "$CUE" "$DATA/disks/sotn.us.cue"
for i in "${!cue_sources[@]}"; do
    rsync -tL "${cue_sources[$i]}" "$DATA/disks/${cue_files[$i]}"
done
step_done

printf '##DONE %s\n' "$OUT"
vpk_size=$(du -h "$OUT/sotn.vpk" | cut -f1)
data_size=$(du -sh "$OUT/ux0" | cut -f1)
cat <<EOF

${C_OK}Build complete.${C_OFF}
  $OUT/sotn.vpk ($vpk_size)
  $OUT/ux0/data/sotn/ ($data_size)

On a PS Vita (HENkaku/Enso):
  1. copy sotn.vpk to the Vita and install it with VitaShell
  2. copy the folder ux0/data/sotn to ux0:data/sotn on the Vita
On Vita3K: install sotn.vpk (File > Install .vpk) and copy ux0/data/sotn
into the emulator's ux0/data/ folder.
EOF
