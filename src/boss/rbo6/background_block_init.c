// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo6.h"

static u8 anim_frames_0[] = {0x40, 0x01, 0xFF, 0x00};
static u8 anim_frames_1[] = {
    0x02, 0x25, 0x02, 0x26, 0x02, 0x27, 0x02, 0x26, 0x00, 0x00, 0x00, 0x00};

ObjInit2 BackgroundBlockInit[] = {
    {.animSet = ANIMSET_DRA(6),
     .zPriority = 0x01FA,
     .facingLeft = 0,
     .unk5A = 0,
     .palette = 0,
     .drawFlags = ENTITY_DEFAULT,
     .blendMode = BLEND_TRANSP,
     .flags = 0,
     .animFrames = anim_frames_0},
    {.animSet = ANIMSET_OVL(1),
     .zPriority = 0x00C0,
     .facingLeft = 0,
     .unk5A = 0,
     .palette = 0,
     .drawFlags = ENTITY_SCALEX | ENTITY_SCALEY,
     .blendMode = BLEND_TRANSP | BLEND_ADD,
     .flags = 0,
     .animFrames = anim_frames_1},
};
