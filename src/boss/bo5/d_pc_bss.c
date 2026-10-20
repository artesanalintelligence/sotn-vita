// SPDX-License-Identifier: AGPL-3.0-or-later
// PC-only definitions of the cutscene variables that the PSX build keeps in
// the overlay's unnamed bss.
#include "bo5.h"
#include <cutscene.h>

u32 g_SkipCutscene;
Dialogue g_Dialogue;
u32 g_CutsceneFlags;
s32 g_IsCutsceneDone;
