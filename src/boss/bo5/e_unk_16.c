// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_unk_16", func_801A8620);
#else
extern EInit g_EInitCommon;

// Widens the camera bounds once the player walks past a given point
void func_801A8620(Entity* self) {
    s16 dist;
    s16 params = self->params;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        break;
    case 1:
        dist = self->posY.i.hi - PLAYER.posY.i.hi;
        if (dist < 0) {
            dist = -dist;
        }
        if (dist >= 0x20) {
            break;
        }
        switch (params) {
        case 0:
        case 11:
            if (g_PlayerX > 0x280) {
                g_Tilemap.width = 0x280;
                g_Tilemap.right--;
                self->step++;
            }
            break;
        case 2:
        case 4:
        case 13:
        case 14:
            if (g_PlayerX > 0x300) {
                g_Tilemap.width = 0x300;
                g_Tilemap.right--;
                self->step++;
            }
            break;
        case 1:
        case 10:
        case 12:
            if (g_PlayerX < 0x180) {
                g_Tilemap.x = 0x180;
                g_Tilemap.left++;
                self->step++;
            }
            break;
        case 3:
            if (g_PlayerX < 0x100) {
                g_Tilemap.x = 0x100;
                g_Tilemap.left++;
                self->step++;
            }
            break;
        case 5:
        case 8:
            if (g_PlayerX < 0x80) {
                g_Tilemap.x = 0x80;
                self->step++;
            }
            break;
        case 6:
        case 7:
            if (g_PlayerX > 0x480) {
                g_Tilemap.width = 0x480;
                self->step++;
            }
            break;
        case 9:
            if (g_PlayerX > 0x280) {
                g_Tilemap.width = 0x280;
                self->step++;
            }
            break;
        }
        break;
    }
}
#endif
