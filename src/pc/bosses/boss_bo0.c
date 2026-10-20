// SPDX-License-Identifier: AGPL-3.0-or-later
#include <psyz/module.h>
#include <game.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stages/overlay.h"
#include "../boss/bo0/bo0.h"

extern AbbreviatedOverlay g_Overlay;
extern PfnEntityUpdate EntityUpdates[];
extern LayoutEntity* entityLayoutHorizontal[];
extern LayoutEntity* entityLayoutVertical[];
extern PfnEntityUpdate* PfnEntityUpdates;
extern LayoutEntity** g_pStObjLayoutHorizontal;
extern LayoutEntity** g_pStObjLayoutVertical;

// Headless test aid, only active when SOTN_BO0_TEST is set:
// 1 keeps the player alive, 2 also makes Olrox's first form fragile,
// 3 also kills the true form after a while to run its death sequence.
static void (*s_Update)(void);
static int s_TestLevel;
static int s_TrueFormFrames;
static void TestUpdate(void) {
    Entity* e;
    int i;

    g_Status.hp = g_Status.hpMax;
    for (i = 0; i < LEN(g_Entities); i++) {
        e = &g_Entities[i];
        if (s_TestLevel >= 2 && e->entityId == E_UNK_2B && e->hitPoints > 8) {
            e->hitPoints = 8;
        }
        if (s_TestLevel >= 3 && e->entityId == E_UNK_33) {
            // kill the true form after it fought for a while
            if (++s_TrueFormFrames == 0x600) {
                e->flags |= FLAG_DEAD;
            }
        }
    }
    s_Update();
}

void Psyz_ModuleStart(void* param) {
    AbbreviatedOverlay* o = param;
    const char* test;

    memcpy(o, &g_Overlay, sizeof(AbbreviatedOverlay));
    PfnEntityUpdates = EntityUpdates;
    g_pStObjLayoutHorizontal = entityLayoutHorizontal;
    g_pStObjLayoutVertical = entityLayoutVertical;
    test = getenv("SOTN_BO0_TEST");
    if (test) {
        s_TestLevel = atoi(test);
        s_Update = o->Update;
        o->Update = TestUpdate;
    }
}

void Psyz_ModuleStop(void) {}
