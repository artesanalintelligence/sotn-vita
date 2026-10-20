# Distributable PS Vita build

The regular build (`sotn.vpk`) compiles data tables taken from the game disc
into the executable: the sotn-assets headers (`src/**/gen/`), the PC data
files (see [GENERATED.md](GENERATED.md)) and the data of the decompiled
sources. It is fine for your own use but must not be shared.

The release build produces `sotn_release.vpk`, which holds **no byte of the
game disc**. Every run of the executable's data that comes from the disc is
zeroed, and a manifest (`sotn_assets.bin`, inside the VPK) records where to
find it in the game files: file name, offset, length and CRC32, nothing
else. At startup, right after the game files are extracted from the user's
disc image (`vita_disc.c`), `vita_assets.c` copies the bytes back into place
and checks them, before any game code runs. This is the model of ports like
Ship of Harkinian: the code is distributed, the data comes from the user.

## Building it

You need what the regular build needs, including the extracted disc in
`disks/us` (`make extract_disk`, `make extract_assets build_assets`,
`tools/vita/generate_pc_data.sh`), plus Python 3 with numpy.

```sh
export VITASDK=/path/to/vitasdk
tools/vita/make_release.sh
```

The script builds into `build_release/` (override with `BUILD_DIR`, the disc
location with `DISC_DIR`) and then:

1. `mkblank.py blank` finds the disc data in the linked ELF, writes the
   blanked ELF `build_release/sotn_release`, the manifest
   `build_release/sotn_assets.bin` and a report
   `build_release/mkblank_report.txt`.
2. `mkblank.py verify` restores the blanked ELF from the disc exactly like
   the game does and checks that every loaded byte equals the original.
3. `vita-elf-create`, `vita-make-fself` and `vita-pack-vpk` package
   `build_release/sotn_release.vpk` (eboot.bin, param.sfo, sotn_assets.bin).
4. `mkblank.py scan --strict` decompresses the segments of the final
   `eboot.bin`, looks every 16-byte window up on the disc and fails if a run
   found there holds 16 or more non-zero bytes outside the C library and Vita
   backend objects (see below). Zeros do not count: the blanked regions match
   the zero padding of the disc files.

For reference, the tree of October 2026 blanks 8.1 MB of the 8.6 MB of data
in about 40,800 regions read from 96 disc files; the manifest is 820 KB and
the restore takes 60 to 200 ms in Vita3K. The release VPK is 3.7 MB, the
regular one 6.6 MB.

The manifest only fits the executable it was made with: `mkblank.py` stores
a build ID in both, and the game refuses a mismatching pair. Always ship the
`sotn_release.vpk` of one run as a whole.

Installing: as with the regular build, the user puts their disc image in
`ux0:data/sotn/`; the first launch extracts the game files and every launch
restores the data (a fraction of a second). A wrong or damaged disc is
reported on screen and in `ux0:data/sotn/sotn.log`.

## How the matching works

`mkblank.py` hashes every 16-byte window of the ELF's loaded data (`.data`,
`.rodata`, the overlay data sections `psyzovl_*_data`) and every offset of the
disc files (all of `disks/us` but the movies, the XA audio and the CD-DA
track), and extends each hit byte by byte in both directions.

- **Pointers** differ between the PSX and the Vita. The words the ELF
  relocates (the link keeps the relocations, `-Wl,-q`) are never blanked; a
  match runs through them when the disc holds a PSX RAM address at the same
  place, so tables of structs holding pointers are covered too. The manifest
  lists the pieces between the pointers.
- Windows made of a repeated 1, 2 or 4-byte unit do not start a match, but
  matches extend through them, and a second pass looks up the remaining gaps
  (rows of the same tile in a tilemap, ...) on the disc as a whole.
- The data of the **toolchain libraries** (newlib, libgcc, vita2d, VitaSDK
  stubs) and of the **Vita backend** (`tools/psyz/psyz/src/vita/vita_*.c`)
  is never blanked: it runs before the restore. The link map
  (`build_release/sotn.map`) tells which object every byte belongs to. For
  that the Vita build compiles with `-fno-merge-constants`: otherwise the
  linker pools the string literals of every object under the first one of
  the pool, a C runtime object, and the game's strings would be kept;
  `mkblank.py` refuses a map where project objects have pooled constants.
  A few C library tables (`_ctype_`, `jp2uc`) have short runs that happen to
  equal bytes of the disc; the scan counts them apart.
- `.rodata` sits in the text segment by default, which the Vita maps
  read-only, so the Vita link moves it to the data segment
  (`vita_writable_rodata()` in `tools/vita/vita_link.cmake`). This applies to
  the regular build too, so both builds share the same code.

The report lists the bytes blanked per section, the disc files used and the
symbols with data left that is neither blanked, a pointer nor zero, first
those defined by generated sources (expected: none), then the others (port
tables such as the PSX GTE sine table of psyz, shader binaries, C library
tables).

## Development builds

The regular `sotn.vpk` has no manifest: `Vita_RestoreAssets()` sees the
placeholder build ID and does nothing. Installing a release eboot without its
manifest stops with an error instead of running on blank data.

## Checking a release

```sh
# blank + verify + scan, as make_release.sh does
python3 tools/vita/mkblank.py verify --disc disks/us --elf build_release/sotn \
    --blank-elf build_release/sotn_release --manifest build_release/sotn_assets.bin
python3 tools/vita/mkblank.py scan --disc disks/us --elf build_release/sotn \
    --map build_release/sotn.map build_release/release/eboot.bin \
    --absent BOSS/RBO0/RBO0.BIN+0xB2EC:32   # a palette: must be absent
```

On the console or in Vita3K, creating `ux0:data/sotn/restore_dump.flag`
makes the game write its data sections right after the restore to
`ux0:data/sotn/restore_dump.bin`; compare them with the original ELF:

```sh
python3 tools/vita/mkblank.py check-dump --elf build_release/sotn \
    --manifest build_release/sotn_assets.bin restore_dump.bin
```

Every restored byte must equal the original; the other differences listed
come from code that ran before the dump (C library state, the backend).
