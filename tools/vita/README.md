# PS Vita build kit

This folder builds the PS Vita port of Castlevania: Symphony of the Night
from **your own copy of the game**. No game data is distributed: the build
extracts the game files, the graphics and the remaining data tables from the
disc image you provide, compiles the port with the VitaSDK and gives you:

- `sotn.vpk`: the application, installed with VitaShell or in Vita3K
- `ux0/data/sotn/`: the game data from your disc, copied to `ux0:data/sotn`

The easiest way is Docker: one command on Windows, Linux or macOS, nothing
else to install. A native Linux build is described at the end.

## What you need

- **The disc image** of *Castlevania: Symphony of the Night* for the
  PlayStation, **USA release (SLUS-00067)**, as a raw BIN/CUE dump: a `.cue`
  file and the `.bin` files it references (Track 1, data, `MODE2/2352`, and
  Track 2, audio, for the CD music) in the same folder. Other regions, PSP
  or Saturn versions, ISO or CHD images are not supported (convert CHD with
  `chdman extractcd` first). The kit checks the image; a Redump-verified dump
  matches these SHA-1 hashes:

  | File | SHA-1 |
  | --- | --- |
  | `Castlevania - Symphony of the Night (USA) (Track 1).bin` | `f967119e006695a59a6442237f9fc7c7811cf7bf` |
  | `Castlevania - Symphony of the Night (USA) (Track 2).bin` | `1cb96a57f7363a2a43e79683f1a79c62bb6190dc` |

  The file names do not matter as long as the `.cue` points to them.
  Keep the image **outside the repository** (or in `disks/`, which git
  ignores) so it is never committed or sent anywhere.
- **Docker**: [Docker Desktop](https://www.docker.com/products/docker-desktop/)
  on Windows 10/11 (WSL 2 backend, Linux containers) or macOS, Docker Engine
  on Linux.
- About **10 GB of free disk** (Docker image about 2 GB, build cache and
  output), 8 GB of RAM, an internet connection for the first image build.
- **git**, to clone the repository with its submodules.

## 1. Get the source

```sh
git clone --recursive https://github.com/artesanalintelligence/sotn-vita
cd sotn-vita
```

The repository has a `.gitattributes` that keeps LF line endings on every
platform, which the Linux build needs. If you cloned without `--recursive`,
run `git submodule update --init --recursive`.

## 2. Build

### Windows (PowerShell + Docker Desktop)

Start Docker Desktop, then from the repository folder:

```powershell
powershell -ExecutionPolicy Bypass -File tools\vita\build-vita.ps1 `
    -Cue "D:\roms\Castlevania - Symphony of the Night (USA).cue" `
    -Out "D:\sotn-vita-out"
```

Options: `-Out <folder>` (default `build\vita-out` in the repository),
`-Jobs <n>` to limit the parallel compile jobs, `-SkipImageBuild` to reuse
the Docker image from a previous run, `-Clean` to redo every step.

### Linux and macOS

```sh
tools/vita/build-vita.sh "$HOME/roms/Castlevania - Symphony of the Night (USA).cue" ~/sotn-vita-out
```

Options: `--jobs <n>`, `--skip-image-build`, `--clean`, `--image <tag>`.
The output is owned by your user. On Apple Silicon Docker builds an arm64
image; the kit has been tested on x86_64 hosts.

### What happens

The wrapper builds the `sotn-vita-kit` Docker image (Ubuntu 26.04, Go, Rust,
Python, CMake and the VitaSDK with libvita2d; 5 to 10 minutes the first
time, cached afterwards) and runs `tools/vita/build_from_rom.sh` in a
throwaway container. The folder of the `.cue` is mounted read-only at `/rom`
and the output folder at `/out`. The steps:

| # | id | What it does |
| --- | --- | --- |
| 1 | `tools` | check the build tools and the VitaSDK |
| 2 | `disc-check` | check the disc image (cue layout, sector size, SHA-1) |
| 3 | `disc-stage` | copy the disc into `disks/` as `sotn.us.cue` plus its `.bin` files |
| 4 | `disc-extract` | extract the game files to `disks/us`; verify `SLUS_000.67`, `DRA.BIN` and `RIC.BIN` against `config/check.us.sha` |
| 5 | `assets` | extract and build the assets (`make extract_assets build_assets`) |
| 6 | `splat` | split the game binaries with splat (`asm/us`) |
| 7 | `pc-data` | regenerate the data files listed in `tools/vita/GENERATED.md` (`tools/vita/generate_pc_data.sh`) |
| 8 | `compile` | compile the Vita build with CMake (`build_vita/sotn.vpk`) |
| 9 | `output` | assemble the output folder |

The build itself takes about 5 minutes on a 12-thread machine (splat and the
compile are the long steps).

For front ends, `build_from_rom.sh` also prints one machine readable line per
step, `##STEP <n>/<total> <id>` (e.g. `##STEP 4/9 disc-extract`), and ends
with `##DONE <output dir>` on success or `##ERROR <message>` on failure.

### Output

```
<out>/
  sotn.vpk                         the application (about 7 MB)
  ux0/data/sotn/
    disks/sotn.us.cue              cue sheet, used for the CD music
    disks/<Track 1>.bin            the .bin files named in the cue
    disks/<Track 2>.bin
    disks/us/...                   extracted game files (no FMV or XA audio)
  logs/                            one log per build step
```

`ux0/data/sotn` is about 630 MB: 580 MB for the two `.bin` files and 47 MB
of extracted game files. The FMVs (`*.STR`), the `XA/` folder, the XA music
stream `SD/XA_STR1.` and the CD-DA entry `SD/CD1.DA` are left out: the music
plays from the `.bin` through the cue sheet.

## 3. Install

### PS Vita

You need a Vita or PS TV running **HENkaku / h-encore / Ensō** (any firmware
supported by those) with **VitaShell**. No plugins are needed.

1. Copy `sotn.vpk` to the Vita (VitaShell: *SELECT* for USB or FTP) and
   install it by pressing ✕ on it in VitaShell.
2. Copy the folder `ux0/data/sotn` from the output to `ux0:data/sotn` on the
   Vita, so that `ux0:data/sotn/disks/sotn.us.cue` exists.
3. Launch *Castlevania SOTN* (title ID `SOTN00001`) from the LiveArea.

Saves (memory cards) are stored in `ux0:data/sotn/bu00` and `bu10`; the log
of the last run in `ux0:data/sotn/sotn.log`.

### Vita3K

1. *File > Install .vpk* and pick `sotn.vpk` (or extract the vpk, which is a
   zip, into `<Vita3K>/ux0/app/SOTN00001`).
2. Copy `ux0/data/sotn` into the emulator's `ux0/data/` folder:
   - Windows: `%APPDATA%\Vita3K\Vita3K\ux0\data\sotn`
   - Linux: `~/.local/share/Vita3K/Vita3K/ux0/data/sotn`
   - or the folder set in *Configuration > Settings > Emulator*.
3. Start *Castlevania SOTN*.

## Controls

The Vita controls map 1:1 to a PS1 digital pad, and the in-game *Button
Config* menu works as on the PS1.

| Vita | PS1 |
| --- | --- |
| D-pad or left stick | D-pad |
| ✕ ○ □ △ | ✕ ○ □ △ |
| L / R | L1 / R1 |
| Rear touchpad, left half | L2 |
| Rear touchpad, right half | R2 |
| START / SELECT | START / SELECT |

The right stick is only read when a game mode asks for an analog pad.

## Options: `ux0:data/sotn/args.txt`

Vita apps have no command line, so the port reads its arguments from
`ux0:data/sotn/args.txt` (space or newline separated). The file is optional;
an empty or missing file starts the game normally.

| Argument | Effect |
| --- | --- |
| `--nearest` | nearest-neighbour upscaling instead of bilinear |
| `--shots <n>` | save a BMP of the screen every *n* frames into `ux0:data/sotn/shots/` |
| `--shots-max <n>` | stop after *n* screenshots |
| `--watchdog` | log frame and audio progress to `sotn.log` every 2 seconds |
| `--stage <name>` | boot straight into a stage (e.g. `no0`, `are`, `rbo8`) |
| `--player <name>` | pick the player character: `alu`, `ric` or `mar` |
| `--spawn-point <name>` | start at a named spawn point |
| `--demo <n>`, `--record <file>`, `--replay <file>` | input demos and recordings |
| `--disk <path>` | use another cue sheet (default `ux0:data/sotn/disks/sotn.us.cue`) |

Example `args.txt` that dumps one screenshot after 10 seconds:

```
--shots 600 --shots-max 1
```

Delete the file (or leave it empty) to go back to normal.

## Troubleshooting

- **Docker is not running / cannot talk to the Docker daemon**: start Docker
  Desktop and wait until it says *running*; on Linux add your user to the
  `docker` group or use `sudo`.
- **`curl: (35) TLS connect error` or other download errors while the image
  builds**: the VitaSDK comes from GitHub and the step retries on its own;
  if it still fails, run the same command again (finished layers are
  cached).
- **`cue sheet not found` / `'...bin' is referenced by ... but is not in`**:
  the `.bin` files must be in the same folder as the `.cue`, with the names
  written inside it (open the `.cue` in a text editor to check).
- **`the first track is ..., expected MODE2/2352`** or **`not a raw 2352-byte
  sector image`**: the image is an ISO or another format. Make a raw BIN/CUE
  dump, or convert CHD with `chdman extractcd`.
- **`SLUS_000.67 is not on the disc`** or **`does not match the USA
  release`**: the image is another region or a modified game. Only the USA
  release is supported.
- **Warning `does not match the reference dump`**: the track hashes differ
  from the Redump dump, but the build goes on as long as the extracted game
  files are correct. If the music is wrong, re-dump Track 2.
- **The build fails**: the last lines are printed and every step has a full
  log in `<out>/logs/`. Run again with `-Clean` / `--clean` after updating the
  repository. Out of memory errors while compiling: lower the jobs
  (`-Jobs 4`) or give Docker Desktop more memory.
- **CRLF warnings on Windows**: the clone was made without the
  `.gitattributes` (or with an old git). In the repository run
  `git rm --cached -r -q . ; git reset --hard` and build again.
- **Black screen or the app closes on start**: check that
  `ux0:data/sotn/disks/us/SLUS_000.67` and `ux0:data/sotn/disks/sotn.us.cue`
  exist and look at `ux0:data/sotn/sotn.log`. Remove `args.txt` if you
  created one.
- **No music**: the `.bin` files named in `sotn.us.cue` must be next to it in
  `ux0:data/sotn/disks/`.

## Building without Docker (Linux or WSL)

On Ubuntu 24.04 or newer (Debian and others work with the equivalent
packages):

```sh
sudo apt install build-essential bzip2 ca-certificates cargo cmake curl git \
    golang-go make ninja-build p7zip-full pkg-config python3 python3-dev \
    python3-pip python3-venv rsync rustc wget xz-utils

# VitaSDK in ~/vitasdk, see https://vitasdk.org
export VITASDK=$HOME/vitasdk PATH=$HOME/vitasdk/bin:$PATH
git clone https://github.com/vitasdk/vdpm ~/vdpm
~/vdpm/bootstrap-vitasdk.sh
vdpm install zlib libpng libjpeg-turbo freetype libvita2d vita-rss-libdl

tools/vita/build_from_rom.sh "/path/to/Castlevania - Symphony of the Night (USA).cue" ~/sotn-vita-out
```

Go 1.24 or newer is needed (older `golang-go` packages download a newer
toolchain automatically). `build_from_rom.sh` works in the repository: it
copies the disc into `disks/`, keeps the build in `build_vita/` and caches the
finished steps in `build/vita_kit/`, so running it again only redoes what
changed. `--jobs <n>` and `--clean` work as above; `SOTN_VITA_BUILD_DIR`
changes the CMake build folder.

The pinned versions (Ubuntu image, vdpm commit, VitaSDK release series) are
the `ARG`s in `tools/vita/docker/Dockerfile`.
