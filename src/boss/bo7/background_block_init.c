// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo7.h"

// D_us_80180484
static u8 AnimFrames_80180730[] = {0x40, 0x01, 0xFF, 0x00};

// D_us_80180488
static u8 AnimFrames_80180734[] = {
    0x02, 0x25, 0x02, 0x26, 0x02, 0x27, 0x02, 0x26, 0x00, 0x00, 0x00, 0x00,
};

// Background Block
// BackgroundBlockInit
ObjInit2 BackgroundBlockInit[] = {
    {
        .animSet = ANIMSET_DRA(6),
        .zPriority = 0x1FA,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = DRAW_DEFAULT,
        .blendMode = BLEND_TRANSP,
        .animFrames = AnimFrames_80180730,
    },
    {
        .animSet = ANIMSET_OVL(1),
        .zPriority = 0xC0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = DRAW_TRANSP | DRAW_UNK02,
        .blendMode = BLEND_TRANSP | BLEND_ADD,
        .flags = 0,
        .animFrames = AnimFrames_80180734,
    }};
