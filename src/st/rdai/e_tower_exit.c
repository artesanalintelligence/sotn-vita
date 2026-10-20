// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rdai.h"

#ifndef NON_MATCHING
INCLUDE_ASM("st/rdai/nonmatchings/e_tower_exit", func_us_801B1C34);
#else
extern EInit g_EInitCommon;

// Inverted counterpart of DAI's EntityTowerExit: handles screen transitions
// that are not at the edge of the room when leaving the towers. Unlike DAI,
// the RDAI version leaves debug prints in and runs its logic only once.
void func_us_801B1C34(Entity* self) {
    s16 params = self->params;
    s16 unkX;
    s16 offsetY;

    FntPrint("set:%04x\n", params);
    FntPrint("sx:%04x\n", g_Tilemap.left);
    FntPrint("ex:%04x\n", g_Tilemap.right);
    unkX = g_PlayerX - (g_Player.unk7C - PLAYER.posX.i.hi);

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        self->animCurFrame = 1;
        self->animSet = 0;
        self->zPriority = 0xB0;
        break;
    case 1:
        offsetY = self->posY.i.hi - PLAYER.posY.i.hi;
        if (abs(offsetY) >= 48) {
            break;
        }
        if (g_Player.status & PLAYER_STATUS_DEAD) {
            PLAYER.velocityX = 0;
            return;
        }
        switch (params) {
        case 0:
        case 11:
            if (unkX < 0x180 && g_PlayerX < 0x180) {
                g_Tilemap.x = 0x180;
                g_Tilemap.left++;
                self->step++;
            }
            break;
        case 2:
        case 4:
        case 13:
        case 14:
            if (unkX < 0x100 && g_PlayerX < 0x100) {
                g_Tilemap.x = 0x100;
                g_Tilemap.left++;
                self->step++;
            }
            break;
        case 1:
        case 10:
        case 12:
            if (unkX > 0x280 && g_PlayerX > 0x280) {
                g_Tilemap.width = 0x280;
                g_Tilemap.right--;
                self->step++;
            }
            break;
        case 3:
            if (unkX > 0x300 && g_PlayerX > 0x300) {
                g_Tilemap.width = 0x300;
                g_Tilemap.right--;
                self->step++;
            }
            break;
        case 5:
            if (unkX > 0x480 && g_PlayerX > 0x480) {
                g_Tilemap.width = 0x480;
                self->step++;
            }
            break;
        case 6:
        case 7:
        case 9:
            if (unkX < 0x80 && g_PlayerX < 0x80) {
                g_Tilemap.x = 0x80;
                self->step++;
            }
            break;
        case 8:
            if (unkX > 0x280 && g_PlayerX > 0x280) {
                g_Tilemap.width = 0x280;
                self->step++;
            }
            break;
        }
        break;
    }
}
#endif
