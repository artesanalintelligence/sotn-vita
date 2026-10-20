// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo6.h"

extern u8 D_us_80181524[][8];
extern u8 D_us_8018299C[];

// Equivalent of func_801641A0 in RIC
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", func_us_801BE79C);
#else
void func_us_801BE79C(Entity* self) {
    Primitive* prim;

    self->posX.i.hi = RIC.posX.i.hi;
    self->posY.i.hi = RIC.posY.i.hi - 8;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->ext.circleExpand.width = 16;
        self->ext.circleExpand.height = 12;
        prim = &g_PrimBuf[self->primIndex];
        prim->u0 = prim->u2 = 64;
        prim->v0 = prim->v1 = 192;
        prim->u1 = prim->u3 = 127;
        prim->v2 = prim->v3 = 255;
        PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3) = 128;
        prim->tpage = 0x1A;
        prim->clut = PAL_CC_FIRE_EFFECT;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        prim->priority = RIC.zPriority + 8;
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->step++;
        break;
    case 1:
        self->ext.circleExpand.width += 2;
        self->ext.circleExpand.height += 2;
        if (self->ext.circleExpand.width > 56) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    prim = &g_PrimBuf[self->primIndex];
    prim->x0 = self->posX.i.hi - self->ext.circleExpand.width;
    prim->y0 = self->posY.i.hi - self->ext.circleExpand.height;
    prim->x1 = self->posX.i.hi + self->ext.circleExpand.width;
    prim->y1 = self->posY.i.hi - self->ext.circleExpand.height;
    prim->x2 = self->posX.i.hi - self->ext.circleExpand.width;
    prim->y2 = self->posY.i.hi + self->ext.circleExpand.height;
    prim->x3 = self->posX.i.hi + self->ext.circleExpand.width;
    prim->y3 = self->posY.i.hi + self->ext.circleExpand.height;
    if (prim->b3 >= 12) {
        prim->b3 -= 12;
    }
    PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3);
}
#endif

extern s16 D_us_80181CAC[][6];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityShrinkingPowerUpRing);
#else
void RicEntityShrinkingPowerUpRing(Entity* self) {
    s16 variant;
    s16 rMod;
    s16* loadedParams;
    s16 x;
    s16 gMod;
    s16 bMod;
    s16 gOffset;
    s16 bOffset;
    s16 angleStart;
    s16 y;
    s16 baseAngle;
    s32 i;
    s16 angle;
    Primitive* prim1;
    Primitive* prim2;

    variant = (self->params & 0x7F00) >> 8;
    loadedParams = D_us_80181CAC[variant];
    rMod = loadedParams[2];
    gMod = loadedParams[3];
    bMod = loadedParams[4];
    gOffset = loadedParams[0];
    bOffset = loadedParams[1];
    self->posX.i.hi = RIC.posX.i.hi;
    self->posY.i.hi = RIC.posY.i.hi;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 32);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        prim2 = prim1 = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 16; i++) {
            prim1 = prim1->next;
        }
        for (i = 0; i < 16; i++) {
            angle = i << 8;
            prim2->u0 = (rcos(angle) >> 4 << 5 >> 8) + 0x20;
            prim2->v0 = -(rsin(angle) >> 4 << 5 >> 8) + 0xDF;
            angle = (i + 1) << 8;
            prim2->u1 = (rcos(angle) >> 4 << 5 >> 8) + 0x20;
            prim2->v1 = -(rsin(angle) >> 4 << 5 >> 8) + 0xDF;
            prim1->u2 = prim1->u3 = 0x20;
            prim1->v2 = prim1->v3 = 0xDF;
            prim2->u2 = prim1->u0 = (prim2->u0 + prim1->u2) / 2;
            prim2->v2 = prim1->v0 = (prim2->v0 + prim1->v2) / 2;
            prim2->u3 = prim1->u1 = (prim2->u1 + prim1->u3) / 2;
            prim2->v3 = prim1->v1 = (prim2->v1 + prim1->v3) / 2;
            prim1->tpage = prim2->tpage = 0x1A;
            prim1->clut = prim2->clut = PAL_FILL_WHITE;
            prim1->priority = prim2->priority = RIC.zPriority + 2;
            prim1->drawMode = prim2->drawMode = DRAW_TPAGE2 | DRAW_TPAGE |
                                                DRAW_COLORS | DRAW_UNK02 |
                                                DRAW_TRANSP;
            prim2 = prim2->next;
            prim1 = prim1->next;
        }
        self->ext.ricPowerRing.unk80 = self->ext.ricPowerRing.unk82 = 0x280;
        self->ext.ricPowerRing.unk84 = self->ext.ricPowerRing.unk86 = 0x240;
        self->ext.ricPowerRing.unk8A = loadedParams[5];
        self->ext.ricPowerRing.unk88 = 0xC0;
        self->step++;
        break;
    case 1:
        self->ext.ricPowerRing.unk7E += 0x40;
        self->ext.ricPowerRing.unk86 -= 10;
        if (self->ext.ricPowerRing.unk86 < 0) {
            self->ext.ricPowerRing.unk86 = 0;
            self->ext.ricPowerRing.unk7C = 0x20;
            self->step++;
        }
        self->ext.ricPowerRing.unk84 = self->ext.ricPowerRing.unk86;
        self->ext.ricPowerRing.unk82 -= 5;
        self->ext.ricPowerRing.unk80 -= 5;
        break;
    case 2:
        self->ext.ricPowerRing.unk7E += 0x40;
        self->ext.ricPowerRing.unk82 -= 3;
        self->ext.ricPowerRing.unk80 -= 6;
        if (--self->ext.ricPowerRing.unk7C == 0) {
            self->step++;
        }
        break;
    case 3:
        self->ext.ricPowerRing.unk7E += 0x40;
        self->ext.ricPowerRing.unk82 -= 3;
        self->ext.ricPowerRing.unk80 -= 6;
        self->ext.ricPowerRing.unk88 -= 12;
        if (self->ext.ricPowerRing.unk88 < 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    angleStart = self->ext.ricPowerRing.unk8A;
    x = self->posX.i.hi;
    y = self->posY.i.hi;
    prim1 = prim2 = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 16; i++) {
        prim1 = prim1->next;
    }
    for (i = 0; i < 16; i++) {
        prim2->x0 =
            x + (prim2->u0 - 0x20) * self->ext.ricPowerRing.unk80 / 0x100;
        prim2->y0 =
            y + (prim2->v0 - 0xE0) * self->ext.ricPowerRing.unk82 / 0x100;
        prim2->x1 =
            x + (prim2->u1 - 0x20) * self->ext.ricPowerRing.unk80 / 0x100;
        prim2->y1 =
            y + (prim2->v1 - 0xE0) * self->ext.ricPowerRing.unk82 / 0x100;
        prim1->x2 = x + (rcos((i + 1) << 8) >> 4 << 5 >> 8) *
                            self->ext.ricPowerRing.unk84 / 0x100;
        prim1->y2 = y - (rsin((i + 1) << 8) >> 4 << 5 >> 8) *
                            self->ext.ricPowerRing.unk86 / 0x100;
        prim1->x3 = x + (rcos((i + 2) << 8) >> 4 << 5 >> 8) *
                            self->ext.ricPowerRing.unk84 / 0x100;
        prim1->y3 = y - (rsin((i + 2) << 8) >> 4 << 5 >> 8) *
                            self->ext.ricPowerRing.unk86 / 0x100;
        prim2->x2 = prim1->x0 = (prim2->x0 + prim1->x2) / 2;
        prim2->y2 = prim1->y0 = (prim2->y0 + prim1->y2) / 2;
        prim2->x3 = prim1->x1 = (prim2->x1 + prim1->x3) / 2;
        prim2->y3 = prim1->y1 = (prim2->y1 + prim1->y3) / 2;
        baseAngle = i * angleStart;
        angle = self->ext.ricPowerRing.unk7E + baseAngle;
        prim1->r0 = prim2->r2 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / rMod;
        angle = self->ext.ricPowerRing.unk7E + gOffset + baseAngle;
        prim1->g0 = prim2->g2 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / gMod;
        angle = self->ext.ricPowerRing.unk7E + bOffset + baseAngle;
        prim1->b0 = prim2->b2 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / bMod;
        angle = self->ext.ricPowerRing.unk7E + angleStart + baseAngle;
        prim1->r1 = prim2->r3 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / rMod;
        angle = self->ext.ricPowerRing.unk7E + gOffset + angleStart + baseAngle;
        prim1->g1 = prim2->g3 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / gMod;
        angle = self->ext.ricPowerRing.unk7E + bOffset + angleStart + baseAngle;
        prim1->b1 = prim2->b3 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / bMod;
        prim2->r0 = prim2->g0 = prim2->b0 = prim2->r1 = prim2->g1 = prim2->b1 =
            0;
        prim1->r2 = prim1->g2 = prim1->b2 = prim1->r3 = prim1->g3 = prim1->b3 =
            0;
        prim2 = prim2->next;
        prim1 = prim1->next;
    }
}
#endif

extern Point16* D_us_80181D54[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityHitByIce);
#else
void RicEntityHitByIce(Entity* self) {
    const int PrimCount = 24;
    s16 x;
    s16 y;
    s32 i;
    bool terminateFlag;
    s16 deltaX;
    s16 deltaY;
    s16 yMod;
    s32 distance;
    s16 angle;
    Point16* pos;
    Primitive* prim;

    self->posX.i.hi = RIC.posX.i.hi;
    self->posY.i.hi = RIC.posY.i.hi;
    terminateFlag = 0;
    if (!(g_Ric.status & PLAYER_STATUS_UNK10000)) {
        terminateFlag = 1;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT3, PrimCount);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_HAS_PRIMS | FLAG_POS_CAMERA_LOCKED;
        prim = &g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->r0 = prim->r1 = prim->r2 = prim->r3 = (rand() & 0x0F) + 0x30;
            prim->b0 = prim->b1 = prim->b2 = prim->b3 = (rand() & 0x7F) + 0x80;
            prim->g0 = prim->g1 = prim->g2 = prim->g3 = (rand() & 0x1F) + 0x30;
            if (rand() & 1) {
                prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE2 | DRAW_TPAGE |
                                 DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
            } else {
                prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE | DRAW_COLORS |
                                 DRAW_UNK02 | DRAW_TRANSP;
            }
            prim->type = PRIM_G4;
            prim->priority = RIC.zPriority + 2;
            prim = prim->next;
        }
        if (RIC.velocityY != 0) {
            self->ext.hitbyice.unk7E = 1;
        }
        if (RIC.velocityY != 0) {
            if (RIC.facingLeft) {
                self->rotate = ROT(22.5);
            } else {
                self->rotate = ROT(-22.5);
            }
        } else {
            if (RIC.velocityX > 0) {
                self->rotate = ROT(11.25);
            } else {
                self->rotate = ROT(348.75);
            }
        }
        if (RIC.step == PL_S_DEAD) {
            if (RIC.facingLeft) {
                self->rotate = ROT(33.75);
            } else {
                self->rotate = ROT(-33.75);
            }
            self->ext.hitbyice.unk80 = 1;
            self->ext.hitbyice.unk82 = 0x3C;
            if (self->params & 0x7F00) {
                self->ext.hitbyice.unk82 = 0x14;
            }
            self->ext.hitbyice.unk7E = 0;
        }
        self->step++;
        break;
    case 1:
        if (RIC.step == PL_S_DEAD) {
            if ((RIC.animCurFrame & 0x7FFF) == 0x21) {
                if (RIC.facingLeft) {
                    self->rotate = ROT(56.25);
                } else {
                    self->rotate = ROT(-56.25);
                }
            }
            if ((RIC.animCurFrame & 0x7FFF) == 0x22) {
                if (RIC.facingLeft) {
                    self->rotate = ROT(78.75);
                } else {
                    self->rotate = ROT(-78.75);
                }
            }
            if ((RIC.animCurFrame & 0x7FFF) == 0x20) {
                if (RIC.facingLeft) {
                    self->rotate = ROT(33.75);
                } else {
                    self->rotate = ROT(-33.75);
                }
            }
        }
        if (self->ext.hitbyice.unk80 && --self->ext.hitbyice.unk82 == 0) {
            terminateFlag = true;
        }
        if (self->ext.hitbyice.unk7E &&
            g_Ric.vram_flag & (TOUCHING_L_WALL | TOUCHING_R_WALL)) {
            terminateFlag = true;
        }
        if (terminateFlag) {
            self->ext.hitbyice.unk7C = 0x40;
            if (self->ext.hitbyice.unk80) {
                self->ext.hitbyice.unk7C = 0x80;
            }
            self->step++;
        }
        break;
    case 2:
        if (--self->ext.hitbyice.unk7C == 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    x = self->posX.i.hi;
    y = self->posY.i.hi;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < PrimCount; i++) {
        pos = D_us_80181D54[i * 3];
        if (prim->u0 < 2) {
            distance = SquareRoot12((pos->x * pos->x + pos->y * pos->y) << 0xC);
            angle = self->rotate + ratan2(pos->y, pos->x);
            deltaX = (((rcos(angle) >> 4) * distance) + 0x80000) >> 0x14;
            deltaY = (((rsin(angle) >> 4) * distance) + 0x80000) >> 0x14;
            prim->x0 = x + deltaX;
            prim->y0 = y + deltaY;

            pos = D_us_80181D54[i * 3 + 1];
            distance = SquareRoot12((pos->x * pos->x + pos->y * pos->y) << 0xC);
            angle = self->rotate + ratan2(pos->y, pos->x);
            deltaX = (((rcos(angle) >> 4) * distance) + 0x80000) >> 0x14;
            deltaY = (((rsin(angle) >> 4) * distance) + 0x80000) >> 0x14;
            prim->x1 = x + deltaX;
            prim->y1 = y + deltaY;

            pos = D_us_80181D54[i * 3 + 2];
            distance = SquareRoot12((pos->x * pos->x + pos->y * pos->y) << 0xC);
            angle = self->rotate + ratan2(pos->y, pos->x);
            deltaX = (((rcos(angle) >> 4) * distance) + 0x80000) >> 0x14;
            deltaY = (((rsin(angle) >> 4) * distance) + 0x80000) >> 0x14;
            prim->x2 = prim->x3 = x + deltaX;
            prim->y2 = prim->y3 = y + deltaY;
        }
        if (prim->u0 == 0 && terminateFlag) {
            prim->u0++;
            prim->v0 = (rand() & 15) + 1;
            if (self->ext.hitbyice.unk80) {
                prim->v0 = (rand() % 60) + 1;
            }
        }
        if (prim->u0 == 1) {
            if (--prim->v0 == 0) {
                prim->v0 = 0x20;
                prim->u2 = 0xF0;
                prim->u0++;
                if (self->ext.hitbyice.unk80) {
                    prim->v0 = (rand() & 31) + 0x28;
                }
            }
        }
        if (prim->u0 == 2) {
            if (prim->u2 < 0x70 || prim->u2 > 0xD0) {
                prim->u2 += 4;
            }
            yMod = (s8)prim->u2 >> 4;
            if (self->ext.hitbyice.unk80) {
                yMod = yMod >> 1;
            }
            prim->y0 += yMod;
            prim->y2 += yMod;
            prim->y1 += yMod;
            prim->y3 += yMod;
            if (prim->r3 < 4) {
                prim->r3 -= 4;
            }
            if (prim->g3 < 4) {
                prim->g3 -= 4;
            }
            if (prim->b3 < 4) {
                prim->b3 -= 4;
            }
            prim->r0 = prim->r1 = prim->r2 = prim->r3;
            prim->b0 = prim->b1 = prim->b2 = prim->b3;
            prim->g0 = prim->g1 = prim->g2 = prim->g3;
            prim->drawMode &= ~(DRAW_UNK_100 | DRAW_UNK02);
            if (--prim->v0 == 0) {
                prim->drawMode |= DRAW_HIDE;
            }
        }
        prim = prim->next;
    }
}
#endif

extern u16 D_us_80181E74[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityHitByLightning);
#else
void RicEntityHitByLightning(Entity* self) {
    s16 x;
    s16 y;
    s16 mul2;
    s32 i;
    s32 xMod;
    s32 yMod;
    bool terminate;
    s32 mul;
    s16 angle;
    Primitive* primPrev;
    Primitive* prim;

    terminate = false;
    if ((self->params & 0xFF00) == 0x100) {
        if ((++self->ext.hitbylightning.unk9C) > 0xA8) {
            terminate = true;
        }
    } else if ((self->params & 0xFF00) == 0x200) {
        if (++self->ext.hitbylightning.unk9C > 0x90) {
            terminate = true;
        }
    } else if (RIC.step != PL_S_HIT) {
        terminate = true;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 6);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0x3F);
        self->ext.hitbylightning.unk80 = rand();
        self->ext.hitbylightning.unk82 = (rand() & 0x1FF) + 0x100;
        prim = &g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->x0 = prim->x1 = prim->x2 = prim->x3 = self->posX.i.hi;
            prim->y0 = prim->y1 = prim->y2 = prim->y3 = self->posY.i.hi;
            prim->tpage = 0x1A;
            prim->clut = D_us_80181E74[rand() & 1];
            prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 = prim->b1 =
                prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 =
                    prim->b3 = 0x80;
            prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim->priority = RIC.zPriority - 2;
            prim = prim->next;
        }
        if ((RIC.velocityY != 0) && (RIC.step != PL_S_DEAD)) {
            self->ext.hitbylightning.unk92 = 1;
        }
        self->ext.hitbylightning.unk94 = 0x10;
        g_api.PlaySfx(0x6E4);
        self->step++;
        break;
    case 1:
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0x1FF);
        mul = rsin(self->ext.hitbylightning.unk80);
        self->ext.hitbylightning.unk80 += self->ext.hitbylightning.unk82;
        xMod = ((rcos(self->ext.hitbylightning.unk7C) * mul) >> 7) * 12;
        yMod = (-((rsin(self->ext.hitbylightning.unk7C) * mul) >> 7) * 7) << 1;
        self->posX.val = xMod + RIC.posX.val;
        self->posY.val = yMod + RIC.posY.val;
        if (self->ext.hitbylightning.unk92 &&
            g_Ric.vram_flag &
                (TOUCHING_L_WALL | TOUCHING_R_WALL | TOUCHING_CEILING)) {
            terminate = true;
        }
        if (terminate) {
            self->ext.hitbylightning.unk90 = (rand() & 0xF) + 0x10;
            self->step++;
        }
        break;
    case 2:
        if (--self->ext.hitbylightning.unk90 == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.hitbylightning.unk94 > 0) {
            self->ext.hitbylightning.unk94--;
        }
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0xFF);
        mul = rsin(self->ext.hitbylightning.unk80);
        self->ext.hitbylightning.unk80 += self->ext.hitbylightning.unk82;
        xMod = ((rcos(self->ext.hitbylightning.unk7C) * mul) >> 7) *
               ((rand() % 8) + 8);
        yMod = -((rsin(self->ext.hitbylightning.unk7C) * mul) >> 7) *
                   ((rand() % 8) + 10) +
               self->ext.hitbylightning.unk98;
        self->posX.val = RIC.posX.val + xMod;
        self->posY.val = RIC.posY.val + yMod;
        self->ext.hitbylightning.unk98 -= 0x8000;
        break;
    }

    x = (self->posX.i.hi + (rand() & 7)) - 4;
    y = (self->posY.i.hi + (rand() & 0x1F)) - 0x18;
    mul2 = self->ext.hitbylightning.unk94;
    mul2 = (mul2 * rsin(self->ext.hitbylightning.unk80)) >> 0xC;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 5; i++) {
        primPrev = prim;
        prim = prim->next;
        *primPrev = *prim;
        primPrev->next = prim;
        primPrev->u0 = primPrev->u2 = (i * 0x10) + 0x90;
        primPrev->u1 = primPrev->u3 = ((i + 1) * 0x10) + 0x90;
        primPrev->v0 = primPrev->v1 = 0xC0;
        primPrev->v2 = primPrev->v3 = 0xCF;
    }
    prim->x0 = prim->x1;
    prim->y0 = prim->y1;
    prim->x2 = prim->x3;
    prim->y2 = prim->y3;
    angle = self->ext.hitbylightning.unk7C + 0x400;
    prim->x1 = x + (((rcos(angle) >> 4) * mul2) >> 8);
    prim->y1 = y - (((rsin(angle) >> 4) * mul2) >> 8);
    angle = self->ext.hitbylightning.unk7C - 0x400;
    prim->x3 = x + (((rcos(angle) >> 4) * mul2) >> 8);
    prim->y3 = y - (((rsin(angle) >> 4) * mul2) >> 8);
    angle = self->ext.hitbylightning.unk80 & 0xFFF;
    if (angle < 0x400) {
        prim->priority = RIC.zPriority + 2;
    } else if (angle < 0xC00) {
        prim->priority = RIC.zPriority - 2;
    } else {
        prim->priority = RIC.zPriority + 2;
    }
    prim->u0 = prim->u2 = (i << 4) + 0x90;
    prim->u1 = prim->u3 = ((i + 1) << 4) + 0x90;
    prim->v0 = prim->v1 = 0xC0;
    prim->v2 = prim->v3 = 0xCF;
}
#endif

extern EInit D_us_801804B4;
extern AnimationFrame D_us_80181E78[];
extern s32 D_us_801D0850;
extern s32 D_us_801D0854[];
extern s32 D_us_801D169C;

// possibly ShaftOrb?
void func_us_801C03E8(Entity* self) {
    Primitive* prim; // s0
    s32 i;           // s1

    u16 palette;   // 0x6E(sp)
    s32 temp_s0_5; // 0x68(sp)
    s32 temp_v0_7; // 0x64(sp)
    s32 var_v1_2;  // 0x60(sp)
    s32 var_a0_2;  // 0x5C(sp)

    s32 distanceX; // 0x58(sp)
    s32 distanceY; // 0x54(sp)

    s32 scale;   // 0x50(sp)
    s32 posX;    // 0x4C(sp)
    s32 posY;    // 0x48(sp)
    s32 ricPosX; // 0x44(sp)
    s32 ricPosY; // s8

    s32 anotherX;
    s32 anotherY;

    s32 var_s4;    // 0x38(sp)
    s32 sp30;      // 0x34(sp)
    s32 j;         // 0x30(sp)
    s32 angle;     // s7
    s32 distance;  // s6
    s32 direction; // s5
    s32 primX;     // s4
    s32 primY;     // s3
    s32 temp_s0_2; // s2

    scale = 4;
    D_us_801D169C = 0;
    var_s4 = 0;
    sp30 = 0;

    if (self->flags & FLAG_DEAD) {
        if (self->step < 0x14) {
            D_us_801D169C = 1;
            self->step = 0x14;
        }
    } else {
#ifdef VERSION_PSP
        if ((self->hitFlags) && (self->step != 10)) {
#else
        if ((self->hitFlags) && (self->step == 2)) {
#endif
            self->ext.et_801C03E8.hitTimer = 10;
            self->step = 0xA;
        }
        self->hitFlags = 0;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804B4);
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x20);

        if (self->primIndex == -1) {
            self->step = 0;
            return;
        }
        prim = &g_PrimBuf[self->primIndex];

        for (i = 0; i < 8; i++) {
            prim->clut = 0x252;
            prim->tpage = 0x12;
            prim->u0 = prim->u2 = 0;
            prim->u1 = prim->u3 = 0x1F;
            prim->v0 = prim->v1 = 0;
            prim->v2 = prim->v3 = 0x1F;
            prim->priority = RIC.zPriority + 4;
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_HIDE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
            D_us_801D0854[i] = 0;
            prim = prim->next;
        }

        for (i = 0; i < 24; i++) {
            prim->priority = RIC.zPriority - 2;
            prim->r0 = prim->g0 = prim->r1 = prim->g1 = 0x3F;
            prim->b0 = prim->b1 = 0x7F;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_HIDE | DRAW_UNK02 | DRAW_TRANSP;
            prim->type = PRIM_LINE_G2;
            prim = prim->next;
        }

        self->flags |= FLAG_UNK_20000000 | FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->posX.i.hi = 0x80;
        self->posY.i.hi = 0x30;
        self->ext.et_801C03E8.orbitAngle = 0x400;
        self->ext.et_801C03E8.orbitAngleStep = 0x10;
        self->ext.et_801C03E8.maxTurnRate = 0x30;
        self->ext.et_801C03E8.moveAngle = 0xC00;
        self->animSet = ANIMSET_OVL(5);
        self->animCurFrame = 0;
        self->unk5A = 0x48;
        self->palette = PAL_FLAG(0x252);
        self->ext.et_801C03E8.savedHitboxState = self->hitboxState;
        self->anim = D_us_80181E78;
        self->zPriority = RIC.zPriority + 4;
        self->step = 1;
        self->opacity = 0;
        self->drawFlags = ENTITY_OPACITY;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        break;

    case 1:
        self->opacity++;
        if (self->opacity >= 0x80) {
            self->drawFlags = ENTITY_DEFAULT;
            self->blendMode = BLEND_NO;
            self->step++;
        }
        break;

    case 2:
        distanceX = RIC.posX.i.hi + RIC.hitboxOffX;
        distanceY = (RIC.posY.i.hi + RIC.hitboxOffY) - 0x40;

        angle = self->ext.et_801C03E8.orbitAngle;

        distanceX += (((rcos(angle) >> 4) * scale) >> 8);
        distanceY -= (((rsin(angle) >> 4) * scale) >> 8);
        self->ext.et_801C03E8.orbitAngle +=
            self->ext.et_801C03E8.orbitAngleStep;

        posX = distanceX - self->posX.i.hi;
        posY = distanceY - self->posY.i.hi;
        angle = ratan2(-posY, posX) & 0xFFF;

        temp_s0_2 = (self->ext.et_801C03E8.moveAngle & 0xFFF);
        var_v1_2 = abs(temp_s0_2 - angle);

        var_a0_2 = self->ext.et_801C03E8.maxTurnRate;
        if (self->ext.et_801C03E8.maxTurnRate > var_v1_2) {
            var_a0_2 = var_v1_2;
        }

        if (temp_s0_2 < angle) {
            if (var_v1_2 < 0x800) {
                temp_s0_2 += var_a0_2;
            } else {
                temp_s0_2 -= var_a0_2;
            }
        } else {
            if (var_v1_2 < 0x800) {
                temp_s0_2 -= var_a0_2;
            } else {
                distance = var_a0_2;
                temp_s0_2 += distance;
            }
        }
        self->ext.et_801C03E8.moveAngle = temp_s0_2 & 0xFFF;
        temp_s0_5 = rcos(temp_s0_2) * 0x10;
        temp_v0_7 = rsin(temp_s0_2) * 0x10;
        self->posX.val = temp_s0_5 + self->posX.val;
        self->posY.val -= temp_v0_7;
        break;
    case 10:
        if (g_Timer & 1) {
            self->palette = PAL_FLAG(PAL_FILL_WHITE);
        } else {
            self->palette = PAL_FLAG(PAL_CC_GREEN_EFFECT_A);
        }

        self->poseTimer++;
        if (--self->ext.et_801C03E8.hitTimer == 0) {
            self->step = 2;
        }

        break;
    case 20:
        RicCreateEntFactoryFromEntity(self, 0x49, 0);
        RicCreateEntFactoryFromEntity(self, 0x4B, 0);
        self->step++;
        break;
    case 21:
        DestroyEntity(self);
        return;
    }

    if (g_api.CheckEquipmentItemCount(0x22U, 1U) != 0) {
        palette = PAL_FLAG(0x252);
        self->hitboxState = self->ext.et_801C03E8.savedHitboxState;
        self->ext.et_801C03E8.showPrims = 1;
    } else {
        palette = PAL_FLAG(PAL_PLAYER_HIDDEN);
        self->hitboxState = 0;
        self->ext.et_801C03E8.showPrims = 0;
    }
    if (RIC.step == PL_S_DEAD || RIC.step == PL_S_ENDING_1) {
        self->hitboxState = 0;
    }
    if (self->step != PL_S_9) {
        self->palette = palette;
    }
    if (!(g_Timer % 4) && (self->step == 2)) {
        D_us_801D0850++;
        D_us_801D0850 %= 8;
        var_s4 = 1;
    }

    if (g_Timer % 0x100 == 0) {
        if (self->step == 2) {
            if ((abs(self->posX.i.hi - RIC.posX.i.hi) < 0x20) &&
                (RIC.step != PL_S_DEAD)) {
                sp30 = 1;
                if (self->ext.et_801C03E8.showPrims) {
                    RicCreateEntFactoryFromEntity(self, 0x590021, 0);
                }
            }
        }
    }

    posX = self->posX.i.hi;
    posY = self->posY.i.hi;
    prim = &g_PrimBuf[self->primIndex];

    for (i = 0; i < 8; i++) {
        if (D_us_801D0854[i] == 0) {
            if ((var_s4 != 0) && (D_us_801D0850 == i)) {
                prim->x0 = prim->x2 = posX - 0x10;
                prim->x1 = prim->x3 = posX + 0xF;
                prim->y0 = prim->y1 = posY - 0x10;
                prim->y2 = prim->y3 = posY + 0xF;
                prim->drawMode &= ~DRAW_HIDE;
                prim->r0 = prim->r1 = prim->r2 = prim->r3 = prim->g0 =
                    prim->g1 = prim->g2 = prim->g3 = prim->b0 = prim->b1 =
                        prim->b2 = prim->b3 = 0x80;
                D_us_801D0854[i] += 1;
            }
        } else {
            prim->b3 -= 4;
            if (prim->b3 < 0x10) {
                D_us_801D0854[i] = 0;
            }
            prim->r0 = prim->r1 = prim->r2 = prim->r3 = prim->g0 = prim->g1 =
                prim->g2 = prim->g3 = prim->b0 = prim->b1 = prim->b2 = prim->b3;
        }
        if (!self->ext.et_801C03E8.showPrims) {
            prim->drawMode |= DRAW_HIDE;
        }
        prim = prim->next;
    }

    ricPosX = RIC.posX.i.hi;
    ricPosY = RIC.posY.i.hi;

    if (g_Timer & 1) {
        direction = -1;
    } else {
        direction = 1;
    }
    posX -= 3;

    for (j = 0; j < 3; j++, posX += 3) {
        primX = posX;
        primY = posY;
        distance = 3;
        // > lw a1, 0x48(sp)
        anotherY = ricPosY - posY;
        anotherX = ricPosX - posX;
        angle = ratan2(-anotherY, anotherX);
        distance = (SquareRoot12(I_TO_FLT(
                        (anotherX * anotherX) + (anotherY * anotherY))) /
                    7);
        distance = FLT_TO_I(distance);

        for (i = 0; i < 8; i++) {
            direction = -direction;
            if (prim->r2 == 0) {
                if (sp30 != 0) {
                    prim->r2++;
                    prim->b2 = 0xC;
                    prim->drawMode &= ~DRAW_HIDE;
                }
            } else if (--prim->b2 == 0) {
                prim->drawMode |= DRAW_HIDE;
                prim->r2 = 0;
            }
            prim->x0 = primX;
            prim->y0 = primY;
            temp_s0_2 = angle + (rand() & 0x1FF) * direction;
            prim->x1 = (((rcos(temp_s0_2) >> 4) * distance) >> 8) + primX;
            prim->y1 = -(((rsin(temp_s0_2) >> 4) * distance) >> 8) + primY;
            primX = prim->x1;
            primY = prim->y1;

            if (i == 7) {
                prim->x1 = ricPosX;
                prim->y1 = ricPosY;
            }
            if (!self->ext.et_801C03E8.showPrims) {
                prim->drawMode |= DRAW_HIDE;
            }
            prim = prim->next;
        }
    }
#ifndef VERSION_PSP
    FntPrint("tama_step:%02x\n", self->step);
#endif
}

extern s32 g_CutsceneFlags;

// TODO: I AM SHAFT!
void EntityShaft(Entity* self) {
    Entity* entity;
    FntPrint("I AM SHAFT\n");
    switch (self->step) {
    case 0:
#ifdef VERSION_PSP
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
#else
        self->flags = FLAG_UNK_10000000;
#endif
        self->animSet = -0x7FFB;
        self->animCurFrame = 0x8B;
        self->unk5A = 0x48;
        self->palette = PAL_FLAG(0x250);
        self->zPriority = RIC.zPriority + 2;
        self->opacity = 0;
        self->drawFlags = ENTITY_OPACITY;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->step++;
        break;

    case 1:
        self->opacity += 4;
        if (self->opacity > 48) {
            self->step++;
            entity = &g_Entities[0xC8];
            CreateEntityFromCurrentEntity(E_ID(ID_17), entity);
            entity->params = 3;
            self->ext.shaft.timer = 0x100;
        }
        self->posY.val += rsin(self->ext.shaft.bobAngle) * 4;
        self->ext.shaft.bobAngle += 0x20;
        break;

    case 2:
        if (!self->ext.shaft.timer) {
            if ((g_CutsceneFlags & 0x40) || (g_DemoMode != Demo_None)) {
                self->drawFlags |= ENTITY_SCALEY | ENTITY_SCALEX;
                self->scaleX = self->scaleY = 0x100;
                self->step++;
            }
        } else {
            self->ext.shaft.timer--;
        }
        self->posY.val += rsin(self->ext.shaft.bobAngle) * 4;
        self->ext.shaft.bobAngle += 0x20;
        break;

    case 3:
        self->scaleX -= 0x20;
        if (self->scaleX < 0x10) {
            self->scaleX = 0x10;
        }

        self->scaleY += 64;
        if (self->scaleY > 0x800) {
            self->scaleY = 0x800;
        }
        self->opacity += 6;
        if (self->opacity > 0xF0) {
            self->step++;
        }
        break;

    case 4:
        self->opacity -= 3;
        if (self->opacity < 4) {
            DestroyEntity(self);
        }
        break;
    }
}

// TODO: rename ShaftOrb
Entity* RicGetFreeEntity(s16, s16);
extern u8 D_us_80181E9C[];

void func_us_801C0FE8(Entity* self) {
    Entity* entity;
    Primitive* prim;
    s32 posX;
    s32 posY;
    s32 accelX;
    s32 accelY;
    s16 primIndex;
    s16 params;
    s32 velocity;

    params = self->params & 0xFF;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[self->primIndex];
        prim->clut = 0x252;
        prim->tpage = 0x12;

        // temp_a1 = &D_us_80181E9C[temp_a0];
        prim->u0 = prim->u2 = D_us_80181E9C[params * 2] - 2;
        prim->u1 = prim->u3 = D_us_80181E9C[params * 2] + 2;

        prim->v0 = prim->v1 = D_us_80181E9C[params * 2 + 1] - 2;
        prim->v2 = prim->v3 = D_us_80181E9C[params * 2 + 1] + 2;

        prim->priority = RIC.zPriority + 4;
        prim->drawMode = DRAW_UNK02;

        accelX = D_us_80181E9C[params * 2] - 16;
        accelY = D_us_80181E9C[params * 2 + 1] - 16;
        self->posX.i.hi += accelX;
        self->posY.i.hi += accelY;

        velocity = ratan2(-accelY, accelX);
        velocity += ((rand() & 0x7F) - 0x40);
        self->ext.shaftOrb.velocityAngle = velocity;
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->ext.shaftOrb.timer = 8;
        self->step++;
        break;

    case 1:
        if (--self->ext.shaftOrb.timer == 0) {
            self->ext.shaftOrb.timer = 16;
            velocity = self->ext.shaftOrb.velocityAngle;
            self->velocityX = (rcos(velocity) * 32) + (rand() & 0xF);
            self->velocityY = -((rsin(velocity) * 32) + (rand() & 0xF));
            self->step++;
        }
        break;

    case 2:
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        if (--self->ext.shaftOrb.timer == 0) {
            RicCreateEntFactoryFromEntity(self, 0x4A, 0);
            self->velocityY = (rand() & 0x7FFF) + 0xFFFF0000;
            self->velocityX = self->velocityX >> 2;
            self->ext.shaftOrb.timer = 1;
            self->step++;
        }
        break;
    case 3:
        if ((self->ext.shaftOrb.timer % 4) == 0) {
            entity = RicGetFreeEntity(0x50, 0x8F);
            if (entity != NULL) {
                DestroyEntity(entity);
                entity->entityId = 0x43;
                entity->params = 0x100;
                // not shaft orb
                entity->ext.shaftOrb.parent = self->ext.shaftOrb.parent;
                entity->posX.val = self->posX.val;
                entity->posY.val = self->posY.val;
            }
        }
        self->ext.shaftOrb.timer += 1;
        self->velocityY += 0xC00;
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        self->flags &= ~FLAG_UNK_10000000;
        break;
    }

    posX = self->posX.i.hi;
    posY = self->posY.i.hi;
    prim = &g_PrimBuf[self->primIndex];
    prim->x0 = prim->x2 = posX - 2;
    prim->x1 = prim->x3 = posX + 2;
    prim->y0 = prim->y1 = posY - 2;
    prim->y2 = prim->y3 = posY + 2;
}

extern AnimationFrame D_us_80181EDC[];

void func_us_801C13A8(Entity* self) {
    s16 params = self->params & 0x7F00;
    switch (self->step) {
    case 0:
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        self->unk5A = 0x79;
        self->animSet = ANIMSET_DRA(14);
        self->zPriority = RIC.zPriority + 6;
        self->palette = PAL_FLAG(0x25E);
        self->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = 0xC0;
        self->anim = D_us_80181EDC;
        if (params) {
            self->scaleX = self->scaleY = 0x80;
            self->anim = D_us_80181EDC;
        }
        self->velocityY = -FIX(0.25);
        self->step++;
        break;

    case 1:
        self->posY.val += self->velocityY;
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

extern EInit D_us_8018040C;
extern EInit D_us_80180418;
extern Point16 D_us_801824EC[];
extern Point16 D_us_80182508[];
extern Point16 D_us_80182524[];
extern s16 D_us_80182548[];
extern Point16 D_us_8018255C[];
extern s16 D_us_80182580[];
extern Point16 D_us_80182594[];
extern s16 D_us_801825B8[];
extern Point32 D_us_801825EC[];
extern Point32 D_us_80182624[];
extern s32 D_us_8018266C[];
extern s32 D_us_80182690[];
extern s32 D_us_801826B4[];
extern s32 D_us_801826D8[];
extern s32 D_us_801826FC[];
extern s32 D_us_80182720[];
extern s32 D_us_80182744[];
extern s32 D_us_80182768[];
extern s32 D_us_8018278C[];
extern u16 D_us_801827B0[];
extern s16 D_us_801827B8[];
extern s16 D_us_801827D8[];
extern s32 D_us_801D0874;
extern s32 D_us_801D0878;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityWhip);
#else
void RicEntityWhip(Entity* self) {
    s16 sp4E;
    s16 radius;
    s32 sp44;
    s32 sp40;
    s32 prevX;
    s32 prevY;
    s16 sp4A;
    s32 aimPath;
    s32 newY;
    s16 angle;
    s32 diffY;
    s16 psp_s4;
    s32 diffX;
    s32 newX;
    Primitive* prim;
    s32 psp_s0;

    psp_s4 = self->params & 0xFF;
    sp4A = self->params & 0xFF00;
    if (psp_s4 == 0) {
        if (self->ext.whip.unkA6) {
            self->ext.whip.unkA6--;
        }
    }
    if (g_Ric.unk46 == 0) {
        DestroyEntity(self);
        return;
    }
    if (self->step == 2 && g_Ric.unk46 == 2) {
        self->step = 1;
        self->velocityX = 0;
        self->velocityY = 0;
        self->palette = PAL_FLAG(0x248);
        if (RIC.facingLeft == 0) {
            self->ext.whip.unk88 = 2;
        } else {
            self->ext.whip.unk88 = 3;
        }
        self->ext.whip.unk8C = 0x600;
    }
    if (self->step) {
        diffX = self->posX.val - self->ext.whip.curX;
        diffY = self->posY.val - self->ext.whip.curY;
        self->ext.whip.prevX += diffX;
        self->ext.whip.prevY += diffY;
    }
    // up+down or left+right can't be pressed together on a PS pad
    psp_s0 = 0;
    aimPath = g_Ric.padPressed & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT);
    if (aimPath == PAD_UP) {
        psp_s0 = D_us_8018266C[self->ext.whip.unk88];
    }
    if (aimPath == PAD_DOWN) {
        psp_s0 = D_us_801826D8[self->ext.whip.unk88];
    }
    if (aimPath == PAD_RIGHT) {
        psp_s0 = D_us_80182690[self->ext.whip.unk88];
    }
    if (aimPath == PAD_LEFT) {
        psp_s0 = D_us_801826B4[self->ext.whip.unk88];
    }
    if (aimPath == (PAD_UP + PAD_RIGHT)) {
        psp_s0 = D_us_801826FC[self->ext.whip.unk88];
    }
    if (aimPath == (PAD_UP + PAD_LEFT)) {
        psp_s0 = D_us_80182720[self->ext.whip.unk88];
    }
    if (aimPath == (PAD_DOWN + PAD_RIGHT)) {
        psp_s0 = D_us_80182744[self->ext.whip.unk88];
    }
    if (aimPath == (PAD_DOWN + PAD_LEFT)) {
        psp_s0 = D_us_80182768[self->ext.whip.unk88];
    }
    if (aimPath == 0) {
        psp_s0 = D_us_8018278C[self->ext.whip.unk88];
    }

    self->ext.whip.unk86 = 0;
    if (psp_s4 == 0) {
        if (self->step == 1) {
            if (RIC.step == PL_S_CROUCH) {
                if (RIC.facingLeft == 0) {
                    newX = D_us_80182580[psp_s0];
                } else {
                    newX = D_us_8018255C[psp_s0].x;
                }
                newY = D_us_8018255C[psp_s0].y;
            } else if (RIC.step == PL_S_STAND) {
                if (RIC.facingLeft == 0) {
                    newX = D_us_80182548[psp_s0];
                } else {
                    newX = D_us_80182524[psp_s0].x;
                }
                newY = D_us_80182524[psp_s0].y;
            } else {
                if (RIC.facingLeft == 0) {
                    newX = D_us_801825B8[psp_s0];
                } else {
                    newX = D_us_80182594[psp_s0].x;
                }
                newY = D_us_80182524[psp_s0].y;
            }
        } else {
            if (RIC.step == PL_S_CROUCH) {
                newX = D_us_80182508[RIC.pose].x;
                newY = D_us_80182508[RIC.pose].y;
            } else {
                newX = D_us_801824EC[RIC.pose].x;
                newY = D_us_801824EC[RIC.pose].y;
            }
            if (RIC.facingLeft == 0) {
                newX = -newX;
            }
        }
        newX <<= 16;
        newY <<= 16;
        newX += RIC.posX.val;
        newY += RIC.posY.val;
    } else {
        newX = (self - 1)->posX.val;
        newY = (self - 1)->posY.val;
    }
    switch (self->step) {
    case 0:
        if (sp4A) {
            InitializeEntity(D_us_80180418);
        } else {
            InitializeEntity(D_us_8018040C);
        }
        self->animSet = ANIMSET_OVL(3);
        self->animCurFrame = 0x1E;
        self->palette = PAL_FLAG(0x238);
        self->unk5A = 0x24;
        if (psp_s4 < 8) {
            self->hitboxWidth = 3;
            self->hitboxHeight = 3;
            self->hitboxOffX = 0;
            self->hitboxOffY = 0;
        }
        if (psp_s4 == 0) {
            self->zPriority = RIC.zPriority - 2;
        }
        if (psp_s4 == 7) {
            self->animCurFrame = 0x1F;
            self->hitboxWidth++;
            self->hitboxHeight++;
        }
        self->posX.val = newX;
        self->posY.val = newY;
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->ext.whip.unk8C = 0x500;
        self->ext.whip.prevX = self->posX.val;
        self->ext.whip.prevY = self->posY.val;
        self->zPriority = RIC.zPriority + 4;
        self->ext.whip.curX = self->ext.whip.prevX;
        self->ext.whip.curY = self->ext.whip.prevY;
        self->primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 1);
        if (self->primIndex != -1) {
            prim = &g_PrimBuf[self->primIndex];
            self->flags |= FLAG_HAS_PRIMS;
            prim->r0 = 0x7F;
            prim->g0 = 0x7F;
            prim->b0 = 0x7F;
            prim->r1 = 0x1F;
            prim->g1 = 0x1F;
            prim->b1 = 0x1F;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            prim->priority = RIC.zPriority + 2;
        }
        self->step = 2;
        RicEntityWhip(self);
        break;
    case 1:
        if (sp4A == 0) {
            if (self->palette < PAL_FLAG(0x248) &&
                self->palette >= PAL_FLAG(0x240)) {
                if (self->ext.whip.unkA4) {
                    self->ext.whip.unkA4--;
                } else {
                    self->palette++;
                }
            }
        } else if (self->ext.whip.unkA4) {
            if (!(psp_s4 & 1)) {
                RicCreateEntFactoryFromEntity(self, BP_20, 0);
                self->ext.whip.unkA4 = 0;
            }
        }
        if (psp_s4 < 8) {
            if (self->ext.whip.unk84) {
                if (--self->ext.whip.unk84 == 0) {
                    if (self->velocityY < 0) {
                        self->velocityY = 0;
                    }
                    self->velocityX = 0;
                }
            } else {
                self->velocityX = self->velocityX * 3 / 4;
            }
            if (self->ext.whip.unk88 == psp_s0) {
                break;
            }
            if (psp_s4 == 0) {
                if (self->ext.whip.unkA6 == 0) {
                    g_api.PlaySfx(SFX_RIC_WHIP_RATTLE_A);
                    self->ext.whip.unkA6 = 0x20;
                }
                if (sp4A == 0) {
                    RicCreateEntFactoryFromEntity(self, FACTORY(BP_18, 0), 0);
                } else {
                    RicCreateEntFactoryFromEntity(self, FACTORY(BP_18, 1), 0);
                }
            }

            self->ext.whip.unk84 = 6;
            if (psp_s0 == 0) {
                if (RIC.facingLeft == 0) {
                    diffX = D_us_80182548[0];
                } else {
                    diffX = D_us_80182524[0].x;
                }
                diffY = D_us_80182524[0].y;
                diffX += RIC.posX.i.hi;
                diffY += RIC.posY.i.hi;
                diffX <<= 16;
                diffY <<= 16;
                diffX -= self->posX.val;
                diffY -= self->posY.val;
                angle = ratan2(-diffY, diffX);
                radius = 0x80;
                self->velocityX = (radius * rcos(angle)) >> 4;
                self->velocityY = -(radius * rsin(angle)) >> 4;
            } else {
                self->velocityY = D_us_80182624[psp_s0].x;
                self->velocityX = D_us_80182624[psp_s0].y;
            }
            self->ext.whip.unk88 = psp_s0;
        }
        break;
    case 2:
        if (psp_s4 > 7) {
            if (self->flags & FLAG_HAS_PRIMS) {
                if (RIC.pose == 5) {
                    g_api.FreePrimitives(self->primIndex);
                    self->flags &= ~FLAG_HAS_PRIMS;
                }
            }
            if (RIC.pose == 6) {
                if (RIC.poseTimer > 14 || RIC.poseTimer == 0) {
                    self->palette = PAL_FLAG(0x238);
                } else if ((RIC.poseTimer == 11 || RIC.poseTimer == 12) ||
                           (RIC.poseTimer == 13 || RIC.poseTimer == 14)) {
                    self->palette = PAL_FLAG(0x23C);
                } else {
                    self->palette = D_us_801827B0[(s16)(RIC.poseTimer % 3)];
                }
            }
        } else {
            self->velocityX = D_us_801825EC[RIC.pose].x;
            self->velocityY = D_us_801825EC[RIC.pose].y;
            switch (RIC.pose) {
            case 0:
                radius = 0x800;
                angle = (rand() & 0x3FF) - 0x200;
                self->velocityX = (radius * rcos(angle)) >> 4;
                self->velocityY = -(radius * rsin(angle)) >> 4;
                break;
            case 1:
                radius = 0x800;
                angle = (rand() & 0x3FF) - 0x400;
                self->velocityX = (radius * rcos(angle)) >> 4;
                self->velocityY = -(radius * rsin(angle)) >> 3;
                self->ext.whip.unk8C = 0x600;
                break;
            case 4:
                self->ext.whip.unk8C = 0x3C0;
                if (RIC.poseTimer == 1) {
                    self->velocityX = FIX(-8);
                }
                break;
            case 5:
                self->ext.whip.unk8C += 0xC0;
                if (self->flags & FLAG_HAS_PRIMS) {
                    g_api.FreePrimitives(self->primIndex);
                    self->flags &= ~FLAG_HAS_PRIMS;
                }
                break;
            case 6:
                if (sp4A) {
                    if (RIC.poseTimer == D_us_801827B8[psp_s4]) {
                        RicCreateEntFactoryFromEntity(self, BP_20, 0);
                    }
                }
                if (psp_s4 == (0x10 - RIC.poseTimer)) {
                    self->ext.whip.unk8C = 0x600;
                }
                if (RIC.poseTimer > 14 || RIC.poseTimer == 0) {
                    self->palette = PAL_FLAG(0x238);
                } else if (RIC.poseTimer == 11 || RIC.poseTimer == 12 ||
                           RIC.poseTimer == 13 || RIC.poseTimer == 14) {
                    self->palette = PAL_FLAG(0x23C);
                } else {
                    self->palette = D_us_801827B0[(s16)(RIC.poseTimer % 3)];
                }
                break;
            }
            if (RIC.facingLeft == 0) {
                self->velocityX = -self->velocityX;
            }
            self->velocityX = self->velocityX / 2;
            self->velocityY = self->velocityY / 2;
        }
        break;
    }
    sp4E = self->ext.whip.unk8C;
    prevX = self->posX.val;
    prevY = self->posY.val;
    sp40 = 0;
    if (psp_s4 == 0) {
        self->posX.val = newX;
        self->posY.val = newY;
    } else if (psp_s4 > 7) {
        diffX = (self - 8)->posX.val;
        diffY = (self - 8)->posY.val;
        sp44 = (self - 7)->posX.val;
        sp40 = (self - 7)->posY.val;
        self->posX.val = (diffX + sp44) / 2;
        self->posY.val = (diffY + sp40) / 2;
    } else {
        sp44 = self->posX.val - self->ext.whip.prevX;
        sp40 = self->posY.val - self->ext.whip.prevY;
        if (self->step == 1) {
            if (self->velocityY < FIX(0.5)) {
                self->posX.val += sp44 * 16 / 16;
                self->posY.val += sp40 * 16 / 16;
            }
            if (self->velocityY >= FIX(0.5) && self->velocityY < FIX(1.25)) {
                self->posX.val += sp44 * 15 / 16;
                self->posY.val += sp40 * 15 / 16;
            }
            if (self->velocityY >= FIX(1.25) || self->velocityY < 0) {
                self->posX.val += sp44 * 13 / 16;
                self->posY.val += sp40 * 13 / 16;
            }
        } else {
            self->posX.val += sp44 * 3 / 4;
            self->posY.val += sp40 * 3 / 4;
        }
        if (self->velocityY < FIX(8) || self->velocityY < 0) {
            self->velocityY += 0xA00;
        }
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        diffY = self->posY.val - newY;
        diffX = self->posX.val - newX;
        if (((sp4E * sp4E) >> 0x10) < ((diffY * (diffY >> 0x10)) >> 0x10) +
                                          ((diffX * (diffX >> 0x10)) >> 0x10)) {
            angle = ratan2(-diffY, diffX);
            diffX = (rcos(angle) * sp4E) >> 4;
            diffY = -(rsin(angle) * sp4E) >> 4;
            self->posX.val = newX + diffX;
            self->posY.val = newY + diffY;
        }
    }
    self->ext.whip.prevX = prevX;
    self->ext.whip.prevY = prevY;
    self->ext.whip.unkA0 = sp40;
    self->ext.whip.curX = self->posX.val;
    self->ext.whip.curY = self->posY.val;
    if (psp_s4 == 0) {
        D_us_801D0874 = psp_s0;
        D_us_801D0878 = self->ext.whip.unk88;
    }
    if (self->primIndex != -1 && (self->flags & FLAG_HAS_PRIMS)) {
        prim = &g_PrimBuf[self->primIndex];
        prim->x0 = self->posX.i.hi;
        prim->y0 = self->posY.i.hi;
        prim->x1 = self->ext.whip.prevX >> 0x10;
        prim->y1 = self->ext.whip.prevY >> 0x10;
        if (RIC.pose == 4) {
            prim->priority = RIC.zPriority + 4;
        } else {
            prim->priority = RIC.zPriority + 2;
        }
    }
    if (sp4A) {
        if (psp_s4 != 7) {
            angle = (((g_GameTimer >> 2) & 7) + D_us_801827D8[psp_s4]) * 512;
            self->opacity = (rsin(angle) >> 6) + 0x80;
            self->blendMode = BLEND_TRANSP | BLEND_ADD;
            self->drawFlags |= ENTITY_OPACITY;
        }
        self->palette = PAL_FLAG(PAL_CC_FIRE_EFFECT);
    }
}
#endif

extern s16 D_us_801827F8[];
extern s16 D_us_8018280C[];
extern s16 D_us_80182820[];
extern s16 D_us_80182834[];
extern s16 D_us_80182848[];
extern s16 D_us_8018285C[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityArmBrandishWhip);
#else
void RicEntityArmBrandishWhip(Entity* entity) {
    if (g_Ric.unk46 == 0) {
        DestroyEntity(entity);
        return;
    }
    entity->facingLeft = RIC.facingLeft;
    if (entity->step == 0) {
        entity->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        entity->animSet = ANIMSET_OVL(3);
        entity->unk5A = 0x24;
        entity->palette = PAL_FLAG(0x220);
        entity->zPriority = RIC.zPriority + 2;
    }
    if (RIC.step == PL_S_CROUCH) {
        if (RIC.facingLeft) {
            entity->animCurFrame = D_us_8018280C[D_us_801D0874];
        } else {
            entity->animCurFrame = D_us_801827F8[D_us_801D0874];
        }
    } else if (RIC.step == PL_S_STAND) {
        if (RIC.facingLeft) {
            entity->animCurFrame = D_us_80182834[D_us_801D0874];
        } else {
            entity->animCurFrame = D_us_80182820[D_us_801D0874];
        }
    } else if (RIC.facingLeft) {
        entity->animCurFrame = D_us_8018285C[D_us_801D0874];
    } else {
        entity->animCurFrame = D_us_80182848[D_us_801D0874];
    }
    entity->posX.val = RIC.posX.val;
    entity->posY.val = RIC.posY.val;
}
#endif

extern s16 D_us_80182870[];
// same as `ric` `func_80167964` except `g_Ric`/`g_Player` reference and lookup
// table
void func_us_801C2688(Entity* entity) {
    if (g_Ric.unk46 == 0) {
        DestroyEntity(entity);
        return;
    }
    if (entity->step == 0) {
        entity->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
    }
    if (!(entity->params & 0xFF00)) {
        g_Entities[D_us_80182870[entity->poseTimer]].palette = PAL_FLAG(0x240);
    }
    g_Entities[D_us_80182870[entity->poseTimer]].ext.player.unkA4 = 4;
    entity->poseTimer++;
    if (entity->poseTimer == 15) {
        DestroyEntity(entity);
    }
}

void func_us_801C277C(void) {}

void func_us_801C2784(void) {}

extern s16 D_us_80182890[][6];

#define FAKEPRIM ((FakePrim*)prim)

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnHolyWaterBreakGlass);
#else
void RicEntitySubwpnHolyWaterBreakGlass(Entity* self) {
    Point16 sp10[8];
    Primitive* prim;
    s16 posX;
    s16 posY;
    s16 arrIndex;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 16);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        posX = self->posX.i.hi;
        posY = self->posY.i.hi;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; prim != NULL; i++, prim = prim->next) {
            if (i < 8) {
                sp10[i].x = FAKEPRIM->posX.i.hi = FAKEPRIM->x0 = posX;
                sp10[i].y = FAKEPRIM->posY.i.hi = FAKEPRIM->y0 = posY;
                FAKEPRIM->velocityX.val = (rand() & 0x3FFF) + FIX(0.25);
                if (i & 1) {
                    FAKEPRIM->velocityX.val = -FAKEPRIM->velocityX.val;
                }
                FAKEPRIM->velocityY.val = -((rand() * 2) + FIX(2.5));
                FAKEPRIM->drawMode = DRAW_HIDE | DRAW_UNK02;
                FAKEPRIM->type = PRIM_TILE;
            } else {
                prim->r0 = prim->r1 = prim->r2 = prim->r3 =
                    (rand() & 0xF) + 0x30;
                prim->b0 = prim->b1 = prim->b2 = prim->b3 =
                    (rand() & 0x7F) + 0x80;
                prim->g0 = prim->g1 = prim->g2 = prim->g3 =
                    (rand() & 0x1F) + 0x30;
                if (rand() & 1) {
                    prim->drawMode = (DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                      DRAW_UNK02 | DRAW_TRANSP);
                } else {
                    prim->drawMode = (DRAW_COLORS | DRAW_UNK02);
                }
                posX = sp10[i - 8].x;
                posY = sp10[i - 8].y;
                prim->u0 = arrIndex = i & 3;
                prim->x0 = posX + D_us_80182890[arrIndex][0];
                prim->y0 = posY + D_us_80182890[arrIndex][1];
                prim->x1 = posX + D_us_80182890[arrIndex][2];
                prim->y1 = posY + D_us_80182890[arrIndex][3];
                prim->x3 = prim->x2 = posX + D_us_80182890[arrIndex][4];
                prim->y3 = prim->y2 = posY + D_us_80182890[arrIndex][5];
                prim->type = PRIM_G4;
                prim->priority = RIC.zPriority + 2;
            }
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->ext.timer.t = 20;
        self->step++;
        break;
    case 1:
        if (--self->ext.timer.t == 0) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; prim != NULL; i++, prim = prim->next) {
            if (i < 8) {
                FAKEPRIM->posX.i.hi = FAKEPRIM->x0;
                FAKEPRIM->posY.i.hi = FAKEPRIM->y0;
                FAKEPRIM->posX.val += FAKEPRIM->velocityX.val;
                FAKEPRIM->posY.val += FAKEPRIM->velocityY.val;
                FAKEPRIM->velocityY.val += FIX(36.0 / 128);
                sp10[i].x = FAKEPRIM->posX.i.hi;
                sp10[i].y = FAKEPRIM->posY.i.hi;
                FAKEPRIM->x0 = FAKEPRIM->posX.i.hi;
                FAKEPRIM->y0 = FAKEPRIM->posY.i.hi;
            } else {
                posX = sp10[i - 8].x;
                posY = sp10[i - 8].y;
                arrIndex = prim->u0;
                prim->x0 = posX + D_us_80182890[arrIndex][0];
                prim->y0 = posY + D_us_80182890[arrIndex][1];
                prim->x1 = posX + D_us_80182890[arrIndex][2];
                prim->y1 = posY + D_us_80182890[arrIndex][3];
                prim->x3 = prim->x2 = posX + D_us_80182890[arrIndex][4];
                prim->y3 = prim->y2 = posY + D_us_80182890[arrIndex][5];
            }
        }
        break;
    }
}
#endif

extern EInit D_us_80180484;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityCrashHydroStorm);
#else
void RicEntityCrashHydroStorm(Entity* self) {
    PrimLineG2* line;
    s16 primcount;

    if (self->params < 24) {
        primcount = 32;
    } else {
        primcount = 33 - (self->params - 32) * 2;
    }

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, primcount);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->ext.subweapon.subweaponId = PL_W_HYDROSTORM;
        InitializeEntity(D_us_80180484);
        self->posX.i.hi = 0x80;
        self->posY.i.hi = 0x70;
        self->hitboxWidth = 0x78;
        self->hitboxHeight = 0x78;
        self->facingLeft = 0;
        self->flags |= FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        line = (PrimLineG2*)&g_PrimBuf[self->primIndex];
        while (line != NULL) {
            line->r0 = 0x1F;
            line->g0 = 0x1F;
            line->b0 = 0x30;
            line->r1 = 0x3F;
            line->g1 = 0x50;
            line->b1 = 0x7F;
            line->x0 = rand() & 0x1FF;
            line->x1 = line->x0;
            line->y0 = -(rand() & 0xF);
            line->y1 = line->y0;
            line->preciseX.i.hi = line->x1;
            line->preciseY.i.hi = line->y1;
            line->velocityX.val = -(rcos(ROT(258.75)) * -16) * 12;
            line->velocityY.val = -(rsin(ROT(258.75)) * 16) * 12;
            line->timer = 0;
            line->delay = (rand() & 0xF) + 12;
            if (rand() & 1) {
                line->priority = RIC.zPriority + 2;
            } else {
                line->priority = RIC.zPriority - 2;
            }
            line->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
            line = line->next;
        }
        if (self->params == 1) {
            g_api.SetFadeMode(FADE_BLUE_TINT);
        }
        self->ext.subweapon.timer = 0x160;
        if ((self->params < 16) && !(self->params & 3)) {
            g_api.PlaySfx(0x830);
        }
        self->step = 1;
        break;
    case 1:
        line = (PrimLineG2*)&g_PrimBuf[self->primIndex];
        while (line != NULL) {
            if (line->timer == 0) {
                line->preciseX.i.hi = line->x1;
                line->preciseY.i.hi = line->y1;
                line->preciseX.val += line->velocityX.val;
                line->preciseY.val += line->velocityY.val;
                line->x1 = line->preciseX.i.hi;
                line->y1 = line->preciseY.i.hi;
                if (line->y1 > line->delay) {
                    line->timer++;
                    line->xLength = line->x0 - line->x1;
                    line->yLength = line->y0 - line->y1;
                }
            } else {
                line->preciseX.i.hi = line->x1;
                line->preciseY.i.hi = line->y1;
                line->preciseX.val += line->velocityX.val;
                line->preciseY.val += line->velocityY.val;
                line->x1 = line->preciseX.i.hi;
                line->y1 = line->preciseY.i.hi;
                line->y0 = line->y1 + line->yLength;
                line->x0 = line->x1 + line->xLength;
                if (line->y0 >= 0xD8) {
                    self->step = 2;
                }
            }
            line = line->next;
        }
        self->ext.subweapon.timer++;
        break;
    case 2:
        if (self->params == 24) {
            g_Ric.unk4E = 1;
        }
        DestroyEntity(self);
        break;
    }
    g_Ric.timers[PL_T_3] = 16;
}
#endif

extern s32 D_us_801D087C;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", DebugShowWaitInfo);
#else
static void DebugShowWaitInfo(const char* msg) {
    g_CurrentBuffer = g_CurrentBuffer->next;
    FntPrint(msg);
    if (D_us_801D087C++ & 4) {
        FntPrint("\no\n");
    }
    DrawSync(0);
    VSync(0);
    PutDrawEnv(&g_CurrentBuffer->draw);
    PutDispEnv(&g_CurrentBuffer->disp);
    FntFlush(-1);
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", DebugInputWait);
#else
static void DebugInputWait(const char* msg) {
    while (PadRead(0)) {
        DebugShowWaitInfo(msg);
    }
    while (PadRead(0) == 0) {
        DebugShowWaitInfo(msg);
    }
}
#endif

s32 RicCheckHolyWaterCollision(s16 height, s16 width) {
    Collider collider;
    Collider collider2;
    s16 maskedEffects;
    s16 maskedEffects2;
    s16 posX;
    s16 posY;
    s16 newPosY;

    if ((g_CurrentEntity->posX.val + width) < 0 ||
        (g_CurrentEntity->posX.i.hi + width) > 256) {
        if ((g_CurrentEntity->posY.i.hi + height) >= 212) {
            g_CurrentEntity->posY.i.hi = 212 - height;
            return EFFECT_SOLID;
        }
        return EFFECT_NONE;
    }
    posX = g_CurrentEntity->posX.i.hi + width;
    posY = g_CurrentEntity->posY.i.hi + height;

    g_api.CheckCollision(posX, posY, &collider, 0);
    maskedEffects = collider.effects &
                    (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                     EFFECT_UNK_1000 | EFFECT_UNK_0800 | EFFECT_SOLID);
    posY = posY - 1 + collider.unk18;
    g_api.CheckCollision(posX, posY, &collider2, 0);

    newPosY = height + (g_CurrentEntity->posY.i.hi + collider.unk18);
    if ((maskedEffects & (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID)) ==
            EFFECT_SOLID ||
        (maskedEffects & (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID)) ==
            (EFFECT_UNK_0800 | EFFECT_SOLID)) {
        maskedEffects = collider2.effects &
                        (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                         EFFECT_UNK_1000 | EFFECT_SOLID);
        if (!(maskedEffects & EFFECT_SOLID)) {
            g_CurrentEntity->posY.i.hi = newPosY;
            return EFFECT_SOLID;
        }
        if (((s32)collider2.effects & (EFFECT_UNK_8000 | EFFECT_SOLID)) ==
            (EFFECT_UNK_8000 | EFFECT_SOLID)) {
            g_CurrentEntity->posY.i.hi = newPosY - 1 + collider2.unk18;
            return maskedEffects;
        }
        return EFFECT_NONE;
    }
    if ((maskedEffects & (EFFECT_UNK_8000 | EFFECT_SOLID)) ==
        (EFFECT_UNK_8000 | EFFECT_SOLID)) {
        g_CurrentEntity->posY.i.hi = newPosY;
        return maskedEffects &
               (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                EFFECT_UNK_1000 | EFFECT_SOLID);
    }
    return EFFECT_NONE;
}

static int func_8016840C() { return EFFECT_NONE; }

extern EInit D_us_80180460;

void RicEntitySubwpnHolyWater(Entity* self) {
    s16 xMod;
    s32 colRes;

    if (self->step > 2) {
        self->posY.i.hi += 5;
    }
    switch (self->step) {
    case 0:
        self->ext.holywater.subweaponId = PL_W_HOLYWATER;
        InitializeEntity(D_us_80180460);
        self->flags = FLAG_POS_CAMERA_LOCKED;
        self->animSet = ANIMSET_OVL(3);
        self->animCurFrame = 0x23;
        self->zPriority = RIC.zPriority + 2;
        self->unk5A = 0x24;
        self->palette = PAL_FLAG(0x22F);
        xMod = 0;
        if (self->facingLeft) {
            xMod = -xMod;
        }
        self->posX.i.hi += xMod;
        self->posY.i.hi += -16;
        self->ext.holywater.angle = (rand() & 0x7F) + ROT(309.375);
        if (RIC.facingLeft == true) {
            self->ext.holywater.angle = (rand() & 0x7F) + ROT(219.375);
        }
        self->velocityX =
            (FLT_TO_FIX(rcos(self->ext.holywater.angle)) * FIX(3.0 / 128.0)) >>
            8;
        self->velocityY =
            -(FLT_TO_FIX(rsin(self->ext.holywater.angle)) * FIX(3.0 / 128.0)) >>
            8;
        self->hitboxWidth = 4;
        self->hitboxHeight = 4;
        self->ext.holywater.unk80 = 0x200;
        self->step = 1;
        break;

    case 1:
        self->posY.val += self->velocityY;
        colRes = RicCheckHolyWaterCollision(0, 0);
        self->posX.val += self->velocityX;

        if ((colRes & EFFECT_SOLID) || (self->hitFlags != 0)) {
            RicCreateEntFactoryFromEntity(self, 0x28, 0);
            g_api.PlaySfx(SFX_RIC_HOLY_WATER_ATTACK);
            self->ext.holywater.timer = 80;
            self->animSet = 0;
            self->step = 3;
            self->velocityX >>= 2;
        } else if (self->flags & FLAG_DEAD) {
            RicCreateEntFactoryFromEntity(self, 0x28, 0);
            g_api.PlaySfx(SFX_RIC_HOLY_WATER_ATTACK);
            self->ext.holywater.timer = 80;
            self->animSet = 0;
            self->step = 3;
            self->velocityX = -((s32)self->velocityX >> 2);
        }
        break;
    case 2:
        if (self->flags & FLAG_DEAD) {
            DestroyEntity(self);
            return;
        }
        if (--self->ext.holywater.timer == 0) {
            self->velocityX >>= 2;
            self->ext.holywater.timer = 80;
            self->step++;
        }
        break;
    case 3:
        if (self->flags & FLAG_DEAD) {
            self->velocityX = 0;
        }
        if (!(self->ext.holywater.timer & 3)) {
            RicCreateEntFactoryFromEntity(
                self, FACTORY(BP_HOLYWATER_FIRE, self->ext.holywater.unk82), 0);
            self->ext.holywater.unk82 += 1;
            self->velocityX -= (self->velocityX / 32);
        }

        self->posX.val += self->velocityX;
        colRes = RicCheckHolyWaterCollision(6, 0);
        if (!(colRes & EFFECT_SOLID)) {
            self->velocityX >>= 1;
            self->step++;
        }
        break;
    case 4:
        if (self->flags & FLAG_DEAD) {
            self->velocityX = 0;
        }

        if (!(self->ext.holywater.timer & 3)) {
            RicCreateEntFactoryFromEntity(
                self, FACTORY(BP_HOLYWATER_FIRE, self->ext.holywater.unk82), 0);
            self->ext.holywater.unk82 += 1;
        }
        self->velocityY += FIX(12.0 / 128);
        if (self->velocityY > FIX(4)) {
            self->velocityY = FIX(4);
        }
        self->posY.val += self->velocityY;
        colRes = RicCheckHolyWaterCollision(0, 0);
        self->posX.val += self->velocityX;
        xMod = 4;
        if (self->velocityX < 0) {
            xMod = -xMod;
        }
        colRes |= func_8016840C(-7, xMod);
        if (colRes & EFFECT_SOLID) {
            self->velocityX <<= 1;
            self->step--;
        }
        break;
    case 5:
        break;
    }

    if (self->step > 2) {
        if (--self->ext.holywater.timer < 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.holywater.timer == 2) {
            self->step = 5;
        }
        self->posY.i.hi -= 5;
        self->animCurFrame = 0;
    }
    g_Ric.timers[PL_T_3] = 2;
    self->hitFlags = 0;
    self->flags &= ~FLAG_DEAD;
    FntPrint("judge:%02x\n", self->hitboxState);
}

extern EInit D_us_8018046C;
extern s32 D_us_801828C8[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnHolyWaterFlame);
#else
void RicEntitySubwpnHolyWaterFlame(Entity* self) {
    s16 sp10[5];
    s16 sp20[5];
    s16 xMod;
    s16 variant;
    u8 randR;
    u8 randG;
    u8 randB;
    s16 flameHeight;
    s16 angle;
    u8 texX;
    u8 texY;
    s32 i;
    Primitive* prim;

    variant = self->params >> 8;
    texX = D_us_801828C8[(g_GameTimer & 7) * 2 + 0];
    texY = D_us_801828C8[(g_GameTimer & 7) * 2 + 1];
    switch (self->step) {
    case 0:
        self->ext.holywater.subweaponId = PL_W_HOLYWATER_FLAMES;
        InitializeEntity(D_us_8018046C);
        self->ext.holywater.hitboxState = self->hitboxState;
        self->hitboxState = 0;
        randR = (rand() & 0x3F) + 0x2F;
        randG = (rand() & 0x3F) + 0x6F;
        randB = (rand() & 0x7F) + 0x6F;
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 4);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[self->primIndex];
        i = 0;
        while (prim) {
            prim->r0 = prim->r1 = prim->r2 = prim->r3 = randR;
            prim->g0 = prim->g1 = prim->g2 = prim->g3 = randG;
            prim->b0 = prim->b1 = prim->b2 = prim->b3 = randB;
            if (i == 0) {
                prim->r0 = prim->r1 = 0;
                prim->g0 = prim->g1 = 0;
                prim->b0 = prim->b1 = 0;
            }
            prim->clut = PAL_UNK_1B0;
            prim->tpage = 0x1A;
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_HIDE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
            prim->priority = RIC.zPriority + 2;
            i++;
            prim = prim->next;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->ext.holywater.timer = 1;
        self->step = 1;
        break;
    case 1:
        if (--self->ext.holywater.timer == 0) {
            self->hitboxWidth = 4;
            self->hitboxState = self->ext.holywater.hitboxState;
            self->posY.i.hi -= 10;
            RicCreateEntFactoryFromEntity(self, FACTORY(BP_EMBERS, 3), 0);
            self->ext.holywater.timer = 0x50;
            self->posY.i.hi += 10;
            self->ext.holywater.unk80 = (rand() & 0xF) + 0x12;
            self->ext.holywater.angle = rand() & 0xFFF;
            self->step++;
        }
        break;
    case 2:
        if (self->flags & FLAG_DEAD) {
            DestroyEntity(self);
            return;
        }
        xMod = -1;
        if (self->facingLeft) {
            xMod = -xMod;
        }
        angle = self->ext.holywater.angle;
        self->ext.holywater.angle += 0xC0;
        for (i = 0; i < 4; i++) {
            sp10[i] = self->posX.i.hi + (rsin(angle + (i * 1024)) >> 10);
        }
        sp10[4] = self->posX.i.hi;
        sp10[0] = self->posX.i.hi + xMod;
        angle = (self->ext.holywater.timer - 16) * 64 + 0xC00;
        flameHeight = (rsin(angle) >> 8) + self->ext.holywater.unk80;
        flameHeight = flameHeight * 3 >> 1;
        sp20[0] = self->posY.i.hi - flameHeight;
        sp20[4] = self->posY.i.hi;
        sp20[2] = (sp20[0] + sp20[4]) / 2;
        sp20[1] = (sp20[0] + sp20[2]) / 2;
        sp20[3] = (sp20[2] + sp20[4]) / 2;
        prim = &g_PrimBuf[self->primIndex];
        if (--self->ext.holywater.timer < 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.holywater.timer & 3) {
            self->hitboxState = 0;
        } else {
            self->hitboxState = self->ext.holywater.hitboxState;
        }
        if (self->ext.holywater.timer < 0x15) {
            self->hitboxState = 0;
        }
        i = 0;
        while (prim) {
            if (self->ext.holywater.timer <= variant * 2 + 0x18) {
                if (prim->g0 >= 10) {
                    prim->g0 -= 5;
                }
                if (prim->b0 >= 10) {
                    prim->b0 -= 5;
                }
                if (prim->r0 >= 16) {
                    prim->r0 -= 8;
                }
                prim->g1 = prim->g2 = prim->g3 = prim->g0;
                prim->r1 = prim->r2 = prim->r3 = prim->r0;
                prim->b1 = prim->b2 = prim->b3 = prim->b0;
            }
            prim->x0 = sp10[i] - 8;
            prim->x1 = sp10[i] + 8;
            prim->y0 = sp20[i];
            prim->y1 = sp20[i];
            prim->x2 = sp10[i + 1] - 8;
            prim->x3 = sp10[i + 1] + 8;
            prim->y2 = sp20[i + 1];
            prim->y3 = sp20[i + 1];
            prim->drawMode &= ~DRAW_HIDE;
            prim->u0 = prim->u1 = texX + 0x80 - i * 7;
            prim->u2 = prim->u3 = texX + 0x80 - ((i + 1) * 7);
            prim->v0 = prim->v2 = texY + 0x80;
            prim->v1 = prim->v3 = texY + 0x90;
            if ((sp20[4] - sp20[0]) < 7) {
                prim->drawMode |= DRAW_HIDE;
                DestroyEntity(self);
                return;
            }
            i++;
            prim = prim->next;
        }
        self->hitboxHeight = flameHeight >> 1;
        self->hitboxOffY = (-flameHeight >> 1);
        break;
    }
    g_Ric.timers[PL_T_3] = 2;
}
#endif

extern EInit D_us_8018049C;
extern u16 D_us_80182908[];
extern RECT D_us_80182968;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnCrashCross);
#else
void RicEntitySubwpnCrashCross(Entity* self) {
    s16 psp_s4;
    s16 psp_s3;
    s16 right;
    s16 left;
    Primitive* prim;

    psp_s4 = 3;
    psp_s3 = 1;
    right = left = 0;
    self->posY.i.hi = 0x78;
    self->posX.i.hi = RIC.posX.i.hi;
    switch (self->step) {
    case 0:
        self->ext.crashcross.subweaponId = PL_W_CRASH_CROSS;
        InitializeEntity(D_us_8018049C);
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->ext.crashcross.unk80 = 1;
        self->zPriority = 0xC2;
        LoadImage(&D_us_80182968, (u_long*)D_us_80182908);
        g_api.PlaySfx(0x6DF);
        g_api.PlaySfx(0x636);
        self->step = 1;
        break;
    case 1:
        self->ext.crashcross.unk7E.val += psp_s4;
        self->ext.crashcross.unk82 += psp_s4 * 2;
        if (self->ext.crashcross.unk7E.i.lo >= 0x70) {
            RicCreateEntFactoryFromEntity(self, BP_CRASH_CROSSES_ONLY, 0);
            RicCreateEntFactoryFromEntity(self, BP_CRASH_CROSS_PARTICLES, 0);
            self->step++;
        }
        break;
    case 2:
        if (g_Timer & 1) {
            self->ext.crashcross.unk80 += psp_s3 * 2;
            self->ext.crashcross.unk7C += psp_s3;
            if (self->ext.crashcross.unk80 >= 0x2C) {
                self->ext.crashcross.unk84 = 0x80;
                self->step++;
            }
        }
        break;
    case 3:
        if (--self->ext.crashcross.unk84 == 0) {
            g_api.SetFadeMode(FADE_NONE);
            left = self->posX.i.hi - self->ext.crashcross.unk7C;
            if (left < 0) {
                left = 0;
            }
            right = self->posX.i.hi + self->ext.crashcross.unk7C;
            if (right > 0xFF) {
                right = 0xFF;
            }
            g_api.PlaySfx(SFX_WEAPON_APPEAR);
            self->step++;
        }
        break;
    case 4:
        psp_s3 *= 3;
        left = abs(self->posX.i.hi - 0x80);
        psp_s3 = psp_s3 * (left + 0x80) / 112;
        self->ext.crashcross.unk7C += psp_s3;
        left = self->posX.i.hi - self->ext.crashcross.unk7C;
        if (left < 0) {
            left = 0;
        }
        right = self->posX.i.hi + self->ext.crashcross.unk7C;
        if (right > 0xFF) {
            right = 0xFF;
        }
        if (right - left > 0xF8) {
            g_Ric.unk4E = 1;
            DestroyEntity(self);
            return;
        }
        break;
    }
    self->hitboxOffY = 0;
    self->hitboxHeight = self->ext.crashcross.unk7E.i.lo;
    if (self->step == 4) {
        self->hitboxWidth = (right - left) >> 1;
        self->hitboxOffX = ((left + right) >> 1) - self->posX.i.hi;
    } else {
        self->hitboxOffX = 0;
        self->hitboxWidth = self->ext.crashcross.unk7C;
    }
    prim = &g_PrimBuf[self->primIndex];
    prim->x0 = prim->x2 = self->posX.i.hi - self->ext.crashcross.unk7C;
    prim->y1 = prim->y0 = self->posY.i.hi - self->ext.crashcross.unk7E.val;
    prim->x1 = prim->x3 = prim->x0 + self->ext.crashcross.unk80;
    prim->u0 = prim->u2 = 1;
    prim->u1 = prim->u3 = 0x30;
    prim->v0 = prim->v1 = prim->v2 = prim->v3 = 0xF8;
    prim->tpage = 0x11C;
    prim->y2 = prim->y3 = prim->y0 + self->ext.crashcross.unk82;
    if (self->step == 4) {
        prim->x0 = prim->x2 = left;
        prim->x1 = prim->x3 = right;
    }
    prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
    prim->priority = self->zPriority;
    g_Ric.timers[PL_T_3] = 2;
}
#endif

extern EInit D_us_80180454;
extern s16 D_us_801D10C8;

extern AnimationFrame anim_cross_boomerang[];
extern Point16 D_us_801D08C4[4][128];
extern s32 D_us_801D10C4;

void RicEntitySubwpnCross(Entity* self) {
    s16 playerHitboxX;
    s16 playerHitboxY;
    s16 rotate;
    s16* psp_s1;
    s32 xAccel;

    rotate = self->rotate;
    switch (self->step) {
    case 0:
        self->ext.crossBoomerang.subweaponId = PL_W_CROSS;
        InitializeEntity(D_us_80180454);
        self->flags =
            FLAG_UNK_20000000 | FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        D_us_801D10C8 = self->hitboxState;
        // gets used by shadow, must align with that entity
        self->ext.crossBoomerang.unk84 = D_us_801D08C4[D_us_801D10C4];
        D_us_801D10C4++;
        D_us_801D10C4 &= 3;
        RicCreateEntFactoryFromEntity(self, BP_5, 0);
        self->animSet = ANIMSET_OVL(4);
        self->unk5A = 0x44;
        self->anim = anim_cross_boomerang;
        self->facingLeft = RIC.facingLeft;
        self->zPriority = RIC.zPriority;
        RicSetSpeedX(FIX(3.5625));
        self->drawFlags = ENTITY_ROTATE;
        self->rotate = ROT(270);
        self->hitboxWidth = 8;
        self->hitboxHeight = 8;
        self->posY.i.hi -= 8;
        g_api.PlaySfx(SFX_RIC_CRASH_CROSS);
        self->step = 1;
        break;
    case 1:
        if (RIC.pose == 1) {
            self->step++;
        }
    case 2:
        // First phase. We spin at 0x80 angle units per frame.
        // Velocity gets decremented by 1/16 per frame until we slow
        // down to less than 0.75.
        self->rotate -= ROT(11.25);
        self->posX.val += self->velocityX;
        if (self->facingLeft) {
            xAccel = FIX(-1.0 / 16);
        } else {
            xAccel = FIX(1.0 / 16);
        }
        self->velocityX -= xAccel;

        if (abs(self->velocityX) < FIX(0.75)) {
            self->step = 3;
        }

        if ((self->hitFlags == 2) || (self->flags & FLAG_DEAD)) {
            if (self->velocityX < 0) {
                self->velocityX = FIX(-0.03125);
            } else {
                self->velocityX = FIX(0.03125);
            }
            self->ext.crossBoomerang.timer = 30;
            self->step = 3;
            self->ext.crossBoomerang.timer = 16;
            self->hitboxState = 0;
        }

        break;
    case 3:
        // Second phase. Once we are slow, we spin twice as fast, and then
        // wait until our speed gets higher once again (turned around).
        self->rotate -= ROT(22.50);
        self->posX.val += self->velocityX;
        if (self->facingLeft) {
            xAccel = FIX(-1.0 / 16);
        } else {
            xAccel = FIX(1.0 / 16);
        }
        if (self->hitFlags == 2 || (self->flags & FLAG_DEAD)) {
            if (self->facingLeft) {
                xAccel = FIX(-1.0 / 16);
            } else {
                xAccel = FIX(1.0 / 16);
            }
        }
        self->velocityX -= xAccel;
        if (abs(self->velocityX) > FIX(0.75)) {
            self->step++;
        }
        break;
    case 4:
        // Third phase. We've now sped up and we're coming back.
        // Increase speed until a terminal velocity of 2.5.
        if (self->facingLeft) {
            xAccel = FIX(-1.0 / 16);
        } else {
            xAccel = FIX(1.0 / 16);
        }
        self->velocityX -= xAccel;
        if (abs(self->velocityX) > FIX(2.5)) {
            self->hitboxState = D_us_801D10C8;
            self->step++;
        }
    case 5:
        if (--self->ext.crossBoomerang.timer < 0 &&
            ((self->hitFlags == 2) || (self->flags & FLAG_DEAD))) {
            self->velocityY = FIX(-3.0);
            self->ext.holywater.timer = 50;
            self->hitboxState = 0;
            self->step = 6;
            self->velocityX = -((s32)self->velocityX / 2);
        }

        // Now we check 2 conditions. If we're within the player's hitbox...
        playerHitboxX = (RIC.posX.i.hi + RIC.hitboxOffX);
        playerHitboxY = (RIC.posY.i.hi + RIC.hitboxOffY);
        if (abs(self->posX.i.hi - playerHitboxX) <
                RIC.hitboxWidth + self->hitboxWidth &&
            abs(self->posY.i.hi - playerHitboxY) <
                RIC.hitboxHeight + self->hitboxHeight) {
            // ... Then we go to step 7 to be destroyed.
            self->step = 7;
            self->ext.crossBoomerang.timer = 32;
            return;
        }
        // Alternatively, if we're offscreen, we will also be destroyed.
        if ((self->facingLeft == 0 && self->posX.i.hi < -32) ||
            (self->facingLeft && self->posX.i.hi > 0x120)) {
            self->step = 7;
            self->ext.crossBoomerang.timer = 32;
            return;
        }
        // Otherwise, we keep trucking. spin at the slower rate again.
        self->rotate -= ROT(11.25);
        self->posX.val += self->velocityX;
        break;
    case 6:
        if (--self->ext.crossBoomerang.timer == 0) {
            DestroyEntity(self);
            return;
        }
        self->velocityY += FIX(0.15625);
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        self->rotate += ROT(33.75);
        break;
    case 7:
        if (--self->ext.crossBoomerang.timer == 0) {
            DestroyEntity(self);
            return;
        }
        self->hitboxState = 0;
        self->animSet = 0;
        self->posX.val += self->velocityX;
        break;
    }
    // We will increment through these states, creating trails.
    // Factory 3 is entity #4, func_80169C10. Appears to make tiny sparkles.
    // Factory 4 is entity #5, RicEntityHitByCutBlood. Appears to make a
    // "shadow" of the cross boomerang.
    self->ext.crossBoomerang.unk7E++;
    if (1 < self->step && self->step < 6) {
        if ((self->ext.crossBoomerang.unk7E & 0xF) == 1) {
            RicCreateEntFactoryFromEntity(self, BP_SUBWPN_CROSS_PARTICLES, 0);
        }
        if ((self->ext.crossBoomerang.unk7E & 0xF) == 4) {
            RicCreateEntFactoryFromEntity(self, FACTORY(BP_EMBERS, 6), 0);
        }
        if ((self->ext.crossBoomerang.unk7E & 0xF) == 6) {
            RicCreateEntFactoryFromEntity(self, BP_SUBWPN_CROSS_PARTICLES, 0);
        }
        if ((self->ext.crossBoomerang.unk7E & 0xF) == 8) {
            RicCreateEntFactoryFromEntity(self, FACTORY(BP_EMBERS, 6), 0);
        }
        if ((self->ext.crossBoomerang.unk7E & 0xF) == 12) {
            RicCreateEntFactoryFromEntity(self, FACTORY(BP_EMBERS, 6), 0);
        }
        if ((self->ext.crossBoomerang.unk7E & 0xF) == 11) {
            RicCreateEntFactoryFromEntity(self, BP_SUBWPN_CROSS_PARTICLES, 0);
        }
    }
    // Applies a flickering effect
    if ((g_GameTimer >> 1) & 1) {
        self->palette = PAL_FLAG(PAL_UNK_1B0);
    } else {
        self->palette = PAL_FLAG(PAL_UNK_1B1);
    }
    psp_s1 = (s16*)self->ext.crossBoomerang.unk84;
    psp_s1 = &psp_s1[self->ext.crossBoomerang.unk80 * 2];
    *psp_s1 = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
    psp_s1++;
    *psp_s1 = self->posY.i.hi + g_Tilemap.scrollY.i.hi;
    self->ext.crossBoomerang.unk80++;
    self->ext.crossBoomerang.unk80 &= 0x3F;
    rotate ^= self->rotate;
    g_Ric.timers[PL_T_3] = 2;
    self->hitFlags = 0;
    self->flags &= ~FLAG_DEAD;
}

// Equivalent of func_80169C10 in RIC
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", func_us_801C488C);
#else
void func_us_801C488C(Entity* self) {
    Primitive* prim;

    if (self->step == 0) {
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->velocityY = FIX(0.5);
        self->posX.i.hi = self->posX.i.hi - 8 + (rand() & 0xF);
        self->posY.i.hi = self->posY.i.hi - 4 + (rand() & 0xF);
        prim = &g_PrimBuf[self->primIndex];
        prim->clut = PAL_UNK_1B0;
        prim->tpage = 0x1A;
        prim->b0 = 0;
        prim->b1 = 0;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        prim->priority = self->zPriority + 4;
        func_us_801BB5BC(prim, self->posX.i.hi, self->posY.i.hi);
        self->step++;
    } else {
        self->posY.val += self->velocityY;
        if (func_us_801BB5BC(&g_PrimBuf[self->primIndex], self->posX.i.hi,
                             self->posY.i.hi)) {
            DestroyEntity(self);
        }
    }
}
#endif

extern s16 D_us_80182994[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnCrossTrail);
#else
void RicEntitySubwpnCrossTrail(Entity* self) {
    s16* temp;

    switch (self->step) {
    case 0:
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->animSet = ANIMSET_OVL(4);
        // the parent pointer is set in RicEntityFactory.
        // the value of unk84 is set in RicEntitySubwpnCross
        self->ext.crossBoomerang.unk84 =
            self->ext.crossBoomerang.parent->ext.crossBoomerang.unk84;
        self->unk5A = 0x44;
        self->palette = PAL_FLAG(PAL_UNK_1B0);
        self->blendMode = BLEND_TRANSP;
        self->animCurFrame = D_us_80182994[self->params];
        self->facingLeft = RIC.facingLeft;
        self->drawFlags = ENTITY_ROTATE;
        self->rotate = ROT(270);
        self->step++;
        self->zPriority = RIC.zPriority;
        break;
    case 1:
        self->rotate -= ROT(11.25);
        if (self->ext.crossBoomerang.parent->step == 6 ||
            self->ext.crossBoomerang.parent->step == 7) {
            self->step++;
            self->ext.crossBoomerang.timer = (self->params + 1) * 4;
        }
        break;
    case 2:
        self->rotate -= ROT(11.25);
        if (--self->ext.crossBoomerang.timer == 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    // get the x and y position from the parent (must align)
    temp = (s16*)&self->ext.crossBoomerang.unk84[0];
    temp += self->ext.crossBoomerang.unk80 * 2;
    self->posX.i.hi = *temp - g_Tilemap.scrollX.i.hi;
    self->ext.crossBoomerang.unk80 = (self->ext.crossBoomerang.unk80 + 1) & 0x3F;
    temp++;
    self->posY.i.hi = *temp - g_Tilemap.scrollY.i.hi;
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnCrashCrossParticles);
#else
void RicEntitySubwpnCrashCrossParticles(Entity* self) {
    Primitive* prim;
    s16 psp_s4;
    s16 psp_s3;
    s16 psp_s2;
    u8* psp_s1;
    s16 temp_s0;

    if (self->step == 0) {
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x20);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->ext.timer.t = 0xC0;
        self->step++;
        return;
    }
    if (--self->ext.timer.t == 0) {
        DestroyEntity(self);
        return;
    }
    if (self->ext.timer.t > 8 && !(self->ext.timer.t & 3)) {
        for (prim = &g_PrimBuf[self->primIndex]; prim; prim = prim->next) {
            if (prim->r0 == 0) {
                prim->r0 = 1;
                prim->r1 = 0;
                break;
            }
        }
    }
    for (prim = &g_PrimBuf[self->primIndex]; prim; prim = prim->next) {
        if (!prim->r0) {
            continue;
        }
        if (prim->r1 == 0) {
            temp_s0 = rand() & 0x3F;
            prim->g0 = (rand() % 237) + 9;
            prim->g1 = 0xF0 - (rand() & 0x20);
            prim->clut = PAL_UNK_1B0;
            prim->tpage = 0x1A;
            prim->b0 = 0;
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
            prim->g3 = ((u8)temp_s0 >> 2) + 4;
            prim->priority = RIC.zPriority + temp_s0 - 0x20;
            prim->r1++;
        } else {
            prim->g1 -= prim->g3;
            if ((u8)prim->b0 >= 6 || (u8)prim->g1 < 0x18) {
                prim->drawMode = DRAW_HIDE;
                prim->r0 = 0;
            }
        }
        if (!prim->r0) {
            continue;
        }
        psp_s4 = prim->g0;
        psp_s3 = prim->g1;
        psp_s1 = D_us_80181524[prim->b0];
        if (prim->b0 >= 3) {
            psp_s2 = 4;
        } else {
            psp_s2 = 8;
        }
        prim->x0 = psp_s4 - psp_s2;
        prim->y0 = psp_s3 - psp_s2;
        prim->x1 = psp_s4 + psp_s2;
        prim->y1 = psp_s3 - psp_s2;
        prim->x2 = psp_s4 - psp_s2;
        prim->y2 = psp_s3 + psp_s2;
        prim->x3 = psp_s4 + psp_s2;
        prim->y3 = psp_s3 + psp_s2;
        prim->u0 = *psp_s1++;
        prim->v0 = *psp_s1++;
        prim->u1 = *psp_s1++;
        prim->v1 = *psp_s1++;
        prim->u2 = *psp_s1++;
        prim->v2 = *psp_s1++;
        prim->u3 = *psp_s1++;
        prim->v3 = *psp_s1++;
        if (!(g_GameTimer & 1)) {
            prim->b0++;
        }
    }
}
#endif

extern EInit D_us_80180478;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnThrownAxe);
#else
void RicEntitySubwpnThrownAxe(Entity* self) {
    u8 rVal;
    u8 gVal;
    u8 bVal;
    s16 xVar;
    s16 yVar;
    s32 rgbIdx;
    s16 angle0;
    s16 angle1;
    s16 angle2;
    s16 angle3;
    s16 angleOffset;
    s32 twentyone;
    Primitive* prevPrim;
    s32 i;
    Primitive* prim;
    s32 graphicsTemp;

    twentyone = 21;

    switch (self->step) {
    case 0:
        self->ext.subwpnAxe.subweaponId = PL_W_AXE;
        InitializeEntity(D_us_80180478);
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 5);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED |
                      FLAG_HAS_PRIMS;
        self->facingLeft = (RIC.facingLeft + 1) & 1;
        RicSetSpeedX(FIX(-2));
        self->velocityY = FIX(-6);
        if (self->facingLeft) {
            self->ext.subwpnAxe.unk7C = 0x400;
        } else {
            self->ext.subwpnAxe.unk7C = 0xC00;
        }
        self->posY.i.hi -= 12;
        for (prim = &g_PrimBuf[self->primIndex], i = 0; prim != NULL; i++,
            prim = prim->next) {
            prim->tpage = 0x1C;
            prim->u0 = 0;
            prim->v0 = 0;
            prim->u1 = 0x18;
            prim->v1 = 0;
            prim->u2 = 0;
            prim->v2 = 0x28;
            prim->u3 = 0x18;
            prim->v3 = 0x28;
            prim->priority = RIC.zPriority - 2;
            if (i != 0) {
                prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE2 | DRAW_TPAGE |
                                 DRAW_HIDE | DRAW_COLORS | DRAW_TRANSP;
                self->ext.subwpnAxe.unk8C[i - 1] = 0;
                self->ext.subwpnAxe.unk90[i - 1] = 0;
                self->ext.subwpnAxe.unk94[i - 1] = 0;
            } else {
                prim->drawMode = DRAW_UNK_100 | DRAW_HIDE;
            }
        }
        self->hitboxWidth = 12;
        self->hitboxHeight = 12;
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        self->ext.subwpnAxe.unk98 = 0x7F;
        self->step = 1;
        break;
    case 1:
        if (self->facingLeft) {
            angleOffset = -0x80;
        } else {
            angleOffset = 0x80;
        }
        self->ext.subwpnAxe.unk7C += angleOffset;
        if (!(self->ext.subwpnAxe.unk7C & 0x3FF)) {
            g_api.PlaySfxVolPan(
                SFX_WEAPON_SWISH_C, self->ext.subwpnAxe.unk98, 0);
            self->ext.subwpnAxe.unk98 -= 8;
            if (self->ext.subwpnAxe.unk98 < 0) {
                self->ext.subwpnAxe.unk98 = 0;
            }
        }
        self->velocityY += FIX(34.0 / 128);
        if (self->velocityY > FIX(8)) {
            self->velocityY = FIX(8);
        }
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if (self->posY.i.hi > 0x100) {
            DestroyEntity(self);
            return;
        }
        if (self->hitFlags == 2 || (self->flags & FLAG_DEAD)) {
            self->velocityY = FIX(-3);
            self->hitboxState = 0;
            self->step = 2;
            self->velocityX = -(self->velocityX / 2);
        }
        break;
    case 2:
        if (self->facingLeft) {
            angleOffset = 0xC0;
        } else {
            angleOffset = -0xC0;
        }
        self->ext.subwpnAxe.unk7C += angleOffset;
        self->velocityY += FIX(18.0 / 128);
        if (self->velocityY > FIX(8)) {
            self->velocityY = FIX(8);
        }
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if (self->posY.i.hi > 0x100) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    if (self->poseTimer == 0) {
        graphicsTemp = self->pose;
        self->ext.subwpnAxe.unk8C[graphicsTemp] = 0;
        self->ext.subwpnAxe.unk90[graphicsTemp] = 1;
        self->ext.subwpnAxe.unk94[graphicsTemp] = 1;
        graphicsTemp++;
        graphicsTemp &= 3;
        self->poseTimer = 2;
        self->pose = graphicsTemp;
    } else {
        self->poseTimer--;
    }
    for (prim = &g_PrimBuf[self->primIndex], prevPrim = prim,
        graphicsTemp = ((g_GameTimer >> 1) & 1) + 0x1AB, i = 0;
         prim != NULL; i++, prim = prim->next) {
        prim->clut = graphicsTemp;
        if (i == 0) {
            if (self->facingLeft) {
                angle0 = 0x560;
                angle1 = 0x2A0;
                angle2 = 0xAA0;
                angle3 = 0xD60;
            } else {
                angle1 = 0x560;
                angle0 = 0x2A0;
                angle3 = 0xAA0;
                angle2 = 0xD60;
            }
            xVar = self->posX.i.hi;
            yVar = self->posY.i.hi;
            angleOffset = self->ext.subwpnAxe.unk7C;
            angle0 += angleOffset;
            angle1 += angleOffset;
            angle2 += angleOffset;
            angle3 += angleOffset;
            prim->x0 = xVar + (((rcos(angle0) << 4) * twentyone) >> 0x10);
            prim->y0 = yVar + -(((rsin(angle0) << 4) * twentyone) >> 0x10);
            prim->x1 = xVar + (((rcos(angle1) << 4) * twentyone) >> 0x10);
            prim->y1 = yVar + -(((rsin(angle1) << 4) * twentyone) >> 0x10);
            prim->x2 = xVar + (((rcos(angle2) << 4) * twentyone) >> 0x10);
            prim->y2 = yVar + -(((rsin(angle2) << 4) * twentyone) >> 0x10);
            prim->x3 = xVar + (((rcos(angle3) << 4) * twentyone) >> 0x10);
            prim->y3 = yVar + -(((rsin(angle3) << 4) * twentyone) >> 0x10);
            prim->drawMode &= ~DRAW_HIDE;
        } else if (self->ext.subwpnAxe.unk90[i - 1]) {
            if (self->ext.subwpnAxe.unk94[i - 1]) {
                self->ext.subwpnAxe.unk94[i - 1] = 0;
                prim->x0 = prevPrim->x0;
                prim->y0 = prevPrim->y0;
                prim->x1 = prevPrim->x1;
                prim->y1 = prevPrim->y1;
                prim->x2 = prevPrim->x2;
                prim->y2 = prevPrim->y2;
                prim->x3 = prevPrim->x3;
                prim->y3 = prevPrim->y3;
            }
            rgbIdx = self->ext.subwpnAxe.unk8C[i - 1]++;
            if (rgbIdx < 10) {
                rVal = D_us_8018299C[rgbIdx * 4 + 0];
                gVal = D_us_8018299C[rgbIdx * 4 + 1];
                bVal = D_us_8018299C[rgbIdx * 4 + 2];
                prim->r0 = rVal;
                prim->g0 = gVal;
                prim->b0 = bVal;
                prim->r1 = rVal;
                prim->g1 = gVal;
                prim->b1 = bVal;
                prim->r2 = rVal;
                prim->g2 = gVal;
                prim->b2 = bVal;
                prim->r3 = rVal;
                prim->g3 = gVal;
                prim->b3 = bVal;
                prim->drawMode &= ~DRAW_HIDE;
            } else {
                self->ext.subwpnAxe.unk90[i - 1] = 0;
                prim->drawMode |= DRAW_HIDE;
            }
        }
    }
}
#endif

extern EInit D_us_80180490;
extern u8 D_us_8018299C[];

void RicEntityCrashAxe(Entity* self) {
    Primitive* primFirst;
    Primitive* prim;
    s16 angle1;
    s16 angle2;
    s16 angle3;
    s16 angle4;
    s32 mod;
    s32 i;
    u8 r;
    u8 g;
    u8 b;
    s16 angleMod;
    s16 x;
    s16 y;
    s16 angle;
    s32 pose;
    s32 velocity;
    s32 colorRef;

    mod = 21;
    switch (self->step) {
    case 0:
        self->ext.subwpnAxe.subweaponId = 2;
        InitializeEntity(D_us_80180490);
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 5);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->facingLeft = 0;
        self->ext.subwpnAxe.unk7C = ((self->params & 0xFF) << 9) + ROT(270);
        self->posY.i.hi -= 12;
        prim = &g_PrimBuf[self->primIndex];
        i = 0;
        while (prim) {
            prim->tpage = 0x1C;
            prim->u0 = prim->v0 = prim->v1 = prim->u2 = 0;
            prim->u1 = prim->u3 = 0x18;
            prim->v2 = prim->v3 = 0x28;
            prim->priority = RIC.zPriority + 4;
            if (i != 0) {
                prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE2 | DRAW_TPAGE |
                                 DRAW_HIDE | DRAW_COLORS | DRAW_TRANSP;
                self->ext.subwpnAxe.unk8C[i - 1] = 0;
                self->ext.subwpnAxe.unk90[i - 1] = 0;
                self->ext.subwpnAxe.unk94[i - 1] = 0;
            } else {
                prim->drawMode = DRAW_UNK_100 | DRAW_HIDE;
            }
            i++;
            prim = prim->next;
        }
        self->hitboxHeight = self->hitboxWidth = 12;
        self->ext.subwpnAxe.angle = (self->params & 0xFF) << 9;
        self->ext.subwpnAxe.velocity = 16;
        self->step = 1;
        break;
    case 1:
        velocity = self->ext.subwpnAxe.velocity;
        self->ext.subwpnAxe.velocity++;
        if (self->ext.subwpnAxe.velocity > 0x28) {
            self->ext.subwpnAxe.unkA2 = 16;
            self->step++;
        }
        angle = self->ext.subwpnAxe.angle;
        self->ext.subwpnAxe.angle += 0xC0;
        self->ext.subwpnAxe.unk7C += 0x80;
        self->velocityX = velocity * rcos(angle);
        self->velocityY = velocity * -rsin(angle);
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        break;
    case 2:
        if (--self->ext.subwpnAxe.unkA2 == 0) {
            self->ext.subwpnAxe.unkA2 = 8;
            self->step++;
        }
        velocity = self->ext.subwpnAxe.velocity;
        angle = self->ext.subwpnAxe.angle;
        self->ext.subwpnAxe.angle += 0xC0;
        self->ext.subwpnAxe.unk7C += 0x80;
        self->velocityX = rcos(angle) * velocity;
        self->velocityY = -rsin(angle) * velocity;
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        break;
    case 3:
        if (--self->ext.subwpnAxe.unkA2 == 0) {
            g_Ric.unk4E = 1;
            self->flags &= ~FLAG_UNK_10000000;
        }
        velocity = self->ext.subwpnAxe.velocity;
        self->ext.subwpnAxe.velocity += 2;
        angle = self->ext.subwpnAxe.angle;
        self->ext.subwpnAxe.angle += 0x28;
        self->ext.subwpnAxe.unk7C += 0x80;
        self->velocityX = rcos(angle) * velocity;
        self->velocityY = -rsin(angle) * velocity;
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        if (self->poseTimer == 0) {
            pose = self->pose;
            self->ext.subwpnAxe.unk8C[pose] = 0;
            self->ext.subwpnAxe.unk90[pose] = 1;
            self->ext.subwpnAxe.unk94[pose] = 1;
            pose++;
            pose &= 3;
            self->pose = pose;
            self->poseTimer = 2;
        } else {
            self->poseTimer--;
        }
        if ((self->hitFlags == 2) || (self->flags & FLAG_DEAD)) {
            self->velocityY = FIX(-3.0);
            self->hitboxState = 0;
            self->step = 4;
            self->velocityX = -((s32)self->velocityX / 2);
        }

        break;
    case 4:
        if (self->facingLeft) {
            angleMod = 0xC0;
        } else {
            angleMod = -0xC0;
        }
        self->ext.subwpnAxe.unk7C += angleMod;
        self->velocityY += 0x2400;
        if (self->velocityY > FIX(8.0)) {
            self->velocityY = FIX(8.0);
        }
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if (self->posY.i.hi > 256) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    prim = &g_PrimBuf[self->primIndex];
    primFirst = prim;
    pose = ((g_GameTimer >> 1) & 1) + 0x1AB;
    i = 0;
    while (prim != NULL) {
        prim->clut = pose;
        if (i == 0) {
            if (self->facingLeft) {
                angle1 = 0x800 - 0x2A0;
                angle2 = 0x2A0;
                angle3 = 0x800 + 0x2A0;
                angle4 = 0x800 + 0x800 - 0x2A0;
            } else {
                angle2 = 0x800 - 0x2A0;
                angle1 = 0x2A0;
                angle4 = 0x800 + 0x2A0;
                angle3 = 0x800 + 0x800 - 0x2A0;
            }
            x = self->posX.i.hi;
            y = self->posY.i.hi;
            angleMod = self->ext.subwpnAxe.unk7C;
            angle1 += angleMod;
            angle2 += angleMod;
            angle3 += angleMod;
            angle4 += angleMod;

            prim->x0 = x + +(((rcos(angle1) << 4) * mod) >> 0x10);
            prim->y0 = y + -(((rsin(angle1) << 4) * mod) >> 0x10);
            prim->x1 = x + +(((rcos(angle2) << 4) * mod) >> 0x10);
            prim->y1 = y + -(((rsin(angle2) << 4) * mod) >> 0x10);
            prim->x2 = x + +(((rcos(angle3) << 4) * mod) >> 0x10);
            prim->y2 = y + -(((rsin(angle3) << 4) * mod) >> 0x10);
            prim->x3 = x + +(((rcos(angle4) << 4) * mod) >> 0x10);
            prim->y3 = y + -(((rsin(angle4) << 4) * mod) >> 0x10);
            prim->drawMode &= ~DRAW_HIDE;
        } else if (self->ext.subwpnAxe.unk90[i - 1]) {
            if (self->ext.subwpnAxe.unk94[i - 1]) {
                self->ext.subwpnAxe.unk94[i - 1] = 0;
                prim->x0 = primFirst->x0;
                prim->y0 = primFirst->y0;
                prim->x1 = primFirst->x1;
                prim->y1 = primFirst->y1;
                prim->x2 = primFirst->x2;
                prim->y2 = primFirst->y2;
                prim->x3 = primFirst->x3;
                prim->y3 = primFirst->y3;
            }
            colorRef = (self->ext.subwpnAxe.unk8C[i - 1]++);
            if (colorRef < 10) {
                r = D_us_8018299C[colorRef * 4 + 0];
                g = D_us_8018299C[colorRef * 4 + 1];
                b = D_us_8018299C[colorRef * 4 + 2];
                prim->r0 = r;
                prim->g0 = g;
                prim->b0 = b;
                prim->r1 = r;
                prim->g1 = g;
                prim->b1 = b;
                prim->r2 = r;
                prim->g2 = g;
                prim->b2 = b;
                prim->r3 = r;
                prim->g3 = g;
                prim->b3 = b;
                prim->drawMode &= ~DRAW_HIDE;
            } else {
                self->ext.subwpnAxe.unk90[i - 1] = 0;
                prim->drawMode |= DRAW_HIDE;
            }
        }
        i++;
        prim = prim->next;
    }
}

extern s16 D_us_801829C4[];
extern s32 D_us_801D10CC;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnKnife);
#else
void RicEntitySubwpnKnife(Entity* self) {
    Collider collider;
    Primitive* prim;
    s16 offsetX;
    s16 offsetY;
    s16 angle1;
    s16 angle2;
    s16 angle3;
    s16 angle4;
    s16 x;
    s16 y;
    s16 xCol;
    s32 modX;
    s32 modY;
    s32 i;

    xCol = 0;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 2);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->hitboxWidth = 4;
        self->hitboxHeight = 2;
        self->hitboxOffX = 4;
        self->hitboxOffY = 0;
        self->facingLeft = RIC.facingLeft;
        if (self->params & 0xFF00) {
            self->posY.i.hi += D_us_801829C4[D_us_801D10CC & 7];
            D_us_801D10CC++;
        } else {
            self->posY.i.hi -= 9;
        }
        prim = &g_PrimBuf[self->primIndex];
        prim->tpage = 0x1C;
        prim->clut = PAL_UNK_1AB;
        prim->u0 = prim->u1 = 0x18;
        prim->v0 = prim->v2 = 0x18;
        prim->u2 = prim->u3 = 0x20;
        prim->v1 = prim->v3 = 0;
        prim->drawMode = DRAW_HIDE | DRAW_UNK02;
        prim->priority = RIC.zPriority + 2;

        prim = prim->next;
        prim->type = PRIM_LINE_G2;
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_HIDE | DRAW_UNK02 | DRAW_TRANSP;
        prim->r0 = 0x7F;
        prim->g0 = 0x3F;
        prim->b0 = 0;
        prim->priority = RIC.zPriority + 2;
        RicSetSpeedX(FIX(8));
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        self->step++;
        break;
    case 1:
        self->ext.subweapon.timer++;
        if (self->velocityX > 0) {
            xCol = 8;
        }
        if (self->velocityX < 0) {
            xCol = -8;
        }
        if (self->hitFlags == 1) {
            self->ext.subweapon.timer = 4;
            self->step = 3;
            self->hitboxState = 0;
            return;
        }
        for (i = 0; i < 8; i++) {
            if (self->velocityX > 0) {
                self->posX.i.hi++;
            }
            if (self->velocityX < 0) {
                self->posX.i.hi--;
            }
            g_api.CheckCollision(
                self->posX.i.hi + xCol, self->posY.i.hi, &collider, 0);
            if ((self->hitFlags == 2) || (collider.effects & EFFECT_BLOCK)) {
                self->ext.subweapon.timer = 64;
                self->velocityY = FIX(-2.5);
                self->hitboxState = 0;
                self->velocityX = -(self->velocityX >> 3);
                self->posX.i.hi += xCol;
                RicCreateEntFactoryFromEntity(self, FACTORY(BP_42, 2), 0);
                self->posX.i.hi -= xCol;
                g_api.PlaySfx(SFX_UI_SUBWEAPON_TINK);
                self->step++;
                return;
            }
        }
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        offsetX = 12;
        offsetY = 8;
        if (self->facingLeft) {
            offsetX = -offsetX;
            offsetY = -offsetY;
        }
        prim = &g_PrimBuf[self->primIndex];
        prim->x0 = x - offsetX;
        prim->y0 = y - 4;
        prim->x1 = x + offsetX;
        prim->y1 = y - 4;
        prim->x2 = x - offsetX;
        prim->y2 = y + 4;
        prim->x3 = x + offsetX;
        prim->y3 = y + 4;
        prim->clut = ((g_GameTimer >> 1) & 1) + 0x1AB;
        prim->drawMode &= ~DRAW_HIDE;
        prim = prim->next;
        prim->x0 = x - offsetY;
        prim->y0 = y - 1;
        prim->x1 = x - (offsetX * (self->ext.subweapon.timer / 2));
        prim->y1 = y - 1;
        prim->drawMode &= ~DRAW_HIDE;
        if (self->step != 1) {
            prim->drawMode |= DRAW_HIDE;
        }
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        if (--self->ext.subweapon.timer == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.subweapon.timer == 0x20) {
            prim->drawMode |=
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3) =
                0x60;
        }
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        self->velocityY += FIX(0.125);
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        if (self->facingLeft == 0) {
            angle1 = 0x800 - 0xD2;
            angle2 = 0xD2;
            angle3 = 0x800 + 0xD2;
            angle4 = -0xD2;
            self->rotate -= ROT(11.25);
        } else {
            angle2 = 0x800 - 0xD2;
            angle1 = 0xD2;
            angle4 = 0x800 + 0xD2;
            angle3 = -0xD2;
            self->rotate += ROT(11.25);
        }
        angle1 += self->rotate;
        angle2 += self->rotate;
        angle3 += self->rotate;
        angle4 += self->rotate;
        prim = &g_PrimBuf[self->primIndex];
        modX = (rcos(angle1) * 0xCA0) >> 0x14;
        modY = -(rsin(angle1) * 0xCA0) >> 0x14;
        prim->x0 = x + (s16)modX;
        prim->y0 = y - (s16)modY;
        modX = (rcos(angle2) * 0xCA0) >> 0x14;
        modY = -(rsin(angle2) * 0xCA0) >> 0x14;
        prim->x1 = x + (s16)modX;
        prim->y1 = y - (s16)modY;
        modX = (rcos(angle3) * 0xCA0) >> 0x14;
        modY = -(rsin(angle3) * 0xCA0) >> 0x14;
        prim->x2 = x + (s16)modX;
        prim->y2 = y - (s16)modY;
        modX = (rcos(angle4) * 0xCA0) >> 0x14;
        modY = -(rsin(angle4) * 0xCA0) >> 0x14;
        prim->x3 = x + (s16)modX;
        prim->y3 = y - (s16)modY;
        prim->clut = ((g_GameTimer >> 1) & 1) + 0x1AB;
        if (self->ext.subweapon.timer < 0x21) {
            prim->r0 -= 2;
            prim->g0 = prim->b0 = PGREY(prim, 1) = PGREY(prim, 2) =
                PGREY(prim, 3) = prim->r0;
        }
        prim->drawMode &= ~DRAW_HIDE;
        prim = prim->next;
        prim->drawMode |= DRAW_HIDE;
        break;
    case 3:
        if (--self->ext.subweapon.timer == 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", ReboundStoneBounce1);
#else
static void ReboundStoneBounce1(s16 bounceAngle) {
    g_CurrentEntity->ext.reboundStone.stoneAngle =
        (bounceAngle * 2) - g_CurrentEntity->ext.reboundStone.stoneAngle;
    if (!g_CurrentEntity->ext.reboundStone.unk82) {
        g_CurrentEntity->ext.reboundStone.unk80++;
        g_CurrentEntity->ext.reboundStone.unk82++;
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", ReboundStoneBounce2);
#else
static void ReboundStoneBounce2(s16 bounceAngle) {
    if (!g_CurrentEntity->ext.reboundStone.unk82) {
        g_CurrentEntity->ext.reboundStone.stoneAngle =
            (bounceAngle * 2) - g_CurrentEntity->ext.reboundStone.stoneAngle;
        g_CurrentEntity->ext.reboundStone.unk80++;
        g_CurrentEntity->ext.reboundStone.unk82++;
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnReboundStone);
#else
void RicEntitySubwpnReboundStone(Entity* self) {
    s16 playerX;
    s16 playerY;
    Collider collider;
    s32 speed;
    s32 currX;
    s32 currY;
    s32 collX;
    s32 collY;
    s32 deltaX;
    s32 deltaY;
    s32 i;
    s32 colliderFlags;
    PrimLineG2* prim;

    speed = 0x400;
    self->ext.reboundStone.unk82 = 0;
    playerX = self->posX.i.hi;
    playerY = self->posY.i.hi;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 16);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->posY.i.hi -= 0x10;
        playerY = self->posY.i.hi;
        for (prim = (PrimLineG2*)&g_PrimBuf[self->primIndex], i = 0;
             prim != NULL; i++, prim = prim->next) {
            PGREY(prim, 0) = PGREY(prim, 1) = 0xFF;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
            prim->priority = RIC.zPriority + 2;
            if (i != 0) {
                prim->drawMode |= DRAW_HIDE;
            }
            prim->x0 = prim->x1 = playerX;
            prim->y0 = prim->y1 = playerY;
            prim->timer = 20;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->zPriority = RIC.zPriority + 2;
        if (RIC.facingLeft) {
            self->ext.reboundStone.stoneAngle = 0x980;
        } else {
            self->ext.reboundStone.stoneAngle = 0xE80;
        }
        self->ext.reboundStone.stoneAngle += (rand() & 0x7F) - 0x40;
        self->ext.reboundStone.lifeTimer = 0x40;
        self->hitboxWidth = 4;
        self->hitboxHeight = 4;
        g_api.CheckCollision(self->posX.i.hi, self->posY.i.hi, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            self->ext.reboundStone.unk84 = 4;
        }
        self->step++;
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        break;
    case 1:
        deltaX = rcos(self->ext.reboundStone.stoneAngle) * 0x10;
        deltaY = -rsin(self->ext.reboundStone.stoneAngle) * 0x10;
        currX = self->posX.val;
        currY = self->posY.val;
        if (!self->ext.reboundStone.unk84) {
            for (i = 0; i < 6; i++) {
                collX = FIX_TO_I(currX);
                collY = FIX_TO_I(currY + deltaY);
                g_api.CheckCollision(collX, collY, &collider, 0);
                colliderFlags =
                    collider.effects &
                    (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                     EFFECT_UNK_1000 | EFFECT_UNK_0800 | EFFECT_BLOCK);
                if (colliderFlags & EFFECT_SOLID) {
                    colliderFlags &= 0xFF00;
                    if (deltaY > 0) {
                        if ((colliderFlags == 0) ||
                            (colliderFlags & EFFECT_UNK_0800)) {
                            ReboundStoneBounce1(0x800);
                        }
                        if (colliderFlags == EFFECT_UNK_8000) {
                            ReboundStoneBounce2(0x200);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_1000) {
                            ReboundStoneBounce2(0x12E);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_2000) {
                            ReboundStoneBounce2(0xA0);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_4000) {
                            ReboundStoneBounce2(0x600);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            ReboundStoneBounce2(0x6D2);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            ReboundStoneBounce2(0x760);
                        }
                    }
                    if (deltaY < 0) {
                        if ((colliderFlags == 0) ||
                            (colliderFlags & EFFECT_UNK_8000)) {
                            ReboundStoneBounce1(0x800);
                        }
                        if (colliderFlags == EFFECT_UNK_0800) {
                            ReboundStoneBounce2(0xE00);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_1000) {
                            ReboundStoneBounce2(0xED2);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_2000) {
                            ReboundStoneBounce2(0xF60);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_4000) {
                            ReboundStoneBounce2(0xA00);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            ReboundStoneBounce2(0x92E);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            ReboundStoneBounce2(0x8A0);
                        }
                    }
                }
                collY = FIX_TO_I(currY);
                collX = FIX_TO_I(currX + deltaX);
                g_api.CheckCollision(collX, collY, &collider, 0);
                colliderFlags =
                    collider.effects &
                    (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                     EFFECT_UNK_1000 | EFFECT_UNK_0800 | EFFECT_BLOCK);
                if (colliderFlags & EFFECT_SOLID) {
                    colliderFlags &= 0xFF00;
                    if (deltaX > 0) {
                        if ((colliderFlags == 0) ||
                            TEST_BITS(colliderFlags, 0x4800) ||
                            TEST_BITS(colliderFlags, 0xC000)) {
                            ReboundStoneBounce1(0x400);
                        }
                        if (colliderFlags == EFFECT_UNK_0800) {
                            ReboundStoneBounce2(0xE00);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_1000) {
                            ReboundStoneBounce2(0xED2);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_2000) {
                            ReboundStoneBounce2(0xF60);
                        }
                        if (colliderFlags == EFFECT_UNK_8000) {
                            ReboundStoneBounce2(0x200);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_1000) {
                            ReboundStoneBounce2(0x12E);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_2000) {
                            ReboundStoneBounce2(0xA0);
                        }
                    }
                    if (deltaX < 0) {
                        if ((colliderFlags == 0) ||
                            ((colliderFlags & 0x4800) == 0x800) ||
                            ((colliderFlags & 0xC000) == 0x8000)) {
                            ReboundStoneBounce1(0x400);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_4000) {
                            ReboundStoneBounce2(0xA00);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            ReboundStoneBounce2(0x92E);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            ReboundStoneBounce2(0x8A0);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_4000) {
                            ReboundStoneBounce2(0x600);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            ReboundStoneBounce2(0x6D2);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            ReboundStoneBounce2(0x760);
                        }
                    }
                }
                if (self->ext.reboundStone.unk82) {
                    break;
                }
                currX += deltaX;
                currY += deltaY;
            }
        } else {
            self->ext.reboundStone.unk84--;
        }
        if (self->ext.reboundStone.unk82) {
            g_api.CreateEntFactoryFromEntity(self, FACTORY(BP_42, 2), 0);
            g_api.PlaySfx(SFX_UI_SUBWEAPON_TINK);
        }
        if (self->posX.i.hi < -0x40 || self->posX.i.hi > 0x140 ||
            self->posY.i.hi < -0x40 || self->posY.i.hi > 0x140 ||
            self->ext.reboundStone.unk80 == 15) {
            self->step = 2;
        } else {
            deltaX =
                ((rcos(self->ext.reboundStone.stoneAngle) << 4) * speed) >> 8;
            self->posX.val += deltaX;
            deltaY =
                -((rsin(self->ext.reboundStone.stoneAngle) << 4) * speed) >> 8;
            self->posY.val += deltaY;
        }
        break;
    case 2:
        if (--self->ext.reboundStone.lifeTimer == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.reboundStone.lifeTimer == 0x20) {
            self->hitboxState = 0;
        }
        prim = (PrimLineG2*)&g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->timer = 0;
            prim = prim->next;
        }
        break;
    }

    prim = (PrimLineG2*)&g_PrimBuf[self->primIndex];
    i = 0;
    if (self->step == 2) {
        colliderFlags = 4;
    } else {
        colliderFlags = 2;
    }
    for (; prim != NULL; i++, prim = prim->next) {
        if (self->ext.reboundStone.unk82) {
            if (i == self->ext.reboundStone.unk80) {
                prim->x0 = playerX;
                prim->y0 = playerY;
                prim->drawMode &= ~DRAW_HIDE;
            }
        }
        if (i == self->ext.reboundStone.unk80) {
            prim->x1 = self->posX.i.hi;
            prim->y1 = self->posY.i.hi;
        }
        if (!(prim->drawMode & DRAW_HIDE)) {
            if (prim->timer) {
                prim->timer--;
            } else {
                if (colliderFlags < prim->b1) {
                    prim->b1 -= colliderFlags;
                }
                PGREY(prim, 0) = PGREY(prim, 1);
            }
        }
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnThrownVibhuti);
#else
void RicEntitySubwpnThrownVibhuti(Entity* self) {
    Collider col;
    FakePrim* prim;
    s16 randomAngle;
    s16 x;
    s16 y;
    s16 temp;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, 13);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->hitboxWidth = self->hitboxHeight = 4;
        self->ext.subweapon.timer = 0x80;
        self->posY.i.hi -= 15;
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        if (RIC.facingLeft) {
            self->posX.i.hi -= 13;
        } else {
            self->posX.i.hi += 13;
        }
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        i = 0;
        while (true) {
            prim->drawMode = DRAW_UNK02;
            prim->priority = RIC.zPriority - 1;
            if (prim->next == NULL) {
                prim->w = 0;
                prim->x0 = 0;
                prim->y0 = 0;
                prim->drawMode &= ~DRAW_HIDE;
                break;
            }
            prim->posX.i.hi = x;
            prim->posY.i.hi = y;
            prim->posX.i.lo = prim->posY.i.lo = 0;
            randomAngle = (rand() & 0xFF) + 0x100;
            temp = (rand() & 0xFF) + 0x80;
            prim->velocityX.val = ((rcos(randomAngle) << 4) * temp >> 9);
            prim->velocityX.val += FIX(0.5);
            prim->velocityY.val = -((rsin(randomAngle) << 4) * temp >> 9);
            prim->velocityX.val = (prim->velocityX.val * 3) >> 1;
            if (self->facingLeft) {
                prim->velocityX.val = -prim->velocityX.val;
            }
            prim->delay = 1;
            prim->posY.i.hi -= 4;
            prim->r0 = 0xFF;
            prim->g0 = 0xFF;
            prim->b0 = 0xFF;
            prim->w = 2;
            prim->h = 2;
            prim->x0 = prim->posX.i.hi;
            prim->y0 = prim->posY.i.hi;
            i++;
            prim = prim->next;
        }
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        self->step++;
        break;
    case 1:
        temp = 2;
        if (self->facingLeft) {
            temp = -temp;
        }
        if (--self->ext.subweapon.timer == 0) {
            DestroyEntity(self);
            return;
        }
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        i = 0;
        while (true) {
            if (prim->next == NULL) {
                prim->w = 0;
                prim->x0 = 0;
                prim->y0 = 0;
                prim->drawMode &= ~DRAW_HIDE;
                break;
            }
            prim->posX.i.hi = prim->x0;
            prim->posY.i.hi = prim->y0;
            if (prim->delay) {
                if (prim->velocityX.val != 0) {
                    prim->posX.val += prim->velocityX.val;
                    g_api.CheckCollision(
                        prim->posX.i.hi + temp, prim->posY.i.hi, &col, 0);
                    if (col.effects & EFFECT_SIDE) {
                        prim->velocityX.val = 0;
                    }
                }
                prim->posY.val += prim->velocityY.val;
                prim->velocityY.val += FIX(12.0 / 128);
                if (prim->velocityY.val > FIX(4)) {
                    prim->velocityY.val = FIX(4);
                }
                if (prim->velocityY.val > 0) {
                    g_api.CheckCollision(
                        prim->posX.i.hi, prim->posY.i.hi, &col, 0);
                    if (col.effects & EFFECT_SOLID) {
                        prim->delay = 0;
                        prim->w = prim->h = 3;
                        prim->posY.i.hi += col.unk18;
                        prim->posY.i.hi -= (i % 3 + 1);
                    }
                }
            }
            if ((self->ext.subweapon.timer & 7) == i) {
                self->posX.i.hi = prim->posX.i.hi;
                self->posY.i.hi = prim->posY.i.hi;
                if (prim->drawMode & DRAW_HIDE) {
                    self->hitboxWidth = self->hitboxHeight = 0;
                } else {
                    self->hitboxWidth = self->hitboxHeight = 4;
                }
                if (prim->delay) {
                    self->hitboxOffY = 0;
                } else {
                    self->hitboxOffY = -6;
                }
            }
            if (self->hitFlags && ((self->ext.subweapon.timer + 1) & 7) == i) {
                prim->drawMode = DRAW_HIDE;
            }
            if (self->ext.subweapon.timer - 1 == i) {
                prim->drawMode = DRAW_HIDE;
            }
            prim->x0 = prim->posX.i.hi;
            prim->y0 = prim->posY.i.hi;
            i++;
            prim = prim->next;
        }
        self->hitFlags = 0;
        break;
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", BO6_PrimDecreaseBrightness);
#else
u8 BO6_PrimDecreaseBrightness(Primitive* prim, u8 amount) {
    s32 i;
    s32 j;
    u8* colorPtr;
    u8* channelPtr;
    u8 isEnd;

    isEnd = 0;
    colorPtr = &prim->r0;
    for (i = 0; i < 4; colorPtr += OFF(Primitive, r1) - OFF(Primitive, r0),
        i++) {
        for (j = 0; j < 3; j++) {
            channelPtr = &colorPtr[j];
            *channelPtr -= amount;
            if (*channelPtr < 16) {
                *channelPtr = 16;
            } else {
                isEnd |= 1;
            }
        }
    }
    return isEnd;
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnAgunea);
#else
void RicEntitySubwpnAgunea(Entity* self) {
    Primitive* prim;
    s16 tempX;
    s16 tempY;

    if (g_Ric.status & (PLAYER_STATUS_TRANSFORM | PLAYER_STATUS_UNK10000)) {
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->hitboxWidth = self->hitboxHeight = 4;
        self->hitboxOffX = 4;
        self->hitboxOffY = 0;
        self->facingLeft = RIC.facingLeft;
        self->posY.i.hi = self->ext.agunea.unk82 =
            RIC.posY.i.hi + RIC.hitboxOffY - 8;
        prim = &g_PrimBuf[self->primIndex];
        self->posX.i.hi = self->ext.agunea.unk80 = RIC.posX.i.hi;
        prim->type = PRIM_LINE_G2;
        prim->drawMode = DRAW_UNK_200 | DRAW_UNK_100 | DRAW_TPAGE2 |
                         DRAW_TPAGE | DRAW_TRANSP;
        prim->r1 = 0x60;
        prim->g1 = 0;
        prim->b1 = 0x80;
        prim->priority = RIC.zPriority + 2;
        RicSetSpeedX(FIX(6));
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        self->step++;
        break;
    case 1:
        self->posX.val += self->velocityX;
        if (self->posX.i.hi < -0x40 || self->posX.i.hi > 0x140 ||
            self->posY.i.hi < -0x20 || self->posY.i.hi > 0x120) {
            self->step = 2;
        }
        if (self->hitFlags) {
            self->step = 3;
            self->ext.agunea.parent = self->unkB8;
        }
        break;
    case 4:
        self->posX.i.hi = self->ext.agunea.parent->posX.i.hi;
        self->posY.i.hi = self->ext.agunea.parent->posY.i.hi;
        if (++self->ext.agunea.unk7C < 16) {
            break;
        }
        // fallthrough
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        if (prim->r1 < 5) {
            DestroyEntity(self);
            return;
        }
        break;
    case 3:
        if ((g_Ric.padPressed & (PAD_UP + PAD_SQUARE)) !=
            (PAD_UP + PAD_SQUARE)) {
            self->step = 4;
        }
        if (!self->ext.agunea.parent->entityId) {
            self->step = 2;
            return;
        }
        if (self->ext.agunea.unk7C &&
            (self->ext.agunea.parent->hitPoints > 0x7000 ||
             !self->ext.agunea.parent->hitPoints ||
             !self->ext.agunea.parent->hitboxState)) {
            self->step = 2;
            return;
        }
        tempX = self->posX.i.hi = self->ext.agunea.parent->posX.i.hi;
        tempY = self->posY.i.hi = self->ext.agunea.parent->posY.i.hi;
        if ((self->ext.agunea.unk7C % 12) == 0) {
            self->posX.i.hi += (rand() & 0xF) - 8;
            self->posY.i.hi += (rand() & 0xF) - 8;
            if (g_Status.hearts >= 5) {
                g_Status.hearts -= 5;
                RicCreateEntFactoryFromEntity(self, BP_AGUNEA_HIT_ENEMY, 0);
                g_api.PlaySfx(SFX_THUNDER_B);
            } else if (self->ext.agunea.unk84 == 0) {
                RicCreateEntFactoryFromEntity(self, BP_AGUNEA_HIT_ENEMY, 0);
                g_api.PlaySfx(SFX_THUNDER_B);
                self->ext.agunea.unk84++;
            } else {
                self->step = 4;
            }
        }
        self->posX.i.hi = tempX;
        self->posY.i.hi = tempY;
        self->ext.agunea.unk7C++;
        break;
    }
    prim = &g_PrimBuf[self->primIndex];
    if (prim->r1 >= 4) {
        prim->r1 -= 4;
    }
    if (prim->g1 >= 4) {
        prim->g1 -= 4;
    }
    if (prim->b1 >= 4) {
        prim->b1 -= 4;
    }
    if (prim->b1 < 5) {
        prim->drawMode |= DRAW_HIDE;
    }
    prim->x0 = self->ext.agunea.unk80;
    prim->y0 = self->ext.agunea.unk82;
    prim->x1 = self->posX.i.hi;
    prim->y1 = self->posY.i.hi;
}
#endif

u8 BO6_PrimDecreaseBrightness(Primitive* prim, u8 amount);

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityAguneaHitEnemy);
#else
void RicEntityAguneaHitEnemy(Entity* self) {
    Entity* parent;
    Primitive* prim;
    Primitive* temp_s3;
    s16 arctan;
    s16 angle;
    s16 xOffset;
    s16 yOffset;
    s16 temp_s2;
    u8 var_s3;
    s32 i;
    u8 var_s8;

    self->posX.i.hi = RIC.posX.i.hi;
    self->posY.i.hi = (RIC.posY.i.hi + RIC.hitboxOffY) - 8;
    parent = self->ext.et_801291C4.parent;
    if (self->ext.et_801291C4.parent->entityId != 0x2C) {
        if (self->step == 0) {
            DestroyEntity(self);
            return;
        }
        self->step = 3;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x28);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            break;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->facingLeft = RIC.facingLeft;
        self->ext.et_801291C4.unk84 = ((rand() & 0x3FF) - 0x200) & 0xFFF;
        prim = &g_PrimBuf[self->primIndex];
        self->ext.et_801291C4.prim1 = prim;
        self->ext.et_801291C4.prim2 = prim;
        self->ext.et_801291C4.unk90 = (self->params >> 8) & 0xFF;
        for (i = 0; prim != NULL;) {
            prim->tpage = 0x1A;
            prim->clut = PAL_UNK_194;
            prim->u0 = prim->u1 = i * 0x10 + 0x90;
            prim->u2 = prim->u3 = prim->u0 + 0x10;
            prim->v0 = prim->v2 = 0xD0;
            prim->v1 = prim->v3 = 0xC0;
            prim->x0 = self->posX.i.hi;
            prim->y0 = self->posY.i.hi;
            prim->x2 = self->posX.i.hi;
            prim->y2 = self->posX.i.hi;
            prim->r0 = prim->g0 = prim->b0 = 0xF0;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->drawMode = DRAW_HIDE;
            prim->priority = self->zPriority;
            prim = prim->next;
            i++;
            if (i > 5) {
                i = 0;
            }
        }
        prim = self->ext.et_801291C4.prim1;
        prim->x0 = self->posX.i.hi;
        prim->y0 = self->posY.i.hi;
        prim->x1 = prim->x0;
        prim->y1 = prim->y1 - 0x10;
        prim->x2 = self->posX.i.hi;
        prim->y2 = self->posY.i.hi;
        prim->x3 = prim->x2;
        prim->y3 = prim->y2 - 0x10;
        self->ext.et_801291C4.prim2 = prim;
        while (prim != NULL) {
            prim->r0 = prim->g0 = prim->b0 = 0x80;
            prim->clut = PAL_UNK_194;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->drawMode = DRAW_HIDE;
            prim->priority = self->zPriority;
            prim = prim->next;
        }
        self->ext.et_801291C4.unk88 = 0;
        self->step++;
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            prim = self->ext.et_801291C4.prim2;
            temp_s2 = self->ext.et_801291C4.unk84;
            xOffset = parent->posX.i.hi - prim->x2;
            yOffset = parent->posY.i.hi - prim->y2;
            if (abs(xOffset) < 8 && abs(yOffset) < 8) {
                self->step++;
                break;
            }
            if (abs(xOffset) < 0x40 && abs(yOffset) < 0x40) {
                var_s3 = 1;
            } else {
                var_s3 = 0;
            }
            if (!self->ext.et_801291C4.unk88) {
                self->ext.et_801291C4.unk88 = 4;
                if (var_s3) {
                    self->ext.et_801291C4.unk88 = 2;
                }
                arctan = ratan2(-yOffset, xOffset);
                angle = arctan - temp_s2;
                if (angle > 0x800) {
                    angle = 0x1000 - angle;
                }
                if (angle < -0x800) {
                    angle = 0x1000 + angle;
                }
                if (!var_s3) {
                    angle /= 4;
                } else {
                    angle /= 2;
                }
                self->ext.et_801291C4.unk86 = angle;
            }
            temp_s2 += self->ext.et_801291C4.unk86;
            if (!var_s3) {
                temp_s2 += 0x180 - ((rand() & 3) << 8);
            }
            temp_s2 &= 0xFFF;
            temp_s3 = prim->next;
            if (temp_s3 == NULL) {
                self->step++;
                return;
            }
            LOW(temp_s3->x0) = LOW(prim->x2);
            LOW(temp_s3->x1) = LOW(prim->x3);
            self->ext.et_801291C4.unk84 = temp_s2;
            self->ext.et_801291C4.prim2 = temp_s3;
            xOffset = (rcos(temp_s2) * 0xC) >> 0xC;
            yOffset = -((rsin(temp_s2) * 0xC) >> 0xC);
            temp_s3->x2 = temp_s3->x0 + xOffset;
            temp_s3->y2 = temp_s3->y0 + yOffset;
            angle = temp_s2 - 0x400;
            var_s8 = 0x10 - (self->params * 4);
            xOffset = (var_s8 * rcos(angle)) >> 0xC;
            yOffset = -((var_s8 * rsin(angle)) >> 0xC);
            temp_s3->drawMode = DRAW_COLORS | DRAW_UNK02;
            temp_s3->x3 = temp_s3->x2 + xOffset;
            temp_s3->y3 = temp_s3->y2 + yOffset;
            self->ext.et_801291C4.unk88--;
        }
        break;
    case 2:
        if (!self->step_s) {
            prim = self->ext.et_801291C4.prim1;
            while (prim != NULL) {
                prim->clut = PAL_FILL_WHITE;
                prim = prim->next;
            }
            self->step_s++;
            return;
        }
        prim = self->ext.et_801291C4.prim1;
        while (prim != NULL) {
            prim->r0 = prim->g0 = prim->b0 = 0x60;
            prim->clut = PAL_UNK_194;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim = prim->next;
        }
        self->step_s = 0;
        self->step++;
        break;
    case 3:
        var_s8 = 1;
        prim = self->ext.et_801291C4.prim1;
        while (prim != NULL) {
            var_s8 &= !BO6_PrimDecreaseBrightness(prim, 4);
            prim = prim->next;
        }
        if (var_s8) {
            prim = self->ext.et_801291C4.prim1;
            while (prim != NULL) {
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
            }
            DestroyEntity(self);
        }
        break;
    }
}
#endif

extern AnimationFrame D_us_801829D4[];

void RicEntityVibhutiCrashCloud(Entity* self) {
    s32 angle;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }

        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->posX.val = self->ext.vibCrashCloud.parent->ext.vibhutiCrash.x;
        self->posY.val = self->ext.vibCrashCloud.parent->ext.vibhutiCrash.y;
        self->facingLeft =
            self->ext.vibCrashCloud.parent->ext.vibhutiCrash.facing;
        self->flags |= FLAG_UNK_20000000;
        self->unk5A = 0x64;
        self->animSet = 0xE;
        self->palette = PAL_FLAG(PAL_UNK_19E);
        self->anim = D_us_801829D4;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->drawFlags = ENTITY_OPACITY;
        self->opacity = 0x60;
        self->hitboxWidth = 8;
        self->hitboxHeight = 8;

        angle = (rand() % 512) + 0x300;
        self->velocityX = rcos(angle) << 5;
        self->velocityY = -(rsin(angle) << 5);
        self->step++;
        break;

    case 1:
        self->ext.vibCrashCloud.unk7C++;
        if (self->ext.vibCrashCloud.unk7C > 38) {
            DestroyEntity(self);
        } else {
            self->posX.val += self->velocityX;
            self->posY.val += self->velocityY;
        }
        break;
    }
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityCrashVibhuti);
#else
void RicEntityCrashVibhuti(Entity* self) {
    FakePrim* prim;
    s32 angle;
    s32 magnitude;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_TILE, 9);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            g_Ric.unk4E = 1;
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        for (i = 0; i < 9; i++) {
            prim->r0 = prim->g0 = prim->b0 = 0xFF;
            prim->w = prim->h = 1;
            prim->drawMode = DRAW_HIDE | DRAW_UNK02;
            prim->priority = RIC.zPriority + 8;
            prim = prim->next;
        }
        self->step++;
        break;
    case 1:
        self->ext.vibhutiCrash.unk7E++;
        if (!(self->ext.vibhutiCrash.unk7E & 1) &&
            self->ext.vibhutiCrash.timer < 8) {
            self->ext.vibhutiCrash.timer++;
            if (++self->ext.vibhutiCrash.unk80 >= 0x30) {
                self->step++;
            }
            prim = (FakePrim*)&g_PrimBuf[self->primIndex];
            for (i = 0; i < 9; i++) {
                if (prim->drawMode & DRAW_HIDE) {
                    break;
                }
                prim = prim->next;
            }
            prim->posX.val = RIC.posX.val;
            prim->posY.val = RIC.posY.val - FIX(24);
            angle = rand() % 0x200 + 0x300;
            magnitude = (rand() % 24) + 0x20;
            prim->velocityX.val = (rcos(angle) * magnitude);
            prim->delay = 0x10;
            prim->drawMode &= ~DRAW_HIDE;
            prim->velocityY.val = -(rsin(angle) * magnitude);
        }
        // fallthrough
    case 2:
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        for (i = 0; i < 9; i++) {
            if (!(prim->drawMode & DRAW_HIDE)) {
                if (!--prim->delay) {
                    prim->drawMode |= DRAW_HIDE;
                    self->ext.vibhutiCrash.timer--;
                    self->ext.vibhutiCrash.x = prim->posX.val;
                    self->ext.vibhutiCrash.y = prim->posY.val;
                    self->ext.vibhutiCrash.facing = prim->velocityX.val <= 0;
                    RicCreateEntFactoryFromEntity(
                        self, BP_VITHUBI_CRASH_CLOUD, 0);
                } else {
                    prim->posX.val += prim->velocityX.val;
                    prim->posY.val += prim->velocityY.val;
                    prim->velocityY.val += FIX(0.25);
                    prim->x0 = prim->posX.i.hi;
                    prim->y0 = prim->posY.i.hi;
                }
            }
            prim = prim->next;
        }
        if (self->step == 2 && !self->ext.vibhutiCrash.timer) {
            self->step++;
        }
        break;
    case 3:
        g_Ric.unk4E = 1;
        DestroyEntity(self);
        break;
    }
}
#endif

// Short-lived invisible hitbox
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", func_us_801C8590);
#else
void func_us_801C8590(Entity* self) {
    switch (self->step) {
    case 0:
        self->flags = FLAG_UNK_10000000;
        self->hitboxWidth = 4;
        self->hitboxHeight = 4;
        self->step++;
        break;
    case 1:
        if (++self->ext.timer.t >= 4) {
            DestroyEntity(self);
        }
        break;
    }
}
#endif

extern s32 D_us_80182A0C[];

// same as RIC func_8016D9C4
void func_us_801C8618(Entity* self) {
    PrimLineG2* prim;
    Primitive* prim2;
    s32 i;
    long angle;
    s32 var_s6;
    s32 var_s5;
    s32 var_s7;
    s32 brightness;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 20);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = (PrimLineG2*)&g_PrimBuf[self->primIndex];
        for (i = 0; i < 4; i++) {
            prim->preciseX.val = RIC.posX.val;
            prim->preciseY.val = RIC.posY.val - FIX(40);
            prim->priority = 194;
            prim->drawMode = DRAW_HIDE;
            prim->x0 = prim->x1 = RIC.posX.i.hi;
            prim->y0 = prim->y1 = RIC.posY.i.hi - 0x1C;
            prim->r0 = prim->g0 = prim->b0 = 0x80;
            prim->r1 = prim->g1 = prim->b1 = 0x70;
            prim->angle = D_us_80182A0C[i];
            prim->delay = 1;
            prim = (PrimLineG2*)prim->next;
        }
        for (brightness = 0x80; i < 20; i++) {
            if (!(i % 4)) {
                brightness -= 0x10;
                switch (i / 4) {
                case 1:
                    self->ext.et_8016D9C4.lines[0] = prim;
                    break;
                case 2:
                    self->ext.et_8016D9C4.lines[1] = prim;
                    break;
                case 3:
                    self->ext.et_8016D9C4.lines[2] = prim;
                    break;
                case 4:
                    self->ext.et_8016D9C4.lines[3] = prim;
                    break;
                }
            }
            prim->priority = 0xC2;
            prim->drawMode = DRAW_HIDE;
            prim->x0 = prim->x1 = RIC.posX.i.hi;
            prim->y0 = prim->y1 = RIC.posY.i.hi - 0x1C;
            prim->r0 = prim->g0 = prim->b0 = brightness;
            prim->r1 = prim->g1 = prim->b1 = brightness - 0x10;
            prim = (PrimLineG2*)prim->next;
        }
        self->ext.et_8016D9C4.unk90 = 4;
        self->ext.et_8016D9C4.unk8C = self->ext.et_8016D9C4.unk8E = 0;
        self->step++;
        break;
    case 1:
        self->ext.et_8016D9C4.unk8E = 1;
        switch (self->ext.et_8016D9C4.unk8C) {
        case 0:
            prim = (PrimLineG2*)&g_PrimBuf[self->primIndex];
            break;
        case 1:
            prim = self->ext.et_8016D9C4.lines[0];
            break;
        case 2:
            prim = self->ext.et_8016D9C4.lines[1];
            break;
        case 3:
            prim = self->ext.et_8016D9C4.lines[2];
            break;
        case 4:
            prim = self->ext.et_8016D9C4.lines[3];
            break;
        }
        for (i = 0; i < 4; i++) {
            prim->drawMode &= ~DRAW_HIDE;
            prim = (PrimLineG2*)prim->next;
        }
        self->ext.et_8016D9C4.unk8C++;
        if (self->ext.et_8016D9C4.unk8C > 4) {
            self->step++;
        }
        break;
    case 2:
        if (!self->ext.et_8016D9C4.unk90) {
            self->step++;
            break;
        }
        break;
    case 3:
        self->ext.et_8016D9C4.unk90++;
        if (self->ext.et_8016D9C4.unk90 > 4) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    if (!self->ext.et_8016D9C4.unk8E) {
        return;
    }
    prim = (PrimLineG2*)&g_PrimBuf[self->primIndex];
    for (i = 0; i < 4; i++) {
        if (prim->delay) {
            prim->x1 = prim->x0;
            prim->y1 = prim->y0;
            prim->x0 = prim->preciseX.i.hi;
            prim->y0 = prim->preciseY.i.hi;
            var_s7 = ratan2(prim->preciseY.val, FIX(128) - prim->preciseX.val) &
                     0xFFF;
            angle = prim->angle - var_s7;
            if (labs(angle) > 0x800) {
                if (angle < 0) {
                    angle += 0x1000;
                } else {
                    angle -= 0x1000;
                }
            }
            if (angle >= 0) {
                if (angle > 0x80) {
                    var_s6 = 0x80;
                } else {
                    var_s6 = angle;
                }
                angle = var_s6;
            } else {
                if (angle < -0x80) {
                    var_s5 = -0x80;
                } else {
                    var_s5 = angle;
                }
                angle = var_s5;
            }
            prim->angle = prim->angle - angle;
            prim->angle &= 0xFFF;
            prim->velocityX.val = (rcos(prim->angle) << 4 << 4);
            prim->velocityY.val = -(rsin(prim->angle) << 4 << 4);
            prim->preciseX.val += prim->velocityX.val;
            prim->preciseY.val += prim->velocityY.val;
            self->posX.i.hi = prim->preciseX.i.hi;
            self->posY.i.hi = prim->preciseY.i.hi;
            RicCreateEntFactoryFromEntity(
                self, BP_CRASH_REBOUND_STONE_PARTICLES, 0);
            if (prim->preciseY.val < 0) {
                prim->delay = 0;
                prim->drawMode |= DRAW_HIDE;
                self->ext.et_8016D9C4.unk90--;
            }
        }
        prim = (PrimLineG2*)prim->next;
    }
    prim = self->ext.et_8016D9C4.lines[0];
    prim2 = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 16; i++) {
        prim->x1 = prim->x0;
        prim->y1 = prim->y0;
        prim->x0 = prim2->x1;
        prim->y0 = prim2->y1;
        prim = (PrimLineG2*)prim->next;
        prim2 = prim2->next;
    }
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityCrashReboundStoneExplosion);
#else
void RicEntityCrashReboundStoneExplosion(Entity* self) {
    Primitive* prim;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_G4, 0x10);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 0x10; i++) {
            prim->priority = 0xC2;
            prim->drawMode = DRAW_DITHERING | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_COLORS | DRAW_TRANSP;
            prim->x0 = prim->x1 = 0x80;
            prim->y0 = prim->y1 = 0;
            prim = prim->next;
        }
        self->ext.reboundStoneCrashExplosion.unk7C = 0x40;
        self->ext.reboundStoneCrashExplosion.unk80 = 0x10;
        self->ext.reboundStoneCrashExplosion.unk7E = 0;
        self->ext.reboundStoneCrashExplosion.unk84 = 0;
        self->ext.reboundStoneCrashExplosion.unk82 = 8;
        self->step++;
        break;
    case 1:
        self->ext.reboundStoneCrashExplosion.unk84 += 0x20;
        if (self->ext.reboundStoneCrashExplosion.unk84 > 0x120) {
            self->posX.val = FIX(128.0);
            self->posY.val = FIX(128.0);
            self->hitboxWidth = 0x80;
            self->hitboxHeight = 0x80;
            self->step++;
        }
        break;
    case 2:
        self->ext.reboundStoneCrashExplosion.unk86++;
        if (self->ext.reboundStoneCrashExplosion.unk86 == 5) {
            self->ext.reboundStoneCrashExplosion.unk80 = -0x18;
        } else if (self->ext.reboundStoneCrashExplosion.unk86 >= 0xF) {
            self->ext.reboundStoneCrashExplosion.unk82 = -0x18;
            self->step++;
        }
        break;
    case 3:
        break;
    case 4:
        DestroyEntity(self);
        return;
    }
    self->ext.reboundStoneCrashExplosion.unk7C +=
        self->ext.reboundStoneCrashExplosion.unk80;
    if (self->ext.reboundStoneCrashExplosion.unk7C > 0xFF) {
        self->ext.reboundStoneCrashExplosion.unk7C = 0xFF;
        self->ext.reboundStoneCrashExplosion.unk80 = 0;
    } else if (self->ext.reboundStoneCrashExplosion.unk7C < 0) {
        self->ext.reboundStoneCrashExplosion.unk7C =
            self->ext.reboundStoneCrashExplosion.unk80 = 0;
    }
    self->ext.reboundStoneCrashExplosion.unk7E +=
        self->ext.reboundStoneCrashExplosion.unk82;
    if (self->ext.reboundStoneCrashExplosion.unk7E > 0xFF) {
        self->ext.reboundStoneCrashExplosion.unk7E = 0xFF;
        self->ext.reboundStoneCrashExplosion.unk82 = 0;
    } else if (self->ext.reboundStoneCrashExplosion.unk7E < 0) {
        self->ext.reboundStoneCrashExplosion.unk7E =
            self->ext.reboundStoneCrashExplosion.unk82 = 0;
        self->step += 1;
    }
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 16; i++) {
        prim->b0 = prim->b1 = self->ext.reboundStoneCrashExplosion.unk7C;
        prim->b2 = prim->b3 = self->ext.reboundStoneCrashExplosion.unk7E;
        prim->r0 = prim->r1 = prim->g0 = prim->g1 =
            self->ext.reboundStoneCrashExplosion.unk7C;
        prim->r2 = prim->r3 = prim->g2 = prim->g3 =
            self->ext.reboundStoneCrashExplosion.unk7E;
        if (self->step < 2) {
            prim->x2 =
                ((rcos(i << 7) * self->ext.reboundStoneCrashExplosion.unk84) >>
                 0xC) +
                0x80;
            prim->x3 = ((rcos((i + 1) << 7) *
                         self->ext.reboundStoneCrashExplosion.unk84) >>
                        0xC) +
                       0x80;
            prim->y2 =
                ((rsin(i << 7) * self->ext.reboundStoneCrashExplosion.unk84) >>
                 0xC);
            prim->y3 = ((rsin((i + 1) << 7) *
                         self->ext.reboundStoneCrashExplosion.unk84) >>
                        0xC);
        }
        prim = prim->next;
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityCrashReboundStone);
#else
void RicEntityCrashReboundStone(Entity* entity) {
    switch (entity->step) {
    case 0:
        entity->flags = FLAG_UNK_10000000;
        entity->ext.timer.t = 0x14;
        entity->step++;
        // fallthrough
    case 1:
        if (--entity->ext.timer.t) {
            break;
        }
    case 3:
    case 5:
        RicCreateEntFactoryFromEntity(entity, BP_57, 0);
        entity->step++;
    case 2:
    case 4:
    case 6:
        entity->ext.timer.t++;
        if (entity->ext.timer.t > 10) {
            entity->ext.timer.t = 0;
            entity->posX.val = FIX(128.0);
            entity->posY.val = 0;
            RicCreateEntFactoryFromEntity(entity, FACTORY(BP_EMBERS, 1), 0);
            entity->step++;
        }
        break;
    case 7:
        entity->ext.timer.t++;
        if (entity->ext.timer.t > 15) {
            DestroyEntity(entity);
            g_Ric.unk4E = 1;
            RicCreateEntFactoryFromEntity(
                entity, BP_CRASH_REBOUND_STONE_EXPLOSION, 0);
        }
        break;
    }
}
#endif

extern EInit D_us_801804CC;
extern Point16 D_us_801D10D0[6]; // bible pages position
extern u16 D_us_801D10E8;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityCrashBibleBeam);
#else
void RicEntityCrashBibleBeam(Entity* self) {
    Primitive* prim;
    s32 i;
    s32 var_s3;
    s32 psp_s3;
    s16 var_s7;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804CC);
        D_us_801D10E8 = self->hitboxState;
        self->ext.bibleBeam.subweaponId = PL_W_BIBLE_BEAM;
        self->hitboxState = 0;
        self->primIndex = g_api.AllocPrimitives(PRIM_G4, 6);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        if (self->facingLeft) {
            self->ext.bibleBeam.unk7C = -16;
            self->ext.bibleBeam.unk7E = -2;
        } else {
            self->ext.bibleBeam.unk7C = 16;
            self->ext.bibleBeam.unk7E = 2;
        }
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 6; i++) {
            var_s3 = i + 2;
            if (var_s3 >= 6) {
                var_s3 -= 6;
            }
            prim->x0 = prim->x1 = D_us_801D10D0[i].x;
            prim->y0 = prim->y1 = D_us_801D10D0[i].y;
            prim->x2 = prim->x3 = D_us_801D10D0[var_s3].x;
            prim->y2 = prim->y3 = D_us_801D10D0[var_s3].y;
            prim->priority = 0xC2;
            prim->drawMode = DRAW_DITHERING | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_COLORS | DRAW_TRANSP;
            prim = prim->next;
        }
        self->step = 1;
        break;
    case 1:
        self->ext.bibleBeam.unk80++;
        if (self->ext.bibleBeam.unk80 >= 0x3C) {
            g_api.PlaySfx(SFX_WEAPON_APPEAR);
            g_api.PlaySfx(SFX_TELEPORT_BANG_A);
            self->hitboxState = D_us_801D10E8;
            self->step++;
        }
        break;
    case 2:
        self->ext.bibleBeam.unk7E += self->ext.bibleBeam.unk7C;
        self->ext.bibleBeam.unk80++;
        var_s3 = D_us_801D10D0[1].x + self->ext.bibleBeam.unk7E;
        if (var_s3 < -0x50 || var_s3 > 0x150) {
            self->step++;
        }
        break;
    case 3:
        self->ext.bibleBeam.unk80++;
        if (self->ext.bibleBeam.unk80 >= 0x78) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    prim = &g_PrimBuf[self->primIndex];
    var_s7 = 0;
    for (i = 0; i < 6; i++) {
        var_s3 = i + 2;
        if (var_s3 >= 6) {
            var_s3 -= 6;
        }
        psp_s3 = i * 256;
        prim->r0 = prim->r1 =
            abs((rsin((self->ext.bibleBeam.unk80 * 20) + psp_s3) * 96) >> 0xc);
        prim->g0 = prim->g1 =
            abs((rsin((self->ext.bibleBeam.unk80 * 15) + psp_s3) * 96) >> 0xc);
        prim->b0 = prim->b1 =
            abs((rsin((self->ext.bibleBeam.unk80 * 10) + psp_s3) * 96) >> 0xc);
        psp_s3 = var_s3 * 256;
        prim->r2 = prim->r3 =
            abs((rsin((self->ext.bibleBeam.unk80 * 15) + psp_s3) * 96) >> 0xc);
        prim->g2 = prim->g3 =
            abs((rsin((self->ext.bibleBeam.unk80 * 10) + psp_s3) * 96) >> 0xc);
        prim->b2 = prim->b3 =
            abs((rsin((self->ext.bibleBeam.unk80 * 20) + psp_s3) * 96) >> 0xc);
        prim->x1 = D_us_801D10D0[i].x;
        prim->y0 = prim->y1 = D_us_801D10D0[i].y;
        prim->x3 = D_us_801D10D0[var_s3].x;
        prim->y2 = prim->y3 = D_us_801D10D0[var_s3].y;
        prim->x0 = D_us_801D10D0[i].x + self->ext.bibleBeam.unk7E;
        prim->x2 = D_us_801D10D0[var_s3].x + self->ext.bibleBeam.unk7E;
        if (var_s7 < abs(D_us_801D10D0[i].y)) {
            var_s7 = abs(D_us_801D10D0[i].y);
        }
        prim = prim->next;
    }
    self->hitboxOffX = self->facingLeft ? -(self->ext.bibleBeam.unk7E / 2)
                                        : (self->ext.bibleBeam.unk7E / 2);
    self->hitboxWidth = abs(self->hitboxOffX);
    self->hitboxHeight = var_s7 - self->posY.i.hi;
}
#endif

extern s16 D_us_80182A1C[6]; // bible page angles
extern s32 D_us_801D10EC;    // bible pages volume

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityCrashBible);
#else
void RicEntityCrashBible(Entity* self) {
    Primitive* prim;
    s32 i;
    s32 psp_s2;
    s16 psp_s4;
    s16 psp_s3;
    s32 psp_s6;
    s32 psp_s5;
    s32 sp48;
    s32 sp40;
    s32 sp44;
    s32 sp4C;
    s32 psp_s8;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1 + 6);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            g_Ric.unk4E = 1;
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->posX.val = RIC.posX.val;
        self->velocityY = FIX(-4);
        self->posY.val = RIC.posY.val;
        if (self->facingLeft) {
            self->velocityX = FIX(12);
            self->ext.et_8016E9E4.unk88 = FIX(1.5);
            self->ext.et_8016E9E4.unk7E = -0x200;
        } else {
            self->velocityX = FIX(-12);
            self->ext.et_8016E9E4.unk88 = FIX(-1.5);
            self->ext.et_8016E9E4.unk7E = 0x200;
        }
        prim = &g_PrimBuf[self->primIndex];
        prim->tpage = 0x1E;
        prim->clut = 0x17F;
        prim->u0 = prim->u2 = 0x98;
        prim->v0 = prim->v1 = 0xD8;
        prim->u1 = prim->u3 = 0xA8;
        prim->v2 = prim->v3 = 0xF0;
        prim->priority = 0xC2;
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
        for (i = 0; i < 6; i++) {
            prim->tpage = 0x1C;
            prim->clut = PAL_UNK_1AE;
            prim->u0 = prim->u2 = 0x20;
            prim->u1 = prim->u3 = 0x30;
            prim->v0 = prim->v1 = 0;
            prim->v2 = prim->v3 = 0x10;
            prim->priority = 0xC1;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        D_us_801D10EC = 0x60;
        self->ext.et_8016E9E4.unk84 = 0x40;
        self->step++;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        prim->drawMode &= ~DRAW_HIDE;
        self->step++;
    case 2:
        self->posX.val += self->velocityX;
        self->velocityX -= self->ext.et_8016E9E4.unk88;
        self->posY.val += self->velocityY;
        self->velocityY -= FIX(0.5);
        if (++self->ext.et_8016E9E4.unk82 >= 8) {
            self->ext.et_8016E9E4.unk82 = 0;
            self->step++;
        }
        break;
    case 3:
        if (++self->ext.et_8016E9E4.unk82 > 5) {
            prim = &g_PrimBuf[self->primIndex];
            prim->clut = PAL_UNK_19F;
            prim->drawMode |=
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            prim = prim->next;
            for (i = 0; i < 6; i++) {
                prim->drawMode &= ~DRAW_HIDE;
                prim = prim->next;
            }
            self->ext.et_8016E9E4.unk82 = 0;
            self->step++;
        }
        break;
    case 4:
        self->ext.et_8016E9E4.unk84 -= 4;
        if (self->ext.et_8016E9E4.unk84 <= 0) {
            self->step++;
        }
        break;
    case 5:
        prim = &g_PrimBuf[self->primIndex];
        prim->drawMode |= DRAW_HIDE;
        self->step++;
    case 6:
        self->ext.et_8016E9E4.unk80 += 4;
        self->ext.et_8016E9E4.unk7C = (self->ext.et_8016E9E4.unk7C + 0x80) & 0xFFF;
        if (self->ext.et_8016E9E4.unk80 >= 0x30) {
            self->step++;
        }
        break;
    case 7:
        self->ext.et_8016E9E4.unk82++;
        self->ext.et_8016E9E4.unk7C = (self->ext.et_8016E9E4.unk7C + 0x80) & 0xFFF;
        if (self->ext.et_8016E9E4.unk82 >= 0x1E) {
            RicCreateEntFactoryFromEntity(self, BP_CRASH_BIBLE_BEAM, 0);
            self->ext.et_8016E9E4.unk82 = 0;
            self->step++;
        }
        break;
    case 8:
        self->ext.et_8016E9E4.unk82++;
        self->ext.et_8016E9E4.unk7C = (self->ext.et_8016E9E4.unk7C + 0x80) & 0xFFF;
        if (self->ext.et_8016E9E4.unk82 >= 0x60) {
            g_Ric.unk4E = 1;
            self->step++;
        }
        break;
    case 9:
        self->ext.et_8016E9E4.unk80 -= 2;
        self->ext.et_8016E9E4.unk7C = (self->ext.et_8016E9E4.unk7C + 0x80) & 0xFFF;
        if (self->ext.et_8016E9E4.unk80 <= 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    if (self->ext.et_8016E9E4.unk7C == 0x100 ||
        self->ext.et_8016E9E4.unk7C == 0x500 ||
        self->ext.et_8016E9E4.unk7C == 0x900 ||
        self->ext.et_8016E9E4.unk7C == 0xD00) {
        if (self->step < 9) {
            g_api.PlaySfxVolPan(SFX_ARROW_SHOT_A, D_us_801D10EC, 0);
            if (self->step >= 5) {
                D_us_801D10EC -= 4;
            }
            if (D_us_801D10EC < 0) {
                D_us_801D10EC = 0;
            }
        }
    }

    prim = &g_PrimBuf[self->primIndex];
    psp_s4 = self->posX.i.hi;
    psp_s3 = self->posY.i.hi;
    prim->x0 = prim->x2 = psp_s4 - 8;
    prim->x1 = prim->x3 = psp_s4 + 8;
    prim->y0 = prim->y1 = psp_s3 - 12;
    prim->y2 = prim->y3 = psp_s3 + 12;
    prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 = prim->b1 = prim->r2 =
        prim->g2 = prim->b2 = prim->r3 = prim->g3 = prim->b3 =
            self->ext.et_8016E9E4.unk84;
    prim = prim->next;

    sp44 = rsin(self->ext.et_8016E9E4.unk7E);
    sp4C = rcos(self->ext.et_8016E9E4.unk7E);
    for (i = 0; i < 6; i++) {
        sp48 = (rsin(self->ext.et_8016E9E4.unk7C + D_us_80182A1C[i]) *
                self->ext.et_8016E9E4.unk80) >>
               0xC;
        sp40 = (rcos(self->ext.et_8016E9E4.unk7C + D_us_80182A1C[i]) *
                self->ext.et_8016E9E4.unk80) >>
               0xC;
        psp_s6 = (sp44 * sp40) >> 0xC;
        psp_s2 = ((sp4C * sp40) >> 0xC) + 0x200;
        psp_s5 = sp48;
        psp_s6 = (psp_s6 << 9) / psp_s2;
        psp_s5 = (psp_s5 << 9) / psp_s2;
        psp_s4 = self->posX.i.hi + psp_s6;
        psp_s3 = self->posY.i.hi + psp_s5;
        D_us_801D10D0[i].x = psp_s4;
        D_us_801D10D0[i].y = psp_s3;
        psp_s8 = 0x1000 / psp_s2;
        prim->x0 = prim->x2 = psp_s4 - psp_s8;
        prim->x1 = prim->x3 = psp_s4 + psp_s8;
        prim->y0 = prim->y1 = psp_s3 - psp_s8;
        prim->y2 = prim->y3 = psp_s3 + psp_s8;
        prim = prim->next;
    }
}
#endif

// Equivalent of func_8016F198 in RIC
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", func_us_801C9DE8);
#else
void func_us_801C9DE8(Entity* self) {
    const int PrimCount = 16;
    const int MaxHeight = 240;
    const int HalfHeight = 120;
    Primitive* prim;
    s32 i;
    s32 tpage;
    s32 angleX;
    s32 angleY;
    s16 startMod;
    s16 endMod;
    s16 sx;
    s16 ex;
    s16 sy;
    s16 ey;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, PrimCount);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            g_Ric.unk4E = 1;
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 16; i++) {
            prim->priority = 0xC2;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        self->step++;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 16; i++) {
            prim->drawMode &= ~DRAW_HIDE;
            prim = prim->next;
        }
        self->step++;
    case 2:
        self->ext.factory.unk7C++;
        if (self->ext.factory.unk7C >= 0x18) {
            self->step++;
        }
        break;
    case 3:
        g_Ric.unk4E = 1;
        DestroyEntity(self);
        return;
    }
    if (!self->ext.factory.unk7C) {
        return;
    }
    if (g_CurrentBuffer == g_GpuBuffers) {
        tpage = 0x104;
    } else {
        tpage = 0x100;
    }
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < PrimCount; i++) {
        angleX = rsin(i << 8);
        angleY = rcos(i << 8);
        startMod = self->ext.factory.unk7C * 8;
        endMod = 0;
        if (self->ext.factory.unk7C >= 4) {
            endMod = (self->ext.factory.unk7C - 4) * 8;
        }
        sx = ((angleY * startMod) >> 0xC) + 0x80;
        ex = ((angleY * endMod) >> 0xC) + 0x80;
        sy = ((angleX * startMod) >> 0xC) + HalfHeight;
        ey = ((angleX * endMod) >> 0xC) + HalfHeight;
        prim->x0 = sx = CLAMP(sx, 0, 0xFF);
        prim->x2 = ex = CLAMP(ex, 0, 0xFF);
        prim->y0 = sy = CLAMP(sy, 0, MaxHeight);
        prim->y2 = ey = CLAMP(ey, 0, MaxHeight);
        prim->u0 = 0xFF - prim->x0;
        prim->u2 = 0xFF - prim->x2;
        prim->v0 = MaxHeight - prim->y0;
        prim->v2 = MaxHeight - prim->y2;

        angleX = rsin((i + 1) << 8);
        angleY = rcos((i + 1) << 8);
        sx = ((angleY * startMod) >> 0xC) + 0x80;
        ex = ((angleY * endMod) >> 0xC) + 0x80;
        sy = ((angleX * startMod) >> 0xC) + HalfHeight;
        ey = ((angleX * endMod) >> 0xC) + HalfHeight;
        prim->x1 = sx = CLAMP(sx, 0, 0xFF);
        prim->x3 = ex = CLAMP(ex, 0, 0xFF);
        prim->y1 = sy = CLAMP(sy, 0, MaxHeight);
        prim->y3 = ey = CLAMP(ey, 0, MaxHeight);
        prim->u1 = 0xFF - prim->x1;
        prim->u3 = 0xFF - prim->x3;
        prim->v1 = MaxHeight - prim->y1;
        prim->v3 = MaxHeight - prim->y3;
        prim->tpage = tpage;
        prim = prim->next;
    }
}
#endif

void func_us_801CA340(Entity* self) {
    RicCreateEntFactoryFromEntity(self, FACTORY(0x3F, 1), 0);
    DestroyEntity(self);
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", GetAguneaLightningAngle);
#else
static s16 GetAguneaLightningAngle(s16* arg0, s16 arg1, s16 arg2, s16* arg3) {
    arg1 += rand() % 256 - 0x80;
    *arg3 = (rand() % 48) + 0x10;
    arg0[0] = arg0[1];
    arg0[2] = arg0[3];
    if (arg2) {
        arg0[1] += (rcos(arg1) * *arg3) >> 0xC;
        arg0[3] += (rsin(arg1) * *arg3) >> 0xC;
        if (arg2 % 2) {
            return GetAguneaLightningAngle(arg0, arg1 - 0x140, arg2 / 2, arg3);
        } else {
            rand();
            rand();
            return GetAguneaLightningAngle(
                arg0, arg1 + 0x140, (arg2 - 1) / 2, arg3);
        }
    }
    return arg1;
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", AguneaShuffleParams);
#else
static void AguneaShuffleParams(s32 bufSize, s32* buf) {
    s32 i, idx, swapTemp;

    for (i = bufSize - 1; i > 0; i--) {
        idx = rand() % bufSize;
        swapTemp = buf[i];
        buf[i] = buf[idx];
        buf[idx] = swapTemp;
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityAguneaLightning);
#else
void RicEntityAguneaLightning(Entity* self) {
    s16 sp20;
    s16 angle;
    s16 sp18;
    s32 randomSeed;
    s16 sp10[4];
    s16 xCoord;
    s16 yCoord;
    s16 psp_s6;
    s16 psp_s5;
    s32 psp_s4;
    s32 psp_s3;
    s32 i;
    Primitive* prim;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0xF);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->tpage = 0x1A;
            prim->clut = PAL_UNK_194;
            xCoord = (rand() % 5) * 0x10;
            prim->u0 = prim->u2 = xCoord + 0x90;
            prim->u1 = prim->u3 = xCoord + 0xB0;
            if (rand() % 2) {
                prim->v0 = prim->v1 = 0xD0;
                prim->v2 = prim->v3 = 0xE0;
            } else {
                prim->v0 = prim->v1 = 0xE0;
                prim->v2 = prim->v3 = 0xD0;
            }
            prim->priority = 0xC1;
            prim->drawMode = DRAW_HIDE | DRAW_COLORS | DRAW_UNK02;
            prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 = prim->b1 =
                prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 =
                    prim->b3 = 0x80;
            prim = prim->next;
        }
        prim = &g_PrimBuf[self->primIndex];
        sp20 = ((self->params & 0xFF00) >> 8) * 0x200;
        sp20 += rand() % 0x200 - 0x100;
        randomSeed = rand();
        for (i = 0; i < 15; i++) {
            srand(randomSeed);
            sp10[1] = self->posX.i.hi;
            sp10[3] = self->posY.i.hi;
            angle = GetAguneaLightningAngle(sp10, sp20, i, &sp18);
            xCoord = sp10[0];
            yCoord = sp10[2];
            psp_s6 = !i ? 2 : 8;
            psp_s5 = (i < 7) ? 8 : 2;
            psp_s4 = rcos(angle);
            psp_s3 = rsin(angle);
            prim->x0 = xCoord + (-(psp_s3 * -psp_s6) >> 0xC);
            prim->y0 = yCoord + ((psp_s4 * -psp_s6) >> 0xC);
            prim->x1 = xCoord + ((psp_s4 * sp18 - (psp_s3 * -psp_s5)) >> 0xC);
            prim->y1 = yCoord + ((psp_s3 * sp18 + (psp_s4 * -psp_s5)) >> 0xC);
            prim->x2 = xCoord + (-(psp_s3 * psp_s6) >> 0xC);
            prim->y2 = yCoord + ((psp_s4 * psp_s6) >> 0xC);
            prim->x3 = xCoord + ((psp_s4 * sp18 - (psp_s3 * psp_s5)) >> 0xC);
            prim->y3 = yCoord + ((psp_s3 * sp18 + (psp_s4 * psp_s5)) >> 0xC);
            prim = prim->next;
        }
        self->ext.et_8017091C.unk7E = 1;
        self->step++;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->drawMode &= ~DRAW_HIDE;
            prim = prim->next;
        }
        self->step++;
    case 2:
        self->ext.et_8017091C.unk7C++;
        if (self->ext.et_8017091C.unk7C > 4) {
            prim = &g_PrimBuf[self->primIndex];
            for (i = 0; i < 15; i++) {
                prim->clut = PAL_FILL_WHITE;
                prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 =
                    prim->b1 = prim->r2 = prim->g2 = prim->b2 = prim->r3 =
                        prim->g3 = prim->b3 = 0xFF;
                prim->v0 = prim->v1 = prim->v0 - 0x10;
                prim->v2 = prim->v3 = prim->v2 - 0x10;
                prim = prim->next;
            }
            self->ext.et_8017091C.unk7C = 0;
            self->step++;
        }
        break;
    case 3:
    case 5:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->clut = PAL_UNK_194;
            prim = prim->next;
        }
        self->step++;
        break;
    case 4:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->clut = PAL_FILL_WHITE;
            prim = prim->next;
        }
        self->step++;
        break;
    case 6:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 = prim->b1 =
                prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 =
                    prim->b3 = 0x60 - (self->ext.et_8017091C.unk7C * 4);
            prim = prim->next;
        }
        self->ext.et_8017091C.unk7C++;
        if (self->ext.et_8017091C.unk7C > 15) {
            DestroyEntity(self);
            return;
        }
        break;
    }
}
#endif

extern EInit D_us_801804C0;
extern s32 D_us_801D10F0[8]; // agunea crash lightning params

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityAguneaCircle);
#else
void RicEntityAguneaCircle(Entity* self) {
    Primitive* prim;
    s16 rand_angle;
    s16 xCoord;
    s16 yCoord;
    s32 quarterSelfY;
    s32 i;
    s16 psp_s4;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 4);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            g_Ric.unk4E = 1;
            return;
        }
        InitializeEntity(D_us_801804C0);
        self->ext.aguneaCrash.subweaponId = PL_W_CRASH_AGUNEA;
        prim = &g_PrimBuf[self->primIndex];
        self->posY.i.hi -= 0x20;
        self->flags |= FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        quarterSelfY = self->posY.i.hi / 4;
        xCoord = self->posX.i.hi;
        yCoord = 0;
        for (i = 0; i < 4; i++) {
            prim->tpage = 0x1A;
            prim->clut = PAL_UNK_194;
            prim->x0 = xCoord - 8;
            prim->x2 = xCoord + 8;
            prim->y0 = prim->y2 = yCoord;
            if (i < 3) {
                rand_angle = rand() % 0x100 + 0x380;
                xCoord += ((rcos(rand_angle) * quarterSelfY) >> 0xC);
                prim->x1 = xCoord - 8;
                prim->x3 = xCoord + 8;
                yCoord += ((rsin(rand_angle) * quarterSelfY) >> 0xC);
            } else {
                xCoord = self->posX.i.hi;
                yCoord = self->posY.i.hi;
                prim->x1 = xCoord - 4;
                prim->x3 = xCoord + 4;
            }
            prim->y1 = prim->y3 = yCoord;
            psp_s4 = (rand() % 6) * 0x10;
            prim->u0 = prim->u2 = psp_s4 + 0x90;
            prim->u1 = prim->u3 = psp_s4 + 0xA0;
            if (rand() % 2) {
                prim->v0 = prim->v1 = 0xD0;
                prim->v2 = prim->v3 = 0xE0;
            } else {
                prim->v0 = prim->v1 = 0xE0;
                prim->v2 = prim->v3 = 0xD0;
            }
            prim->priority = 0xC2;
            prim->drawMode = DRAW_HIDE | DRAW_UNK02;
            prim = prim->next;
        }
        self->step = 1;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < self->ext.aguneaCrash.unk7C; i++) {
            prim = prim->next;
        }
        prim->drawMode &= ~DRAW_HIDE;
        self->ext.aguneaCrash.unk7C++;
        if (self->ext.aguneaCrash.unk7C >= 4) {
            self->ext.aguneaCrash.unk7C = 0;
            self->step++;
        }
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < self->ext.aguneaCrash.unk7C; i++) {
            prim = prim->next;
        }
        if (!self->ext.aguneaCrash.unk7C) {
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim->tpage = 0x1A;
            prim->clut = PAL_UNK_19F;
            prim->u0 = prim->u2 = 0;
            prim->u1 = prim->u3 = 0x3F;
            prim->v0 = prim->v1 = 0xC0;
            prim->v2 = prim->v3 = 0xFF;
            prim->r0 = prim->g0 = prim->r1 = prim->g1 = prim->r2 = prim->g2 =
                prim->r3 = prim->g3 = 0x7F;
            prim->b0 = prim->b1 = prim->b2 = prim->b3 = 0xFF;
            self->ext.aguneaCrash.unk7E = 1;
            self->ext.aguneaCrash.unk80 = 2;
            self->ext.aguneaCrash.unk82 = 0x64;
        } else {
            self->ext.aguneaCrash.unk80 += 2;
            prim->drawMode |= DRAW_HIDE;
        }
        self->ext.aguneaCrash.unk7C++;
        if (self->ext.aguneaCrash.unk7C > 3) {
            for (i = 7; i >= 0; i--) {
                D_us_801D10F0[i] = i;
            }
            AguneaShuffleParams(8, D_us_801D10F0);
            self->ext.aguneaCrash.unk7C = 0;
            g_api.PlaySfx(SFX_THUNDER_B);
            self->step++;
        }
        break;
    case 3:
        RicCreateEntFactoryFromEntity(
            self,
            FACTORY(BP_CRASH_AGUNEA_THUNDER,
                    D_us_801D10F0[self->ext.aguneaCrash.unk7C]),
            0);
        self->ext.aguneaCrash.unk7C++;
        if (self->ext.aguneaCrash.unk7C > 7) {
            self->hitboxWidth = 0x80;
            self->hitboxHeight = 0x80;
            self->ext.aguneaCrash.unk7C = 0;
            self->step++;
        }
        break;
    case 4:
        self->ext.aguneaCrash.unk7C++;
        if (self->ext.aguneaCrash.unk7C > 8) {
            self->step++;
        }
        break;
    case 5:
        self->ext.aguneaCrash.unk82 -= 10;
        self->ext.aguneaCrash.unk80 += 2;
        if (self->ext.aguneaCrash.unk82 <= 0) {
            self->hitboxWidth = 0;
            self->hitboxHeight = 0;
            self->step++;
        }
        break;
    case 6:
        g_Ric.unk4E = 1;
        DestroyEntity(self);
        return;
    }
    if (self->ext.aguneaCrash.unk7E) {
        prim = &g_PrimBuf[self->primIndex];
        prim->x0 = prim->x2 = self->posX.i.hi - self->ext.aguneaCrash.unk80;
        prim->x1 = prim->x3 = self->posX.i.hi + self->ext.aguneaCrash.unk80;
        prim->y0 = prim->y1 = self->posY.i.hi - self->ext.aguneaCrash.unk80;
        prim->y2 = prim->y3 = self->posY.i.hi + self->ext.aguneaCrash.unk80;
        prim->r0 = prim->g0 = prim->r1 = prim->g1 = prim->r2 = prim->g2 =
            prim->r3 = prim->g3 = (self->ext.aguneaCrash.unk82 * 0x7F) / 100;
        prim->b0 = prim->b1 = prim->b2 = prim->b3 =
            (self->ext.aguneaCrash.unk82 * 0xFF) / 100;
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnStopwatchCircle);
#else
void RicEntitySubwpnStopwatchCircle(Entity* self) {
    s16 temp_s0_4;
    s16 psp_s4;
    s32 sine;
    s32 cosine;
    s16 xCoord;
    s16 yCoord;
    Primitive* prim;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x10);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 16; i++) {
            prim->tpage = 0x1A;
            prim->clut = PAL_FILL_WHITE;
            prim->priority = self->zPriority = 0xC2;
            prim->drawMode = DRAW_DITHERING | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_COLORS | DRAW_TRANSP;
            psp_s4 = i * 0x100;
            prim->u0 = ((rsin(psp_s4) << 5) >> 0xC) + 0x20;
            prim->v0 = -((rcos(psp_s4) << 5) >> 0xC) + 0xDF;
            psp_s4 = (i + 1) * 0x100;
            prim->u1 = ((rsin(psp_s4) << 5) >> 0xC) + 0x20;
            prim->v1 = -((rcos(psp_s4) << 5) >> 0xC) + 0xDF;
            prim->u2 = prim->u3 = 0x20;
            prim->v2 = prim->v3 = 0xE0;
            prim->r0 = prim->r1 = prim->g0 = prim->g1 = prim->b0 = prim->b1 =
                0x40;
            prim->r2 = prim->r3 = prim->g2 = prim->g3 = 0;
            prim->b2 = prim->b3 = 0x20;
            prim = prim->next;
        }
        self->ext.et_stopwatchCircle.size = 0x20;
        self->step++;
        break;
    case 1:
        self->ext.et_stopwatchCircle.size += 0x18;
        self->ext.et_stopwatchCircle.timer++;
        if (self->ext.et_stopwatchCircle.timer > 0x1E) {
            DestroyEntity(self);
            return;
        }
    }
    xCoord = self->posX.i.hi;
    yCoord = self->posY.i.hi;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 16; i++) {
        sine = rsin(i * 0x100);
        cosine = rcos(i * 0x100);
        temp_s0_4 = self->ext.et_stopwatchCircle.size - 0x20;
        prim->x0 = xCoord + ((sine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->y0 =
            yCoord - ((cosine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->x2 = xCoord + ((sine * (temp_s0_4)) >> 0xC);
        prim->y2 = yCoord - ((cosine * temp_s0_4) >> 0xC);
        sine = rsin((i + 1) * 0x100);
        cosine = rcos((i + 1) * 0x100);
        prim->x1 = xCoord + ((sine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->y1 =
            yCoord - ((cosine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->x3 = xCoord + ((sine * temp_s0_4) >> 0xC);
        prim->y3 = yCoord - ((cosine * temp_s0_4) >> 0xC);
        prim = prim->next;
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", EntityStopWatch);
#else
void EntityStopWatch(Entity* self) {
    s32 psp_s6;
    s32 psp_s5;
    s16 psp_s4;
    s16 psp_s3;
    s32 psp_s2;
    s32 psp_s1;
    Primitive* prim;

    if (g_unkGraphicsStruct.pauseEnemies) {
        g_unkGraphicsStruct.D_800973FC = 0;
        if (self->step && self->step < 4) {
            self->step = 4;
        }
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 3);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        prim->tpage = 0x1E;
        prim->clut = 0x17F;
        prim->v0 = prim->v1 = 0xC0;
        prim->v2 = prim->v3 = 0xD8;
        if (self->facingLeft) {
            prim->u0 = prim->u2 = 0x98;
            prim->u1 = prim->u3 = 0xA8;
        } else {
            prim->u0 = prim->u2 = 0xA8;
            prim->u1 = prim->u3 = 0x98;
        }
        prim->drawMode = DRAW_UNK_200 | DRAW_UNK_100 | DRAW_HIDE | DRAW_UNK02;
        prim->priority = RIC.zPriority + 1;
        prim = prim->next;
        prim->tpage = 0x1A;
        prim->clut = 0x186;
        prim->drawMode = DRAW_UNK_200 | DRAW_UNK_100 | DRAW_HIDE | DRAW_UNK02;
        prim->priority = RIC.zPriority + 3;
        prim = prim->next;
        prim->tpage = 0x1A;
        prim->clut = 0x186;
        prim->drawMode = DRAW_UNK_200 | DRAW_UNK_100 | DRAW_HIDE | DRAW_UNK02;
        prim->priority = RIC.zPriority + 3;
        if (self->params & 0xFF00) {
            RicCreateEntFactoryFromEntity(self, BP_66, 0);
        } else {
            RicCreateEntFactoryFromEntity(self, BP_STOPWATCH_CIRCLE, 0);
        }
        g_api.PlaySfx(SFX_UI_ALERT_TINK);
        g_unkGraphicsStruct.D_800973FC = 1;
        self->step++;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        prim->drawMode &= ~DRAW_HIDE;
        self->ext.ricStopWatch.unk84.val += 0x18000;
        if (self->ext.ricStopWatch.unk84.val >= 0x1A0000) {
            self->step++;
        }
        break;
    case 2:
        self->ext.ricStopWatch.unk84.val += 0xFFFF0000;
        if (self->ext.ricStopWatch.unk84.val <= 0x100000) {
            self->ext.ricStopWatch.t = 12;
            g_api.PlaySfx(SFX_STOPWATCH_TICK);
            self->step++;
        }
        break;
    case 3:
        self->ext.ricStopWatch.unk7E++;
        if (self->ext.ricStopWatch.unk7E > 0x50) {
            g_api.PlaySfx(SFX_STOPWATCH_TICK);
            self->ext.ricStopWatch.unk7E = 0;
            self->ext.ricStopWatch.unk90 = 1;
            self->ext.ricStopWatch.t--;
            if (self->ext.ricStopWatch.t < 0) {
                self->step++;
                break;
            }
        }
        if (self->ext.ricStopWatch.t < 12) {
            prim = &g_PrimBuf[self->primIndex];
            prim = prim->next;
            if (self->ext.ricStopWatch.t >= 10) {
                self->ext.ricStopWatch.unk92 = 1;
                psp_s1 = 8 * (self->ext.ricStopWatch.t / 10);
                prim->u0 = prim->u2 = psp_s1 + 0x18;
                prim->v0 = prim->v1 = 0x40;
                prim->u1 = prim->u3 = psp_s1 + 0x1E;
                prim->v2 = prim->v3 = 0x49;
                prim->drawMode &= ~DRAW_HIDE;
                prim = prim->next;
            } else {
                self->ext.ricStopWatch.unk92 = 0;
            }
            psp_s1 = 8 * (self->ext.ricStopWatch.t % 10);
            if (psp_s1 == 0) {
                psp_s1 = 0x50;
            }
            prim->u0 = prim->u2 = psp_s1 + 0x18;
            prim->v0 = prim->v1 = 0x40;
            prim->u1 = prim->u3 = psp_s1 + 0x1E;
            prim->v2 = prim->v3 = 0x49;
            prim->drawMode &= ~DRAW_HIDE;
        }
        break;
    case 4:
        prim = &g_PrimBuf[self->primIndex];
        prim->priority = 0xC2;
        prim->drawMode &= ~DRAW_UNK_200;
        prim = prim->next;
        prim->drawMode |= DRAW_HIDE;
        prim = prim->next;
        prim->drawMode |= DRAW_HIDE;
        self->posX.i.hi += self->facingLeft ? 6 : -6;
        self->ext.ricStopWatch.t = 0;
        self->posY.i.hi -= self->ext.ricStopWatch.unk84.val >> 0x10;
        self->step++;
        break;
    case 5:
        self->ext.ricStopWatch.t++;
        if (self->ext.ricStopWatch.t >= 4) {
            prim = &g_PrimBuf[self->primIndex];
            prim->clut = PAL_FILL_WHITE;
            prim->r0 = prim->r1 = prim->r2 = prim->r3 = 0x40;
            prim->g0 = prim->g1 = prim->g2 = prim->g3 = 0x40;
            prim->b0 = prim->b1 = prim->b2 = prim->b3 = 0x60;
            prim->drawMode |= DRAW_COLORS;
            g_api.PlaySfx(SFX_UI_SUBWEAPON_TINK);
            self->step++;
        }
        break;
    case 6:
        self->ext.ricStopWatch.t++;
        if (self->ext.ricStopWatch.t > 14) {
            RicCreateEntFactoryFromEntity(self, FACTORY(BP_EMBERS, 7), 0);
            self->step++;
        }
        break;
    case 7:
        g_unkGraphicsStruct.D_800973FC = 0;
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        return;
    }
    prim = &g_PrimBuf[self->primIndex];
    if (self->step < 5) {
        psp_s1 = RIC.posX.val + (RIC.facingLeft ? FIX(8) : FIX(-8));
        if (RIC.hitboxOffY == 0 && RIC.hitboxHeight == 0) {
            psp_s2 = RIC.posY.val - FIX(16);
        } else {
            psp_s2 = RIC.posY.val - ((RIC.hitboxHeight - RIC.hitboxOffY) << 16);
        }
        self->posX.val += (psp_s1 - self->posX.val) / 12;
        self->posY.val += (psp_s2 - self->posY.val) / 4;
        if (RIC.facingLeft != self->facingLeft) {
            if (abs(psp_s1 - self->posX.val) < FIX(1)) {
                self->facingLeft = RIC.facingLeft;
            } else if (!self->facingLeft && psp_s1 < self->posX.val) {
                self->facingLeft = RIC.facingLeft;
            } else if (self->facingLeft && self->posX.val < psp_s1) {
                self->facingLeft = RIC.facingLeft;
            }
        }
        if (self->facingLeft) {
            prim->u0 = prim->u2 = 0x98;
            prim->u1 = prim->u3 = 0xA8;
        } else {
            prim->u0 = prim->u2 = 0xA8;
            prim->u1 = prim->u3 = 0x98;
        }
    }
    if (self->step < 3) {
        psp_s1 = self->posX.i.hi + (self->facingLeft ? 6 : -6);
        psp_s2 = self->posY.i.hi - (self->ext.ricStopWatch.unk84.val >> 0x10);
        if (self->ext.ricStopWatch.unk82 < 0x64) {
            self->ext.ricStopWatch.unk82 += 4;
        }
        if (self->ext.ricStopWatch.unk80 < 0x1000) {
            self->ext.ricStopWatch.unk80 += 0x80;
        }
        psp_s4 = (self->ext.ricStopWatch.unk82 * 8) / 100;
        psp_s3 = (self->ext.ricStopWatch.unk82 * 12) / 100;
        psp_s6 = rsin(self->ext.ricStopWatch.unk80);
        psp_s5 = rcos(self->ext.ricStopWatch.unk80);
        prim->x0 = psp_s1 + ((psp_s5 * -psp_s4 - psp_s6 * -psp_s3) >> 0xC);
        prim->y0 = psp_s2 + ((psp_s6 * -psp_s4 + psp_s5 * -psp_s3) >> 0xC);
        prim->x1 = psp_s1 + ((psp_s5 * psp_s4 - psp_s6 * -psp_s3) >> 0xC);
        prim->y1 = psp_s2 + ((psp_s6 * psp_s4 + psp_s5 * -psp_s3) >> 0xC);
        prim->x2 = psp_s1 + ((psp_s5 * -psp_s4 - psp_s6 * psp_s3) >> 0xC);
        prim->y2 = psp_s2 + ((psp_s6 * -psp_s4 + psp_s5 * psp_s3) >> 0xC);
        prim->x3 = psp_s1 + ((psp_s5 * psp_s4 - psp_s6 * psp_s3) >> 0xC);
        prim->y3 = psp_s2 + ((psp_s6 * psp_s4 + psp_s5 * psp_s3) >> 0xC);
    } else if (self->step < 5) {
        if (self->ext.ricStopWatch.unk84.val <= 0x100000) {
            if (self->ext.ricStopWatch.unk90) {
                self->ext.ricStopWatch.unk88 = (rand() % 0x40 + 0x200) * 0x100;
                self->ext.ricStopWatch.unk90 = 0;
            } else {
                self->ext.ricStopWatch.unk88 = (rand() % 0x80 + 0x100) * 0x100;
            }
            if (self->ext.ricStopWatch.unk80 >= 0) {
                self->ext.ricStopWatch.unk80 = -(rand() % 0x40 + 0x40);
            } else {
                self->ext.ricStopWatch.unk80 = rand() % 0x40 + 0x40;
            }
        }
        self->ext.ricStopWatch.unk84.val += self->ext.ricStopWatch.unk88;
        self->ext.ricStopWatch.unk88 -= 0x4000;
        psp_s1 = self->posX.i.hi + (self->facingLeft ? 6 : -6);
        psp_s2 = self->posY.i.hi - (self->ext.ricStopWatch.unk84.val >> 0x10);
        psp_s6 = rsin(self->ext.ricStopWatch.unk80);
        psp_s5 = rcos(self->ext.ricStopWatch.unk80);
        prim->x0 = psp_s1 + (((psp_s5 * -8) - (psp_s6 * -12)) >> 0xC);
        prim->y0 = psp_s2 + (((psp_s6 * -8) + (psp_s5 * -12)) >> 0xC);
        prim->x1 = psp_s1 + (((psp_s5 * 8) - (psp_s6 * -12)) >> 0xC);
        prim->y1 = psp_s2 + (((psp_s6 * 8) + (psp_s5 * -12)) >> 0xC);
        prim->x2 = psp_s1 + (((psp_s5 * -8) - (psp_s6 * 12)) >> 0xC);
        prim->y2 = psp_s2 + (((psp_s6 * -8) + (psp_s5 * 12)) >> 0xC);
        prim->x3 = psp_s1 + (((psp_s5 * 8) - (psp_s6 * 12)) >> 0xC);
        prim->y3 = psp_s2 + (((psp_s6 * 8) + (psp_s5 * 12)) >> 0xC);
    } else {
        psp_s4 = 8 - self->ext.ricStopWatch.t;
        if (psp_s4 <= 0) {
            psp_s4 = 1;
        }
        psp_s3 = (self->ext.ricStopWatch.t << 16 >> 11) + 0xC;
        if (psp_s3 >= 0x80) {
            psp_s3 = 0x80;
        }
        prim->x0 = prim->x2 = self->posX.i.hi - psp_s4;
        prim->x1 = prim->x3 = self->posX.i.hi + psp_s4;
        prim->y0 = prim->y1 = self->posY.i.hi - psp_s3;
        prim->y2 = prim->y3 = self->posY.i.hi + psp_s3;
    }
    if (self->step < 4) {
        psp_s2 = self->posY.i.hi - 0xE;
        if (self->ext.ricStopWatch.unk92) {
            psp_s1 = self->posX.i.hi - (self->facingLeft ? 10 : -4);
            prim = prim->next;
            if (self->ext.ricStopWatch.unk7E < 8) {
                prim->x0 = prim->x2 =
                    psp_s1 - (self->ext.ricStopWatch.unk7E / 2);
                prim->x1 = prim->x3 =
                    psp_s1 + (self->ext.ricStopWatch.unk7E / 2);
                prim->y0 = prim->y1 =
                    (psp_s2 - 8) - (7 - self->ext.ricStopWatch.unk7E);
                prim->y2 = prim->y3 =
                    (psp_s2 + 8) + (7 - self->ext.ricStopWatch.unk7E);
            } else if (self->ext.ricStopWatch.unk7E >= 0x44) {
                psp_s4 = (0x50 - (self->ext.ricStopWatch.unk7E + 4)) / 2;
                if (psp_s4 < 0) {
                    psp_s4 = 0;
                }
                psp_s3 = self->ext.ricStopWatch.unk7E - 0x44;
                if (psp_s3 > 8) {
                    psp_s3 = 8;
                }
                prim->x0 = prim->x2 = psp_s1 - psp_s4;
                prim->x1 = prim->x3 = psp_s1 + psp_s4;
                prim->y0 = prim->y1 = (psp_s2 - 8) - psp_s3;
                prim->y2 = prim->y3 = (psp_s2 + 8) + psp_s3;
            } else {
                prim->x0 = prim->x2 = psp_s1 - 4;
                prim->x1 = prim->x3 = psp_s1 + 4;
                prim->y0 = prim->y1 = psp_s2 - 8;
                prim->y2 = prim->y3 = psp_s2 + 8;
            }
            psp_s1 = self->posX.i.hi - (self->facingLeft ? 4 : -10);
            prim = prim->next;
            if (self->ext.ricStopWatch.unk7E < 0xC) {
                psp_s4 = (self->ext.ricStopWatch.unk7E - 4) / 2;
                if (psp_s4 < 0) {
                    psp_s4 = 0;
                }
                psp_s3 = 7 - (self->ext.ricStopWatch.unk7E - 4);
                if (psp_s3 < 0) {
                    psp_s3 = 0;
                }
                prim->x0 = prim->x2 = psp_s1 - psp_s4;
                prim->x1 = prim->x3 = psp_s1 + psp_s4;
                prim->y0 = prim->y1 = (psp_s2 - 8) - psp_s3;
                prim->y2 = prim->y3 = (psp_s2 + 8) + psp_s3;
            } else if (self->ext.ricStopWatch.unk7E >= 0x48) {
                prim->x0 = prim->x2 =
                    psp_s1 - ((0x50 - self->ext.ricStopWatch.unk7E) / 2);
                prim->x1 = prim->x3 =
                    psp_s1 + ((0x50 - self->ext.ricStopWatch.unk7E) / 2);
                prim->y0 = prim->y1 =
                    (psp_s2 - 8) - (-0x48 + self->ext.ricStopWatch.unk7E);
                prim->y2 = prim->y3 =
                    (psp_s2 + 8) + (-0x48 + self->ext.ricStopWatch.unk7E);
            } else {
                prim->x0 = prim->x2 = psp_s1 - 4;
                prim->x1 = prim->x3 = psp_s1 + 4;
                prim->y0 = prim->y1 = psp_s2 - 8;
                prim->y2 = prim->y3 = psp_s2 + 8;
            }
        } else {
            psp_s1 = self->posX.i.hi - (self->facingLeft ? 4 : -4);
            prim = prim->next;
            if (self->ext.ricStopWatch.unk7E < 8) {
                prim->x0 = prim->x2 =
                    psp_s1 - (self->ext.ricStopWatch.unk7E / 2);
                prim->x1 = prim->x3 =
                    psp_s1 + (self->ext.ricStopWatch.unk7E / 2);
                prim->y0 = prim->y1 =
                    (psp_s2 - 8) - (7 - self->ext.ricStopWatch.unk7E);
                prim->y2 = prim->y3 =
                    (psp_s2 + 8) + (7 - self->ext.ricStopWatch.unk7E);
            } else if (self->ext.ricStopWatch.unk7E >= 0x48) {
                prim->x0 = prim->x2 =
                    psp_s1 - ((0x50 - self->ext.ricStopWatch.unk7E) / 2);
                prim->x1 = prim->x3 =
                    psp_s1 + ((0x50 - self->ext.ricStopWatch.unk7E) / 2);
                prim->y0 = prim->y1 =
                    (psp_s2 - 8) - (-0x48 + self->ext.ricStopWatch.unk7E);
                prim->y2 = prim->y3 =
                    (psp_s2 + 8) + (-0x48 + self->ext.ricStopWatch.unk7E);
            } else {
                prim->x0 = prim->x2 = psp_s1 - 5;
                prim->x1 = prim->x3 = psp_s1 + 5;
                prim->y0 = prim->y1 = psp_s2 - 8;
                prim->y2 = prim->y3 = psp_s2 + 8;
            }
        }
    }
}
#endif

void RicEntitySubwpnBibleTrail(Entity* entity) {
    Primitive* prim;

    switch (entity->step) {
    case 0:
        entity->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (entity->primIndex == -1) {
            DestroyEntity(entity);
            return;
        }
        entity->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[entity->primIndex];
        prim->tpage = 0x1C;
        prim->clut = 0x19D;
        prim->u0 = prim->u2 = 0x20;
        prim->v0 = prim->v1 = 0;
        prim->u1 = prim->u3 = 0x30;
        prim->v2 = prim->v3 = 0x10;
        prim->x0 = prim->x2 = entity->posX.i.hi - 8;
        prim->x1 = prim->x3 = entity->posX.i.hi + 8;
        prim->y0 = prim->y1 = entity->posY.i.hi - 8;
        prim->y2 = prim->y3 = entity->posY.i.hi + 8;
        prim->priority = entity->zPriority;
        prim->drawMode = DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        entity->ext.et_BibleSubwpn.unk7E = 0x60;
        entity->step++;
        break;
    case 1:
        entity->ext.et_BibleSubwpn.unk7C++;
        if (entity->ext.et_BibleSubwpn.unk7C > 5) {
            entity->step++;
        }
        entity->ext.et_BibleSubwpn.unk7E -= 8;
        break;
    case 2:
        DestroyEntity(entity);
        return;
    }
    prim = &g_PrimBuf[entity->primIndex];
    PCOL(prim) = entity->ext.et_BibleSubwpn.unk7E;
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntitySubwpnBible);
#else
void RicEntitySubwpnBible(Entity* self) {
    Primitive* prim;
    s32 sp48;
    s16 selfX;
    s16 selfY;
    s32 psp_s7;
    s32 psp_s6;
    s32 psp_s5;
    s32 psp_s4;
    s32 psp_s3;
    s32 psp_s2;
    s32 psp_s1;

    // n.b.! uninitialized on PSX, like in RIC
    psp_s4 = 0;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        prim->tpage = 0x1E;
        prim->clut = 0x17F;
        prim->u0 = prim->u2 = 0x98;
        prim->v0 = prim->v1 = 0xD8;
        prim->u1 = prim->u3 = 0xA8;
        prim->v2 = prim->v3 = 0xF0;
        prim->drawMode = DRAW_HIDE;
        prim->priority = RIC.zPriority + 1;
        if (self->facingLeft) {
            self->ext.et_BibleSubwpn.unk84 = 0x20;
        } else {
            self->ext.et_BibleSubwpn.unk84 = -0x20;
        }
        self->hitboxWidth = 6;
        self->hitboxHeight = 6;
        self->step++;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        prim->drawMode &= ~DRAW_HIDE;
        self->ext.et_BibleSubwpn.unk86++;
        self->step++;
    case 2:
        self->ext.et_BibleSubwpn.unk7E++;
        self->ext.et_BibleSubwpn.unk7C++;
        if (self->ext.et_BibleSubwpn.unk7E >= 0x30) {
            self->step++;
        }
        break;
    case 3:
        self->ext.et_BibleSubwpn.unk7C++;
        if (self->ext.et_BibleSubwpn.unk7C >= 0x12C) {
            self->flags &= ~FLAG_UNK_10000000;
            if (self->facingLeft) {
                self->velocityX = FIX(-12);
            } else {
                self->velocityX = FIX(12);
            }
            self->velocityY = FIX(-12);
            self->ext.et_BibleSubwpn.unk86++;
            self->step++;
        }
        break;
    }
    switch (self->ext.et_BibleSubwpn.unk86) {
    case 0:
        break;
    case 1:
        psp_s2 = rsin(self->ext.et_BibleSubwpn.unk80);
        psp_s1 = rcos(self->ext.et_BibleSubwpn.unk80);
        psp_s5 = (psp_s2 * self->ext.et_BibleSubwpn.unk7E) >> 0xC;
        psp_s3 = (psp_s1 * self->ext.et_BibleSubwpn.unk7E) >> 0xC;
        psp_s7 = (psp_s1 * psp_s5 + psp_s2 * psp_s3);
        sp48 = (psp_s1 * psp_s3 - psp_s2 * psp_s5);
        psp_s5 = psp_s7 >> 0xC;
        psp_s3 = sp48 >> 0xC;
        psp_s2 = rsin(self->ext.et_BibleSubwpn.unk82);
        psp_s1 = rcos(self->ext.et_BibleSubwpn.unk82);
        psp_s7 = ((psp_s1 * psp_s5) + (psp_s2 * psp_s4)) >> 0xC;
        psp_s6 = ((psp_s1 * psp_s4) - (psp_s2 * psp_s5)) >> 0xC;
        psp_s4 = psp_s6;
        if (self->facingLeft) {
            psp_s6 = ((psp_s1 * psp_s4) + (psp_s2 * psp_s3)) >> 0xC;
        } else {
            psp_s6 = ((psp_s1 * psp_s4) - (psp_s2 * psp_s3)) >> 0xC;
        }
        self->ext.et_BibleSubwpn.unk80 += self->facingLeft ? 0x80 : -0x80;
        self->ext.et_BibleSubwpn.unk80 &= 0xFFF;
        self->ext.et_BibleSubwpn.unk82 += self->ext.et_BibleSubwpn.unk84;
        if (abs(self->ext.et_BibleSubwpn.unk82) >= 0x200) {
            self->ext.et_BibleSubwpn.unk84 *= -1;
        }
        self->posX.i.hi = RIC.posX.i.hi + psp_s7;
        self->posY.i.hi = RIC.posY.i.hi + psp_s6;
        self->zPriority = RIC.zPriority + (psp_s3 < 0 ? 2 : -2);
        break;
    case 2:
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        self->velocityY -= FIX(2);
        break;
    }
    if (self->ext.et_BibleSubwpn.unk86) {
        selfX = self->posX.i.hi;
        selfY = self->posY.i.hi;
        prim = &g_PrimBuf[self->primIndex];
        prim->x0 = prim->x2 = selfX - 8;
        prim->x1 = prim->x3 = selfX + 8;
        prim->y0 = prim->y1 = selfY - 12;
        prim->y2 = prim->y3 = selfY + 12;
        prim->priority = self->zPriority;
        RicCreateEntFactoryFromEntity(self, BP_BIBLE_TRAIL, 0);
        if (g_GameTimer % 10 == 0) {
            g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        }
    }
}
#endif

INCLUDE_RODATA("boss/bo6/nonmatchings/us_3E79C", D_us_801A7028);

INCLUDE_RODATA("boss/bo6/nonmatchings/us_3E79C", D_us_801A7030);

extern EInit D_us_801804A8;
extern SVECTOR* D_us_80182BA8[][4];
extern u8 D_us_80182E88[][5];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_3E79C", RicEntityCrashCrossBeam);
#else
void RicEntityCrashCrossBeam(Entity* self) {
    MATRIX m;
    SVECTOR rot;
    VECTOR trans1;
    SVECTOR pos = {0, 0, 0};
    s32 z;
    s32 nclip;
    Primitive* prim;
    s32 i;
    u8* primUVCoords;
    SVECTOR** vectors_ptr;
    SVECTOR* psp_s7;
    SVECTOR* psp_s6;
    SVECTOR* temp_a3;
    SVECTOR* psp_s4;

    if (self->step == 0) {
        self->ext.giantcross.subweaponId = PL_W_CRASH_CROSS_BEAM;
        InitializeEntity(D_us_801804A8);
        self->primIndex = g_api.func_800EDB58(PRIM_GT4, 46);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[self->primIndex];
        self->flags |= FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        while (prim != NULL) {
            prim->tpage = 0x1C;
            prim->drawMode = DRAW_UNK_100 | DRAW_HIDE;
            prim = prim->next;
        }
        self->hitboxHeight = 0x50;
        self->hitboxWidth = 0xC;
        self->posY.i.hi = 0x160;
        self->velocityY = FIX(-6.0);
        self->facingLeft = 0;
        self->ext.giantcross.unk7C = 0;
        self->ext.giantcross.unk7E = 0x400;
        g_api.PlaySfx(SFX_FIREBALL_SHOT_B);
        prim = &g_PrimBuf[self->primIndex];
        primUVCoords = D_us_80182E88[0];
        for (i = 0; i < 46; i++, prim = prim->next, primUVCoords += 5) {
            prim->clut = (primUVCoords[4] & 0xF) + 0x1A0;
            switch (primUVCoords[4] & 0xF0) {
            case 0x10:
                prim->u0 = primUVCoords[0] + primUVCoords[2];
                prim->v0 = primUVCoords[1];
                prim->u1 = primUVCoords[0];
                prim->v1 = primUVCoords[1];
                prim->u2 = primUVCoords[0] + primUVCoords[2];
                prim->v2 = primUVCoords[1] + primUVCoords[3];
                prim->u3 = primUVCoords[0];
                prim->v3 = primUVCoords[1] + primUVCoords[3];
                break;
            case 0x20:
                prim->u0 = primUVCoords[0];
                prim->v0 = primUVCoords[1] + primUVCoords[3];
                prim->u1 = primUVCoords[0] + primUVCoords[2];
                prim->v1 = primUVCoords[1] + primUVCoords[3];
                prim->u2 = primUVCoords[0];
                prim->v2 = primUVCoords[1];
                prim->u3 = primUVCoords[0] + primUVCoords[2];
                prim->v3 = primUVCoords[1];
                break;
            case 0x30:
                prim->u0 = primUVCoords[0] + primUVCoords[2];
                prim->v0 = primUVCoords[1] + primUVCoords[3];
                prim->u1 = primUVCoords[0];
                prim->v1 = primUVCoords[1] + primUVCoords[3];
                prim->u2 = primUVCoords[0] + primUVCoords[2];
                prim->v2 = primUVCoords[1];
                prim->u3 = primUVCoords[0];
                prim->v3 = primUVCoords[1];
                break;
            case 0x40:
                prim->u3 = primUVCoords[0];
                prim->v3 = primUVCoords[1];
                prim->u1 = primUVCoords[0] + primUVCoords[2];
                prim->v1 = primUVCoords[1];
                prim->u2 = primUVCoords[0];
                prim->v2 = primUVCoords[1] + primUVCoords[3];
                prim->u0 = primUVCoords[0] + primUVCoords[2];
                prim->v0 = primUVCoords[1] + primUVCoords[3];
                break;
            case 0x50:
                prim->u3 = primUVCoords[0] + primUVCoords[2];
                prim->v3 = primUVCoords[1];
                prim->u1 = primUVCoords[0];
                prim->v1 = primUVCoords[1];
                prim->u2 = primUVCoords[0] + primUVCoords[2];
                prim->v2 = primUVCoords[1] + primUVCoords[3];
                prim->u0 = primUVCoords[0];
                prim->v0 = primUVCoords[1] + primUVCoords[3];
                break;
            case 0x60:
                prim->u3 = primUVCoords[0];
                prim->v3 = primUVCoords[1] + primUVCoords[3];
                prim->u1 = primUVCoords[0] + primUVCoords[2];
                prim->v1 = primUVCoords[1] + primUVCoords[3];
                prim->u2 = primUVCoords[0];
                prim->v2 = primUVCoords[1];
                prim->u0 = primUVCoords[0] + primUVCoords[2];
                prim->v0 = primUVCoords[1];
                break;
            case 0x70:
                prim->u3 = primUVCoords[0] + primUVCoords[2];
                prim->v3 = primUVCoords[1] + primUVCoords[3];
                prim->u1 = primUVCoords[0];
                prim->v1 = primUVCoords[1] + primUVCoords[3];
                prim->u2 = primUVCoords[0] + primUVCoords[2];
                prim->v2 = primUVCoords[1];
                prim->u0 = primUVCoords[0];
                prim->v0 = primUVCoords[1];
                break;
            default:
                prim->u0 = primUVCoords[0];
                prim->v0 = primUVCoords[1];
                prim->u1 = primUVCoords[0] + primUVCoords[2];
                prim->v1 = primUVCoords[1];
                prim->u2 = primUVCoords[0];
                prim->v2 = primUVCoords[1] + primUVCoords[3];
                prim->u3 = primUVCoords[0] + primUVCoords[2];
                prim->v3 = primUVCoords[1] + primUVCoords[3];
                break;
            }
        }
        self->step = 1;
        FntPrint("judge:%02x\n", self->hitboxState);
        return;
    }
    if (self->posY.i.hi < -0x40) {
        DestroyEntity(self);
        return;
    }
    if (self->posY.i.hi < 0) {
        self->velocityY -= FIX(0.5);
    }
    self->ext.giantcross.unk7C += 0x60;
    self->ext.giantcross.unk7E += 0x60;
    self->posY.val += self->velocityY;
    trans1.vx = -(rcos(self->ext.giantcross.unk7C) * 0x60) >> 0xC;
    trans1.vy = self->posY.i.hi - 0x80;
    trans1.vz = ((rsin(self->ext.giantcross.unk7C) * 0x60) >> 0xC) + 0x180;
    rot.vy = self->ext.giantcross.unk7E;
    rot.vz = 0x40;
    rot.vx = 0;

    SetGeomOffset(self->posX.i.hi, 120);
    gte_SetGeomScreen(320);
    RotMatrix(&rot, &m);
    TransMatrix(&m, &trans1);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_ldv0(&pos);
    gte_rtps();
    prim = &g_PrimBuf[self->primIndex];
    vectors_ptr = D_us_80182BA8[0];
    gte_stsxy((long*)&prim->x0);
    gte_stszotz((long*)&z);
    self->hitboxOffX = prim->x0 - self->posX.i.hi;
    self->hitboxOffY = prim->y0 - self->posY.i.hi;
    for (i = 0; i < 46; i++, prim = prim->next, vectors_ptr += 4) {
        psp_s7 = vectors_ptr[0];
        psp_s6 = vectors_ptr[1];
        psp_s4 = vectors_ptr[3];
        gte_ldv3(psp_s7, psp_s6, psp_s4);
        gte_rtpt();
        temp_a3 = vectors_ptr[2];
        prim->type = PRIM_GT4;
        gte_nclip();
        prim->drawMode = DRAW_HIDE;
        gte_stopz(&nclip);
        if (nclip < 0) {
            continue;
        }
        gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
        gte_ldv0(temp_a3);
        gte_rtps();
        prim->drawMode = DRAW_DEFAULT;
        if (z < 16) {
            prim->priority = 0x1F6;
        } else if (z > 998) {
            prim->priority = 0x10;
        } else {
            prim->priority = 0xD0 - (z - 0x50);
        }
        gte_stsxy((long*)&prim->x3);
    }
    FntPrint("judge:%02x\n", self->hitboxState);
}
#endif
