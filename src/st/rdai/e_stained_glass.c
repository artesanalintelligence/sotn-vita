// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rdai.h"
#include <scratchpad.h>

#include "../e_stained_glass_blend.h"

#include "../stained_glass_recurse.h"

#ifndef NON_MATCHING
INCLUDE_ASM("st/rdai/nonmatchings/e_stained_glass", func_us_801B2C28);

INCLUDE_ASM("st/rdai/nonmatchings/e_stained_glass", func_us_801B3368);
#else
// Same tables as the DAI ones in e_stained_glass.h, with the background
// translations mirrored for the inverted castle
extern EInit g_EInitInteractable;
extern SVECTOR D_us_80180B1C; // glass points
extern SVECTOR D_us_80180B24;
extern SVECTOR D_us_80180B2C;
extern SVECTOR D_us_80180B34;
extern SVECTOR D_us_80180B3C; // light points
extern SVECTOR D_us_80180B44;
extern SVECTOR D_us_80180B4C;
extern SVECTOR D_us_80180B54;
extern VECTOR D_us_80180B5C[]; // params, pad holds the recursion depth
extern s16 D_us_80180C1C[];    // y rotations
extern CVECTOR D_us_80180C34[]; // colors
extern SVECTOR D_us_80180C64;   // light rotation
extern SVECTOR D_us_80180C6C;   // background points
extern SVECTOR D_us_80180C74;
extern SVECTOR D_us_80180C7C;
extern SVECTOR D_us_80180C84;
extern SVECTOR D_us_80180C8C; // background position
extern VECTOR D_us_80180C94[]; // background translations
extern SVECTOR D_us_80180D24;  // background rotation

#define PAL_STAINED_GLASS 0xE
#define PAL_STAINED_GLASS_BG 0xF
#define PAL_STAINED_GLASS_LIGHT 0x200
#define PAL_STAINED_GLASS_BG_LIGHT 0x201

// Upside down version of DAI's EntityStainedGlass
void func_us_801B2C28(Entity* self) {
    s16 midpointX, midpointY;
    s32 primIndex;
    SVECTOR rotVector;
    VECTOR transVector;
    MATRIX mtx;
    s32 p, flag;
    u8 transparent;
    s32 iterations;
    s16* yValsPtr;
    VECTOR* paramsPtr;
    CVECTOR* colorPtr;
    s32 count;
    Primitive* glassPrim;
    Primitive* lightPrim;
    Primitive* tempPrim;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 60);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        glassPrim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->ext.stainedGlass.glassPrim = glassPrim;
        self->flags |= FLAG_HAS_PRIMS;
        for (count = 0; count < 12; count++) {
            PGREY(glassPrim, 0) = 96;
            glassPrim->priority = 86;
            glassPrim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
            glassPrim->tpage = 15;
            glassPrim->clut = PAL_STAINED_GLASS;
            glassPrim->u0 = glassPrim->u2 = 4;
            glassPrim->u1 = glassPrim->u3 = 28;
            glassPrim->v0 = glassPrim->v1 = 1;
            glassPrim->v2 = glassPrim->v3 = 174;
            LOW(glassPrim->r1) = LOW(glassPrim->r0);
            LOW(glassPrim->r2) = LOW(glassPrim->r0);
            LOW(glassPrim->r3) = LOW(glassPrim->r0);
            glassPrim = glassPrim->next;
        }
        for (self->ext.stainedGlass.lightPrim = glassPrim; glassPrim != NULL;
             glassPrim = glassPrim->next) {
            glassPrim->drawMode = DRAW_HIDE;
        }
        break;

    case 1:
        SetGeomScreen(1024);
        SetGeomOffset(128, 160);
        paramsPtr = D_us_80180B5C;
        colorPtr = D_us_80180C34;
        yValsPtr = D_us_80180C1C;
        glassPrim = self->ext.stainedGlass.glassPrim;
        lightPrim = self->ext.stainedGlass.lightPrim;
        for (count = 0; count < 12; count++) {
            rotVector.vx = 0;
            rotVector.vy = *yValsPtr;
            rotVector.vz = 0;
            RotMatrix(&rotVector, &mtx);
            RotMatrixZ(0x800, &mtx);
            transVector = *paramsPtr;
            transparent = true;
            if (transVector.vx > 0) {
                glassPrim->drawMode =
                    DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            } else {
                transparent = false;
                glassPrim->drawMode = DRAW_COLORS;
            }
            iterations = paramsPtr->pad;
            transVector.vx += self->posX.i.hi - 128;
            transVector.vz += 1024;
            transVector.vy += self->posY.i.hi - 160;
            TransMatrix(&mtx, &transVector);
            SetRotMatrix(&mtx);
            SetTransMatrix(&mtx);
            CVEC(glassPrim->r0) = *colorPtr;
            glassPrim->type = PRIM_GT4;
            LOW(glassPrim->r1) = LOW(glassPrim->r0);
            LOW(glassPrim->r2) = LOW(glassPrim->r0);
            LOW(glassPrim->r3) = LOW(glassPrim->r0);
            if (iterations) {
                lightPrim = StainedGlassRecurse(
                    &D_us_80180B1C, &D_us_80180B24, &D_us_80180B2C,
                    &D_us_80180B34, glassPrim, iterations, lightPrim,
                    (u8*)SP(0));
                glassPrim->drawMode = DRAW_HIDE;
            }
            RotTransPers4(&D_us_80180B1C, &D_us_80180B24, &D_us_80180B2C,
                          &D_us_80180B34, (s32*)&glassPrim->x0,
                          (s32*)&glassPrim->x1, (s32*)&glassPrim->x2,
                          (s32*)&glassPrim->x3, &p, &flag);
            // Renders the light "coming through" the right side glass
            if (transparent && lightPrim != NULL) {
                tempPrim = lightPrim->next;
                *lightPrim = *glassPrim;
                lightPrim->next = tempPrim;
                lightPrim->clut = PAL_FILL_WHITE;
                lightPrim->drawMode = DRAW_COLORS;
                lightPrim->priority = glassPrim->priority - 1;
                lightPrim->r0 = lightPrim->g0 = lightPrim->b0 = colorPtr->cd;
                LOW(lightPrim->r1) = LOW(lightPrim->r0);
                lightPrim->r2 = lightPrim->g2 = lightPrim->b2 =
                    colorPtr->cd / 2;
                LOW(lightPrim->r3) = LOW(lightPrim->r2);

                lightPrim = lightPrim->next;
                if (lightPrim == NULL) {
                    break;
                }
                tempPrim = lightPrim->next;
                *lightPrim = *glassPrim;
                lightPrim->next = tempPrim;
                rotVector.vx = 0;
                rotVector.vy = *yValsPtr;
                rotVector.vz = 1024;
                RotMatrix(&D_us_80180C64, &mtx);
                RotMatrixY(rotVector.vy, &mtx);
                RotMatrixZ(rotVector.vz, &mtx);
                RotMatrixZ(0x800, &mtx);
                transVector = *paramsPtr;
                transVector.vx = 96;
                transVector.vy = -256;
                transVector.vx += self->posX.i.hi - 128;
                transVector.vz += 1024;
                transVector.vy += self->posY.i.hi - 160;
                TransMatrix(&mtx, &transVector);
                SetRotMatrix(&mtx);
                SetTransMatrix(&mtx);
                CVEC(lightPrim->r0) = *colorPtr;
                lightPrim->type = PRIM_GT4;
                LOW(lightPrim->r1) = LOW(lightPrim->r0);
                LOW(lightPrim->r2) = LOW(lightPrim->r0);
                LOW(lightPrim->r3) = LOW(lightPrim->r0);
                RotTransPers4(&D_us_80180B3C, &D_us_80180B44, &D_us_80180B4C,
                              &D_us_80180B54, (s32*)&lightPrim->x0,
                              (s32*)&lightPrim->x1, (s32*)&lightPrim->x2,
                              (s32*)&lightPrim->x3, &p, &flag);
                lightPrim->clut = PAL_STAINED_GLASS_LIGHT;
                lightPrim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                                      DRAW_COLORS | DRAW_TRANSP;
                midpointX = (lightPrim->x0 + lightPrim->x2) / 2;
                midpointY = (lightPrim->y0 + lightPrim->y1) / 2;

                lightPrim = lightPrim->next;
                if (lightPrim == NULL) {
                    break;
                }
                tempPrim = lightPrim->next;
                *lightPrim = *glassPrim;
                lightPrim->next = tempPrim;
                lightPrim->clut = PAL_FILL_WHITE;
                lightPrim->drawMode =
                    DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
                lightPrim->r0 = 24;
                lightPrim->g0 = 8;
                lightPrim->b0 = 32;
                lightPrim->r2 = 8;
                lightPrim->g2 = 24;
                lightPrim->b2 = 8;
                lightPrim->r3 = lightPrim->g3 = lightPrim->b3 = 0;
                lightPrim->priority = glassPrim->priority + 1;
                LOW(lightPrim->r1) = LOW(lightPrim->r0);
                LOW(lightPrim->x0) = LOW(glassPrim->x1);
                LOW(lightPrim->x1) = LOW(glassPrim->x0);
                lightPrim->x3 = midpointX;
                lightPrim->y3 = midpointY;
                LOW(lightPrim->x2) = LOW(glassPrim->x3);
                lightPrim = lightPrim->next;
            }
            colorPtr++;
            yValsPtr++;
            glassPrim = glassPrim->next;
            paramsPtr++;
        }
        count = 0;
        for (; lightPrim != NULL; lightPrim = lightPrim->next) {
            lightPrim->drawMode = DRAW_HIDE;
            count++;
        }
        FntPrint("rest %x\n", count);
        break;
    }
}

// Upside down version of DAI's EntityStainedGlassBackground
void func_us_801B3368(Entity* self) {
    s32 sxy[4];
    s32 p, flag;
    VECTOR transVector;
    MATRIX mtx;
    RECT rect;
    CVECTOR color;
    s32 primIndex;
    VECTOR* transVectorPtr;
    s32 midindex1, midindex2;
    s32 idx;
    u16 clut;
    Primitive* prim;
    s16 tempY, tempX;

    g_GpuBuffers[0].draw.r0 = 8;
    g_GpuBuffers[0].draw.g0 = 0;
    g_GpuBuffers[0].draw.b0 = 56;
    g_GpuBuffers[1].draw.r0 = 8;
    g_GpuBuffers[1].draw.g0 = 0;
    g_GpuBuffers[1].draw.b0 = 56;
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 19);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->ext.stainedGlass.glassPrim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        for (; prim != NULL; prim = prim->next) {
            PGREY(prim, 0) = 128;
            prim->tpage = 15;
            prim->clut = PAL_STAINED_GLASS_BG;
            prim->u0 = prim->u2 = 35;
            prim->u1 = prim->u3 = 92;
            prim->v0 = prim->v1 = 1;
            prim->v2 = prim->v3 = 62;
            prim->priority = 84;
            prim->drawMode = DRAW_DEFAULT;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
        }
        for (prim = self->ext.stainedGlass.glassPrim, idx = 0; idx < 4;
             prim = prim->next, idx++) {
            prim->clut = 5;
            prim->u0 = prim->u2 = 129;
            prim->u1 = prim->u3 = 174;
            prim->v0 = prim->v1 = 1;
            prim->v2 = prim->v3 = 126;
            prim->priority = 80;
        }
        prim = self->ext.stainedGlass.glassPrim;
        PGREY(prim, 0) = 32;
        PGREY(prim, 1) = 48;
        PGREY(prim, 2) = 64;
        PGREY(prim, 3) = 48;
        prim = prim->next;
        prim->r0 = 16;
        prim->g0 = 0;
        prim->b0 = 56;
        prim->clut = PAL_FILL_WHITE;
        prim->priority = 81;
        prim->drawMode = DRAW_HIDE;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim = prim->next;
        PGREY(prim, 0) = 64;
        PGREY(prim, 1) = 48;
        PGREY(prim, 2) = 80;
        PGREY(prim, 3) = 48;
        prim = prim->next;
        prim->r0 = 16;
        prim->g0 = 0;
        prim->b0 = 56;
        prim->clut = PAL_FILL_WHITE;
        prim->priority = 81;
        prim->drawMode = DRAW_HIDE;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim = prim->next;
        prim->type = PRIM_G4;
        prim->r0 = 64;
        prim->g0 = 24;
        prim->b0 = 72;
        prim->g2 = 24;
        prim->r2 = 200;
        prim->b2 = 104;
        prim->priority = 82;
        prim->drawMode = DRAW_COLORS | DRAW_UNK02;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r3);
        rect.x = 0;
        rect.y = 256;
        rect.w = 16;
        rect.h = 1;
        color.r = 8;
        color.g = 0;
        color.b = 56;
        StainedGlassBlendPalette(&rect, PAL_STAINED_GLASS_BG,
                                 PAL_STAINED_GLASS_BG_LIGHT, COLORS_PER_PAL,
                                 &color);
        break;

    case 1:
        SetGeomScreen(1024);
        SetGeomOffset(128, 160);
        prim = self->ext.stainedGlass.glassPrim;
        transVectorPtr = D_us_80180C94;
        clut = PAL_STAINED_GLASS_BG_LIGHT;
        for (idx = 0; idx < 8; idx++, transVectorPtr++, clut += 2) {
            RotMatrix(&D_us_80180D24, &mtx);
            RotMatrixZ(0x800, &mtx);
            transVector = *transVectorPtr;
            transVector.vx += self->posX.i.hi - 128;
            transVector.vz += 1024;
            transVector.vy += self->posY.i.hi - 160;
            TransMatrix(&mtx, &transVector);
            SetRotMatrix(&mtx);
            SetTransMatrix(&mtx);
            if (idx == 0) {
                RotTransPers(&D_us_80180C8C, &sxy[0], &p, &flag);
                tempX = (s16)sxy[0];
                tempY = (s16)(sxy[0] >> 16);
                prim->drawMode = DRAW_COLORS;
                prim->x0 = prim->x2 = tempX - 45;
                prim->x1 = prim->x3 = tempX;
                prim->y0 = prim->y1 = tempY + 62;
                prim->y2 = prim->y3 = tempY - 62;
                prim = prim->next;
                prim->x0 = prim->x2 = tempX - 45;
                prim->x1 = prim->x3 = tempX;
                prim->y0 = prim->y1 = tempY + 62;
                prim->y2 = prim->y3 = tempY - 62;
                prim->drawMode = DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
                prim = prim->next;
                prim->x0 = prim->x2 = tempX + 45;
                prim->x1 = prim->x3 = tempX;
                prim->y0 = prim->y1 = tempY + 62;
                prim->y2 = prim->y3 = tempY - 62;
                prim->drawMode = DRAW_COLORS;
                prim = prim->next;
                prim->x0 = prim->x2 = tempX + 45;
                prim->x1 = prim->x3 = tempX;
                prim->y0 = prim->y1 = tempY + 62;
                prim->y2 = prim->y3 = tempY - 62;
                prim->drawMode = DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
                prim = prim->next;
                prim->x0 = tempX - 8;
                prim->x1 = tempX + 8;
                prim->x2 = self->posX.i.hi - 24;
                prim->y0 = prim->y1 = tempY - 56;
                prim->x3 = self->posX.i.hi + 24;
                prim->drawMode = DRAW_COLORS;
                prim->y2 = prim->y3 = self->posY.i.hi - 194;
                prim = prim->next;
            } else {
                RotTransPers4(&D_us_80180C6C, &D_us_80180C74, &D_us_80180C7C,
                              &D_us_80180C84, &sxy[0], &sxy[1], &sxy[2],
                              &sxy[3], &p, &flag);
                if ((s16)(sxy[2] >> 16) <= 240) {
                    midindex1 = ((sxy[0] + sxy[1]) / 2) & 0xFFFF0000;
                    midindex1 |= ((s16)sxy[0] + (s16)sxy[1]) >> 1;
                    midindex2 = ((sxy[2] + sxy[3]) / 2) & 0xFFFF0000;
                    midindex2 |= ((s16)sxy[2] + (s16)sxy[3]) >> 1;
                    prim->clut = clut;
                    prim->drawMode = DRAW_DEFAULT;
                    LOW(prim->x0) = sxy[0];
                    LOW(prim->x1) = midindex1;
                    LOW(prim->x2) = sxy[2];
                    LOW(prim->x3) = midindex2;
                    prim = prim->next;
                    prim->clut = clut;
                    LOW(prim->x0) = sxy[1];
                    LOW(prim->x1) = midindex1;
                    LOW(prim->x2) = sxy[3];
                    LOW(prim->x3) = midindex2;
                    prim->drawMode = DRAW_DEFAULT;
                    prim = prim->next;
                }
            }
        }
        for (; prim != NULL; prim = prim->next) {
            prim->drawMode = DRAW_HIDE;
        }
        break;
    }
}
#endif
