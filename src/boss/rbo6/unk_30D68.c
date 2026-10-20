// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo6.h"

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo6/nonmatchings/unk_30D68", EntityBackgroundVortex);

INCLUDE_RODATA("boss/rbo6/nonmatchings/unk_30D68", D_us_8019D0AC);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_30D68", func_us_801B1738);
#else
#include <scratchpad.h>

extern u32 g_CutsceneFlags;

// Same as ST0's EntityBackgroundVortex, but it fades in with the cutscene
extern u16 g_EInitInteractable;
extern SVECTOR D_us_801B3634[0x100];
extern u8 D_us_801B3E34[0x100];

void EntityBackgroundVortex(Entity* self) {
    u8 brightness; // writes into an array, later that array is used for colors
    u8 leftV;
    u8 rightV;
    s32 primIndex;
    s32 scale;
    s32 xSize;
    s32 ySize;
    s32 zSize;
    SVECTOR rotVector;
    VECTOR transVector;
    MATRIX m;
    DRAWENV draw_env;
    DR_ENV* dr_env;
    RECT tw_rect;
    CVECTOR unused_Cvector;

    s32 size;
    s16 angle;
    s32 vectorIndex1;
    s32 vectorIndex2;
    s32 j;
    CVECTOR* scratchColor;
    s32 var_s2;
    s32 i;
    Primitive* prim;

    switch (self->step) {
    case 0:
        InitializeEntity(&g_EInitInteractable);
        self->posX.i.hi = 0x80;
        self->posY.i.hi = 0x1A0;
        self->opacity = 0;
        primIndex = g_api.func_800EDB58(PRIM_GT4, 0x110);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.bgVortex.prim = prim;
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        self->ext.bgVortex.unk9C = 0x5F0;
        self->ext.bgVortex.unk9E = 0xB0;
        self->ext.bgVortex.unkA0 = 0;
        self->ext.bgVortex.unkAE = 0xD0;
        // Fallthrough
    case 1:
        size = 0x60;
        scale = 0;
        brightness = 0x10;
        var_s2 = 0;
        angle = 0;
        for (j = 0; j < 16; j++) {
            scale = SquareRoot0(j * 0x120) * 4;
            for (i = 0; i < 16; i++) {
                xSize = (size * rcos(angle + i * 0x100)) >> 0xC;
                ySize = (size * rsin(angle + i * 0x100)) >> 0xC;
                zSize = scale;
                D_us_801B3634[var_s2].vx = xSize;
                D_us_801B3634[var_s2].vy = ySize;
                D_us_801B3634[var_s2].vz = zSize;
                var_s2++;
            }
            D_us_801B3E34[j] = brightness;
            angle += 0x80;
            size += 0x1C;
            brightness += 0x28;
            if (brightness > 0xA0) {
                brightness = 0xA0;
            }
        }
        self->step++;
        break;
    case 2:
        prim = self->ext.bgVortex.prim;
        while (prim != NULL) {
            prim->tpage = 0x16;
            prim->clut = PAL_UNK_19E;
            prim->u0 = prim->u2 = 0x40;
            prim->u1 = prim->u3 = 0x80;
            prim->v0 = prim->v1 = 0;
            prim->v2 = prim->v3 = 0x40;
            prim->priority = 0x80;
            prim->drawMode = DRAW_DEFAULT;
            prim = prim->next;
        }
        prim = self->ext.bgVortex.prim;
        draw_env = g_CurrentBuffer->draw;

        dr_env = g_api.func_800EDB08((POLY_GT4*)prim);
        if (dr_env == NULL) {
            DestroyEntity(self);
            return;
        }
        prim->type = PRIM_ENV;
        draw_env.isbg = 0;
        draw_env.r0 = 0;
        draw_env.g0 = 0;
        draw_env.b0 = 0;
        draw_env.dtd = 0;
        draw_env.clip.x = 0;
        draw_env.ofs[0] = 0;
        tw_rect.x = 0;
        tw_rect.y = 0;
        tw_rect.w = 0x40;
        tw_rect.h = 0x40;
        draw_env.tw = tw_rect;
        SetDrawEnv(dr_env, &draw_env);
        prim->priority = 2;
        prim->drawMode = DRAW_UNK_1000;
        prim = prim->next;
        dr_env = g_api.func_800EDB08((POLY_GT4*)prim);
        if (dr_env == NULL) {
            DestroyEntity(self);
            return;
        }
        prim->type = PRIM_ENV;
        prim->priority = 0x59;
        prim->drawMode = DRAW_UNK_800;
        prim = prim->next;
        self->step++;
        break;
    case 3:
        // RBO6 fades the vortex in once the cutscene starts
        if ((g_CutsceneFlags & 0x20) && self->opacity < 0x80) {
            self->opacity++;
        }
        switch (self->step_s) {
        case 0:
            self->posY.val -= FIX(0.75);
            if (self->posY.i.hi < 0xA0) {
                self->posY.i.hi = 0xA0;
                self->step_s++;
            }
            break;
        case 1:
            self->ext.bgVortex.unkA4 += 16;
            self->ext.bgVortex.unkA6 += 8;
            self->ext.bgVortex.unkA8 += 32;
            self->ext.bgVortex.unkAA += 2;
            self->ext.bgVortex.unk9C =
                ((rsin(self->ext.bgVortex.unkA4) << 6) >> 0xC) + 0x5F0;
            self->ext.bgVortex.unk9E =
                ((rsin(self->ext.bgVortex.unkA6) << 7) >> 0xC) + 0xB0;
            self->ext.bgVortex.unkA0 = rsin(self->ext.bgVortex.unkA8) >> 8;
            self->ext.bgVortex.unkAE =
                ((rsin(self->ext.bgVortex.unkAA) * 24) >> 0xC) + 0xD0;
            break;
        }
        prim = self->ext.bgVortex.prim;
        prim = prim->next;
        prim = prim->next;
        leftV = prim->v0;
        rightV = prim->v2;
        leftV += 2;
        rightV += 2;

        if (rightV < leftV) {
            leftV = rightV;
            rightV = leftV + 0x40;
        }
        while (prim != NULL) {
            prim->v0 = prim->v1 = leftV;
            prim->v2 = prim->v3 = rightV;
            prim = prim->next;
        }
        self->ext.bgVortex.unk80 -= 4;
        SetGeomScreen(0xC0);
        SetGeomOffset(self->posX.i.hi, self->posY.i.hi);
        SetFarColor(0, 0, 0);
        SetFogNear(0x128, 0xC0);
        rotVector.vx = 0;
        rotVector.vy = 0;
        rotVector.vz = self->ext.bgVortex.unk80;
        rotVector.vx += self->ext.bgVortex.unk9C;
        rotVector.vy += self->ext.bgVortex.unk9E;
        rotVector.vz += self->ext.bgVortex.unkA0;

        RotMatrix(&rotVector, &m);
        SetRotMatrix(&m);
        transVector.vx = 0;
        transVector.vy = 0;
        transVector.vz = 0xC0;
        transVector.vz += self->ext.bgVortex.unkAE;
        TransMatrix(&m, &transVector);
        SetTransMatrix(&m);
        // scratchpad memory
        scratchColor = (CVECTOR*)SP(0);
        for (i = 0; i < 16; i++, scratchColor++) {
            var_s2 = ((i & 1) * 2) + 1;
            scratchColor->b =
                ((D_us_801B3E34[i] / var_s2) * self->opacity) / 128;
            scratchColor->r = scratchColor->g =
                (((D_us_801B3E34[i] >> 1) / var_s2) * self->opacity) / 128;
            scratchColor->cd = 4;
        }
        prim = self->ext.bgVortex.prim;
        prim = prim->next;
        prim = prim->next;
        unused_Cvector.r = 0x80;
        unused_Cvector.g = 0x80;
        unused_Cvector.b = 0x80;
        unused_Cvector.cd = 4;
        scratchColor = (CVECTOR*)SP(0);
        for (i = 0; i < 15; i++) {
            for (j = 0; j < 16; j++) {
                var_s2 = i * 0x10;
                vectorIndex1 = var_s2 + j;
                vectorIndex2 = var_s2 + ((j + 1) & 0xF);
                gte_ldv0(&D_us_801B3634[vectorIndex1]);
                gte_rtps();
                gte_stsxy((long*)&prim->x0);
                gte_ldrgb(&scratchColor[i]);
                gte_dpcs();
                gte_strgb(&prim->r0);
                prim->type = PRIM_GT4;
                gte_ldv0(&D_us_801B3634[vectorIndex2]);
                gte_rtps();
                gte_stsxy((long*)&prim->x1);
                gte_ldrgb(&scratchColor[i]);
                gte_dpcs();
                if ((prim->y0 >= 0) || (prim->y1 >= 0)) {
                    gte_strgb(&prim->r1);
                    gte_ldv0(&D_us_801B3634[vectorIndex1] + 0x10);
                    gte_rtps();
                    if (prim->y0 <= 0x100 || prim->y1 <= 0x100) {
                        gte_stsxy((long*)&prim->x2);
                        gte_ldrgb(&scratchColor[i] + 1);
                        gte_dpcs();
                        gte_strgb(&prim->r2);
                        gte_ldv0(&D_us_801B3634[vectorIndex2] + 0x10);
                        gte_rtps();
                        gte_stsxy((long*)&prim->x3);
                        gte_ldrgb(&scratchColor[i] + 1);
                        gte_dpcs();
                        if (prim->x0 <= 0x100 || prim->x1 <= 0x100 ||
                            prim->x2 <= 0x100 || prim->x3 <= 0x100) {
                            gte_strgb(&prim->r3);
                            if (prim->x0 >= 0 || prim->x1 >= 0 ||
                                prim->x2 >= 0 || prim->x3 >= 0) {
                                gte_stszotz((long*)&zSize);
                                if (zSize < 0x55) {
                                    prim->priority = 0x58 - zSize;
                                } else {
                                    prim->priority = 3;
                                }
                                prim->drawMode = DRAW_COLORS;
                                prim = prim->next;
                            }
                        }
                    }
                }
            }
        }
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
    }
}

SVECTOR D_us_8019D0AC = {0};

extern s32 D_us_801806E4;
extern s32 D_us_80181484;
extern s32 D_us_80181488;
extern s16 D_us_8018148C[][4];
extern SVECTOR* D_us_8018150C[];
extern s16 D_us_8018162C[][3];
extern SVECTOR D_us_801B3F34;
extern s16 D_us_801B3F3C;
extern s16 D_us_801B3F40;
extern s16 D_us_801B3F44;
extern s32 D_us_801B3F48;

// res = lhs * res, done column by column like the GTE MVMVA instruction
static void MulMatrixColumns(MATRIX* lhs, MATRIX* res) {
    s32 col;
    s32 row;
    s32 v[3];

    for (col = 0; col < 3; col++) {
        for (row = 0; row < 3; row++) {
            v[row] = (lhs->m[row][0] * res->m[0][col] +
                      lhs->m[row][1] * res->m[1][col] +
                      lhs->m[row][2] * res->m[2][col]) >>
                     0xC;
        }
        for (row = 0; row < 3; row++) {
            res->m[row][col] = v[row];
        }
    }
}

// The 3D background of the final room: a rotating polyhedron rendered into
// VRAM and the four crystals orbiting around it. It also drives the ending
// collapse once Dracula is defeated.
void func_us_801B1738(Entity* self) {
    ET_Rbo6Castle3D* ext = RBO6_EXT(self, ET_Rbo6Castle3D);
    DRAWENV drawEnv;
    RECT clip;
    MATRIX m;
    MATRIX crystalMatrix;
    SVECTOR rot;
    SVECTOR crystalVerts[2];
    CVECTOR color;
    u32 crystalSxy[7][2];
    s32 crystalZ[7];
    u8 crystalColors[7];
    Primitive* prim;
    Entity* entity;
    s16* debris;
    s16 primIndex;
    s32 i;
    s32 j;
    s32 opz;
    s32 sz;
    s32 zOffset;
    s32 priority;
    s32 angle;
    s16 size;
    u8 negCount;
    u16 u;

    FntPrint("d_step %x\n", self->step);
    switch (self->step) {
    case 0:
        InitializeEntity(&g_EInitInteractable);
        self->zPriority = 0x80;
        self->flags |= FLAG_UNK_10000;
        g_Tilemap.flags &= 0xFFFE;
        g_BgLayers[0].flags &= 0xFFFE;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x2E);
        if (primIndex == -1) {
            self->step = 0;
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->flags |= FLAG_HAS_PRIMS;
        if (g_api.func_800EDB08((POLY_GT4*)prim) == NULL) {
            g_api.FreePrimitives(primIndex);
            self->step = 0;
            self->flags &= ~FLAG_HAS_PRIMS;
            return;
        }
        prim->type = PRIM_ENV;
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
        g_api.func_800EDB08((POLY_GT4*)prim);
        if (prim == NULL) {
            g_api.FreePrimitives(primIndex);
            self->step = 0;
            self->flags &= ~FLAG_HAS_PRIMS;
            return;
        }
        prim->type = PRIM_ENV;
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
        ext->prim = prim;
        for (i = 0; i < 20; i++) {
            prim->type = PRIM_GT3;
            prim->tpage = 0x114;
            prim->clut = 0x194;
            prim->u0 = 0xC0;
            prim->v0 = 0x0D;
            prim->u1 = 0x90;
            prim->v1 = 0x60;
            prim->u2 = 0xF0;
            prim->v2 = 0x60;
            prim->priority = 0x80;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        ext->crystalPrim = prim;
        while (prim != NULL) {
            prim->priority = 0x80;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        for (i = 0; i < 4; i++) {
            ext->crystalAngle[i] = D_us_8018148C[i][3];
        }
        self->scaleX = 0x100;
        D_us_801B3F3C = 0x100;
        D_us_801B3F40 = 0x200;
        D_us_801B3F44 = 0x700;
        ext->rotSpeedX = 4;
        ext->rotSpeedY = 8;
        D_us_801B3F48 = 0;
        self->rotate = 2;
        ext->collapse = 0;
        break;
    case 1:
        if (g_CutsceneFlags & 0x20) {
            D_us_80181484 = 1;
            D_us_801B3F48 = 0x10;
            self->step++;
        }
        break;
    case 2:
        D_us_801B3F48++;
        if (D_us_801B3F48 > 0x80) {
            D_us_801B3F48 = 0x80;
            self->step++;
        }
        break;
    case 3:
        if (D_us_801806E4 & 2) {
            self->step++;
        }
        break;
    case 4:
        if (ext->rotSpeedX < 0x18) {
            ext->rotSpeedX += 2;
        }
        if (ext->rotSpeedY < 0x18) {
            ext->rotSpeedY += 2;
        }
        if (self->rotate < 0x200) {
            self->rotate += 2;
        }
        ext->collapse += 0x50;
        if (ext->collapse > 0x1200) {
            D_us_80181488 = 1;
            ext->timer = 0x40;
            CreateEntityFromCurrentEntity(E_UNK_31, &g_Entities[159]);
            self->step++;
        }
        break;
    case 5:
        if (D_us_801B3F48 < 0xA0) {
            D_us_801B3F48 += 2;
        }
        if (ext->rotSpeedX > 0x10) {
            ext->rotSpeedX -= 6;
        }
        if (ext->rotSpeedY > 0x10) {
            ext->rotSpeedY -= 6;
        }
        if (self->rotate > 0x10) {
            self->rotate -= 0xC;
        }
        if (!--ext->timer) {
            self->step++;
        }
        break;
    case 6:
        D_us_801B3F48 -= 0xA;
        if (D_us_801B3F48 < 0x10) {
            D_us_801B3F48 = 0;
            ext->timer = 0x60;
            self->step++;
        }
        break;
    case 7:
        for (prim = ext->prim; prim != NULL; prim = prim->next) {
            prim->drawMode = DRAW_HIDE;
        }
        if (!--ext->timer) {
            DestroyEntity(self);
        }
        return;
    }

    // render the polyhedron into an off-screen area of the VRAM
    prim = &g_PrimBuf[self->primIndex];
    drawEnv = g_CurrentBuffer->draw;
    drawEnv.isbg = 1;
    drawEnv.r0 = 0;
    drawEnv.g0 = 0;
    drawEnv.b0 = 0;
    clip.x = 0x180;
    clip.y = 0x100;
    clip.w = 0x7F;
    clip.h = 0x80;
    drawEnv.clip = clip;
    drawEnv.ofs[0] = 0x180;
    drawEnv.ofs[1] = 0x100;
    SetDrawEnv(*(DR_ENV**)&prim->r1, &drawEnv);
    prim->priority = 0x27;
    prim->drawMode = DRAW_DEFAULT;
    prim = prim->next;
    i = g_Timer & 3;
    prim->priority = 0x29;
    prim->drawMode = DRAW_UNK_800;

    // debris falling from the ceiling
    if (i < 3 && (rand() & 3)) {
        debris = D_us_8018162C[i];
        entity = AllocEntity(&g_Entities[192], &g_Entities[219]);
        if (entity != NULL) {
            CreateEntityFromCurrentEntity(E_UNK_30, entity);
            entity->zPriority = 0x28;
            entity->rotate = debris[2];
            if (D_us_80181488) {
                entity->rotate = entity->rotate - 0x100 + Random() * 2;
            } else {
                entity->rotate = entity->rotate - 0x80 + Random();
            }
            entity->posX.i.hi = debris[0];
            entity->posY.i.hi = debris[1];
        }
    }

    D_us_801B3F34.vx += ext->rotSpeedX;
    D_us_801B3F34.vy += ext->rotSpeedY;
    D_us_801B3F34.vz += self->rotate;
    i = D_us_801B3F3C;
    j = D_us_801B3F40;
    angle = D_us_801B3F44;
    D_us_801B3F3C = i + 2;
    D_us_801B3F40 = j + 4;
    D_us_801B3F44 = angle + 3;
    if (ext->collapse) {
        D_us_801B3F3C = i + 0xA;
        D_us_801B3F40 = j + 0x14;
        D_us_801B3F44 = angle + 0xF;
    }
    color.r = D_us_801B3F48 + (((D_us_801B3F48 / 2) * rcos(D_us_801B3F3C)) >> 0xC);
    color.g = D_us_801B3F48 + (((D_us_801B3F48 / 2) * rsin(D_us_801B3F40)) >> 0xC);
    color.cd = 5;
    color.b = D_us_801B3F48 + (((D_us_801B3F48 / 2) * rcos(D_us_801B3F44)) >> 0xC);
    SetGeomScreen(0x600);
    SetGeomOffset(0x80, 0x80);
    SetFarColor(0, 0, 0);
    SetFogNear(0x200, 0x600);
    rot = D_us_801B3F34;
    RotMatrix(&rot, &m);
    SetRotMatrix(&m);
    zOffset = ext->collapse - 0x18;
    gte_ldtr(0, 0, zOffset);
    zOffset >>= 2;
    prim = ext->prim;
    for (i = 0; i < 20; i++) {
        gte_ldv3(D_us_8018150C[i * 3], D_us_8018150C[i * 3 + 1],
                 D_us_8018150C[i * 3 + 2]);
        gte_rtpt();
        gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
        prim->r0 = color.r;
        prim->g0 = color.g;
        prim->b0 = color.b;
        prim->type = color.cd;
        prim->drawMode = DRAW_COLORS | DRAW_UNK02;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        gte_nclip();
        gte_stopz(&opz);
        if (!ext->collapse && opz > 0) {
            prim->drawMode = DRAW_HIDE;
        }
        if ((u16)((prim->x0 & 0x100) + (prim->x1 & 0x100) +
                  (prim->x2 & 0x100)) > 0x300 ||
            (u16)((prim->y0 & 0x100) + (prim->y1 & 0x100) +
                  (prim->y2 & 0x100)) > 0x300) {
            prim->drawMode = DRAW_HIDE;
        }
        gte_avsz3();
        gte_stszotz(&sz);
        prim->type = PRIM_GT3;
        priority = 0x80 - ((sz - zOffset) >> 2);
        if (!ext->collapse) {
            priority -= 2;
        }
        if (priority > 0x1F8) {
            priority = 0x1F8;
        }
        if (priority < 4) {
            priority = 4;
        }
        prim->priority = priority;
        prim = prim->next;
    }

    if (!D_us_80181484 || (D_us_801806E4 & 1)) {
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        return;
    }

    // the crystals, with a sound when each of them appears
    for (i = 0; i < 4; i++) {
        ext->crystalAngle[i] += 0x38;
        if ((u16)(ext->crystalAngle[i] - 1) < 0x38) {
            g_api.PlaySfx(0x614);
        }
        if (ext->crystalAngle[i] > 0x2000) {
            ext->crystalAngle[i] = -(ext->crystalAngle[i] & 0xFFF);
        }
    }
    prim = ext->crystalPrim;
    SetGeomScreen(0x600);
    SetGeomOffset(0x80, 0x80);
    gte_ldtr(0, 0, 0x600);
    crystalVerts[0].vx = 0;
    crystalVerts[0].vy = -0xC;
    crystalVerts[1].vx = 0;
    crystalVerts[1].vy = 0xC;
    for (i = 0; i < 4; i++) {
        angle = ext->crystalAngle[i];
        if ((s16)angle < 0) {
            continue;
        }
        rot.vx += D_us_8018148C[i][0];
        rot.vy += D_us_8018148C[i][1];
        rot.vz += D_us_8018148C[i][2];
        RotMatrix(&rot, &crystalMatrix);
        negCount = 0;
        for (j = 0; j < 7; j++) {
            size = 0x90 - (((s16)angle * 0x78) / 6144);
            if (size < 0x18) {
                size = 0x18;
            }
            crystalVerts[0].vz = crystalVerts[1].vz = -size;
            m.m[0][0] = m.m[0][1] = m.m[0][2] = 0;
            m.m[1][0] = m.m[1][1] = m.m[1][2] = 0;
            m.m[2][0] = m.m[2][1] = m.m[2][2] = 0;
            m.m[0][0] = m.m[1][1] = m.m[2][2] = 0x1000;
            RotMatrixY((s16)angle, &m);
            MulMatrixColumns(&crystalMatrix, &m);
            SetRotMatrix(&m);
            // the third vertex is left from the polyhedron, like on PSX
            gte_ldv01c(crystalVerts);
            gte_rtpt();
            gte_stsxy01c(crystalSxy[j]);
            gte_stszotz(&crystalZ[j]);
            if ((s16)angle < 0) {
                negCount++;
                if (negCount > 4) {
                    negCount = 4;
                }
                crystalColors[j] = -0x80 - (negCount << 5);
            } else {
                negCount = 0;
                crystalColors[j] = 0x80;
            }
            angle -= ratan2(0x10, size);
            if ((s16)angle > 0x1800) {
                crystalColors[j] = 0;
            }
        }
        for (j = 0; j < 6; j++) {
            u = j * 0x10;
            prim->tpage = 0x1A;
            prim->u0 = prim->u2 = u + 0x90;
            prim->u1 = prim->u3 = u + 0x9F;
            prim->clut = 0x194;
            if (g_Timer & 2) {
                prim->v0 = prim->v1 = 0xC0;
                prim->v2 = prim->v3 = 0xD0;
            } else {
                prim->v0 = prim->v1 = 0xD0;
                prim->v2 = prim->v3 = 0xC0;
            }
            LOW(prim->x0) = crystalSxy[j][0];
            LOW(prim->x1) = crystalSxy[j + 1][0];
            LOW(prim->x2) = crystalSxy[j][1];
            LOW(prim->x3) = crystalSxy[j + 1][1];
            prim->drawMode = DRAW_UNK02;
            prim->priority = (u16)crystalZ[j] - 0x100;
            if (crystalColors[j + 1] != 0x80) {
                prim->r0 = prim->g0 = prim->b0 = crystalColors[j];
                LOW(prim->r2) = LOW(prim->r0);
                prim->r1 = prim->g1 = prim->b1 = crystalColors[j + 1];
                prim->drawMode |= DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                  DRAW_TRANSP;
                LOW(prim->r3) = LOW(prim->r1);
            }
            prim = prim->next;
        }
    }
    while (prim != NULL) {
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
    }
}
#endif

void func_us_801B2864(Entity* self) {
    extern u16 g_EInitInteractable;
    extern s32 D_us_80181488;
    extern u8 D_us_80181648;
    u16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(&g_EInitInteractable);
        self->palette = 0x2E5;
        self->animSet = 0xE;
        self->unk5A = 0x79;
        self->drawFlags = ENTITY_ROTATE;
        self->flags |= FLAG_UNK_10000;
        self->blendMode = BLEND_QUARTER | BLEND_TRANSP;
        self->flags &= ~FLAG_POS_CAMERA_LOCKED;
        self->facingLeft = Random() & 1;
        angle = self->rotate;
        if (self->facingLeft != 0) {
            self->rotate = -angle;
        }
        self->velocityX = rsin((s32)(s16)angle) * 0x28;
        self->velocityY = -(rcos((s32)(s16)angle) * 0x28);
        self->scaleX = 0xE0;
        self->drawFlags |= ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleY = 0x140;
        if (D_us_80181488 != 0) {
            self->scaleX = 0x120;
            self->scaleY = 0x120;
        }
        /* fall through */
    case 1:
        MoveEntity();
        if (AnimateEntity(&D_us_80181648, self) == 0) {
            DestroyEntity(self);
        }
        break;
    }
}
