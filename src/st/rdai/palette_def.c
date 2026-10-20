// SPDX-License-Identifier: AGPL-3.0-or-later
// The PSX build still uses the raw data from splat, only the PC port
// builds these from the extracted assets
#include "rdai.h"

u16 pal_8C[] = {
#include <gen/pal_8C.h>
};
u16 pal_9F20[] = {
#include <gen/pal_9F20.h>
};
u16 pal_A1E0[] = {
#include <gen/pal_A1E0.h>
};
u16 pal_A220[] = {
#include <gen/pal_A220.h>
};
u16 pal_A420[] = {
#include <gen/pal_A420.h>
};
u16 pal_A4E0[] = {
#include <gen/pal_A4E0.h>
};
u16 pal_A540[] = {
#include <gen/pal_A540.h>
};
u16 pal_A640[] = {
#include <gen/pal_A640.h>
};

#include <gen/palette_def.h>
