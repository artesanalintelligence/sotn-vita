// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rcat.h"

// PSX and PSP keep the palettes as raw data (see the splat configs), only the
// PC port builds them from the extracted assets
u16 pal_02F[] = {
#include <gen/pal_02F.h>
};
u16 pal_07C[] = {
#include <gen/pal_07C.h>
};
u16 pal_091[] = {
#include <gen/pal_091.h>
};
u16 pal_096[] = {
#include <gen/pal_096.h>
};
u16 pal_094[] = {
#include <gen/pal_094.h>
};
u16 pal_09C[] = {
#include <gen/pal_09C.h>
};
u16 pal_3CC[] = {
#include <gen/pal_3CC.h>
};
u16 pal_0A0[] = {
#include <gen/pal_0A0.h>
};
u16 pal_21A[] = {
#include <gen/pal_21A.h>
};
u16 pal_22A[] = {
#include <gen/pal_22A.h>
};
u16 pal_235[] = {
#include <gen/pal_235.h>
};
u16 pal_frozen_half[] = {
#include <gen/pal_frozen_half.h>
};
u16 pal_salome[] = {
#include <gen/pal_salome.h>
};
u16 pal_breakable[] = {
#include <gen/pal_breakable.h>
};
u16 pal_blood_skeleton[] = {
#include <gen/pal_blood_skeleton.h>
};
u16 pal_bat[] = {
#include <gen/pal_bat.h>
};
u16 pal_skeleton[] = {
#include <gen/pal_skeleton.h>
};

#include <gen/palette_def.h>
