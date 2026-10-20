// SPDX-License-Identifier: AGPL-3.0-or-later
// PC only: on PSX this data is still kept as assembly. Same as BO4's.
#include "bo6.h"

static u8 AnimFrames_801804D8[] = {0x10, 0x0B, 0x00, 0x00};
static u8 AnimFrames_801804DC[] = {
    0x08, 0x0C, 0x08, 0x0D, 0x08, 0x0E, 0x08, 0x0F, 0x08, 0x10, 0x00, 0x00,
};

ObjInit2 BackgroundBlockInit[] = {
    {
        .animSet = ANIMSET_OVL(1),
        .zPriority = 0x69,
        .facingLeft = 0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = ENTITY_DEFAULT,
        .blendMode = BLEND_NO,
        .flags = 0,
        .animFrames = AnimFrames_801804D8,
    },
    {
        .animSet = ANIMSET_OVL(1),
        .zPriority = 0x69,
        .facingLeft = 0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = ENTITY_DEFAULT,
        .blendMode = BLEND_TRANSP | BLEND_ADD,
        .flags = 0,
        .animFrames = AnimFrames_801804DC,
    }};
