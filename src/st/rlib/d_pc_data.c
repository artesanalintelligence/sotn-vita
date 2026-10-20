// SPDX-License-Identifier: AGPL-3.0-or-later
// Data the PSX build still takes from the raw splat segments 0x824 and 0x1820
// (see config/splat.us.strlib.yaml). Only the PC port compiles this file.
#include "rlib.h"

// 0x824: breakable objects
u8 D_us_80180824[] = {3, 4, 3, 5, 0, 0, 0, 0};
u8 D_us_8018082C[] = {3, 6, 3, 7, 0, 0, 0, 0};
static u8 D_us_80180834[] = {6, 8, 6, 9, 6, 10, 6, 11, 6, 12, 0, 0};
static u8 D_us_80180840[] = {5, 9, 5, 10, 5, 11, 5, 12, 0, 0, 0, 0};
static u8 D_us_8018084C[] = {5, 13, 5, 14, 5, 15, 5, 16, 0, 0, 0, 0};
static u8 D_us_80180858[] = {5, 17, 5, 18, 5, 19, 0, 0};
static u8 D_us_80180860[] = {5, 23, 0, 0};
static u8 D_us_80180864[] = {5, 22, 0, 0};
static u8 D_us_80180868[] = {5, 20, 0xFF, 0xFF, 5, 21, 5, 21, 0xFF, 0, 0, 0};
AnimationFrame* g_RlibBreakableAnimations[] = {
    (AnimationFrame*)D_us_80180834, (AnimationFrame*)D_us_80180834,
    (AnimationFrame*)D_us_80180824, (AnimationFrame*)D_us_8018082C,
    (AnimationFrame*)D_us_80180840, (AnimationFrame*)D_us_8018084C,
    (AnimationFrame*)D_us_80180858, (AnimationFrame*)D_us_80180860,
    (AnimationFrame*)D_us_80180864, (AnimationFrame*)D_us_80180868,
};
u8 g_RlibBreakableHitboxHeights[] = {8, 8, 8, 8, 16, 16, 8, 8, 8, 8, 8, 0};
u8 g_RlibBreakableExplosionTypes[] = {2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0};
u16 g_RlibBreakablePalettes[] = {
    PAL_NONE,       PAL_NONE,       PAL_NONE,       PAL_NONE,
    PAL_BREAKABLE,  PAL_BREAKABLE,  PAL_BREAKABLE,  PAL_BREAKABLE,
    PAL_BREAKABLE,  PAL_BREAKABLE,
};
u16 g_RlibBreakableAnimSets[] = {
    ANIMSET_OVL(2), ANIMSET_OVL(2), ANIMSET_OVL(2), ANIMSET_OVL(2),
    ANIMSET_OVL(7), ANIMSET_OVL(7), ANIMSET_OVL(7), ANIMSET_OVL(7),
    ANIMSET_OVL(7), ANIMSET_OVL(7),
};
u8 g_RlibBreakableBlendModes[] = {
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0, 0,
};
u16 g_RlibBreakableUnk5A[] = {0, 0, 0, 0, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B};
u16 g_RlibBreakableHitboxOffsetsY[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 0x1820: Tin Man, Scarecrow and Schmoo
// clang-format off
s16 D_us_80181820[] = {0, 34, 0, 4, 24, -4, -48, 0};
s16 D_us_80181830[] = {0, 34, 24, 0};
u8 D_us_80181838[] = {3, 1, 3, 2, 3, 3, 3, 4, 0, 0, 0, 0};
u8 D_us_80181844[] = {
    32, 1, 4, 6, 3, 7, 2, 8, 2, 9, 8, 10, 2, 11, 2, 12, 2, 13, 2, 14,
    2, 15, 2, 16, 2, 17, 2, 18, 8, 17, 2, 19, 2, 21, 2, 20, 8, 21, 3, 22,
    3, 23, 3, 24, 3, 25, 3, 26, 3, 27, 3, 28, 3, 29, 3, 30, 32, 21, 0xFF, 0,
};
u8 D_us_80181880[] = {2, 21, 2, 31, 0, 0, 0, 0};
u8 D_us_80181888[] = {3, 32, 3, 33, 3, 34, 3, 35, 3, 36, 3, 37, 3, 38, 3, 39, 3, 40, 0xFF, 0};
u8 D_us_8018189C[] = {2, 42, 2, 43, 2, 44, 2, 45, 2, 46, 0, 0};
u8 D_us_801818A8[] = {2, 47, 2, 48, 2, 49, 2, 50, 2, 51, 2, 52, 0, 0, 0, 0};
u8 D_us_801818B8[] = {
    17, 53, 3, 54, 3, 55, 3, 56, 3, 57, 17, 58, 3, 57, 3, 56, 3, 55, 3, 54,
    0xFF, 0, 0, 0,
};
// clang-format on

// timer, gravity, velocityX, velocityY of the pieces Tin Man breaks into
struct {
    s16 timer;
    u8 hasGravity;
    s32 velocityX;
    s32 velocityY;
} D_us_801818D0[] = {
    {0x34, 0, FIX(-1.5), FIX(0)},     {0x1C, 1, FIX(0.75), FIX(-1.25)},
    {0x10, 1, FIX(1), FIX(-4)},       {0x18, 1, FIX(0.5), FIX(-1.5)},
    {0x30, 1, FIX(0.375), FIX(-1)},   {0x20, 1, FIX(0.875), FIX(-1)},
    {0x28, 0, FIX(3), FIX(0)},        {0x28, 1, FIX(0), FIX(-3)},
    {0x34, 0, FIX(-0.75), FIX(0)},    {0x08, 1, FIX(0.5), FIX(-2)},
    {0x04, 1, FIX(-0.25), FIX(-1)},
};

// animCurFrame, zPriority, offsetX, offsetY, minRotate, maxRotate of the
// four Scarecrow limbs
s16 D_us_80181954[][6] = {
    {3, -1, 5, -11, -0x400, 0x180},
    {4, -1, -9, -6, -0xFFF, 0xFFF},
    {5, 1, -9, -6, -0xFFF, 0xFFF},
    {6, 1, 5, -11, -0x400, 0x100},
};
s16 D_us_80181984[] = {0, 48, 1, 0};
u8 D_us_8018198C[] = {5, 8, 5, 9, 5, 10, 5, 11, 0, 0, 0, 0};
u8 D_us_80181998[] = {8, 12, 8, 13, 8, 12, 0xFF, 0};

// clang-format off
unsigned char rooms[] = {
    15, 42, 19, 45, 0, 0, 2, 1,
    14, 45, 14, 45, 1, 0, 3, 2,
    13, 45, 13, 45, 2, 0, 2, 3,
    13, 44, 14, 44, 3, 0, 2, 4,
    14, 42, 14, 43, 4, 0, 4, 5,
    12, 43, 13, 43, 5, 0, 2, 6,
    12, 42, 13, 42, 6, 0, 2, 7,
    11, 45, 11, 45, 7, 0, 2, 8,
    11, 42, 11, 44, 8, 0, 2, 9,
    4, 43, 10, 45, 9, 0, 2, 10,
    9, 42, 10, 42, 10, 0, 2, 11,
    8, 42, 8, 42, 11, 0, 2, 12,
    5, 42, 7, 42, 12, 0, 2, 13,
    4, 42, 4, 42, 13, 0, 0, 52,
    12, 44, 12, 44, 14, 0, 0, 50,
    3, 42, 3, 42, 6, -1, 0, 0,
    64,
};
// clang-format on
