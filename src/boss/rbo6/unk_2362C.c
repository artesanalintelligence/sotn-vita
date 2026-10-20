// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo6.h"

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", func_us_801A362C);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", func_us_801A367C);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", func_us_801A37B4);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", func_us_801A399C);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", func_us_801A3BE0);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", func_us_801A4028);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", func_us_801A9208_from_bo6);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", DecreaseBrightness);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", func_us_801A4594);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2362C", func_us_801A4F14);
#else
extern EInit g_EInitInteractable;
extern EInit g_EInitParticle;
extern u32 g_CutsceneFlags;
extern s32 D_us_80180914;
extern s32 D_us_8018091C;
extern s32 D_us_80180920;

void EntityCutscene(Entity* self);

// Runs the cutscene of the second script, or the one of the room layout
void func_us_801A362C(Entity* self) {
    if (D_8003C73C) {
        EntityCutscene(self);
    }
    if (self->entityId) {
        EntityCutscene(self);
    }
}

// Keeps Alucard still while the epilogue plays
void func_us_801A367C(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        g_PauseAllowed = false;
        g_unkGraphicsStruct.pauseEnemies = true;
        PLAYER.facingLeft = 0;
        // the pose is not forced while wearing the body equipment 0x19
        if (g_Status.wornEquipment[1] != 0x19) {
            PLAYER.animCurFrame = 0xBF;
        }
        PLAYER.posX.i.hi = 0x70;
        g_Player.padSim = 0;
        g_Player.demo_timer = 1;
        break;
    case 1:
        if (g_Status.wornEquipment[1] != 0x19) {
            PLAYER.animCurFrame = 0xBF;
        }
        PLAYER.posX.i.hi = 0x70;
        if (g_CutsceneFlags & 2) {
            self->step++;
        }
        g_Player.padSim = 0;
        g_Player.demo_timer = 1;
        break;
    case 2:
        g_PauseAllowed = true;
        g_unkGraphicsStruct.pauseEnemies = false;
        g_Entities[1].ext.afterImage.disableFlag = 1;
        DestroyEntity(self);
        break;
    }
}

// White flash fading out at the beginning of the epilogue
void func_us_801A37B4(Entity* self) {
    Primitive* prim;
    s16 primIndex;

    switch (self->step) {
    case 0:
        primIndex = g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex == -1) {
            return;
        }
        InitializeEntity(g_EInitInteractable);
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->flags |= FLAG_HAS_PRIMS;
        prim->x1 = prim->x3 = 0x100;
        prim->y2 = prim->y3 = 0xF0;
        PCOL(prim) = 0xFF;
        prim->priority = 0x1F8;
        prim->x0 = prim->x2 = 0;
        prim->y0 = prim->y1 = 0;
        prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        break;
    case 1:
        self->step++;
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) = prim->b3 - 4;
        if (prim->r0 < 4) {
            DestroyEntity(self);
            g_CutsceneFlags |= 1;
        }
        break;
    }
}

// White flash triggered by the cutscene, fading out
void func_us_801A399C(Entity* self) {
    Primitive* prim;
    s16 primIndex;

    switch (self->step) {
    case 0:
        primIndex = g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex == -1) {
            return;
        }
        InitializeEntity(g_EInitInteractable);
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->flags |= FLAG_HAS_PRIMS;
        prim->x1 = prim->x3 = 0x100;
        prim->y2 = prim->y3 = 0xF0;
        prim->x0 = prim->x2 = 0;
        prim->y0 = prim->y1 = 0;
        PCOL(prim) = 0;
        prim->priority = 0x80;
        prim->drawMode = DRAW_DEFAULT;
        break;
    case 1:
        if (g_CutsceneFlags & 0x100) {
            prim = &g_PrimBuf[self->primIndex];
            PCOL(prim) = 0xFF;
            prim->drawMode =
                DRAW_UNK_40 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            self->step++;
        }
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) = prim->b3 - 4;
        if (prim->r0 < 4) {
            DestroyEntity(self);
        }
        break;
    }
}

// Light in the middle of the screen during the epilogue
void func_us_801A3BE0(Entity* self) {
    Primitive* prim;
    s16 primIndex;
    u8 color;

    switch (self->step) {
    case 0:
        primIndex = g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex == -1) {
            return;
        }
        InitializeEntity(g_EInitInteractable);
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->flags |= FLAG_HAS_PRIMS;
        prim->x0 = prim->x2 = 0x60;
        prim->x1 = prim->x3 = 0xA0;
        prim->y0 = prim->y1 = 0x40;
        prim->y2 = prim->y3 = 0xC0;
        PCOL(prim) = 0xFF;
        prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        prim->priority = 0x82;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        if (g_CutsceneFlags & 4) {
            PCOL(prim) = prim->b3 - 1;
            if (prim->r0 == 0) {
                PCOL(prim) = 0;
                g_CutsceneFlags |= 0x200;
                self->step++;
            }
        }
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        if (g_CutsceneFlags & 0x80) {
            color = prim->b3;
            prim->b3 = color + 1;
            prim->r0 = prim->r1 = prim->r2 = prim->r3 = color;
            prim->g0 = prim->g1 = prim->g2 = prim->g3 = color;
            prim->b0 = prim->b1 = prim->b2 = color;
            if (prim->r0 > 0xC0) {
                g_CutsceneFlags |= 0x100;
            }
            if (prim->r0 >= 0xFE) {
                PCOL(prim) = 0;
                prim->drawMode = DRAW_DEFAULT;
                SetStep(3);
                g_api.PlaySfx(0x62F);
            }
        }
        break;
    case 3:
        prim = &g_PrimBuf[self->primIndex];
        prim->type = PRIM_G4;
        prim->x1 = prim->x3 = 0x100;
        prim->y0 = prim->y1 = 0x10;
        prim->y2 = prim->y3 = 0xF0;
        prim->x0 = prim->x2 = 0;
        prim->priority = 0x7F;
        prim->drawMode = DRAW_DEFAULT;
        if (self->poseTimer & 1) {
            PCOL(prim) = 0x80;
        } else {
            PCOL(prim) = 0;
        }
        self->poseTimer++;
        if (self->poseTimer > 8) {
            DestroyEntity(self);
            g_CutsceneFlags |= 0x20;
        }
        break;
    }
}

// Dracula's cape, rising and vanishing during the epilogue
void func_us_801A4028(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        self->animSet = ANIMSET_OVL(1);
        self->palette = PAL_DRACULA;
        self->unk5A = 0x48;
        self->animCurFrame = 0x35;
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        self->scaleX = 0x100;
        self->scaleY = 0x100;
        self->zPriority = 0x81;
        break;
    case 1:
        if (g_CutsceneFlags & 0x80) {
            self->velocityY = FIX(0.5);
            self->step++;
        }
        break;
    case 2:
        self->velocityY -= 0x180;
        MoveEntity();
        self->scaleX--;
        self->scaleY--;
        if (g_CutsceneFlags & 0x100) {
            DestroyEntity(self);
        }
        break;
    }
}

// Similar to BO6's func_us_801A9208: flash and fade to the ending
void func_us_801A9208_from_bo6(Entity* self) {
    Primitive* prim;
    s16 primIndex;

    switch (self->step) {
    case 0:
        primIndex = g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex != -1) {
            InitializeEntity(g_EInitInteractable);
            prim = &g_PrimBuf[primIndex];
            g_unkGraphicsStruct.pauseEnemies = true;
            g_PauseAllowed = false;
            self->primIndex = primIndex;
            self->animSet = 0;
            self->flags |= FLAG_HAS_PRIMS;
            prim->x1 = prim->x3 = 0x100;
            prim->y0 = prim->y1 = 4;
            prim->y2 = prim->y3 = 0xE8;
            PCOL(prim) = 0x80;
            prim->priority = 0x1F8;
            prim->x0 = prim->x2 = 0;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            self->ext.utimer.t = 0;
            g_api.PlaySfx(0x62F);
        }
        self->flags |= FLAG_UNK_10000;
        break;
    case 1:
        self->ext.utimer.t++;
        prim = &g_PrimBuf[self->primIndex];
        if (self->ext.utimer.t > 8) {
            self->step++;
            prim->drawMode = DRAW_HIDE;
            DestroyEntity(self);
        } else if (self->ext.utimer.t & 1) {
            prim->drawMode = DRAW_HIDE;
        } else {
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        }
        break;
    case 2:
        if (g_CutsceneFlags & 0x100) {
            g_unkGraphicsStruct.unk28 = 0;
            self->ext.utimer.t = 0;
            self->step++;
        }
        break;
    case 3:
        prim = &g_PrimBuf[self->primIndex];
        self->step++;
        PCOL(prim) = 0;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        break;
    case 4:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) = prim->b3 + 1;
        if (prim->r0 == 0xFF) {
            prim->drawMode = DRAW_COLORS;
            self->step++;
        }
        break;
    case 5:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) = prim->b3 - 4;
        if (prim->r0 < 5) {
            g_PauseAllowed = true;
            g_unkGraphicsStruct.pauseEnemies = false;
            if (g_RoomCount >= 0x737) {
                D_800978B4 = 3;
            } else {
                D_800978B4 = 5;
            }
            g_GameState = Game_Ending;
            g_GameStep = 0;
            self->step++;
        }
        break;
    }
}

// Increases the brightness of the four vertex colours, saturating at 0xFF.
// Returns zero once all the components are saturated.
s32 DecreaseBrightness(Primitive* prim, s32 offset) {
    s32 i;
    s32 j;
    s32 color;
    s32 ret;
    u8* rgb;

    ret = 0;
    rgb = &prim->r0;
    for (i = 0; i < 4; i++, rgb += 12) {
        for (j = 0; j < 3; j++) {
            color = rgb[j] + offset;
            if (color > 0xFF) {
                color = 0xFF;
            } else {
                ret |= 1;
            }
            rgb[j] = color;
        }
    }
    return ret;
}

static void SpawnEndingBubble(Entity* self) {
    Entity* entity;
    s32 x;

    entity = AllocEntity(&g_Entities[64], &g_Entities[TOTAL_ENTITY_COUNT]);
    if (entity != NULL) {
        CreateEntityFromCurrentEntity(E_UNK_32, entity);
        x = Random();
        if (x < 0x20) {
            x += 0x60;
        }
        if (x > 0xE0) {
            x -= 0x80;
        }
        entity->posX.i.hi = x;
        entity->posY.i.hi = 0xF0;
        entity->params = 0;
    }
}

// Ending sequence after defeating Dracula
void func_us_801A4594(Entity* self) {
    ET_Rbo6Ending* ext = RBO6_EXT(self, ET_Rbo6Ending);
    Primitive* prim;
    s16 primIndex;
    s32 color;
    s32 done;
    s32 ret;

    FntPrint("end_step %x\n", self->step);
    g_Player.demo_timer = 0x80;
    g_unkGraphicsStruct.pauseEnemies = true;
    g_Player.padSim = 0;
    g_Player.status |= PLAYER_STATUS_UNK100000;
    g_PauseAllowed = false;
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x20);
        if (primIndex == -1) {
            self->step = 0;
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        ext->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        if (g_Player.status & 2) {
            g_Player.padSim = 4;
        } else if (g_Player.status & 4) {
            g_Player.padSim = 2;
        } else if (g_Player.status & 1) {
            g_Player.padSim = 8;
        }
        // fallthrough
    case 1:
        prim = ext->prim;
        prim->type = PRIM_GT4;
        prim->r0 = prim->g0 = prim->b0 = 0;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim->tpage = 0x1A;
        prim->clut = 0x2E2;
        prim->u1 = prim->u3 = 0x3F;
        prim->v0 = prim->v1 = 0xC0;
        prim->v2 = prim->v3 = 0xFF;
        prim->x0 = prim->x2 = 0x80;
        prim->x1 = prim->x3 = 0x80;
        prim->y0 = prim->y1 = 0x80;
        prim->y3 = 0x80;
        prim->y1 = 0x80; // PSX bug: y2 is left untouched
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        prim->u0 = prim->u2 = 0;
        prim->priority = 0x90;

        prim = prim->next;
        ext->fadePrim = prim;
        prim->type = PRIM_G4;
        prim->x0 = prim->x2 = 0;
        prim->x1 = prim->x3 = 0x100;
        prim->y0 = prim->y1 = 0x10;
        prim->y2 = prim->y3 = 0xF0;
        prim->priority = 0x50;
        prim->drawMode = DRAW_HIDE;

        prim = prim->next;
        ext->lightPrim = prim;
        prim->type = PRIM_G4;
        prim->x0 = prim->x2 = 0x40;
        prim->x1 = prim->x3 = 0;
        prim->y0 = prim->y1 = 0x10;
        prim->y2 = prim->y3 = 0xF0;
        prim->priority = 0x90;
        prim->drawMode = DRAW_HIDE;

        prim = prim->next;
        prim->type = PRIM_G4;
        prim->x0 = prim->x2 = 0xC0;
        prim->x1 = prim->x3 = 0x100;
        prim->y0 = prim->y1 = 0x10;
        prim->y2 = prim->y3 = 0xF0;
        prim->priority = 0x90;
        prim->drawMode = DRAW_HIDE;

        prim = prim->next;
        ext->whitePrim = prim;
        prim->type = PRIM_G4;
        prim->x0 = prim->x2 = 0;
        prim->x1 = prim->x3 = 0x100;
        prim->y0 = prim->y1 = 0x10;
        prim->y2 = prim->y3 = 0xF0;
        prim->priority = 0x1FF;
        prim->drawMode = DRAW_HIDE;
        self->step++;
        // fallthrough
    case 2:
        prim = ext->prim;
        prim->x0 = prim->x2 = prim->x2 - 3;
        prim->x1 = prim->x3 = prim->x3 + 3;
        prim->y0 = prim->y1 = prim->y1 - 3;
        prim->y2 = prim->y3 = prim->y3 + 3;
        color = prim->r0 + 8;
        if (color > 0x60) {
            ext->timer = 0x10;
            g_api.PlaySfx(0x62F);
            color = 0x60;
            self->step++;
        }
        prim->r0 = prim->b0 = prim->g0 = color;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        break;
    case 3:
        prim = ext->prim;
        if (ext->timer & 1) {
            prim->x0 = prim->x2 = prim->x2 - 1;
            prim->x1 = prim->x3 = prim->x3 + 1;
            prim->y0 = prim->y1 = prim->y1 - 1;
            prim->y2 = prim->y3 = prim->y3 + 1;
        }
        if (!--ext->timer) {
            self->step++;
        }
        break;
    case 4:
        prim = ext->prim;
        if (!PrimDecreaseBrightness(prim, 4)) {
            prim->drawMode = DRAW_HIDE;
            self->step++;
        }
        prim = ext->fadePrim;
        prim->r0 += 4;
        prim->b0 += 2;
        prim->g0 += 3;
        prim->drawMode = DRAW_COLORS;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        D_us_80180914 = 1;
        // fallthrough
    case 5:
        prim = ext->lightPrim;
        color = prim->r1;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        if (color < 0xC0) {
            color += 4;
        } else {
            self->step++;
        }
        prim->r1 = prim->b1 = prim->g1 = color;
        LOW(prim->r3) = LOW(prim->r1);
        prim = prim->next;
        prim->r1 = prim->b1 = prim->g1 = color;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        LOW(prim->r3) = LOW(prim->r1);
        if (self->step == 5 && D_us_80180914 < 4 && !(g_Timer & 0xF)) {
            D_us_80180914++;
        }
        break;
    case 6:
        prim = ext->fadePrim;
        prim->drawMode = DRAW_COLORS;
        if (prim->r0 < 0xA0) {
            prim->r0 += 4;
        }
        if (prim->g0 < 0x80) {
            prim->g0 += 3;
        }
        done = true;
        if (prim->g0 < 0x30) {
            prim->b0 += 2;
        }
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);

        prim = ext->lightPrim;
        color = prim->r1 + 1;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        if (color > 0xFF) {
            color = 0xFF;
        }
        prim->r1 = prim->b1 = prim->g1 = color;
        LOW(prim->r3) = LOW(prim->r1);
        if (prim->x0 < 0x80) {
            done = false;
            prim->x0 = ++prim->x2;
        }
        prim = prim->next;
        prim->r1 = prim->b1 = prim->g1 = color;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        LOW(prim->r3) = LOW(prim->r1);
        if (prim->x0 > 0x80) {
            done = false;
            prim->x0 = --prim->x2;
        }
        if (done) {
            self->step++;
        }
        break;
    case 7:
        prim = ext->lightPrim;
        ret = DecreaseBrightness(prim, 2);
        prim = prim->next;
        ret += DecreaseBrightness(prim, 2);
        D_us_80180920 = 0x80 - (prim->r0 >> 1);
        SpawnEndingBubble(self);
        if (!ret) {
            self->step++;
        }
        break;
    case 8:
        if (PLAYER.posX.i.hi > 0x80) {
            D_us_8018091C = PAD_LEFT;
        } else {
            D_us_8018091C = PAD_RIGHT;
        }
        self->step++;
        // fallthrough
    case 9:
        g_Player.padSim = D_us_8018091C;
        prim = ext->whitePrim;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        if (!DecreaseBrightness(prim, 4)) {
            self->step++;
        }
        return;
    case 10:
        prim = ext->whitePrim;
        prim->drawMode = DRAW_DEFAULT;
        if (!PrimDecreaseBrightness(prim, 8)) {
            self->step++;
        }
        return;
    case 11:
        g_PauseAllowed = true;
        g_unkGraphicsStruct.pauseEnemies = false;
        if (g_RoomCount >= 0x737) {
            D_800978B4 = 3;
        } else {
            D_800978B4 = 5;
        }
        g_GameState = Game_Ending;
        g_GameStep = 0;
        self->step++;
        // fallthrough
    case 12:
        g_PauseAllowed = true;
        g_unkGraphicsStruct.pauseEnemies = false;
        return;
    default:
        break;
    }

    // bubbles rising from the bottom of the screen
    if (D_us_80180914) {
        if (!(g_Timer & 7)) {
            if (D_us_80180920) {
                g_api.PlaySfxVolPan(0x69D, D_us_80180920, 0);
            } else {
                g_api.PlaySfx(0x69D);
            }
        }
        if (!(g_Timer & 1)) {
            SpawnEndingBubble(self);
        }
    }
}

// A bubble of the ending, a column of light rising to the top
void func_us_801A4F14(Entity* self) {
    ET_Rbo6Bubble* ext = RBO6_EXT(self, ET_Rbo6Bubble);
    Primitive* prim;
    Primitive* glow;
    s16 primIndex;
    s32 i;
    s32 rnd;
    s32 rnd2;
    u8 width;
    s16 x;
    s16 top;
    s16 left;
    s16 right;
    s16 y;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        self->animCurFrame = 0;
        rnd = Random() % D_us_80180914;
        width = rnd + 2;
        self->hitboxWidth = width;
        self->velocityY =
            FIX(-1.5) - ((Random() % (rnd + 3)) << 0x11);
        rnd2 = Random() % width;
        ext->accelY = -0x1000 - rnd2 * 0x3000;
        self->opacity = ((Random() % (rnd + 1)) * 8) + 0x20;
        if (rnd + 1 >= D_us_80180914 / 2) {
            self->zPriority = 0x90;
        } else {
            self->zPriority = 0x54;
        }
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 7);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        ext->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;

        // the head of the bubble
        prim->tpage = 0x1A;
        prim->clut = 0x2EC;
        prim->u1 = prim->u3 = 0x40;
        prim->v0 = prim->v1 = 0xC0;
        prim->u0 = prim->u2 = 0;
        prim->v2 = prim->v3 = 0xE0;
        prim->r0 = prim->g0 = prim->b0 = self->opacity;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        prim->priority = self->zPriority;
        prim = prim->next;

        // its body
        for (i = 0; i < 4; i++) {
            prim->tpage = 0x1A;
            prim->clut = 0x2EC;
            prim->u0 = prim->u2 = 0;
            prim->u1 = prim->u3 = 0x40;
            prim->v0 = prim->v1 = 0xE0;
            prim->v2 = prim->v3 = 0xE2;
            prim->r0 = prim->g0 = prim->b0 = self->opacity;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim->priority = self->zPriority + 1;
            prim = prim->next;
        }

        // its tail
        prim->u1 = prim->u3 = 0x40;
        prim->tpage = 0x1A;
        prim->clut = 0x2EC;
        prim->u0 = prim->u2 = 0;
        prim->v0 = prim->v1 = 0xE0;
        prim->v2 = prim->v3 = 0xFF;
        prim->r0 = prim->g0 = prim->b0 = self->opacity;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        prim->priority = self->zPriority;
        prim = prim->next;

        // the glow around the head
        ext->glowPrim = prim;
        prim->u1 = prim->u3 = 0x3F;
        prim->tpage = 0x1A;
        prim->clut = 0x2EC;
        prim->u0 = prim->u2 = 0;
        prim->v0 = prim->v1 = 0xC0;
        prim->v2 = prim->v3 = 0xFF;
        prim->r0 = prim->g0 = prim->b0 = self->opacity + 0x20;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                         DRAW_UNK02 | DRAW_TRANSP;
        prim->priority = self->zPriority;
        if (self->hitboxWidth < 4) {
            prim->drawMode = DRAW_HIDE;
        }
        for (i = 0; i < LEN(ext->posYHistory); i++) {
            ext->posYHistory[i] = self->posY.i.hi;
        }
        break;
    case 1:
        MoveEntity();
        self->velocityY += ext->accelY;
        break;
    }

    for (i = LEN(ext->posYHistory) - 1; i > 0; i--) {
        ext->posYHistory[i] = ext->posYHistory[i - 1];
    }
    width = self->hitboxWidth;
    prim = ext->prim;
    x = self->posX.i.hi;
    top = self->posY.i.hi - width;
    left = x - width;
    right = x + width;
    ext->posYHistory[0] = self->posY.i.hi;
    prim->y0 = prim->y1 = top;
    prim->x0 = left;
    prim->x1 = right;
    glow = ext->glowPrim;
    glow->x0 = x - width * 2;
    glow->x1 = x + width * 2;
    glow->y0 = glow->y1 = top - width;
    prim = ext->prim;
    for (i = 0; i < 5; i++) {
        y = ext->posYHistory[i];
        prim->x2 = left;
        prim->x3 = right;
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        prim->y2 = prim->y3 = y;
        prim = prim->next;
        prim->y0 = prim->y1 = y;
        prim->x0 = left;
        prim->x1 = right;
    }
    prim->x2 = x - width;
    prim->x3 = x + width;
    prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
    y += self->hitboxWidth;
    prim->y2 = prim->y3 = y;
    glow = ext->glowPrim;
    glow->x2 = x - width * 2;
    glow->x3 = x + width * 2;
    glow->y2 = glow->y3 = y + self->hitboxWidth * 2;
    if (y < 0x10) {
        DestroyEntity(self);
    }
}
#endif
