// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"
#include <cutscene.h>

enum DialogueSteps {
    DIALOGUE_INIT = 0,
    DIALOGUE_RUN = 1,
    DIALOGUE_LOAD_PORTRAIT = 2,
    DIALOGUE_START_TEXT = 3,
    DIALOGUE_UNLOAD_PORTRAIT = 4,
    DIALOGUE_OPEN_DIALOG_BOX = 5,
    DIALOGUE_CLOSE_DIALOG_BOX = 6,
    DIALOGUE_END = 7,
};

extern Dialogue g_Dialogue;
extern const char* actor_names[];

#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"

#include "../../st/cutscene_unk3.h"

#include "../../st/cutscene_unk4.h"

#include "../../st/cutscene_actor_name.h"

#include "../../st/set_cutscene_events.h"

#include "../../st/cutscene_events.h"

#include "../../st/cutscene_skip.h"

#include "../../st/cutscene_scale_avatar.h"

#ifndef NON_MATCHING
INCLUDE_RODATA("boss/bo2/nonmatchings/e_cutscene_dialogue", D_us_801A1F6C);

INCLUDE_RODATA("boss/bo2/nonmatchings/e_cutscene_dialogue", D_us_801A1F78);

INCLUDE_ASM("boss/bo2/nonmatchings/e_cutscene_dialogue", EntityCutsceneDialogue);
#else
#ifdef VERSION_PC
// On PSX these live in the overlay bss
Dialogue g_Dialogue;
u32 g_CutsceneFlags;
s32 g_SkipCutscene;
#endif
static s32 dialogue_started;

extern s32 D_us_80180B78;
// Dialogue before the fight (Alucard only) and after it (Richter appears)
extern u8 cutscene_data_bo2[];
#define cutscene_script_intro (&cutscene_data_bo2[0x80181CCC - 0x80181CCC])
#define cutscene_script_outro (&cutscene_data_bo2[0x80181DC5 - 0x80181CCC])

static u8 u_coords[] = {0, 64, 0, 0};
static u8 v_coords[] = {0, 0, 0, 0};
static u16 cluts[] = {0x230, 0x238};
static u16 x_vals[] = {0, 32};
static u16 cutscene_unk_4_cluts[] = {PAL_UNK_1A1, PAL_UNK_1A1};
// Defines the increment of each line (by pixel, not text) of the dialogue box
// background. Results in the staggered red draw in.
static s16 red_line_increment[] = {
    8,  19, 17, 49, 79, 38, 54, 29, 27, 51, 44, 33, 25, 10, 51, 31, 72, 47,
    19, 25, 77, 75, 23, 29, 18, 2,  27, 42, 80, 69, 50, 13, 42, 77, 6,  39,
    7,  72, 47, 27, 54, 34, 57, 20, 57, 29, 10, 53, 16, 27, 61, 23, 46, 11,
    73, 66, 61, 42, 1,  12, 27, 52, 65, 53, 8,  14, 77, 17, 52, 65, 41, 72};

static u8* ReadScriptPtr(void) {
    u32 ptr;

    ptr = *g_Dialogue.scriptCur++;
    ptr <<= 4;
    ptr |= *g_Dialogue.scriptCur++;
    ptr <<= 4;
    ptr |= *g_Dialogue.scriptCur++;
    ptr <<= 4;
    ptr |= *g_Dialogue.scriptCur++;
#ifdef VERSION_PC
    return CS_PTR(ptr);
#else
    return (u8*)ptr + 0x100000;
#endif
}

void EntityCutsceneDialogue(Entity* self) {
    Primitive* prim;
    s32 primIndex;
    s32 i, j;
    s16 uCoord, vCoord;
    u16 nextChar;
    RECT rect;
    u32 ptr;
    u8* script;
    bool skipAllowed;

    FntPrint("moji %x\n", g_Dialogue.scriptCur);
    if (self->step) {
        if (dialogue_started && !g_SkipCutscene) {
            if (g_GameClearFlag) {
                skipAllowed = true;
            } else if (self->params) {
                skipAllowed = g_Settings.D_8003CB04 & 0x4000;
            } else {
                skipAllowed = g_Settings.D_8003CB04 & 0x10;
            }
            if (skipAllowed) {
                CutsceneSkip(self);
            }
        }
        if (self->step && g_Dialogue.hasEvents) {
            RunCutsceneEvents();
        }
    }

    switch (self->step) {
    case DIALOGUE_INIT:
        if (self->params) {
            if (g_CastleFlags[RICHTER_CS_AFTER_M_AND_W] ||
                g_PlayableCharacter != PLAYER_ALUCARD ||
                g_DemoMode != Demo_None) {
                DestroyEntity(self);
                return;
            }
            script = cutscene_script_outro;
        } else {
            if (g_CastleFlags[RICHTER_CS_AFTER_M_AND_W] || D_us_80180B78 ||
                g_PlayableCharacter != PLAYER_ALUCARD ||
                g_DemoMode != Demo_None) {
                DestroyEntity(self);
                return;
            }
            script = cutscene_script_intro;
        }
        if (SetCutsceneScript(script)) {
            g_CutsceneFlags = CUTSCENE_FLAG_NONE;
            dialogue_started = false;
            g_SkipCutscene = false;
            g_CutsceneHasControl = true;
            self->flags |= FLAG_HAS_PRIMS | FLAG_UNK_2000;
            self->step++;
            self->primIndex = g_Dialogue.primIndex[2];
        }
        return;
    case DIALOGUE_RUN:
        while (true) {
            if (g_Dialogue.nextCharTimer && !g_SkipCutscene) {
                g_Dialogue.nextCharTimer--;
                return;
            }
            nextChar = *g_Dialogue.scriptCur++;
            switch (nextChar) {
            case CSOP_END_CUTSCENE:
                self->step = DIALOGUE_END;
                return;
            case CSOP_LINE_BREAK:
                if (g_SkipCutscene) {
                    continue;
                }
                g_Dialogue.nextCharX = g_Dialogue.nextLineX;
                if (!(g_Dialogue.unk12 & 1)) {
                    g_Dialogue.nextLineY += CS_LINE_SPACING;
                }
                g_Dialogue.nextCharY++;
                if (g_Dialogue.nextCharY > CS_LINE_MAX) {
                    g_Dialogue.nextCharY = 0;
                }
                CutsceneUnk4();
                if (!(g_Dialogue.unk12 & 1)) {
                    if (g_Dialogue.nextCharY < CS_LINE_MAX) {
                        continue;
                    }
                    g_Dialogue.unk12 |= 1;
                }
                g_Dialogue.portraitAnimTimer = 0;
                self->step_s = 0;
                self->step++;
                return;
            case CSOP_SET_SPEED:
                g_Dialogue.unk17 = *g_Dialogue.scriptCur++;
                continue;
            case CSOP_SET_WAIT:
                g_Dialogue.nextCharTimer = *g_Dialogue.scriptCur++;
                if (g_SkipCutscene) {
                    continue;
                }
                return;
            case CSOP_HIDE_DIALOG:
                if (g_SkipCutscene) {
                    continue;
                }
                prim = g_Dialogue.prim[0];
                for (i = 1; i < 5; i++) {
                    prim->drawMode = DRAW_HIDE;
                    prim = prim->next;
                }
                return;
            case CSOP_SET_PORTRAIT:
                if (g_SkipCutscene) {
                    g_Dialogue.scriptCur += 2;
                    continue;
                }
                i = *g_Dialogue.scriptCur++;
                j = *g_Dialogue.scriptCur++;
                prim = g_Dialogue.prim[5];
                uCoord = u_coords[j & 1];
                vCoord = v_coords[j & 1];
                prim->tpage = 0x90;
                prim->clut = cluts[i];
                if (j & 0x80) {
                    prim->u0 = prim->u2 = uCoord + 47;
                    prim->u1 = prim->u3 = uCoord;
                } else {
                    prim->u0 = prim->u2 = uCoord;
                    prim->u1 = prim->u3 = uCoord + 47;
                }
                prim->v0 = prim->v1 = vCoord;
                prim->v2 = prim->v3 = vCoord + 72;
                prim->x0 = prim->x1 = prim->x2 = prim->x3 =
                    g_Dialogue.startX - 30;
                prim->y0 = prim->y1 = prim->y2 = prim->y3 =
                    g_Dialogue.startY + 36;
                g_Dialogue.clutIndex = cutscene_unk_4_cluts[i];
                CutsceneUnk1();
                CutsceneUnk4();
                prim->priority = 0x1FE;
                prim->drawMode = DRAW_DEFAULT;
                DrawCutsceneActorName(i, self);
                g_Dialogue.portraitAnimTimer = 6;
                self->step = DIALOGUE_START_TEXT;
                return;
            case CSOP_NEXT_DIALOG:
                if (g_SkipCutscene) {
                    continue;
                }
                prim = g_Dialogue.prim[0];
                for (i = 1; i < 5; i++) {
                    prim->drawMode = DRAW_HIDE;
                    prim = prim->next;
                }
                g_api.FreePrimitives(g_Dialogue.primIndex[1]);
                g_Dialogue.primIndex[1] = -1;
                g_Dialogue.portraitAnimTimer = 6;
                self->step = DIALOGUE_UNLOAD_PORTRAIT;
                return;
            case CSOP_SET_POS:
                if (g_SkipCutscene) {
                    g_Dialogue.scriptCur += 2;
                    continue;
                }
                g_Dialogue.startX = *g_Dialogue.scriptCur++;
                g_Dialogue.startY = *g_Dialogue.scriptCur++;
                prim = g_Dialogue.prim[5];
                prim = prim->next;
                prim->y0 = prim->y1 = g_Dialogue.startY;
                prim->y2 = prim->y3 = g_Dialogue.startY + 72;
                prim = prim->next;
                prim->y0 = g_Dialogue.startY - 1;
                prim->u0 = 246;
                g_Dialogue.portraitAnimTimer = 24;
                self->step = DIALOGUE_OPEN_DIALOG_BOX;
                self->step_s = 0;
                return;
            case CSOP_CLOSE_DIALOG:
                if (g_SkipCutscene) {
                    continue;
                }
                g_Dialogue.portraitAnimTimer = 24;
                self->step = DIALOGUE_CLOSE_DIALOG_BOX;
                return;
            case CSOP_PLAY_SOUND:
                if (g_SkipCutscene) {
                    g_Dialogue.scriptCur += 2;
                    continue;
                }
                // fallthrough
            case CSOP_SCRIPT_UNKNOWN_20:
                nextChar = *g_Dialogue.scriptCur++;
                nextChar <<= 4;
                nextChar |= *g_Dialogue.scriptCur++;
                g_api.PlaySfx(nextChar);
                continue;
            case CSOP_WAIT_FOR_SOUND:
                if (g_SkipCutscene) {
                    continue;
                }
                if (g_api.func_80131F68()) {
                    continue;
                }
                g_Dialogue.scriptCur--;
                return;
            case CSOP_SCRIPT_UNKNOWN_11:
                if (g_SkipCutscene) {
                    continue;
                }
                if (g_api.func_80131F68() != 1) {
                    continue;
                }
                g_Dialogue.scriptCur--;
                return;
            case CSOP_SET_EVENTS:
                ptr = *g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= *g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= *g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= *g_Dialogue.scriptCur++;
                SetCutsceneEvents((u8*)(uintptr_t)ptr);
                continue;
            case CSOP_SCRIPT_UNKNOWN_13:
                continue;
            case CSOP_SCRIPT_SWITCH:
                script = ReadScriptPtr();
                g_Dialogue.scriptCur += *script << 2;
                g_Dialogue.scriptCur = ReadScriptPtr();
                continue;
            case CSOP_SCRIPT_UNKNOWN_15:
                g_Dialogue.scriptCur = ReadScriptPtr();
                continue;
            case CSOP_WAIT_FOR_FLAG:
                if (!((g_CutsceneFlags >> *g_Dialogue.scriptCur) & 1)) {
                    g_Dialogue.scriptCur--;
                    return;
                }
                g_CutsceneFlags &= ~(1 << *g_Dialogue.scriptCur);
                g_Dialogue.scriptCur++;
                continue;
            case CSOP_SET_FLAG:
                g_CutsceneFlags |= 1 << *g_Dialogue.scriptCur++;
                continue;
            case CSOP_STOP_EVENTS:
                g_Dialogue.hasEvents = 0;
                continue;
            case CSOP_LOAD_PORTRAIT:
                if (g_SkipCutscene) {
                    g_Dialogue.scriptCur += 5;
                } else {
                    script = ReadScriptPtr();
                    j = *g_Dialogue.scriptCur++;
                    LoadTPage((u_long*)script, 1, 0, x_vals[j], 256, 48, 72);
                }
                continue;
            case CSOP_SCRIPT_UNKNOWN_21:
                g_CutsceneFlags = CUTSCENE_FLAG_NONE;
                g_SkipCutscene = false;
                dialogue_started = false;
                continue;
            case CSOP_SCRIPT_UNKNOWN_22:
                g_CutsceneFlags &= ~(1 << *g_Dialogue.scriptCur++);
                continue;
            case CSOP_SCRIPT_UNKNOWN_23:
                return;
            case CSOP_WAIT_FOR_FLAG_RESET:
                if (!((g_CutsceneFlags >> *g_Dialogue.scriptCur) & 1)) {
                    g_Dialogue.scriptCur--;
                    return;
                }
                g_Dialogue.scriptCur++;
                continue;
            default:
                if (g_SkipCutscene) {
                    continue;
                }
                g_Dialogue.nextCharTimer = g_Dialogue.unk17;
                break;
            }
            break;
        }
        if (nextChar == 32) {
            g_Dialogue.nextCharX += 2;
            return;
        }
        rect.x = ((nextChar & 0xF) * 2) + 896;
        rect.y = ((nextChar & 0xF0) >> 1) + 240;
        rect.w = 2;
        rect.h = 8;
        vCoord = (g_Dialogue.nextCharY * 12) + 384;
        MoveImage(&rect, g_Dialogue.nextCharX, vCoord);
        g_Dialogue.nextCharX += 2;
        break;
    case DIALOGUE_LOAD_PORTRAIT:
        ScaleCutsceneAvatar(2);
        if (g_Dialogue.portraitAnimTimer >= 6) {
            self->step--;
        }
        break;
    case DIALOGUE_START_TEXT:
        prim = g_Dialogue.prim[5];
        prim->x0 = prim->x2 -= 4;
        prim->x1 = prim->x3 += 4;
        prim->y0 = prim->y1 -= 6;
        prim->y2 = prim->y3 += 6;
        g_Dialogue.portraitAnimTimer--;
        if (!g_Dialogue.portraitAnimTimer) {
            self->step = DIALOGUE_RUN;
            for (prim = &g_PrimBuf[g_Dialogue.primIndex[1]]; prim != NULL;
                 prim = prim->next) {
                prim->drawMode = DRAW_DEFAULT;
            }
        }
        break;
    case DIALOGUE_UNLOAD_PORTRAIT:
        prim = g_Dialogue.prim[5];
        prim->x0 = prim->x2 += 4;
        prim->x1 = prim->x3 -= 4;
        prim->y0 = prim->y1 += 6;
        prim->y2 = prim->y3 -= 6;
        if (prim->x1 >= (g_Dialogue.startX - 2)) {
            prim->x1 = prim->x3 = g_Dialogue.startX - 3;
        }
        g_Dialogue.portraitAnimTimer--;
        if (!g_Dialogue.portraitAnimTimer) {
            self->step = DIALOGUE_RUN;
        }
        break;
    case DIALOGUE_OPEN_DIALOG_BOX:
        switch (self->step_s) {
        case 0:
            dialogue_started = true;
            primIndex =
                g_api.AllocPrimitives(PRIM_LINE_G2, LEN(red_line_increment));
            if (primIndex == -1) {
                DestroyEntity(self);
                return;
            }
            g_Dialogue.primIndex[0] = primIndex;
            for (prim = &g_PrimBuf[primIndex], uCoord = 0; prim != NULL;
                 prim = prim->next) {
                prim->r0 = prim->r1 = 127;
                prim->b0 = prim->b1 = prim->g0 = prim->g1 = 0;
                prim->x0 = prim->x1 = 247;
                prim->y0 = prim->y1 = g_Dialogue.startY + uCoord;
                prim->priority = 0x1FE;
                prim->drawMode = DRAW_DEFAULT;
                prim->x2 = red_line_increment[uCoord];
                prim->x3 = 3952;
                uCoord++;
            }
            self->step_s++;
            return;
        case 1:
            uCoord = false;
            for (prim = &g_PrimBuf[g_Dialogue.primIndex[0]]; prim != NULL;
                 prim = prim->next) {
                prim->x3 -= prim->x2;
                prim->x2 += 2;
                prim->x0 = prim->x3 / 16;
                if (prim->x0 < 5) {
                    prim->x0 = 4;
                } else {
                    uCoord = true;
                }
            }
            if (!uCoord) {
                g_api.FreePrimitives(g_Dialogue.primIndex[0]);
                g_Dialogue.primIndex[0] = -1;
                prim = g_Dialogue.prim[5];
                prim = prim->next;
                prim->drawMode = DRAW_TPAGE | DRAW_TRANSP;
                prim = prim->next;
                prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE | DRAW_TRANSP;
                self->step_s++;
            }
            break;
        case 2:
            uCoord = false;
            prim = g_Dialogue.prim[5];
            prim = prim->next;
            if (prim->r0 < 16) {
                PRED(prim) = 0;
            } else {
                PRED(prim) -= 16;
                uCoord = true;
            }
            if (prim->b0 >= 127) {
                prim->b0 = prim->b1 = 127;
            } else {
                prim->b0 = prim->b1 += 8;
                uCoord = true;
            }
            prim = prim->next;
            if (prim->r0 < 16) {
                PGREY(prim, 0) = 16;
            } else {
                PGREY(prim, 0) -= 15;
                uCoord = true;
            }
            if (!uCoord) {
                self->step = DIALOGUE_RUN;
            }
            break;
        }
        break;
    case DIALOGUE_CLOSE_DIALOG_BOX:
        prim = g_Dialogue.prim[5];
        prim = prim->next;
        g_Dialogue.portraitAnimTimer--;
        if (g_Dialogue.portraitAnimTimer >= 12) {
            prim = prim->next;
            prim->u0 -= 20;
            if (g_Dialogue.portraitAnimTimer & 1) {
                prim->u0--;
            }
        } else {
            if (!g_Dialogue.portraitAnimTimer) {
                self->step = DIALOGUE_RUN;
                prim->drawMode = DRAW_HIDE;
            } else {
                prim->y2 = (prim->y3 -= 6);
            }
            prim = prim->next;
            prim->drawMode = DRAW_HIDE;
        }
        break;
    case DIALOGUE_END:
        if (self->params) {
            g_CastleFlags[RICHTER_CS_AFTER_M_AND_W] = 1;
            g_CastleFlags[0x85] = 1;
            g_CastleFlags[0x62] = 1;
            g_Settings.D_8003CB04 |= 0x4300;
        } else {
            D_us_80180B78 = 1;
            stopMusicFlag = false;
            currentMusicId = 0x31D;
            g_Settings.D_8003CB04 |= 0x10;
        }
        DestroyEntity(self);
        g_CutsceneHasControl = false;
        break;
    }
}
#endif
