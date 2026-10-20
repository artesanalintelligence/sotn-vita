// SPDX-License-Identifier: AGPL-3.0-or-later
// Data still kept as raw bytes by the PSX build (0x808 in the splat config),
// rebuilt here for the PC port. Same layout as ARE, mirrored for the inverted
// castle and using the second overlay animation set.
#include "rare.h"

static AnimateEntityFrame anim_one[] = {{16, 0xB}, POSE_LOOP(0)};
static AnimateEntityFrame anim_two[] = {
    {8, 0x0C}, {8, 0x0D}, {8, 0x0E}, {8, 0x0F}, {8, 0x10}, POSE_LOOP(0),
};
static AnimateEntityFrame anim_three[] = {{16, 0x13}, POSE_END};
static AnimateEntityFrame anim_four[] = {{16, 0xD}, POSE_END};
static AnimateEntityFrame anim_five[] = {{16, 0xE}, POSE_END};

ObjInit2 BackgroundBlockInit[] = {
    {
        .animSet = ANIMSET_OVL(2),
        .zPriority = 105,
        .facingLeft = 0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = ENTITY_DEFAULT,
        .blendMode = BLEND_NO,
        .animFrames = (u8*)anim_one,
    },
    {
        .animSet = ANIMSET_OVL(2),
        .zPriority = 105,
        .facingLeft = 0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = ENTITY_DEFAULT,
        .blendMode = BLEND_TRANSP | BLEND_ADD,
        .animFrames = (u8*)anim_two,
    },
    {
        .animSet = ANIMSET_OVL(2),
        .zPriority = 108,
        .facingLeft = 0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = ENTITY_DEFAULT,
        .blendMode = BLEND_NO,
        .animFrames = (u8*)anim_three,
    },
    {
        .animSet = ANIMSET_OVL(2),
        .zPriority = 108,
        .facingLeft = 0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = ENTITY_DEFAULT,
        .blendMode = BLEND_NO,
        .animFrames = (u8*)anim_four,
    },
    {
        .animSet = ANIMSET_OVL(2),
        .zPriority = 108,
        .facingLeft = 0,
        .unk5A = 0,
        .palette = 0,
        .drawFlags = ENTITY_DEFAULT,
        .blendMode = BLEND_NO,
        .animFrames = (u8*)anim_five,
    },
};

// Owned by EntityRedDoor to animate the tiles behind the door itself
u16 g_RedDoorTiles[][8] = {
    {0x0032, 0x0033, 0x0032, 0x0031, 0x01C8, 0x0266, 0x01C8, 0x01C1},
    {0x0035, 0x0036, 0x0035, 0x0034, 0x01CB, 0x0269, 0x01CB, 0x01C4},
};
