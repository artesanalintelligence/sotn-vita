// SPDX-License-Identifier: AGPL-3.0-or-later
#include <psyz/module.h>
#include <game.h>
#include <string.h>
#include "overlay.h"
#include "../../st/nz1/nz1.h"

extern Overlay g_Overlay;
extern PfnEntityUpdate EntityUpdates[];
extern LayoutEntity* entityLayoutHorizontal[];
extern LayoutEntity* entityLayoutVertical[];
extern PfnEntityUpdate* PfnEntityUpdates;
extern LayoutEntity** g_pStObjLayoutHorizontal;
extern LayoutEntity** g_pStObjLayoutVertical;

// palettes referenced by palette_def; the PSX build still takes them from the
// raw data that follows stage_data
u16 pal_12DD8[] = {
#include <gen/pal_12DD8.h>
};
u16 pal_12FD8[] = {
#include <gen/pal_12FD8.h>
};
u16 pal_13098[] = {
#include <gen/pal_13098.h>
};
u16 pal_13158[] = {
#include <gen/pal_13158.h>
};
u16 pal_131D8[] = {
#include <gen/pal_131D8.h>
};
u16 pal_13278[] = {
#include <gen/pal_13278.h>
};
u16 pal_13498[] = {
#include <gen/pal_13498.h>
};
u16 pal_134F8[] = {
#include <gen/pal_134F8.h>
};
u16 pal_13558[] = {
#include <gen/pal_13558.h>
};
u16 pal_135D8[] = {
#include <gen/pal_135D8.h>
};
u16 pal_136D8[] = {
#include <gen/pal_136D8.h>
};
u16 pal_13798[] = {
#include <gen/pal_13798.h>
};

void Psyz_ModuleStart(void* param) {
    Overlay* o = param;

    memcpy(o, &g_Overlay, sizeof(Overlay));
    PfnEntityUpdates = EntityUpdates;
    g_pStObjLayoutHorizontal = entityLayoutHorizontal;
    g_pStObjLayoutVertical = entityLayoutVertical;
}

void Psyz_ModuleStop(void) {}
