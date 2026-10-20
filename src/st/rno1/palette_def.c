// SPDX-License-Identifier: AGPL-3.0-or-later
// The PSX build keeps the palette definitions and the palettes they point to
// as raw data (see the splat config); the PC port builds them from the
// extracted assets instead.
#include "rno1.h"

u16 pal_C680[] = {
#include "gen/pal_C680.h"
};
u16 pal_C6A0[] = {
#include "gen/pal_C6A0.h"
};
u16 pal_C6C0[] = {
#include "gen/pal_C6C0.h"
};
u16 pal_C6E0[] = {
#include "gen/pal_C6E0.h"
};
u16 pal_C7A0[] = {
#include "gen/pal_C7A0.h"
};
u16 pal_C800[] = {
#include "gen/pal_C800.h"
};
u16 pal_C820[] = {
#include "gen/pal_C820.h"
};
u16 pal_CAE0[] = {
#include "gen/pal_CAE0.h"
};
u16 pal_CB20[] = {
#include "gen/pal_CB20.h"
};
u16 pal_CBC0[] = {
#include "gen/pal_CBC0.h"
};
u16 pal_CC20[] = {
#include "gen/pal_CC20.h"
};
u16 pal_CD20[] = {
#include "gen/pal_CD20.h"
};

#include <gen/palette_def.h>
