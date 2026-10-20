// SPDX-License-Identifier: AGPL-3.0-or-later
// Demon switch and the secret wall it opens. Same entities as CHI's
// en_demon_switch_wall.c, except that any hit presses the switch.
#include "bo7.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo7/nonmatchings/e_demon_switch_wall", UpdateFallingPebble);
INCLUDE_ASM("boss/bo7/nonmatchings/e_demon_switch_wall", EntityDemonSwitch);
INCLUDE_ASM("boss/bo7/nonmatchings/e_demon_switch_wall", EntityDemonSwitchWall);
#else
extern EInit g_EInitSecret;

#include "../../st/e_falling_pebble.h"

// E_DEMON_SWITCH
void EntityDemonSwitch(Entity* self) {
    enum Step {
        INIT = 0,
        PRESS = 1,
    };

    switch (self->step) {
    case INIT:
        InitializeEntity(g_EInitSecret);

        self->animCurFrame = 3;
        self->hitPoints = 32767;
        self->hitboxState = 3;
        self->hitboxWidth = 6;
        self->hitboxHeight = 8;

        if (g_CastleFlags[CHI_DEMON_SWITCH]) {
            self->animCurFrame = 4;
        }
        // fallthrough
    case PRESS:
        if (self->hitParams) {
            g_api.PlaySfx(SFX_ANIME_SWORD_B);
            g_CastleFlags[CHI_DEMON_SWITCH] = 1;
            g_api.RevealSecretPassageAtPlayerPositionOnMap(CHI_DEMON_SWITCH);
            self->animCurFrame = 4;
            self->step++;
        }
        break;
    }
}

// clang-format off
// D_us_801805F0
static s16 WallTiles[] = {
    // With Collision
    0x039D, 0x03A0, 0x03A0, 0x039E, 0x03A0, 0x03A0, 0x03A0, 0x039F,
    0x03A0, 0x03A0, 0x03A0, 0x03A0,
    // Without Collision
    0x01C2, 0x01BF, 0x01BF, 0x01D2, 0x01BF, 0x01BF, 0x01BF, 0x01D3,
    0x01BF, 0x01BF, 0x01BF, 0x01BF,
};
// clang-format on

// E_DEMON_SWITCH_WALL
void EntityDemonSwitchWall(Entity* self) {
    enum Step {
        INIT = 0,
        IDLE_CLOSED = 1,
        PREP_TO_OPEN = 2,
        OPENING = 3,
        IDLE_OPEN = 16,
    };

    s32 tileIdx;
    s16* pSrcTile;
    s32 iRow;
    s32 iCol;
    s32 primIdx;
    Primitive* prim;
    Entity* newEntity;
    s32 remainingColumnCount;
    s32 xPos;
    s32 yPos;

    switch (self->step) {
    case INIT:
        InitializeEntity(g_EInitSecret);

        self->animCurFrame = 1;

        pSrcTile = WallTiles;
        if (g_CastleFlags[CHI_DEMON_SWITCH]) {
            pSrcTile += 0xC;
        }

        tileIdx = 0x6D;
        for (iCol = 0; iCol < 3; tileIdx++, iCol++) {
            for (iRow = 0; iRow < 4; iRow++, pSrcTile++) {
                *(&g_Tilemap.fg[tileIdx] + iRow * 16) = *pSrcTile;
            }
        }

        if (g_CastleFlags[CHI_DEMON_SWITCH]) {
            self->animCurFrame = 0;
            self->step = IDLE_OPEN;
            break;
        }
        // fallthrough
    case IDLE_CLOSED:
        if (g_CastleFlags[CHI_DEMON_SWITCH]) {
            self->step++;
        }
        break;
    case PREP_TO_OPEN:
        primIdx = g_api.AllocPrimitives(PRIM_TILE, 16);
        if (primIdx != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIdx;
            prim = &g_PrimBuf[primIdx];
            self->ext.demonSwitchWall.prim = prim;

            while (prim != NULL) {
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
            }
        } else {
            DestroyEntity(self);
            return;
        }
        self->step++;
        return;
    case OPENING:
        self->ext.demonSwitchWall.unk80++;
        if (self->ext.demonSwitchWall.unk80 & 1) {
            self->posY.i.hi++;
        } else {
            self->posY.i.hi--;
        }

        if (!(self->ext.demonSwitchWall.unk80 % 8)) {
            g_api.PlaySfx(SFX_WALL_DEBRIS_B);
        }
        MoveEntity();

        if (self->velocityX < FIX(0.25)) {
            self->velocityX += FIX(0.0078125);
        }

        prim = self->ext.demonSwitchWall.prim;
        prim = FindFirstUnkPrim(prim);
        if (prim != NULL) {
            prim->p3 = 1;

            xPos = self->posX.i.hi + (Random() & 63) + -24;
            if (xPos > 0x100) {
                xPos -= 0x10;
            }

            yPos = self->posY.i.hi - 0x20;
            prim->x0 = xPos;
            prim->y0 = yPos;
        }

        prim = self->ext.demonSwitchWall.prim;
        while (prim != NULL) {
            if (prim->p3) {
                UpdateFallingPebble(prim);
            }
            prim = prim->next;
        }

        xPos = self->posX.i.hi - 0x18;
        yPos = self->posY.i.hi + 0x20;
        newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (newEntity != NULL) {
            CreateEntityFromCurrentEntity(E_GREY_PUFF, newEntity);
            newEntity->posX.i.hi = xPos + (Random() & 0x1F);
            newEntity->posY.i.hi = yPos;
            newEntity->params = Random() & 3;
            newEntity->zPriority = 0xA0;
        }

        remainingColumnCount = self->posX.i.hi - 0xE8;
        remainingColumnCount >>= 4;
        if (remainingColumnCount > 3) {
            remainingColumnCount = 3;
        }

        pSrcTile = WallTiles;
        pSrcTile += 0xC;
        tileIdx = 0x6D;
        for (iCol = 0; iCol < remainingColumnCount; tileIdx++, iCol++) {
            for (iRow = 0; iRow < 4; iRow++, pSrcTile++) {
                *((&g_Tilemap.fg[tileIdx]) + iRow * 16) = *pSrcTile;
            }
        }

        if (self->posX.i.hi > 0x128) {
            DestroyEntity(self);
        }
        break;
    case IDLE_OPEN:
        if (g_pads[1].pressed & PAD_SQUARE) {
            if (self->params) {
                break;
            }
            self->animCurFrame++;
            self->params |= 1;
        } else {
            self->params = 0;
        }

        if (g_pads[1].pressed & PAD_CIRCLE) {
            if (self->step_s) {
                break;
            }
            self->animCurFrame--;
            self->step_s |= 1;
        } else {
            self->step_s = 0;
        }
        break;
    }
}
#endif
