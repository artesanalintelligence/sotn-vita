// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo6.h"
#include <cutscene.h>

extern Dialogue g_Dialogue;
extern const char* actor_names[];

#define CUTSCENE_UNK1_NEXT_X (DISP_STAGE_W + 2)
#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"

#define CUTSCENE_UNK3_RECT_X DISP_STAGE_W
#include "../../st/cutscene_unk3.h"

#define CUTSCENE_UNK4_TPAGE 0x14
#include "../../st/cutscene_unk4.h"

#include "../../st/cutscene_actor_name.h"

#include "../../st/set_cutscene_events.h"

#define CUTSCENE_TILEMAP_SCROLL
#include "../../st/cutscene_events.h"

#include "../../st/cutscene_skip.h"

#include "../../st/cutscene_scale_avatar.h"

// Richter's cutscene entity. Same as RBO6's EntityCutscene, with BO6's own
// skip flags and scripts.
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

enum DialogueSubsteps {
    DIALOG_BOX_INIT = 0,
    DIALOG_BOX_DRAW_RED = 1,
    DIALOG_BOX_DRAW_BLUE = 2,
};

extern u16 PrizeDrops[]; // n.b.! its bytes are also the portrait U coords
extern u8 D_us_80180614[]; // portrait V coords
extern u16 D_us_80180618[]; // portrait cluts
extern u16 D_us_80180620[]; // portrait VRAM X
extern u16 D_us_80180624[]; // dialogue box cluts
extern s16 D_us_8018062C[]; // red line increments
extern u8 D_us_8019A0C8; // script switch: castle flag 0xD8
extern u8 D_us_8019A0CC; // script switch: holy glasses / variant
extern u8 D_us_8019A0D0[];
extern u8 D_us_8019A2E1[];
extern u8 D_us_8019A2FB[];
extern s32 g_SkipCutscene;
extern s32 g_IsCutsceneDone;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/cutscene", func_us_801A7DC0);
#else
void func_us_801A7DC0(Entity* self) {
    u16 nextChar;
    s32 primIndex;
    s16 uCoord, vCoord;
    Primitive* prim;
    s32 i, j;
    RECT rect;
    u_long ptr;

    if (self->step && g_IsCutsceneDone && !g_SkipCutscene) {
        if (g_GameClearFlag) {
            CutsceneSkip(self);
        } else {
            switch (self->params) {
            case 0:
                if (g_Settings.D_8003CB04 & 0x8000) {
                    CutsceneSkip(self);
                }
                break;
            case 2:
                if (g_Settings.D_8003CB04 & 0x10000) {
                    CutsceneSkip(self);
                }
                break;
            case 3:
                if (g_Settings.D_8003CB04 & 0x20000) {
                    CutsceneSkip(self);
                }
                break;
            }
        }
    }
    if (self->step && g_Dialogue.hasEvents) {
        RunCutsceneEvents();
    }

    switch (self->step) {
    case DIALOGUE_INIT:
        i = 0;
        switch (self->params) {
        case 0:
            if (g_DemoMode != Demo_None || g_CastleFlags[SHAFT_ORB_DEFEATED] ||
                g_PlayableCharacter != PLAYER_ALUCARD) {
                DestroyEntity(self);
                return;
            }
            i = SetCutsceneScript(D_us_8019A0D0);
            D_us_8019A0C8 = 0;
            if (g_CastleFlags[0xD8]) {
                D_us_8019A0C8 = 1;
            }
            D_us_8019A0CC = 0;
            if (g_api.CheckEquipmentItemCount(0x22, 1)) {
                D_us_8019A0CC = 1;
            }
            break;
        case 1:
            i = SetCutsceneScript(D_us_8019A2E1);
            break;
        case 2:
        case 3:
            if (g_DemoMode != Demo_None || g_CastleFlags[SHAFT_ORB_DEFEATED]) {
                DestroyEntity(self);
                return;
            }
            i = SetCutsceneScript(D_us_8019A2FB);
            D_us_8019A0CC = self->params - 2;
            break;
        }
        if (i & 0xFF) {
            g_CutsceneHasControl = true;
            self->flags |= FLAG_HAS_PRIMS | FLAG_UNK_2000;
            self->step++;
            g_CutsceneFlags = 0;
            g_IsCutsceneDone = false;
            g_SkipCutscene = false;
            self->primIndex = g_Dialogue.primIndex[2];
            self->flags |= FLAG_UNK_10000;
        }
        break;
    case DIALOGUE_RUN:
        nextChar = 0;
        while (true) {
            if ((g_Dialogue.nextCharTimer) && !g_SkipCutscene) {
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
                    g_Dialogue.nextLineY += 12;
                }
                g_Dialogue.nextCharY++;
                if (g_Dialogue.nextCharY > 4) {
                    g_Dialogue.nextCharY = 0;
                }
                CutsceneUnk4();
                if (!(g_Dialogue.unk12 & 1)) {
                    if (g_Dialogue.nextCharY > 3) {
                        g_Dialogue.unk12 |= 1;
                        g_Dialogue.portraitAnimTimer = 0;
                        self->step_s = DIALOG_BOX_INIT;
                        self->step++;
                        return;
                    }
                    continue;
                }
                g_Dialogue.portraitAnimTimer = 0;
                self->step_s = DIALOG_BOX_INIT;
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
                for (i = 0; i < LEN(g_Dialogue.prim) - 1; i++) {
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
                prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
                j = *g_Dialogue.scriptCur++;
                uCoord = ((u8*)PrizeDrops)[j & 1];
                vCoord = D_us_80180614[j & 1];
                prim->tpage = 0x94;
                prim->clut = D_us_80180618[i];
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
                g_Dialogue.clutIndex = D_us_80180624[i];
                CutsceneUnk1();
                CutsceneUnk4();
                prim->priority = 510;
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
                for (i = 0; i < LEN(g_Dialogue.prim) - 1; i++) {
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
                prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
                prim = prim->next;
                prim->y0 = prim->y1 = g_Dialogue.startY;
                prim->y2 = prim->y3 = g_Dialogue.startY + 72;
                prim = prim->next;
                prim->y0 = g_Dialogue.startY - 1;
                prim->u0 = 246;
                g_Dialogue.portraitAnimTimer = 24;
                self->step = DIALOGUE_OPEN_DIALOG_BOX;
                self->step_s = DIALOG_BOX_INIT;
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
                ptr = (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                SetCutsceneEvents((u8*)ptr);
                continue;
            case CSOP_SCRIPT_UNKNOWN_13:
                continue;
            case CSOP_SCRIPT_SWITCH:
                ptr = (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                ptr += 0x100000;
                g_Dialogue.scriptCur += *(u8*)CS_NEXT(ptr) << 2;

                ptr = (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur;
                g_Dialogue.scriptCur = CS_PTR(ptr);
                continue;
            case CSOP_SCRIPT_UNKNOWN_15:
                ptr = (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur++;
                ptr <<= 4;
                ptr |= (u_long)*g_Dialogue.scriptCur;
                g_Dialogue.scriptCur = CS_PTR(ptr);
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
                    ptr = (u_long)*g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= (u_long)*g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= (u_long)*g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= (u_long)*g_Dialogue.scriptCur++;
                    ptr = (u_long)CS_PTR(ptr);
                    j = *g_Dialogue.scriptCur++;
                    LoadTPage((u_long*)ptr, 1, 0, D_us_80180620[j], 256, 48,
                              72);
                }
                continue;
            case CSOP_SCRIPT_UNKNOWN_21:
                g_CutsceneFlags = 0;
                g_SkipCutscene = false;
                g_IsCutsceneDone = false;
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
        prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
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
        prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
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
        case DIALOG_BOX_INIT:
            g_IsCutsceneDone = true;
            primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 72);
            if (primIndex == -1) {
                DestroyEntity(self);
                break;
            }
            g_Dialogue.primIndex[0] = primIndex;
            for (prim = &g_PrimBuf[primIndex], uCoord = 0; prim != NULL;
                 prim = prim->next) {
                prim->r0 = prim->r1 = 127;
                prim->b0 = prim->b1 = prim->g0 = prim->g1 = 0;
                prim->x0 = prim->x1 = 247;
                prim->y0 = prim->y1 = g_Dialogue.startY + uCoord;
                prim->priority = 510;
                prim->drawMode = DRAW_DEFAULT;
                prim->x2 = D_us_8018062C[uCoord];
                prim->x3 = 3952;
                uCoord++;
            }
            self->step_s++;
            break;
        case DIALOG_BOX_DRAW_RED:
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
                prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
                prim = prim->next;
                prim->drawMode = DRAW_TPAGE | DRAW_TRANSP;
                prim = prim->next;
                prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE | DRAW_TRANSP;
                self->step_s++;
            }
            break;
        case DIALOG_BOX_DRAW_BLUE:
            uCoord = false;
            prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
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
        }
        break;
    case DIALOGUE_CLOSE_DIALOG_BOX:
        prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
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
        switch (self->params) {
        case 0:
            g_Settings.D_8003CB04 |= 0x8000;
            break;
        case 2:
            g_Settings.D_8003CB04 |= 0x10000;
            break;
        case 3:
            g_CastleFlags[SHAFT_ORB_DEFEATED] = 1;
            g_Settings.D_8003CB04 |= 0x20000;
            break;
        }
        DestroyEntity(self);
        g_CutsceneHasControl = false;
        break;
    }
}
#endif
