// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"
#include <cutscene.h>

extern Dialogue g_Dialogue;

#ifdef VERSION_PSP
#define CUTSCENE_UNK1_NEXT_X 0
#define CUTSCENE_UNK1_UNK17 2

#include "../../get_lang.h"

#include "../../st/cutscene_actor_name.h"

#include "../../st/cutscene_unk3.h"

#include "../../st/cutscene_unk4.h"

#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"

#include "../../st/set_cutscene_events.h"

#define CUTSCENE_WAIT_FOR_FLAG_REWIND
#include "../../st/cutscene_events.h"
#else
static const char* actor_names[] = {_S("Alucard"), _S("Maria")};

#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"

#include "../../st/cutscene_unk3.h"

#include "../../st/cutscene_unk4.h"

#include "../../st/cutscene_actor_name.h"

#include "../../st/set_cutscene_events.h"

#define CUTSCENE_WAIT_FOR_FLAG_REWIND
#include "../../st/cutscene_events.h"

#include "../../st/cutscene_scale_avatar.h"
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_cutscene_dialogue", EntityCutscene);
#else
static u8 D_us_80180B78[] = {0x00, 0x40}; // shares PrizeDrops' storage
extern u8 D_us_80180B7C[];
extern u16 D_us_80180B80[];
extern u16 D_us_80180B84[];
extern u16 D_us_80180B88[];
extern s16 D_us_80180B8C[];
extern s32 D_us_801806DC;
extern u32 g_SkipCutscene;
extern u32 g_CutsceneFlags;
extern s32 g_IsCutsceneDone;
extern u8 cutscene_script[];

// Alucard and Maria talk after the Hippogryph is defeated.
// Same flow as NZ0's EntityCutscene, with the BO5 specific tables.
void EntityCutscene(Entity* self) {
    s16 uCoord;
    s16 vCoord;
    s32 primIndex;
    RECT rect;
    u16 nextChar;
    s32 var_s7;
    s32 var_s5;
    u_long ptr;
    Primitive* prim;
    s32 i;

    if (self->step) {
        if (g_IsCutsceneDone && !g_SkipCutscene &&
            (g_Settings.D_8003CB04 & 0x200 || g_GameClearFlag) &&
            g_pads[0].tapped == PAD_START) {
            g_SkipCutscene = 1;
            g_api.FreePrimitives(self->primIndex);
            self->flags ^= FLAG_HAS_PRIMS;
            if (g_Dialogue.primIndex[1] != -1) {
                g_api.FreePrimitives(g_Dialogue.primIndex[1]);
            }
            if (g_Dialogue.primIndex[0] != -1) {
                g_api.FreePrimitives(g_Dialogue.primIndex[0]);
            }
            g_api.PlaySfx(SET_STOP_MUSIC);
            self->step = 1;
            self->step_s = 0;
        }
        if (self->step && g_Dialogue.hasEvents) {
            RunCutsceneEvents();
        }
    }
    switch (self->step) {
    case 0:
        if (g_CastleFlags[0x62] || g_DemoMode != Demo_None ||
            g_PlayableCharacter != PLAYER_ALUCARD) {
            D_us_801806DC = 0;
            DestroyEntity(self);
            return;
        }
        if (SetCutsceneScript(cutscene_script)) {
            g_CutsceneFlags = 0;
            g_IsCutsceneDone = 0;
            g_SkipCutscene = 0;
            g_CutsceneHasControl = 1;
            self->flags |= FLAG_HAS_PRIMS | FLAG_UNK_2000;
            self->step++;
            self->primIndex = g_Dialogue.primIndex[2];
        }
        break;
    case 1:
        while (1) {
            if (g_Dialogue.nextCharTimer && !g_SkipCutscene) {
                g_Dialogue.nextCharTimer--;
                return;
            }
            nextChar = *g_Dialogue.scriptCur++;
            switch (nextChar) {
            case CSOP_END_CUTSCENE:
                self->step = 7;
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
                        self->step_s = 0;
                        self->step++;
                        return;
                    }
                } else {
                    g_Dialogue.portraitAnimTimer = 0;
                    self->step_s = 0;
                    self->step++;
                    return;
                }
                continue;
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
                var_s5 = *g_Dialogue.scriptCur++;
                prim = g_Dialogue.prim[5];
                var_s7 = var_s5 & 1;
                uCoord = D_us_80180B78[var_s7];
                vCoord = D_us_80180B7C[var_s7];
                prim->tpage = 0x90;
                prim->clut = D_us_80180B80[i];
                if (var_s5 & 0x80) {
                    prim->u0 = prim->u2 = uCoord + 0x2F;
                    prim->u1 = prim->u3 = uCoord;
                } else {
                    prim->u0 = prim->u2 = uCoord;
                    prim->u1 = prim->u3 = uCoord + 0x2F;
                }
                prim->v0 = prim->v1 = vCoord;
                prim->v2 = prim->v3 = vCoord + 0x48;
                prim->x0 = prim->x1 = prim->x2 = prim->x3 =
                    g_Dialogue.startX - 0x1E;
                prim->y0 = prim->y1 = prim->y2 = prim->y3 =
                    g_Dialogue.startY + 0x24;
                g_Dialogue.clutIndex = D_us_80180B88[i];
                CutsceneUnk1();
                CutsceneUnk4();
                prim->priority = 0x1FE;
                prim->drawMode = DRAW_DEFAULT;
                DrawCutsceneActorName(i, self);
                g_Dialogue.portraitAnimTimer = 6;
                self->step = 3;
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
                self->step = 4;
                return;
            case CSOP_SET_POS:
                if (g_SkipCutscene) {
                    g_Dialogue.scriptCur += 2;
                    continue;
                }
                g_Dialogue.startX = *g_Dialogue.scriptCur++;
                g_Dialogue.startY = *g_Dialogue.scriptCur++;
                prim = g_Dialogue.prim[5];
                prim->y0 = prim->y1 = g_Dialogue.startY;
                prim->y2 = prim->y3 = g_Dialogue.startY + 0x48;
                prim = prim->next;
                prim->y0 = g_Dialogue.startY - 1;
                prim->u0 = 0xF6;
                g_Dialogue.portraitAnimTimer = 0x18;
                self->step = 5;
                self->step_s = 0;
                return;
            case CSOP_CLOSE_DIALOG:
                if (g_SkipCutscene) {
                    continue;
                }
                g_Dialogue.portraitAnimTimer = 0x18;
                self->step = 6;
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
                if (g_api.func_80131F68() != false) {
                    continue;
                }
                g_Dialogue.scriptCur--;
                return;
            case CSOP_SCRIPT_UNKNOWN_11:
                if (g_SkipCutscene) {
                    continue;
                }
                if (g_api.func_80131F68() != true) {
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
                g_Dialogue.scriptCur += *CS_NEXT(ptr) * 4;
                // fallthrough
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
                    var_s5 = g_Dialogue.scriptCur++[0];
                    LoadTPage((u_long*)ptr, 1, 0, D_us_80180B84[var_s5], 0x100,
                              0x30, 0x48);
                }
                continue;
            case CSOP_SCRIPT_UNKNOWN_21:
                g_CutsceneFlags = 0;
                g_SkipCutscene = 0;
                g_IsCutsceneDone = 0;
                continue;
            case CSOP_SCRIPT_UNKNOWN_22:
                g_CutsceneFlags &= ~(1 << *g_Dialogue.scriptCur++);
                continue;
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
        if (nextChar == 0x20) {
            g_Dialogue.nextCharX += 2;
            return;
        }
        rect.x = ((nextChar & 0xF) << 1) + 0x380;
        rect.y = ((s32)(nextChar & 0xF0) >> 1) + 0xF0;
        rect.w = 2;
        rect.h = 8;
        vCoord = (g_Dialogue.nextCharY * 0xC) + 0x180;
        MoveImage(&rect, g_Dialogue.nextCharX, vCoord);
        g_Dialogue.nextCharX += 2;
        break;
    case 2:
        ScaleCutsceneAvatar(2);
        if (g_Dialogue.portraitAnimTimer >= 6) {
            self->step--;
        }
        break;
    case 3:
        prim = g_Dialogue.prim[5];
        prim->x0 = prim->x2 -= 4;
        prim->x1 = prim->x3 += 4;
        prim->y0 = prim->y1 -= 6;
        prim->y2 = prim->y3 += 6;
        g_Dialogue.portraitAnimTimer--;
        if (!g_Dialogue.portraitAnimTimer) {
            self->step = 1;
            for (prim = &g_PrimBuf[g_Dialogue.primIndex[1]]; prim != NULL;
                 prim = prim->next) {
                prim->drawMode = DRAW_DEFAULT;
            }
        }
        break;
    case 4:
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
            self->step = 1;
        }
        break;
    case 5:
        switch (self->step_s) {
        case 0:
            g_IsCutsceneDone = 1;
            primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 0x48);
            if (primIndex == -1) {
                DestroyEntity(self);
                return;
            }
            g_Dialogue.primIndex[0] = primIndex;
            for (prim = &g_PrimBuf[primIndex], uCoord = 0; prim != NULL;
                 prim = prim->next) {
                prim->r0 = prim->r1 = 0x7F;
                prim->b0 = prim->b1 = prim->g0 = prim->g1 = 0;
                prim->x0 = prim->x1 = 0xF7;
                prim->priority = 0x1FE;
                prim->drawMode = DRAW_DEFAULT;
                prim->y0 = prim->y1 = g_Dialogue.startY + uCoord;
                prim->x3 = 0xF70;
                prim->x2 = D_us_80180B8C[uCoord];
                uCoord++;
            }
            self->step_s++;
            break;
        case 1:
            for (uCoord = 0, prim = &g_PrimBuf[g_Dialogue.primIndex[0]];
                 prim != NULL; prim = prim->next) {
                prim->x3 -= prim->x2;
                prim->x2 += 2;
                prim->x0 = prim->x3 / 16;
                if (prim->x0 < 5) {
                    prim->x0 = 4;
                } else {
                    uCoord = 1;
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
            uCoord = 0;
            prim = g_Dialogue.prim[5];
            prim = prim->next;
            if (prim->r0 < 0x10) {
                prim->r0 = prim->r1 = prim->r2 = prim->r3 = 0;
            } else {
                prim->r0 = prim->r1 = prim->r2 = prim->r3 -= 0x10;
                uCoord = 1;
            }
            if (prim->b0 >= 0x7F) {
                prim->b0 = prim->b1 = 0x7F;
            } else {
                prim->b0 = prim->b1 += 8;
                uCoord = 1;
            }
            prim = prim->next;
            if (prim->r0 < 0x10) {
                prim->r0 = prim->g0 = prim->b0 = 0x10;
            } else {
                prim->r0 = prim->g0 = prim->b0 -= 0xF;
                uCoord = 1;
            }
            if (!uCoord) {
                self->step = 1;
            }
            break;
        }
        break;
    case 6:
        prim = g_Dialogue.prim[5];
        prim = prim->next;
        g_Dialogue.portraitAnimTimer--;
        if (g_Dialogue.portraitAnimTimer >= 0xC) {
            prim = prim->next;
            prim->u0 -= 20;
            if (g_Dialogue.portraitAnimTimer & 1) {
                prim->u0--;
            }
        } else {
            if (!g_Dialogue.portraitAnimTimer) {
                self->step = 1;
                prim->drawMode = DRAW_HIDE;
            } else {
                prim->y2 = (prim->y3 -= 6);
            }
            prim = prim->next;
            prim->drawMode = DRAW_HIDE;
        }
        break;
    case 7:
        DestroyEntity(self);
        g_CastleFlags[0x62] = 1;
        g_CastleFlags[0x85] = 1;
        g_CutsceneHasControl = 0;
        g_Settings.D_8003CB04 |= 0x300;
        break;
    }
}
#endif
