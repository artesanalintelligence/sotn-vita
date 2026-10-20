# Castlevania: Symphony of the Night — PS Vita port

A native PlayStation Vita port of *Castlevania: Symphony of the Night* (PSX, USA), built from the
[sotn-decomp](https://github.com/Xeeynamo/sotn-decomp) decompilation and the
[PSY-Z](https://github.com/Xeeynamo/psyz) PSY-Q replacement library.

This is not an emulator: the game code is compiled C running natively on the Vita's ARM CPU. The
original PlayStation GPU is reproduced by a software rasterizer and presented with vita2d.

*[Leer en español](#español)*

> **No game data is included.** The VPK contains only code. Every graphic, sound and data table
> comes from your own legally owned disc image: on first launch the app reads your disc, checks
> that it is the right version and restores the game data from it. Nothing has to be built on a PC.

## What you need

- A PS Vita with HENkaku/Ensō and [VitaShell](https://github.com/TheOfficialFloW/VitaShell), or the
  [Vita3K](https://vita3k.org) emulator.
- Your own disc image of **Castlevania: Symphony of the Night (USA)** — SLUS-00067, `.cue` + two `.bin`
  tracks (redump format):
  ```
  Castlevania - Symphony of the Night (USA).cue
  Castlevania - Symphony of the Night (USA) (Track 1).bin
  Castlevania - Symphony of the Night (USA) (Track 2).bin
  ```
  Other regions (Europe, Japan) and other versions are not supported.

## Install

1. Download `sotn-vita.vpk` from the [latest release](https://github.com/artesanalintelligence/sotn-vita/releases/latest).
2. Install it: on a PS Vita with VitaShell; on Vita3K with *File → Install .vpk*.
3. Copy the `.cue` and both `.bin` files to `ux0:data/sotn/` (create the folder). On Vita3K, `ux0` is
   inside the emulator's preference folder (on Windows `%APPDATA%\Vita3K\Vita3K\ux0`).
4. Launch **Castlevania SOTN**. The first launch extracts the game files from your disc image (about
   50 MB); later launches start straight away.

If something goes wrong, the app shows a message and writes details to `ux0:data/sotn/sotn.log`.

## Building from source

The regular build compiles data from your disc into the executable, so it is for personal use only;
[`tools/vita/RELEASE.md`](tools/vita/RELEASE.md) explains how the distributable, data-free VPK is made.
The easiest way to build is the Docker kit:

```sh
git clone --recursive https://github.com/artesanalintelligence/sotn-vita
cd sotn-vita
./tools/vita/build-vita.sh "/path/to/Castlevania - Symphony of the Night (USA).cue" ./out   # Linux/macOS
.\tools\vita\build-vita.ps1 -Cue "C:\path\to\...\(USA).cue" -Out .\out                      # Windows
```

See [`tools/vita/README.md`](tools/vita/README.md) for the details and for non-Docker builds.

## Controls

| Vita | PlayStation |
| --- | --- |
| D-pad / left stick | D-pad |
| ✕ ◯ □ △ | ✕ ◯ □ △ |
| L / R | L1 / R1 |
| Rear touchpad, left / right half | L2 / R2 |
| START / SELECT | START / SELECT |

## Options

Optional launch arguments can be written in `ux0:data/sotn/args.txt` (whitespace separated). Besides the
PC port options (`--stage <name>`, `--spawn-point <name>`, `--demo <n>`, …) the Vita build accepts:

| Option | Effect |
| --- | --- |
| `--nearest` | nearest neighbour scaling instead of bilinear |
| `--shots <n>` / `--shots-max <n>` | dump the screen as BMP to `ux0:data/sotn/shots/` every *n* frames |
| `--watchdog` | log frame and audio progress every 2 seconds to `ux0:data/sotn/sotn.log` |

Saves (memory card) are stored in `ux0:data/sotn/bu00/`.

## Status

Work in progress. Tested in Vita3K; not yet benchmarked on real hardware.

- Title screen, prologue, menus, and every castle area (normal and inverted), bosses, weapons and
  familiars are wired into the build and load in Vita3K.
- Overlays that were not yet part of the upstream PC port were added. Functions that were still in
  assembly were decompiled as *non-matching* C (behaviour-equivalent, not byte-identical).
- Music and sound effects work (CD audio is read from the second disc track).

Known issues:

- FMV videos are not played (not implemented in the PC port either).
- Some menu text functions are still unimplemented in the PC port.
- Performance of the software renderer on real hardware (444 MHz) has not been measured yet.
- A few gameplay glitches are under investigation (some weapon effects not showing, a camera
  issue after the Maria cutscene in the Marble Gallery clock room, knockback during the
  Doppleganger10 fight).

Bug reports are welcome in the issue tracker. Please don't send them to the upstream decompilation
project: this port is not affiliated with it.

## How it works

- `tools/psyz/psyz/src/vita/` — Vita backend for PSY-Z: file I/O (`sceIo`), input (`sceCtrl`, rear
  touch), audio (`sceAudioOut` thread with the PSY-Z software SPU), logging, and the vita2d presenter.
- `tools/psyz/psyz/src/soft/` — portable software PlayStation GPU rasterizer (1024×512 16-bit VRAM).
- Stage, boss, weapon and familiar overlays, which the PC port loads as shared libraries, are linked
  statically into the single executable. Each overlay is merged with `ld -r`, its symbols are
  localized with `objcopy`, and its `.data`/`.bss` are restored on every load to mimic a fresh load.
- `tools/psyz/psyz/src/vita/vita_disc.c` — finds the user's `.cue`, validates the disc and extracts its
  ISO 9660 file system on first launch.
- `tools/psyz/psyz/src/vita/vita_assets.c` + `tools/vita/mkblank.py` — the release executable has every
  disc-derived byte zeroed; a manifest of file offsets and CRC32s (no data) lets the app restore and
  verify them from the user's disc at startup.
- `tools/vita/` — the Docker build kit, the release tooling and generators for data that must come
  from the disc.

## Credits and license

- [sotn-decomp](https://github.com/Xeeynamo/sotn-decomp) and its contributors — the decompilation
  this port is built on. The original README is kept in [README.decomp.md](README.decomp.md).
- [PSY-Z](https://github.com/Xeeynamo/psyz) by Xeeynamo.
- [VitaSDK](https://vitasdk.org) and [vita2d](https://github.com/xerpi/libvita2d).

Code is licensed under the [GNU AGPL v3](LICENSE) like the upstream project; PSY-Z keeps its own
licenses (see `tools/psyz`). *Castlevania* is a trademark of Konami. This project is not affiliated
with or endorsed by Konami or Sony.

---

## Español

Port nativo para PlayStation Vita de *Castlevania: Symphony of the Night* (PSX, versión USA), basado
en la decompilación [sotn-decomp](https://github.com/Xeeynamo/sotn-decomp) y la librería
[PSY-Z](https://github.com/Xeeynamo/psyz). No es un emulador: el código del juego es C compilado que
corre de forma nativa en la Vita.

> **No se incluyen datos del juego.** El VPK solo contiene código. Los gráficos, sonidos y datos salen
> de tu propia imagen del disco: en el primer arranque la app lee tu disco, verifica que sea la versión
> correcta y restaura los datos del juego. No hay que compilar nada en la PC.

**Necesitas:** una PS Vita con HENkaku/Ensō y VitaShell (o el emulador Vita3K) y tu imagen del disco
*Castlevania: Symphony of the Night (USA)* (SLUS-00067, un `.cue` con sus dos `.bin`). Otras regiones
no son compatibles.

**Instalar:**
1. Descarga `sotn-vita.vpk` del [último release](https://github.com/artesanalintelligence/sotn-vita/releases/latest).
2. Instálalo con VitaShell (en Vita3K: *File → Install .vpk*).
3. Copia el `.cue` y los dos `.bin` a `ux0:data/sotn/` (crea la carpeta). En Vita3K, `ux0` está dentro
   de la carpeta del emulador (en Windows `%APPDATA%\Vita3K\Vita3K\ux0`).
4. Abre **Castlevania SOTN**. El primer arranque extrae los archivos de tu disco (unos 50 MB); los
   siguientes arrancan directo.

Si algo falla, la app muestra un mensaje y deja detalles en `ux0:data/sotn/sotn.log`.

**Controles:** la cruceta o el stick izquierdo mueven; ✕ ◯ □ △ son iguales; L/R son L1/R1; tocar la
mitad izquierda o derecha del panel trasero equivale a L2/R2.

**Estado:** en desarrollo. Todas las zonas del castillo (normal e invertido), jefes, armas y familiares
están integrados y cargan en Vita3K. Faltan los videos FMV, algunos textos de menú y medir el
rendimiento en una Vita real. Los reportes de errores van en los issues de este repositorio, no en el
proyecto de decompilación original.
