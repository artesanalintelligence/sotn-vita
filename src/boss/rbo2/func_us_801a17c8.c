// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo2.h"

#ifdef VERSION_PC
extern EInit g_EInitInteractable;
extern u32 g_CutsceneFlags;
void CutsceneCameraPan(s16 target);
s32 func_us_801A8FC0_from_bo6(void);

// Walks Alucard to the middle of the room and pans the camera for the
// cutscene that precedes the Death fight
void func_us_801A17C8(Entity* self) {
    s16 posX;

    posX = PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi;
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        break;
    case 1:
        if ((u16)(posX - 0x41) < 0x17F) {
            g_unkGraphicsStruct.pauseEnemies = 1;
            g_Entities[E_AFTERIMAGE_1].ext.afterImage.disableFlag = 0;
            g_Player.demo_timer = 1;
            g_PauseAllowed = false;
            self->step++;
            if (!func_us_801A8FC0_from_bo6()) {
                if (posX < 0x100) {
                    g_Player.padSim = PAD_RIGHT;
                } else {
                    g_Player.padSim = PAD_LEFT;
                    self->step++;
                }
            }
        }
        break;
    case 2:
        g_Player.padSim = PAD_NONE;
        if (!func_us_801A8FC0_from_bo6()) {
            if (posX < 0x80) {
                g_Player.padSim = PAD_RIGHT;
            } else {
                g_CutsceneFlags |= 1;
            }
            CutsceneCameraPan(0x40);
            if (g_unkGraphicsStruct.unk14 == 0x40) {
                self->step += 2;
            }
        }
        g_Player.demo_timer = 1;
        break;
    case 3:
        g_Player.padSim = PAD_NONE;
        if (!func_us_801A8FC0_from_bo6()) {
            if (posX > 0x180) {
                g_Player.padSim = PAD_LEFT;
            } else {
                g_CutsceneFlags |= 1;
            }
            CutsceneCameraPan(0xC0);
            if (g_unkGraphicsStruct.unk14 == 0xC0) {
                self->step++;
            }
        }
        g_Player.demo_timer = 1;
        break;
    case 4:
        if (g_CutsceneFlags & 2) {
            if (g_unkGraphicsStruct.pauseEnemies) {
                g_unkGraphicsStruct.pauseEnemies = 0;
            }
            g_Entities[E_AFTERIMAGE_1].ext.afterImage.disableFlag = 1;
            self->step++;
            CutsceneCameraPan(0x80);
        }
        g_Player.demo_timer = 1;
        break;
    case 5:
        g_PauseAllowed = true;
        CutsceneCameraPan(0x80);
        self->step++;
        break;
    case 6:
        CutsceneCameraPan(0x80);
        if (g_unkGraphicsStruct.unk14 == 0x80) {
            DestroyEntity(self);
        }
        break;
    }
}
#else
INCLUDE_ASM("boss/rbo2/nonmatchings/func_us_801a17c8", func_us_801A17C8);
#endif
