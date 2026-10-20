// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_richter_prologue", func_us_801A5F88);
#else
extern s32 D_us_801806DC;
extern u32 g_SkipCutscene;
extern u32 g_CutsceneFlags;
extern EInit g_EInitSpawner;
extern EInit g_EInitObtainable;
void CutsceneCameraPan(s16 target);

// Cutscene director: waits for the Life Max Up to be collected, then walks
// Alucard towards Maria and pans the camera
void func_us_801A5F88(Entity* self) {
    Entity* entity;
    s16 posX;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitSpawner);
        break;
    case 1:
        switch (self->step_s) {
        case 0:
            for (entity = &g_Entities[160]; entity < &g_Entities[192];
                 entity++) {
                if (entity->entityId == E_PRIZE_DROP) {
                    self->ext.prim = (Primitive*)entity;
                    self->step_s++;
                }
            }
            break;
        case 1:
            entity = (Entity*)self->ext.prim;
            if (entity->entityId != E_PRIZE_DROP) {
                D_us_801806DC = 0;
                self->step_s++;
                g_CutsceneFlags |= 0x8000;
            }
            break;
        case 2:
            SetStep(2);
            break;
        }
        break;
    case 2:
        g_PauseAllowed = false;
        g_unkGraphicsStruct.pauseEnemies = 1;
        g_Player.padSim = PAD_RIGHT;
        if (g_Player.status & 4) {
            g_Player.padSim = 2;
        }
        g_Player.demo_timer = 1;
        self->step++;
        break;
    case 3:
        g_Player.padSim = 0;
        if (g_Player.status & 4) {
            if (g_Timer & 1) {
                g_Player.padSim = 2;
            }
        } else {
            g_Player.padSim = PAD_RIGHT;
            posX = PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi;
            if (posX > 0x120 && !(g_CutsceneFlags & 1)) {
                g_CutsceneFlags |= 1;
            }
            if (posX > 0x140) {
                g_Player.padSim = 0;
                PLAYER.posX.i.hi = 0x140 - g_Tilemap.scrollX.i.hi;
                self->step++;
            }
            if (posX < 0x100) {
                posX = 0x100;
            }
            CutsceneCameraPan(posX);
        }
        g_Player.demo_timer = 1;
        break;
    case 4:
        CutsceneCameraPan(0x140);
        if (g_CutsceneFlags & 2) {
            self->step++;
        }
        g_Player.padSim = 0;
        g_Player.demo_timer = 1;
        break;
    case 5:
        CutsceneCameraPan(0x100);
        if (g_unkGraphicsStruct.unk14 == 0x80) {
            g_PauseAllowed = true;
            if (g_unkGraphicsStruct.pauseEnemies) {
                g_unkGraphicsStruct.pauseEnemies = 0;
            }
            DestroyEntity(self);
        }
        g_Player.padSim = 0;
        g_Player.demo_timer = 1;
        break;
    }
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_richter_prologue", func_us_801A62B4);
#else
extern AnimateEntityFrame D_us_80180C24[];
extern AnimateEntityFrame D_us_80180C48[];
extern AnimateEntityFrame D_us_80180C5C[];
extern AnimateEntityFrame D_us_80180C6C[];
extern AnimateEntityFrame D_us_80180C74[];
extern AnimateEntityFrame D_us_80180C84[];
extern AnimateEntityFrame D_us_80180C90[];
extern AnimateEntityFrame D_us_80180C98[];
extern AnimateEntityFrame D_us_80180CA8[];
extern AnimateEntityFrame D_us_80180CC0[];
extern AnimateEntityFrame D_us_80180CCC[];
extern AnimateEntityFrame D_us_80180CD4[];

// Maria during the cutscene, each pose waits for a cutscene flag
void func_us_801A62B4(Entity* self) {
    if (g_SkipCutscene && self->step < 0xD) {
        SetStep(0xD);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitObtainable);
        self->animSet = ANIMSET_OVL(4);
        self->animCurFrame = 1;
        self->unk5A = 0x50;
        self->palette = 0x220;
        self->hitboxState = 0;
        self->posX.i.hi = 0x1C0 - g_Tilemap.scrollX.i.hi;
        break;
    case 1:
        if (g_CutsceneFlags & 4) {
            self->step++;
        }
        break;
    case 2:
        self->animCurFrame = 2;
        if (g_CutsceneFlags & 8) {
            self->step++;
        }
        break;
    case 3:
        AnimateEntity(D_us_80180C24, self);
        if (g_CutsceneFlags & 0x10) {
            SetStep(4);
        }
        break;
    case 4:
        AnimateEntity(D_us_80180C48, self);
        if (g_CutsceneFlags & 0x20) {
            SetStep(5);
        }
        break;
    case 5:
        AnimateEntity(D_us_80180C5C, self);
        if (g_CutsceneFlags & 0x40) {
            SetStep(6);
        }
        break;
    case 6:
        AnimateEntity(D_us_80180C6C, self);
        if (g_CutsceneFlags & 0x80) {
            SetStep(7);
        }
        break;
    case 7:
        AnimateEntity(D_us_80180C74, self);
        if (g_CutsceneFlags & 0x100) {
            SetStep(8);
        }
        break;
    case 8:
        AnimateEntity(D_us_80180C84, self);
        if (g_CutsceneFlags & 0x200) {
            SetStep(9);
        }
        break;
    case 9:
        AnimateEntity(D_us_80180C90, self);
        if (g_CutsceneFlags & 0x400) {
            SetStep(10);
        }
        break;
    case 10:
        AnimateEntity(D_us_80180C98, self);
        if (g_CutsceneFlags & 0x800) {
            SetStep(11);
        }
        break;
    case 11:
        AnimateEntity(D_us_80180CA8, self);
        if (g_CutsceneFlags & 0x1000) {
            SetStep(12);
        }
        break;
    case 12:
        AnimateEntity(D_us_80180CC0, self);
        if (g_CutsceneFlags & 0x2000) {
            SetStep(13);
        }
        break;
    case 13:
        if (!AnimateEntity(D_us_80180CCC, self)) {
            self->velocityX = FIX(0.75);
            SetStep(14);
        }
        break;
    case 14:
        AnimateEntity(D_us_80180CD4, self);
        MoveEntity();
        if (self->posX.i.hi > 0x110) {
            g_CutsceneFlags |= 2;
            DestroyEntity(self);
        }
        break;
    }
}
#endif

void func_us_801A666C(Primitive* prim, s16 arg1) {
    prim->x0 = prim->x2 -= arg1;
    prim->x1 = prim->x3 += arg1;
    prim->y0 = prim->y1 -= arg1;
    prim->y2 = prim->y3 += arg1;
}

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_richter_prologue", func_us_801A66B0);
#else
extern s16 D_us_80180CF0[];
extern s16 D_us_80180D00[];
extern u8 D_us_80180D10[];
extern u8 D_us_80180D20[];
extern u16 D_us_80180D30[];

// Opens and closes the four panels of the cutscene's window effect
void func_us_801A66B0(Entity* self, u16 arg1) {
    Primitive* prim;
    s16* pos;
    u8* uv;
    s16 primIndex;
    s32 i;

    if (g_SkipCutscene) {
        DestroyEntity(self);
        // the original falls through and re-initializes the destroyed
        // entity, leaking its primitives
        return;
    }
    switch (self->step) {
    case 0:
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 4);
        if (primIndex == -1) {
            break;
        }
        InitializeEntity(g_EInitSpawner);
        prim = &g_PrimBuf[primIndex];
        self->animSet = ANIMSET_OVL(0);
        self->primIndex = primIndex;
        self->flags |= FLAG_HAS_PRIMS;
        if (!arg1) {
            pos = D_us_80180CF0;
            uv = D_us_80180D10;
        } else {
            pos = D_us_80180D00;
            uv = D_us_80180D20;
        }
        for (i = 0; i < 4; i++) {
            prim->tpage = 0x14;
            prim->clut = D_us_80180D30[arg1 * 4 + i];
            prim->x0 = prim->x1 = prim->x2 = prim->x3 = pos[0];
            prim->y0 = prim->y1 = prim->y2 = prim->y3 = pos[1];
            prim->u0 = prim->u2 = uv[0];
            prim->v0 = prim->v1 = uv[1];
            prim->u1 = prim->u3 = uv[2];
            prim->v2 = prim->v3 = uv[3];
            prim->drawMode = DRAW_DEFAULT;
            prim->priority = self->zPriority;
            prim = prim->next;
            pos += 2;
            uv += 4;
        }
        self->ext.utimer.t = 0;
        break;
    case 1:
    case 2:
    case 8:
        prim = &g_PrimBuf[self->primIndex];
        if (self->step != 1) {
            prim = prim->next;
        }
        if (self->ext.utimer.t & 1) {
            func_us_801A666C(prim, self->step == 8 ? -1 : 1);
        }
        if (++self->ext.utimer.t >= 8) {
            self->ext.utimer.t = 0;
            self->step++;
        }
        break;
    case 3:
    case 7:
        prim = g_PrimBuf[self->primIndex].next->next;
        func_us_801A666C(prim, self->step == 3 ? 1 : -1);
        if (++self->ext.utimer.t >= 8) {
            self->ext.utimer.t = 0;
            self->step++;
        }
        break;
    case 4:
    case 6:
        prim = g_PrimBuf[self->primIndex].next->next->next;
        func_us_801A666C(prim, self->step == 4 ? 2 : -2);
        if (self->ext.utimer.t & 1) {
            if (self->step == 4) {
                prim->y0 = prim->y1 = prim->y1 - 1;
                prim->y2 = prim->y3 = prim->y3 + 1;
            } else {
                prim->y0 = prim->y1 = prim->y1 + 1;
                prim->y2 = prim->y3 = prim->y3 - 1;
            }
        }
        if (++self->ext.utimer.t >= 8) {
            self->ext.utimer.t = 0;
            self->step++;
        }
        break;
    case 5:
        if (++self->ext.utimer.t >= 0xC0) {
            self->ext.utimer.t = 0;
            self->step++;
        }
        break;
    case 9:
        if (self->ext.utimer.t & 1) {
            func_us_801A666C(&g_PrimBuf[self->primIndex], -1);
        }
        if (++self->ext.utimer.t >= 8) {
            DestroyEntity(self);
        }
        break;
    }
}
#endif

void func_801B1D68(Entity* self) { func_us_801A66B0(self, 0); }

void BO5_RicSetDeadPrologue(Entity* self) { func_us_801A66B0(self, 1); }
