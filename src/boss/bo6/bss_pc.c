// SPDX-License-Identifier: AGPL-3.0-or-later
// PC only: BSS variables whose size differs from PSX (they hold pointers),
// so they can not live in the contiguous BSS copy of d_pc_bss.c.
#include "bo6.h"
#include <cutscene.h>

s32 g_SkipCutscene;
Dialogue g_Dialogue;
u32 g_CutsceneFlags;
PlayerState g_Ric;
s32 g_IsCutsceneDone;
