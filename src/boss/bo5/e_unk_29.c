// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_unk_29", func_us_801A4494);
#else
extern s32 D_us_801806DC;
extern s32 D_us_80180B74;
extern EInit g_EInitInteractable;

// Boss room music: switches to the boss theme when the fight starts
void func_us_801A4494(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        // fallthrough
    case 1:
        if (D_us_801806DC) {
            stopMusicFlag = 1;
            currentMusicId = 0x334;
            self->step++;
        }
        break;
    case 2:
        if (!g_api.func_80131F68()) {
            stopMusicFlag = 0;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        break;
    case 3:
        if (!D_us_80180B74) {
            g_api.PlaySfx(0x91);
            self->step++;
        }
        break;
    case 4:
        break;
    }
}
#endif
