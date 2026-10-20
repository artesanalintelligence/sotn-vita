// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo2/nonmatchings/unk_26EF8", func_us_801A6EF8);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_26EF8", func_us_801A7340);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_26EF8", func_us_801A75B4);
#else
extern EInit g_EInitCommon;
extern s32 D_us_80180AAC;
extern s32 D_us_80180B74;
extern s32 g_CutsceneFlags;
extern s32 g_SkipCutscene;
extern u8 D_us_80180C28[];
extern u8 D_us_80180C30[];
extern u8 D_us_80180C50[];
extern u8 D_us_80180C8C[];
extern u8 D_us_80180C98[];

// Holds the player still while the boss intro cutscene plays
void func_us_801A6EF8(Entity* self) {
    s16 posX;

    posX = PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi;
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitSpawner);
        self->animSet = 0;
        return;
    case 1:
        if (posX <= 0x60 && !self->params) {
            return;
        }
        if (!self->params) {
            D_us_80180B74 = 1;
            PLAYER.posX.i.hi = 0x60;
        }
        g_PauseAllowed = false;
        g_unkGraphicsStruct.pauseEnemies = true;
        if (g_Player.status & PLAYER_STATUS_BAT_FORM) {
            g_Player.padSim = 8;
        } else if (g_Player.status & PLAYER_STATUS_MIST_FORM) {
            g_Player.padSim = 4;
        } else if (g_Player.status & PLAYER_STATUS_WOLF_FORM) {
            g_Player.padSim = 2;
        } else {
            g_Player.padSim = PAD_NONE;
        }
        g_Player.demo_timer = 1;
        if (self->params) {
            self->step = 6;
            return;
        }
        g_CutsceneFlags |= 1;
        self->step++;
        return;
    case 2:
        g_Player.padSim = PAD_NONE;
        if (g_Player.status & PLAYER_STATUS_WOLF_FORM) {
            if (g_Timer & 1) {
                g_Player.padSim = 2;
            }
        } else if (g_CutsceneFlags & 0x80) {
            g_Player.padSim = PAD_RIGHT;
            self->step++;
        }
        g_Player.demo_timer = 1;
        break;
    case 3:
        if (posX < 0xE8) {
            g_Player.padSim = PAD_RIGHT;
        } else {
            g_Player.padSim = PAD_NONE;
            PLAYER.posX.i.hi = 0xE8 - g_Tilemap.scrollX.i.hi;
            if (g_Status.wornEquipment[1] != 0x19) {
                AnimateEntity(D_us_80180C8C, self);
                PLAYER.animCurFrame = self->animCurFrame;
            }
            self->step = 4;
        }
        g_Player.demo_timer = 1;
        break;
    case 4:
        if (g_Status.wornEquipment[1] != 0x19) {
            AnimateEntity(D_us_80180C8C, self);
            PLAYER.animCurFrame = self->animCurFrame;
        }
        g_Player.padSim = PAD_NONE;
        g_Player.demo_timer = 1;
        if (g_CutsceneFlags & 0x40) {
            SetStep(5);
            return;
        }
        break;
    case 5:
        if (!AnimateEntity(D_us_80180C98, self)) {
            g_PauseAllowed = true;
            g_unkGraphicsStruct.pauseEnemies = false;
            D_us_80180AAC = 1;
            DestroyEntity(self);
            return;
        }
        if (g_Status.wornEquipment[1] != 0x19) {
            PLAYER.animCurFrame = self->animCurFrame;
        }
        g_Player.padSim = PAD_NONE;
        g_Player.demo_timer = 1;
        break;
    case 6:
        if (g_Player.status & PLAYER_STATUS_WOLF_FORM && g_Timer & 1) {
            g_Player.padSim = 2;
        }
        if (g_Player.vram_flag & 1) {
            g_CutsceneFlags |= 1;
            self->step++;
        }
        g_Player.padSim = PAD_NONE;
        g_Player.demo_timer = 1;
        break;
    case 7:
        if (g_Player.status & PLAYER_STATUS_WOLF_FORM && g_Timer & 1) {
            g_Player.padSim = 2;
        }
        if (g_CutsceneFlags & 0x1000) {
            g_PauseAllowed = true;
            g_unkGraphicsStruct.pauseEnemies = false;
            DestroyEntity(self);
            return;
        }
        g_Player.padSim = PAD_NONE;
        g_Player.demo_timer = 1;
        break;
    }
}

// Figure fading in and out during the boss intro dialogue
void func_us_801A7340(Entity* self) {
    if (g_SkipCutscene && self->step >= 3 && self->step < 6) {
        SetStep(6);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        self->animSet = ANIMSET_OVL(5);
        self->animCurFrame = 1;
        self->unk5A = 0x5A;
        self->palette = 0x22A;
        self->drawFlags = ENTITY_OPACITY;
        self->opacity = 0;
        return;
    case 1:
        if (g_CutsceneFlags & 0x80) {
            SetStep(2);
        }
        return;
    case 2:
        if (self->opacity < 0x80) {
            self->opacity++;
        }
        AnimateEntity(D_us_80180C28, self);
        if (g_CutsceneFlags & 4) {
            self->animCurFrame = 1;
            SetStep(3);
        }
        break;
    case 3:
        if (g_CutsceneFlags & 8) {
            SetStep(4);
        }
        break;
    case 4:
        AnimateEntity(D_us_80180C30, self);
        if (g_CutsceneFlags & 0x20) {
            SetStep(5);
        }
        break;
    case 5:
        AnimateEntity(D_us_80180C50, self);
        if (g_CutsceneFlags & 0x100) {
            self->animCurFrame = 1;
            SetStep(6);
        }
        break;
    case 6:
        AnimateEntity(D_us_80180C28, self);
        if (self->opacity > 8) {
            self->opacity--;
            return;
        }
        self->step = 7;
        self->palette = PAL_FILL_WHITE;
        self->blendMode = BLEND_SUB | BLEND_TRANSP;
        self->opacity = 0x60;
        return;
    case 7:
        AnimateEntity(D_us_80180C28, self);
        if (self->opacity) {
            self->opacity -= 2;
            return;
        }
        DestroyEntity(self);
        break;
    }
}

// Debug: TRIANGLE on the second pad replays the intro cutscene
void func_us_801A75B4(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitSpawner);
        break;
    case 1:
        if (g_pads[1].tapped & PAD_TRIANGLE) {
            CreateEntityFromCurrentEntity(E_CUTSCENE_DIALOGUE, &g_Entities[200]);
            g_Entities[200].params = 1;
            CreateEntityFromCurrentEntity(E_UNK_26, &g_Entities[96]);
            g_Entities[96].params = 1;
            self->step++;
        }
        break;
    }
}
#endif
