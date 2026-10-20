// SPDX-License-Identifier: AGPL-3.0-or-later
// The PSX build keeps this data as raw bytes (see the splat config); the PC
// port builds it from the extracted assets instead.
#include "rare.h"

u16 pal_125F4[] = {
#include <gen/pal_125F4.h>
};
u16 pal_128B4[] = {
#include <gen/pal_128B4.h>
};
u16 pal_12A34[] = {
#include <gen/pal_12A34.h>
};
u16 pal_12B74[] = {
#include <gen/pal_12B74.h>
};
u16 pal_12C34[] = {
#include <gen/pal_12C34.h>
};
u16 pal_12C54[] = {
#include <gen/pal_12C54.h>
};

#include <gen/palette_def.h>
