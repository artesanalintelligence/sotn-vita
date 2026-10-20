// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo2/nonmatchings/e_boss_torch", EntityBossTorch);
#else
// Same torch as the Colosseum (ARE) boss room
#define g_EInitEnvironment D_us_80180524
enum TorchPalettes {
    PAL_TORCH_A = 0x5A,
    PAL_TORCH_B = 0x5B,
};

#include "../../st/are/e_boss_torch.h"
#endif
