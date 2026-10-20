// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo7.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo7/nonmatchings/unk_13258", EntityBreakable);
INCLUDE_ASM("boss/bo7/nonmatchings/unk_13258", EntityBreakableDebris);
#else
// Same braziers as CHI, but hitting them plays the candle and debris sounds
#define CANDLE_HIT_SFX SFX_CANDLE_HIT
#define CANDLE_HIT_SFX_2 SFX_WALL_DEBRIS_B
#define BREAKABLE_ZPRIORITY 112

// D_us_8018056C: anim_brazier_short
// D_us_8018057C: anim_brazier_tall
// D_us_8018058C: animations
// D_us_801805AC: hitbox_heights
// D_us_801805B4: explosion_types
// D_us_801805BC: anim_sets
// D_us_801805CC: blend_modes
// D_us_801805D4: debris_offsets_y
#include "../../st/e_breakable_with_lighting.h"
#endif
