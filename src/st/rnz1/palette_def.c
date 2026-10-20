// SPDX-License-Identifier: AGPL-3.0-or-later
// The PSX build keeps the palette definitions and the palettes they point to
// as raw data (see the splat config); the PC port builds them from the
// extracted assets instead.
#include "rnz1.h"

u16 pal_12884[] = {
#include "gen/pal_12884.h"
};
u16 pal_12B44[] = {
#include "gen/pal_12B44.h"
};
u16 pal_12C04[] = {
#include "gen/pal_12C04.h"
};
u16 pal_12CA4[] = {
#include "gen/pal_12CA4.h"
};
u16 pal_12CE4[] = {
#include "gen/pal_12CE4.h"
};
u16 pal_12D64[] = {
#include "gen/pal_12D64.h"
};
u16 pal_12E64[] = {
#include "gen/pal_12E64.h"
};
u16 pal_12EC4[] = {
#include "gen/pal_12EC4.h"
};
u16 pal_12F84[] = {
#include "gen/pal_12F84.h"
};

#include <gen/palette_def.h>
