// SPDX-License-Identifier: AGPL-3.0-or-later
#include <psyz/module.h>
#include <game.h>
#include <cutscene.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stages/overlay.h"
#include "../pc.h"
#include "../boss/rbo6/rbo6.h"

extern Overlay g_BossOverlay;
extern PfnEntityUpdate EntityUpdates[];
extern LayoutEntity* entityLayoutHorizontal[];
extern LayoutEntity* entityLayoutVertical[];
extern PfnEntityUpdate* PfnEntityUpdates;
extern LayoutEntity** g_pStObjLayoutHorizontal;
extern LayoutEntity** g_pStObjLayoutVertical;

extern u8 cutscene_script[];
extern u8 D_us_8019113C[]; // Alucard portrait
extern u8 D_us_80191EBC[]; // Dracula portrait

static void InitCutscenePc(void) {
    static const CutsceneSymbolRange symbols[] = {
        {cutscene_script, 0x80181720, 0x80181D32 - 0x80181720},
        {D_us_8019113C, 0x8019113C, 48 * 72},
        {D_us_80191EBC, 0x80191EBC, 48 * 72},
    };
    CutscenePcAlloc(symbols, LEN(symbols));
}

void Psyz_ModuleStart(void* param) {
    Overlay* o = param;

    memcpy(o, &g_BossOverlay, sizeof(Overlay));
    PfnEntityUpdates = EntityUpdates;
    g_pStObjLayoutHorizontal = entityLayoutHorizontal;
    g_pStObjLayoutVertical = entityLayoutVertical;
    InitCutscenePc();
}

void Psyz_ModuleStop(void) {}
