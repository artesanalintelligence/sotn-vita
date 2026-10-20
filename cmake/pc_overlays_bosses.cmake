# Normal castle boss overlays not covered by the top-level CMakeLists.txt.
# bo1 (Granfaloon) and bo3 (Scylla) are already built from there.
# Each boss has its own section so they can be maintained independently.

# --- bo0 (Olrox) ---
set(SOURCE_FILES_BOSS_BO0
    src/pc/bosses/boss_bo0.c
    src/boss/bo0/2D26C.c
    src/boss/bo0/3053C.c
    src/boss/bo0/313A8.c
    src/boss/bo0/34DA4.c
    src/boss/bo0/372E8.c
    src/boss/bo0/background_block_init.c
    src/boss/bo0/bss.c
    src/boss/bo0/create_entity.c
    src/boss/bo0/e_background_house.c
    src/boss/bo0/e_bloody_zombie.c
    src/boss/bo0/e_boss_doors.c
    src/boss/bo0/e_breakable.c
    src/boss/bo0/e_chair.c
    src/boss/bo0/e_collect.c
    src/boss/bo0/e_init.c
    src/boss/bo0/e_laser_attack.c
    src/boss/bo0/e_life_up.c
    src/boss/bo0/e_lock_camera.c
    src/boss/bo0/e_marionette.c
    src/boss/bo0/e_misc.c
    src/boss/bo0/e_olrox_drool.c
    src/boss/bo0/e_owl_knight.c
    src/boss/bo0/e_particles.c
    src/boss/bo0/e_prisoner.c
    src/boss/bo0/e_red_door.c
    src/boss/bo0/e_room_bg.c
    src/boss/bo0/e_room_fg.c
    src/boss/bo0/e_sealed_door.c
    src/boss/bo0/e_secrets.c
    src/boss/bo0/e_skelerang.c
    src/boss/bo0/e_spikes.c
    src/boss/bo0/e_stage_name.c
    src/boss/bo0/gen/e_laydef.c
    src/boss/bo0/gen/e_layout.c
    src/boss/bo0/gen/rooms.c
    src/boss/bo0/gen/sprites.c
    src/boss/bo0/graphics_banks.c
    src/boss/bo0/header.c
    src/boss/bo0/layers.c
    src/boss/bo0/no2_bg.c
    src/boss/bo0/palette_def.c
    src/boss/bo0/popup.c
    src/boss/bo0/prim_helpers.c
    src/boss/bo0/sprite_banks.c
    src/boss/bo0/st_collision.c
    src/boss/bo0/st_common.c
    src/boss/bo0/st_update.c
    src/boss/bo0/stage_data.c
    src/boss/bo0/tilemaps.c
)
add_overlay(bo0 ${SOURCE_FILES_BOSS_BO0})

# --- bo2 (Werewolf & Minotaur) ---
set(SOURCE_FILES_BOSS_BO2
    src/pc/bosses/boss_bo2.c
    src/boss/bo2/background_block_init.c
    src/boss/bo2/collision.c
    src/boss/bo2/create_entity.c
    src/boss/bo2/d_pc_data.c
    src/boss/bo2/d_prize_drops.c
    src/boss/bo2/e_boss_doors.c
    src/boss/bo2/e_boss_torch.c
    src/boss/bo2/e_collect.c
    src/boss/bo2/e_cutscene_dialogue.c
    src/boss/bo2/e_init.c
    src/boss/bo2/e_life_up.c
    src/boss/bo2/e_lock_camera.c
    src/boss/bo2/e_misc.c
    src/boss/bo2/e_particles.c
    src/boss/bo2/e_red_door.c
    src/boss/bo2/e_red_door_tiles.c
    src/boss/bo2/e_room_fg.c
    src/boss/bo2/e_stage_name.c
    src/boss/bo2/gen/e_laydef.c
    src/boss/bo2/gen/e_layout.c
    src/boss/bo2/gen/rooms.c
    src/boss/bo2/gen/us/sprites.c
    src/boss/bo2/graphics_banks.c
    src/boss/bo2/header.c
    src/boss/bo2/layers.c
    src/boss/bo2/palette_def.c
    src/boss/bo2/popup.c
    src/boss/bo2/prim_helpers.c
    src/boss/bo2/sprite_banks.c
    src/boss/bo2/st_common.c
    src/boss/bo2/st_update.c
    src/boss/bo2/stage_data.c
    src/boss/bo2/unk_22258.c
    src/boss/bo2/unk_224DC.c
    src/boss/bo2/unk_26EF8.c
    src/boss/bo2/unk_337D0.c
)
add_overlay(bo2 ${SOURCE_FILES_BOSS_BO2})

# --- bo4 (Doppleganger10) ---
# SOURCE_FILES_BOSS_BO4 is listed in the top-level CMakeLists.txt; the PC
# build also needs the typed copy of the data still kept as asm on PSX.
add_overlay(bo4 ${SOURCE_FILES_BOSS_BO4} src/boss/bo4/d_pc_sprites.c)

# --- bo5 (Hippogryph) ---
set(SOURCE_FILES_BOSS_BO5
    src/pc/bosses/boss_bo5.c
    src/boss/bo5/background_block_init.c
    src/boss/bo5/collision.c
    src/boss/bo5/create_entity.c
    src/boss/bo5/cutscene_data.c
    src/boss/bo5/d_pc_bss.c
    src/boss/bo5/d_pc_data.c
    src/boss/bo5/d_prize_drops.c
    src/boss/bo5/e_bell.c
    src/boss/bo5/e_breakable.c
    src/boss/bo5/e_clouds.c
    src/boss/bo5/e_collect.c
    src/boss/bo5/e_cutscene_actors.c
    src/boss/bo5/e_cutscene_dialogue.c
    src/boss/bo5/e_dai_no2_shortcut.c
    src/boss/bo5/e_hippogryph.c
    src/boss/bo5/e_init.c
    src/boss/bo5/e_life_up.c
    src/boss/bo5/e_lock_camera.c
    src/boss/bo5/e_misc.c
    src/boss/bo5/e_particles.c
    src/boss/bo5/e_rbo3door.c
    src/boss/bo5/e_red_door.c
    src/boss/bo5/e_red_door_tiles.c
    src/boss/bo5/e_richter_prologue.c
    src/boss/bo5/e_room_bg.c
    src/boss/bo5/e_room_bg_2.c
    src/boss/bo5/e_room_fg.c
    src/boss/bo5/e_room_fg_2.c
    src/boss/bo5/e_stage_name.c
    src/boss/bo5/e_stained_glass.c
    src/boss/bo5/e_unk_16.c
    src/boss/bo5/e_unk_29.c
    src/boss/bo5/gen/e_laydef.c
    src/boss/bo5/gen/e_layout.c
    src/boss/bo5/gen/rooms.c
    src/boss/bo5/gen/us/sprites.c
    src/boss/bo5/graphics_banks.c
    src/boss/bo5/header.c
    src/boss/bo5/layers.c
    src/boss/bo5/pal_stained_glass.c
    src/boss/bo5/pal_unk.c
    src/boss/bo5/palette_def.c
    src/boss/bo5/popup.c
    src/boss/bo5/prim_helpers.c
    src/boss/bo5/sprite_banks.c
    src/boss/bo5/st_common.c
    src/boss/bo5/st_update.c
    src/boss/bo5/stage_data.c
    src/boss/bo5/tilemaps.c
)
add_overlay(bo5 ${SOURCE_FILES_BOSS_BO5})

# --- bo6 (Richter) ---
# d_pc_*.c, bss_pc.c and background_block_init.c are PC-only copies of the
# data that the PSX build still keeps as assembly.
set(SOURCE_FILES_BOSS_BO6
    src/pc/bosses/boss_bo6.c
    src/boss/bo6/background_block_init.c
    src/boss/bo6/bss_pc.c
    src/boss/bo6/collision.c
    src/boss/bo6/create_entity.c
    src/boss/bo6/cutscene.c
    src/boss/bo6/d_pc_bss.c
    src/boss/bo6/d_pc_data.c
    src/boss/bo6/d_pc_sprites.c
    src/boss/bo6/d_prize_drops.c
    src/boss/bo6/e_breakable.c
    src/boss/bo6/e_collect.c
    src/boss/bo6/e_cutscene_actors.c
    src/boss/bo6/e_init.c
    src/boss/bo6/e_life_up.c
    src/boss/bo6/e_lock_camera.c
    src/boss/bo6/e_misc.c
    src/boss/bo6/e_particles.c
    src/boss/bo6/e_red_door.c
    src/boss/bo6/e_red_door_tiles.c
    src/boss/bo6/e_room_bg.c
    src/boss/bo6/e_room_fg.c
    src/boss/bo6/e_stage_name.c
    src/boss/bo6/gen/e_laydef.c
    src/boss/bo6/gen/e_layout.c
    src/boss/bo6/gen/rooms.c
    src/boss/bo6/graphics_banks.c
    src/boss/bo6/header.c
    src/boss/bo6/layers.c
    src/boss/bo6/palette_def.c
    src/boss/bo6/popup.c
    src/boss/bo6/prim_helpers.c
    src/boss/bo6/richter.c
    src/boss/bo6/sprite_banks.c
    src/boss/bo6/st_common.c
    src/boss/bo6/st_update.c
    src/boss/bo6/tiledef.c
    src/boss/bo6/unused.c
    src/boss/bo6/us_39144.c
    src/boss/bo6/us_3E79C.c
)
add_overlay(bo6 ${SOURCE_FILES_BOSS_BO6})

# --- bo7 (Cerberus) ---
set(SOURCE_FILES_BOSS_BO7
    src/pc/bosses/boss_bo7.c
    src/boss/bo7/background_block_init.c
    src/boss/bo7/collision.c
    src/boss/bo7/create_entity.c
    src/boss/bo7/d_prize_drops.c
    src/boss/bo7/e_breakable_wall.c
    src/boss/bo7/e_collect.c
    src/boss/bo7/e_debug_cerberus_gate.c
    src/boss/bo7/e_demon_switch_wall.c
    src/boss/bo7/e_init.c
    src/boss/bo7/e_life_up.c
    src/boss/bo7/e_lock_camera.c
    src/boss/bo7/e_misc.c
    src/boss/bo7/e_particles.c
    src/boss/bo7/e_red_door.c
    src/boss/bo7/e_red_door_tiles.c
    src/boss/bo7/e_room_fg.c
    src/boss/bo7/e_stage_name.c
    src/boss/bo7/gen/e_laydef.c
    src/boss/bo7/gen/e_layout.c
    src/boss/bo7/gen/rooms.c
    src/boss/bo7/gen/us/sprites.c
    src/boss/bo7/graphics_banks.c
    src/boss/bo7/header.c
    src/boss/bo7/layers.c
    src/boss/bo7/palette_def.c
    src/boss/bo7/popup.c
    src/boss/bo7/prim_helpers.c
    src/boss/bo7/sprite_banks.c
    src/boss/bo7/st_common.c
    src/boss/bo7/st_update.c
    src/boss/bo7/stage_data.c
    src/boss/bo7/unk_12FBC.c
    src/boss/bo7/unk_13258.c
    src/boss/bo7/unk_14CE0.c
)
add_overlay(bo7 ${SOURCE_FILES_BOSS_BO7})
