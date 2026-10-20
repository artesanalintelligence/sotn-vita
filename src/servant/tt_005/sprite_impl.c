// SPDX-License-Identifier: AGPL-3.0-or-later
// Sprite (Yousei) familiar. Its code is instruction-for-instruction the same as
// the Faerie (TT_002); only the data tables, the sprite parts and the idle
// timers used while sitting on Alucard's shoulder are different.
// This file is the C equivalent of the INCLUDE_ASM functions in sprite.c and is
// used by the PC port.
#define FAERIE_DATA_H "../tt_005/sprite_data.h"
#define FAERIE_FRAME_COUNT1 3600
#define FAERIE_FRAME_COUNT2 1800
#include "../tt_002/faerie.c"
