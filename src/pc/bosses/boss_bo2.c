// SPDX-License-Identifier: AGPL-3.0-or-later
#include <psyz/module.h>
#include <game.h>
#include <cutscene.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stages/overlay.h"
#include "../pc.h"
#include "../boss/bo2/bo2.h"

extern AbbreviatedOverlay g_BossOverlay;
extern PfnEntityUpdate EntityUpdates[];
extern LayoutEntity* entityLayoutHorizontal[];
extern LayoutEntity* entityLayoutVertical[];
extern PfnEntityUpdate* PfnEntityUpdates;
extern LayoutEntity** g_pStObjLayoutHorizontal;
extern LayoutEntity** g_pStObjLayoutVertical;

extern u8 cutscene_data_bo2[];
extern u8 gfx_portrait_bo2_0[];
extern u8 gfx_portrait_bo2_1[];

// The dialogue scripts reference their events and portraits by PSX address
static void InitCutscenePc(void) {
    static const CutsceneSymbolRange symbols[] = {
        {cutscene_data_bo2, 0x80181CCC, 0x180},
        {gfx_portrait_bo2_0, 0x80191E14, 0xD80},
        {gfx_portrait_bo2_1, 0x80192B94, 0xD80},
    };
    CutscenePcAlloc(symbols, LEN(symbols));
}

void Psyz_ModuleStart(void* param) {
    AbbreviatedOverlay* o = param;

    memcpy(o, &g_BossOverlay, sizeof(AbbreviatedOverlay));
    PfnEntityUpdates = EntityUpdates;
    g_pStObjLayoutHorizontal = entityLayoutHorizontal;
    g_pStObjLayoutVertical = entityLayoutVertical;
    InitCutscenePc();
}

void Psyz_ModuleStop(void) {}
