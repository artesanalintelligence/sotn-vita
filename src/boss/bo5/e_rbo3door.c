// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_rbo3door", EntityRbo3Door);
#else
extern s32 D_us_801806DC;
extern EInit g_EInitEnvironment;

// Gates closing the Hippogryph arena while the boss is alive
void EntityRbo3Door(Entity* self) {
    Entity* next;
    s32 fgIndex;
    s32 i;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->zPriority = 0x5C;
        if (self->params & 2) {
            self->animCurFrame = 0xE;
            return;
        }
        if (self->params & 1) {
            self->animCurFrame = 0xD;
            self->posX.i.hi = 0x1D0 - g_Tilemap.scrollX.i.hi;
        } else {
            self->animCurFrame = 0xC;
            self->posX.i.hi = 0x10 - g_Tilemap.scrollX.i.hi;
        }
        self->posY.i.hi = 0x20 - g_Tilemap.scrollY.i.hi;
        next = self + 1;
        CreateEntityFromEntity(E_RBO3DOOR, self, next);
        next->params = 2;
        next->posY.i.hi = 0xA0;
        if (self->params) {
            next->posX.i.hi += 0x10;
        }
        break;
    case 1:
        if (D_us_801806DC) {
            if (self->params & 2) {
                self->step = 8;
                break;
            }
            fgIndex = self->params ? 0xDE : 0xC1;
            for (i = 0; i < 4; i++) {
                g_Tilemap.fg[fgIndex] = 0x4B3;
                fgIndex += 0x20;
            }
            self->step = 2;
        }
        break;
    case 2:
        self->posY.val += FIX(1.125);
        if (self->posY.val > 0x460000) {
            self->posY.val = 0x460000;
            self->step++;
        }
        break;
    case 3:
        if (!D_us_801806DC) {
            fgIndex = self->params ? 0xDE : 0xC1;
            for (i = 0; i < 4; i++) {
                g_Tilemap.fg[fgIndex] = 0;
                fgIndex += 0x20;
            }
            self->step++;
        }
        break;
    case 4:
        self->posY.val -= FIX(0.75);
        if (self->posY.i.hi + g_Tilemap.scrollY.i.hi < 0x20) {
            self->posY.i.hi = 0x20 - g_Tilemap.scrollY.i.hi;
            self->step++;
        }
        break;
    case 8:
        self->posY.val -= FIX(1.125);
        if (self->posY.val < 0x810000) {
            self->posY.val = 0x810000;
            self->step++;
        }
        break;
    case 9:
        if (!D_us_801806DC) {
            self->step++;
        }
        break;
    case 10:
        self->posY.val += FIX(0.75);
        if (self->posY.i.hi + g_Tilemap.scrollY.i.hi > 0xA0) {
            self->posY.i.hi = 0xA0 - g_Tilemap.scrollY.i.hi;
            self->step++;
        }
        break;
    }
}
#endif
