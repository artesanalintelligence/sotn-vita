// SPDX-License-Identifier: AGPL-3.0-or-later
#include <psyz/module.h>
#include <game.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stages/overlay.h"
#include "../pc.h"
#include "../boss/mar/mar.h"

extern AbbreviatedOverlay g_BossOverlay;
extern PfnEntityUpdate EntityUpdates[];
extern LayoutEntity* entityLayoutHorizontal[];
extern LayoutEntity* entityLayoutVertical[];
extern PfnEntityUpdate* PfnEntityUpdates;
extern LayoutEntity** g_pStObjLayoutHorizontal;
extern LayoutEntity** g_pStObjLayoutVertical;

extern u8 cutscene_data[];

// The Alucard and Maria portraits are still raw data in MAR.BIN
#define MAR_PORTRAITS_OFFSET 0x15F0
#define MAR_PORTRAIT_LEN 0xD80
static u8 cutscene_portraits[MAR_PORTRAIT_LEN * 2];

static void InitCutscenePc(void) {
    static const CutsceneSymbolRange symbols[] = {
        {cutscene_data, 0x80181424, MAR_PORTRAITS_OFFSET - 0x1424},
        {cutscene_portraits, 0x801815F0, sizeof(cutscene_portraits)},
    };

    if (FileReadToBuf("disks/us/BOSS/MAR/MAR.BIN", cutscene_portraits,
                      MAR_PORTRAITS_OFFSET, sizeof(cutscene_portraits)) < 0) {
        ERRORF("failed to read the cutscene portraits from MAR.BIN");
    }
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
