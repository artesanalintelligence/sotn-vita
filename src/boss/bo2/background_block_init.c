// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"

static u8 anim_frames_0[] = {0x10, 0x0B, 0x00, 0x00};
static u8 anim_frames_1[] = {
    0x08, 0x0C, 0x08, 0x0D, 0x08, 0x0E, 0x08, 0x0F, 0x08, 0x10, 0x00, 0x00};

ObjInit2 BackgroundBlockInit[] = {
    {
        .animSet = ANIMSET_OVL(1),
        .zPriority = 0x69,
        .facingLeft = 0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = DRAW_DEFAULT,
        .blendMode = BLEND_NO,
        .flags = 0,
        .animFrames = anim_frames_0,
    },
    {
        .animSet = ANIMSET_OVL(1),
        .zPriority = 0x69,
        .facingLeft = 0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = DRAW_DEFAULT,
        .blendMode = BLEND_TRANSP | BLEND_ADD,
        .flags = 0,
        .animFrames = anim_frames_1,
    },
};
