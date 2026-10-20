# Generated PC data

Some overlays still keep part of their data as raw assembly in the PSX build,
so the PC/Vita port compiles a C copy of it. Those copies hold bytes from the
game disc (raw tables, sprite frames, palettes, portraits), so they are not
committed. They are produced at build time from your own disc:

```sh
make extract_disk                    # disks/us
make extract_assets build_assets     # src/**/gen/ headers used by the PC build
tools/vita/generate_pc_data.sh       # the files listed below
```

`generate_pc_data.sh` is idempotent and only rewrites files whose content
changes. All the outputs are listed in `.gitignore`.

## How it works

Each output has a template at `tools/vita/pcdata/<output path>.in`: the C file
where every run of numeric initializer lines is replaced by a placeholder
recording the source disc file, the offset, the value format and the line
layout. `tools/vita/pcdata.py generate-all` expands them, reading only the
files under `disks/us`. The expanded files are byte-identical to the ones the
port was developed with.

To change one of these files, edit the generated C file, then refresh its
template (maintainers only, needs the disc):

```sh
python3 tools/vita/pcdata.py template --source disks/us/BOSS/BO6/BO6.BIN \
    src/boss/bo6/d_pc_data.c tools/vita/pcdata/src/boss/bo6/d_pc_data.c.in
```

The tool reports any numeric line it could not find in the disc file; those
are kept verbatim in the template, so check that they do not hold game data.

## Files

| Output | Disc source | Content |
| --- | --- | --- |
| `src/boss/bo2/d_pc_data.c` | `BOSS/BO2/BO2.BIN` | Entity animations, hitboxes, cutscene scripts, dialogue, portraits |
| `src/boss/bo4/d_pc_sprites.c` | `BOSS/BO4/BO4.BIN` | Doppleganger unarmed attacks and sprite frame tables |
| `src/boss/bo5/d_pc_data.c` | `BOSS/BO5/BO5.BIN` | Hippogryph, cutscene and misc tables |
| `src/boss/bo6/d_pc_bss.c` | `BOSS/BO6/BO6.BIN` | Richter boss BSS layout (all zeros) |
| `src/boss/bo6/d_pc_data.c` | `BOSS/BO6/BO6.BIN` | Richter animations, blueprints, cutscene data |
| `src/boss/bo6/d_pc_sprites.c` | `BOSS/BO6/BO6.BIN` | Richter sprite frames, palettes and sprite banks |
| `src/boss/rbo5/d_pc_sprites.c` | `BOSS/RBO5/RBO5.BIN` | Doppleganger (inverted) attacks and sprite frame tables |
| `src/boss/rbo8/palette_def.c` | `BOSS/RBO8/RBO8.BIN` | Galamoth palettes and palette definitions |
| `src/pc/bosses/mar_data.c` | `BOSS/MAR/MAR.BIN` | MAR raw data segments 0x5D4 and 0x688 |
| `src/pc/weapons/w_NNN.c` (41 files) | `BIN/WEAPON0.BIN` | Weapon overlay data: palettes, animations, hitboxes |
| `src/st/rdai/d_pc_data.c` | `ST/RDAI/RDAI.BIN` | Stained glass, archer, imp and black panther tables |

The weapon files are `w_001 w_003 w_004 w_005 w_006 w_009 w_011 w_013 w_014
w_015 w_016 w_017 w_018 w_019 w_020 w_021 w_022 w_023 w_024 w_025 w_026 w_027
w_028 w_029 w_030 w_031 w_032 w_033 w_036 w_038 w_039 w_042 w_047 w_048 w_049
w_053 w_054 w_055 w_056 w_057 w_058`.

These files were first produced from the splat output (`asm/us/**/data/*.s`)
by one-off converters; the templates replace those converters and need only
the extracted disc.
