// SPDX-License-Identifier: AGPL-3.0-or-later
// On PSX the overlay BSS is still assembly
#include "rbo6.h"
#include <cutscene.h>

s32 g_SkipCutscene;
Dialogue g_Dialogue;
u32 g_CutsceneFlags;
s32 dialogue_started;

// background vortex vertices and brightness
SVECTOR D_us_801B3634[0x100];
u8 D_us_801B3E34[0x100];

// rotation and color phases of the 3D background
SVECTOR D_us_801B3F34;
s16 D_us_801B3F3C;
s16 D_us_801B3F40;
s16 D_us_801B3F44;
s32 D_us_801B3F48;
