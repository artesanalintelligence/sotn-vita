// SPDX-License-Identifier: AGPL-3.0-or-later
// Nose Devil familiar. Its code is instruction-for-instruction the same as the
// Demon (TT_003); only the data tables, the sprite parts and the idle voice
// randomizer (empty in TT_003) are different.
// This file is the C equivalent of the INCLUDE_ASM functions in nosedevil.c
// and is used by the PC port.
#define DEMON_DATA_H "../tt_006/nosedevil_data.h"
#define DEMON_IDLE_SFX_TABLE g_NoseDemonIdleSfx
#include "../tt_003/demon.c"
