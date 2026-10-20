// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo6.h"
#include <cutscene.h>

extern Dialogue g_Dialogue;
extern const char* actor_names[];

#define CUTSCENE_UNK1_NEXT_X 0
#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"

#include "../../st/cutscene_unk3.h"

#include "../../st/cutscene_unk4.h"

#include "../../st/cutscene_actor_name.h"

#include "../../st/set_cutscene_events.h"

#define CUTSCENE_TILEMAP_SCROLL
#include "../../st/cutscene_events.h"

#include "../../st/cutscene_skip.h"

#include "../../st/cutscene_scale_avatar.h"

#ifndef VERSION_PC
INCLUDE_RODATA("boss/rbo6/nonmatchings/e_cutscene_dialogue", D_us_8019CE94);

INCLUDE_RODATA("boss/rbo6/nonmatchings/e_cutscene_dialogue", D_us_8019CEA0);

INCLUDE_ASM("boss/rbo6/nonmatchings/e_cutscene_dialogue", EntityCutscene);
#else
// Same as RCEN's EntityCutscene, with RBO6's own skip flags, scripts and
// clean-up at the end of the cutscene
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

const char* actor_names[] = {_S("Alucard"), _S("Dracula")};
static u8 u_coords[] = {0, 64};
static u8 v_coords[] = {0, 0};
static u16 cluts[] = {0x210, 0x218};
static u16 x_vals[] = {0, 32};
static u16 cutscene_unk_4_cluts[] = {
    PAL_UNK_1A1,
    PAL_UNK_1A1,
    PAL_UNK_1A1,
    PAL_UNK_1A1,
};
static s16 red_line_increment[] = {
    0x0008, 0x0013, 0x0011, 0x0031, 0x004F, 0x0026, 0x0036, 0x001D, 0x001B,
    0x0033, 0x002C, 0x0021, 0x0019, 0x000A, 0x0033, 0x001F, 0x0048, 0x002F,
    0x0013, 0x0019, 0x004D, 0x004B, 0x0017, 0x001D, 0x0012, 0x0002, 0x001B,
    0x002A, 0x0050, 0x0045, 0x0032, 0x000D, 0x002A, 0x004D, 0x0006, 0x0027,
    0x0007, 0x0048, 0x002F, 0x001B, 0x0036, 0x0022, 0x0039, 0x0014, 0x0039,
    0x001D, 0x000A, 0x0035, 0x0010, 0x001B, 0x003D, 0x0017, 0x002E, 0x000B,
    0x0049, 0x0042, 0x003D, 0x002A, 0x0001, 0x000C, 0x001B, 0x0034, 0x0041,
    0x0035, 0x0008, 0x000E, 0x004D, 0x0011, 0x0034, 0x0041, 0x0029, 0x0048,
};

// father and mother scripts followed by their events, contiguous as on PSX
extern u8 cutscene_script[];
#define CUTSCENE_SCRIPT_MOTHER_OFFSET (0x801819E7 - 0x80181720)

extern s32 g_SkipCutscene;
extern u32 g_CutsceneFlags;
extern s32 dialogue_started;

void EntityCutscene(Entity* self) {
    u16 nextChar;
    s32 primIndex;
    s16 uCoord, vCoord;
    Primitive* prim;
    s32 i, j;
    RECT rect;
    u_long ptr;


    if (self->step && dialogue_started && !g_SkipCutscene) {
        if (g_GameClearFlag) {
            CutsceneSkip(self);
        } else if (self->params) {
            if (g_Settings.D_8003CB04 & 0x400000) {
                CutsceneSkip(self);
            }
        } else if (g_Settings.D_8003CB04 & 0x200000) {
            CutsceneSkip(self);
        }
    }
    if (self->step && g_Dialogue.hasEvents) {
        RunCutsceneEvents();
    }

    switch (self->step) {
    case DIALOGUE_INIT:
        if (self->params) {
            D_8003C73C = 2;
            i = SetCutsceneScript(
                cutscene_script + CUTSCENE_SCRIPT_MOTHER_OFFSET);
        } else {
            i = SetCutsceneScript(cutscene_script);
        }
        if (!i) {
            break;
        }
        g_CutsceneHasControl = true;
        g_CutsceneFlags = 0;
        dialogue_started = false;
        g_SkipCutscene = false;
        self->flags |= FLAG_HAS_PRIMS | FLAG_UNK_2000;
        self->step++;
        self->primIndex = g_Dialogue.primIndex[2];
        if (g_PlayableCharacter != PLAYER_ALUCARD || g_DemoMode) {
            dialogue_started = true;
            g_SkipCutscene = true;
            g_api.FreePrimitives(self->primIndex);
            self->flags ^= FLAG_HAS_PRIMS;
        }
        // fallthrough
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
                        g_Dialogue.nextLineY += CS_LINE_SPACING;
                    }
                    // This is line number increment
                    g_Dialogue.nextCharY++;
                if (g_Dialogue.nextCharY > CS_LINE_MAX) {
                    g_Dialogue.nextCharY = 0;
                }
                    CutsceneUnk4();
                    if (!(g_Dialogue.unk12 & 1)) {
                        if (g_Dialogue.nextCharY > CS_LINE_MAX - 1) {
                            g_Dialogue.unk12 |= 1;
                            g_Dialogue.portraitAnimTimer = 0;
                            self->step_s = DIALOG_BOX_INIT;
                            self->step++;
                            return;
                        }
                        continue;
                    } else {
                        g_Dialogue.portraitAnimTimer = 0;
                        self->step_s = DIALOG_BOX_INIT;
                        self->step++;
                    }
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
                uCoord = u_coords[j & 1];
                vCoord = v_coords[j & 1];
                    prim->clut = cluts[i];
                    prim->tpage = 144;
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
// psp and psx refuse to agree on this particular increment
                    g_Dialogue.scriptCur++;
                    g_Dialogue.scriptCur++;
                        continue;
                    }
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
                    *g_Dialogue.scriptCur--;
                    return;
                case CSOP_SCRIPT_UNKNOWN_11:
                    if (g_SkipCutscene) {
                        continue;
                    }
                    if (g_api.func_80131F68() != 1) {
                        continue;
                    }
                    *g_Dialogue.scriptCur--;
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
                    *g_Dialogue.scriptCur++;
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
                        LoadTPage((u_long*)ptr, 1, 0, x_vals[j], 256, 48, 72);
                    }
                    continue;
                case CSOP_SCRIPT_UNKNOWN_20:
                    nextChar = *g_Dialogue.scriptCur++;
                    nextChar <<= 4;
                    nextChar |= *g_Dialogue.scriptCur++;
                    g_api.PlaySfx(nextChar);
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
                        *g_Dialogue.scriptCur--;
                        return;
                    }
                    *g_Dialogue.scriptCur++;
                    continue;
                default:
                    if (g_SkipCutscene) {
                        continue;
                    }
                    g_Dialogue.nextCharTimer = g_Dialogue.unk17;
            } // This closes the CSOP switch for psx
            break; // This breaks the big while loop for psx
        } // This closes the big while loop for psx
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
            // Initializes the dialogue box background by horizontal pixel line
            // Set to red for the initial draw in
            // uCoord is used as a horizontal line offset here
            dialogue_started = true;
            primIndex =
                g_api.AllocPrimitives(PRIM_LINE_G2, LEN(red_line_increment));
            if (primIndex == -1) {
                DestroyEntity(self);
                break;
            }
            g_Dialogue.primIndex[0] = primIndex;
            for (prim = &g_PrimBuf[primIndex], uCoord = 0; prim != NULL;
                 prim = prim->next) {
                prim->r0 = prim->r1 = 127;
                prim->b0 = prim->b1 = prim->g0 = prim->g1 = 0;
                // Left (x0) and right (x1) edge of the dialogue box
                prim->x0 = prim->x1 = 247;
                prim->y0 = prim->y1 = g_Dialogue.startY + uCoord;
                prim->priority = 510;
                prim->drawMode = DRAW_DEFAULT;
                prim->x2 = red_line_increment[uCoord];
                // Initialized as the right side of the dialogue box (247) * 16
                prim->x3 = 3952;
                uCoord++;
            }
            self->step_s++;
            break;
        case DIALOG_BOX_DRAW_RED:
            // prim->x0 and prim->x3 are the left side (end point) of the red
            // lines by pixel and subpixel respectively
            // prim->x2 is the interval value in subpixel for drawing each red
            // line across the screen from right to left
            // uCoord is used here as a flag to indicate line completion
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
        g_CutsceneHasControl = false;
        if (self->params) {
            g_CastleFlags[0xE1] = 1;
            g_Settings.D_8003CB04 |= 0x400000;
        } else {
            g_Settings.D_8003CB04 |= 0x200000;
        }
        g_unkGraphicsStruct.unk28 = 0;
        D_8003C73C = 0;
        DestroyEntity(self);
        break;
    }
}
#endif
