# Inverted castle boss overlays (RBO1, RBO2, RBO5, RBO6, RBO8) and the
# servant overlays TT_005 (Sprite) and TT_006 (Nose Devil).
# Each section is self-contained so the overlays can be maintained separately.

# --- rbo5 ---
set(SOURCE_FILES_BOSS_RBO5
    src/pc/bosses/boss_rbo5.c
    src/boss/rbo5/header.c
    src/boss/rbo5/sprite_banks.c
    src/boss/rbo5/palette_def.c
    src/boss/rbo5/layers.c
    src/boss/rbo5/graphics_banks.c
    src/boss/rbo5/gen/e_laydef.c
    src/boss/rbo5/e_init.c
    src/boss/rbo5/background_block_init.c
    src/boss/rbo5/e_red_door_tiles.c
    src/boss/rbo5/e_lock_camera.c
    src/boss/rbo5/e_breakable.c
    src/boss/rbo5/doors.c
    src/boss/rbo5/unk_354F0.c
    src/boss/rbo5/d_prize_drops.c
    src/boss/rbo5/st_update.c
    src/boss/rbo5/collision.c
    src/boss/rbo5/e_red_door.c
    src/boss/rbo5/st_common.c
    src/boss/rbo5/e_collect.c
    src/boss/rbo5/e_misc.c
    src/boss/rbo5/e_particles.c
    src/boss/rbo5/e_room_fg.c
    src/boss/rbo5/gen/rooms.c
    src/boss/rbo5/doppleganger.c
    src/boss/rbo5/dop_collision.c
    src/boss/rbo5/dop_anim.c
    src/boss/rbo5/unk_44C48.c
    src/boss/rbo5/unk_45650.c
    src/boss/rbo5/unk_45C00.c
    src/boss/rbo5/unk_4907C.c
    src/boss/rbo5/unk_502F8.c
    src/boss/rbo5/doppleganger_anim.c
    src/boss/rbo5/e_life_up.c
    src/boss/rbo5/gen/e_layout.c
    src/boss/rbo5/spritesheets.c
    src/boss/rbo5/stage_data.c
    src/boss/rbo5/d_pc_sprites.c
    src/boss/rbo5/e_room_bg.c
    src/boss/rbo5/create_entity.c
    src/boss/rbo5/e_stage_name.c
    src/boss/rbo5/popup.c
    src/boss/rbo5/prim_helpers.c
    src/boss/rbo5/bss.c
)
add_overlay(rbo5 ${SOURCE_FILES_BOSS_RBO5})

# --- rbo1 ---
set(SOURCE_FILES_BOSS_RBO1
    src/pc/bosses/boss_rbo1.c
    src/boss/rbo1/header.c
    src/boss/rbo1/sprite_banks.c
    src/boss/rbo1/palette_def.c
    src/boss/rbo1/layers.c
    src/boss/rbo1/graphics_banks.c
    src/boss/rbo1/gen/e_laydef.c
    src/boss/rbo1/e_init.c
    src/boss/rbo1/background_block_init.c
    src/boss/rbo1/e_red_door_tiles.c
    src/boss/rbo1/e_lock_camera.c
    src/boss/rbo1/e_boss_room.c
    src/boss/rbo1/d_prize_drops.c
    src/boss/rbo1/st_update.c
    src/boss/rbo1/collision.c
    src/boss/rbo1/e_red_door.c
    src/boss/rbo1/st_common.c
    src/boss/rbo1/e_collect.c
    src/boss/rbo1/e_misc.c
    src/boss/rbo1/e_particles.c
    src/boss/rbo1/e_room_fg.c
    src/boss/rbo1/gen/rooms.c
    src/boss/rbo1/e_life_up.c
    src/boss/rbo1/gen/e_layout.c
    src/boss/rbo1/stage_data.c
    src/boss/rbo1/pal_unk.c
    src/boss/rbo1/tilemaps.c
    src/boss/rbo1/gen/us/sprites.c
    src/boss/rbo1/unk_11FD8.c
    src/boss/rbo1/unk_12274.c
    src/boss/rbo1/create_entity.c
    src/boss/rbo1/e_stage_name.c
    src/boss/rbo1/popup.c
    src/boss/rbo1/prim_helpers.c
    src/boss/rbo1/bss.c
)
add_overlay(rbo1 ${SOURCE_FILES_BOSS_RBO1})

# --- rbo2 ---
set(SOURCE_FILES_BOSS_RBO2
    src/pc/bosses/boss_rbo2.c
    src/boss/rbo2/header.c
    src/boss/rbo2/sprite_banks.c
    src/boss/rbo2/palette_def.c
    src/boss/rbo2/layers.c
    src/boss/rbo2/graphics_banks.c
    src/boss/rbo2/gen/e_laydef.c
    src/boss/rbo2/e_init.c
    src/boss/rbo2/background_block_init.c
    src/boss/rbo2/e_red_door_tiles.c
    src/boss/rbo2/e_lock_camera.c
    src/boss/rbo2/d_prize_drops.c
    src/boss/rbo2/unk_1B000.c
    src/boss/rbo2/unk_1B284.c
    src/boss/rbo2/e_debug_cerberus_gate.c
    src/boss/rbo2/e_cutscene_dialogue.c
    src/boss/rbo2/e_cutscene_actors.c
    src/boss/rbo2/func_us_801a17c8.c
    src/boss/rbo2/st_update.c
    src/boss/rbo2/collision.c
    src/boss/rbo2/create_entity.c
    src/boss/rbo2/e_red_door.c
    src/boss/rbo2/st_common.c
    src/boss/rbo2/e_collect.c
    src/boss/rbo2/e_misc.c
    src/boss/rbo2/e_stage_name.c
    src/boss/rbo2/e_particles.c
    src/boss/rbo2/e_room_fg.c
    src/boss/rbo2/gen/rooms.c
    src/boss/rbo2/e_life_up.c
    src/boss/rbo2/gen/e_layout.c
    src/boss/rbo2/stage_data.c
    src/boss/rbo2/tilemaps.c
    src/boss/rbo2/gen/us/sprites.c
    src/boss/rbo2/popup.c
    src/boss/rbo2/prim_helpers.c
)
add_overlay(rbo2 ${SOURCE_FILES_BOSS_RBO2})

# --- rbo6 ---
set(SOURCE_FILES_BOSS_RBO6
    src/pc/bosses/boss_rbo6.c
    src/boss/rbo6/header.c
    src/boss/rbo6/sprite_banks.c
    src/boss/rbo6/palette_def.c
    src/boss/rbo6/layers.c
    src/boss/rbo6/graphics_banks.c
    src/boss/rbo6/gen/e_laydef.c
    src/boss/rbo6/e_init.c
    src/boss/rbo6/background_block_init.c
    src/boss/rbo6/e_red_door_tiles.c
    src/boss/rbo6/e_breakable.c
    src/boss/rbo6/d_prize_drops.c
    src/boss/rbo6/st_update.c
    src/boss/rbo6/collision.c
    src/boss/rbo6/e_red_door.c
    src/boss/rbo6/st_common.c
    src/boss/rbo6/e_collect.c
    src/boss/rbo6/e_misc.c
    src/boss/rbo6/e_particles.c
    src/boss/rbo6/e_room_fg.c
    src/boss/rbo6/gen/rooms.c
    src/boss/rbo6/gen/e_layout.c
    src/boss/rbo6/cutscene_data.c
    src/boss/rbo6/stage_data.c
    src/boss/rbo6/tilemaps.c
    src/boss/rbo6/gen/us/sprites.c
    src/boss/rbo6/unk_1D0E4.c
    src/boss/rbo6/e_lock_camera.c
    src/boss/rbo6/unk_1D690.c
    src/boss/rbo6/e_cutscene_dialogue.c
    src/boss/rbo6/unk_2362C.c
    src/boss/rbo6/create_entity.c
    src/boss/rbo6/e_stage_name.c
    src/boss/rbo6/popup.c
    src/boss/rbo6/prim_helpers.c
    src/boss/rbo6/unk_30D68.c
    src/boss/rbo6/bss.c
    src/boss/rbo6/d_pc_data.c
)
add_overlay(rbo6 ${SOURCE_FILES_BOSS_RBO6})

# --- rbo8 ---
set(SOURCE_FILES_BOSS_RBO8
    src/pc/bosses/boss_rbo8.c
    src/boss/rbo8/header.c
    src/boss/rbo8/sprite_banks.c
    src/boss/rbo8/palette_def.c
    src/boss/rbo8/layers.c
    src/boss/rbo8/graphics_banks.c
    src/boss/rbo8/gen/e_laydef.c
    src/boss/rbo8/e_init.c
    src/boss/rbo8/d_g__red_door_tiles.c
    src/boss/rbo8/e_lock_camera.c
    src/boss/rbo8/d_prize_drops.c
    src/boss/rbo8/d_pc_data.c
    src/boss/rbo8/unk_151E8.c
    src/boss/rbo8/unk_1546C.c
    src/boss/rbo8/e_blade.c
    src/boss/rbo8/unk_15868.c
    src/boss/rbo8/st_update.c
    src/boss/rbo8/collision.c
    src/boss/rbo8/create_entity.c
    src/boss/rbo8/e_red_door.c
    src/boss/rbo8/st_common.c
    src/boss/rbo8/e_collect.c
    src/boss/rbo8/e_misc.c
    src/boss/rbo8/e_stage_name.c
    src/boss/rbo8/e_particles.c
    src/boss/rbo8/e_room_fg.c
    src/boss/rbo8/gen/rooms.c
    src/boss/rbo8/e_life_up.c
    src/boss/rbo8/gen/e_layout.c
    src/boss/rbo8/stage_data.c
    src/boss/rbo8/tilemaps.c
    src/boss/rbo8/gen/us/sprites.c
    src/boss/rbo8/popup.c
    src/boss/rbo8/giantbro_helpers.c
    src/boss/rbo8/prim_helpers.c
    src/boss/rbo8/bss.c
)
add_overlay(rbo8 ${SOURCE_FILES_BOSS_RBO8})

# --- tt_005 ---
# Sprite runs the Faerie code (TT_002) with its own data and sprite parts
set(SOURCE_FILES_TT_005
    src/servant/tt_002/init.c
    src/servant/tt_005/sprite_impl.c
    src/servant/tt_005/sprite_spriteparts.c
    src/pc/servants/servant_005.c
)
add_overlay(tt_005 ${SOURCE_FILES_TT_005})

# --- tt_006 ---
# Nose Devil runs the Demon code (TT_003) with its own data and sprite parts
set(SOURCE_FILES_TT_006
    src/servant/tt_003/init.c
    src/servant/tt_006/nosedevil_impl.c
    src/servant/tt_006/nosedevil_spriteparts.c
    src/pc/servants/servant_006.c
)
add_overlay(tt_006 ${SOURCE_FILES_TT_006})
