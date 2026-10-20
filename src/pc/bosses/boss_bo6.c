// SPDX-License-Identifier: AGPL-3.0-or-later
#include <psyz/module.h>
#include <game.h>
#include <cutscene.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stages/overlay.h"
#include "../pc.h"
#include "../boss/bo6/bo6.h"

extern AbbreviatedOverlay2 g_BossOverlay;
extern PfnEntityUpdate EntityUpdates[];
extern LayoutEntity* entityLayoutHorizontal[];
extern LayoutEntity* entityLayoutVertical[];
extern PfnEntityUpdate* PfnEntityUpdates;
extern LayoutEntity** g_pStObjLayoutHorizontal;
extern LayoutEntity** g_pStObjLayoutVertical;

// contiguous copies of the PSX data, see d_pc_data.c and d_pc_sprites.c
extern u8 bo6_pc_data_blob[];
extern u8 bo6_pc_sprites_blob[];

static void InitCutscenePc(void) {
    // the cutscene scripts, their events and the portraits all live in the
    // sprite data block
    static const CutsceneSymbolRange symbols[] = {
        {bo6_pc_data_blob, 0x80180614, 0x80182F70 - 0x80180614},
        {bo6_pc_sprites_blob, 0x8018303C, 0x801A6320 - 0x8018303C},
    };
    CutscenePcAlloc(symbols, LEN(symbols));
}

void Psyz_ModuleStart(void* param) {
    Overlay* o = param;

    memcpy(o, &g_BossOverlay, sizeof(AbbreviatedOverlay2));
    PfnEntityUpdates = EntityUpdates;
    g_pStObjLayoutHorizontal = entityLayoutHorizontal;
    g_pStObjLayoutVertical = entityLayoutVertical;
    InitCutscenePc();
}

void Psyz_ModuleStop(void) {}
