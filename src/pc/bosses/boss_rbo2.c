// SPDX-License-Identifier: AGPL-3.0-or-later
#include <psyz/module.h>
#include <game.h>
#include <cutscene.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stages/overlay.h"
#include "../pc.h"
#include "../boss/rbo2/rbo2.h"

extern AbbreviatedOverlay g_BossOverlay;
extern PfnEntityUpdate EntityUpdates[];
extern LayoutEntity* entityLayoutHorizontal[];
extern LayoutEntity* entityLayoutVertical[];
extern PfnEntityUpdate* PfnEntityUpdates;
extern LayoutEntity** g_pStObjLayoutHorizontal;
extern LayoutEntity** g_pStObjLayoutVertical;

extern u8 gfx_portrait_death[];
extern u8 gfx_portrait_alucard[];

// Dialogue before the Death fight, followed by its events
u8 D_us_80181878[] = {
#include "../../boss/rbo2/gen/cutscene_script_psx.h"
#include "../../boss/rbo2/gen/cutscene_events.h"
};

static void InitCutscenePc(void) {
    static const CutsceneSymbolRange symbols[] = {
        {D_us_80181878, 0x80181878, sizeof(D_us_80181878)},
        {gfx_portrait_death, 0x8018d224, 0xd80},
        {gfx_portrait_alucard, 0x8018dfa4, 0xd80},
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
