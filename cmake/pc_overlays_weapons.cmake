# Weapon overlays not covered by the foreach in the top-level CMakeLists.txt.
# Each weapon source is built twice, once per hand (WEAPON0 / WEAPON1), and
# loaded at runtime by HandleWeaponPrg as w<hand>_<id>.
set(PC_EXTRA_WEAPON_IDS
    001 003 004 005 006 009 011 013 014 015 016 017 018 019 020 021 022 023
    024 025 026 027 028 029 030 031 032 033 036 038 039 042 044 047 048 049
    053 054 055 056 057 058
)

# Weapons whose data (or .bss) is not yet migrated to C on the PSX side. Their
# data lives in a generated PC-only translation unit, src/pc/weapons/w_NNN.c.
set(PC_WEAPON_DATA_IDS
    001 003 004 005 006 009 011 013 014 015 016 017 018 019 020 021 022 023
    024 025 026 027 028 029 030 031 032 033 036 038 039 042 047 048 049 053
    054 055 056 057 058
)

# Optional headless harness to exercise weapon and familiar overlays, see
# src/pc/equip_test.c. Off by default.
option(PC_EQUIP_TEST "Build the weapon/familiar overlay test harness" OFF)
if(PC_EQUIP_TEST)
    target_sources(${PROJECT_NAME} PRIVATE src/pc/equip_test.c)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PC_EQUIP_TEST)
endif()

foreach(id ${PC_EXTRA_WEAPON_IDS})
    set(_sources src/weapon/w_${id}.c)
    if(id IN_LIST PC_WEAPON_DATA_IDS)
        list(APPEND _sources src/pc/weapons/w_${id}.c)
    endif()
    add_overlay(w0_${id} ${_sources})
    target_compile_definitions(${w0_${id}_COMPILE_TARGET} PRIVATE WEAPON0)
    add_overlay(w1_${id} ${_sources})
    target_compile_definitions(${w1_${id}_COMPILE_TARGET} PRIVATE WEAPON1)
endforeach()

# Familiars: Demon (tt_003) and Sword (tt_004)
add_overlay(tt_003
    src/servant/tt_003/init.c
    src/servant/tt_003/demon_spriteparts.c
    src/servant/tt_003/demon.c
    src/pc/servants/servant_003.c
)
add_overlay(tt_004
    src/servant/tt_004/init.c
    src/servant/tt_004/servant_clutdata.c
    src/servant/tt_004/sword.c
    src/servant/tt_004/servant_common.c
    src/pc/servants/servant_004.c
)

# --- mar (Marble Gallery clock room cutscene with Maria, BOSS/MAR) ---
set(SOURCE_FILES_BOSS_MAR
    src/pc/bosses/boss_mar.c
    src/pc/bosses/mar_data.c
    src/boss/mar/CA94.c
    src/boss/mar/background_block_init.c
    src/boss/mar/clock_room.c
    src/boss/mar/collision.c
    src/boss/mar/create_entity.c
    src/boss/mar/cutscene.c
    src/boss/mar/cutscene_data.c
    src/boss/mar/d_prize_drops.c
    src/boss/mar/e_breakable.c
    src/boss/mar/e_collect.c
    src/boss/mar/e_init.c
    src/boss/mar/e_life_up.c
    src/boss/mar/e_lock_camera.c
    src/boss/mar/e_misc.c
    src/boss/mar/e_particles.c
    src/boss/mar/e_red_door.c
    src/boss/mar/e_red_door_tiles.c
    src/boss/mar/e_room_bg.c
    src/boss/mar/e_room_fg.c
    src/boss/mar/e_stage_name.c
    src/boss/mar/gen/e_laydef.c
    src/boss/mar/gen/e_layout.c
    src/boss/mar/gen/rooms.c
    src/boss/mar/gen/sprites.c
    src/boss/mar/graphics_banks.c
    src/boss/mar/header.c
    src/boss/mar/layers.c
    src/boss/mar/palette_def.c
    src/boss/mar/popup.c
    src/boss/mar/prim_helpers.c
    src/boss/mar/sprite_banks.c
    src/boss/mar/st_common.c
    src/boss/mar/st_update.c
    src/boss/mar/stage_data.c
    src/boss/mar/unk.c
)
add_overlay(mar ${SOURCE_FILES_BOSS_MAR})
