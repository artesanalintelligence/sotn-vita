// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo7.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_80194338_from_rbo0);
INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_80194D3C);
INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_801959E0);
INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_80195AF0);
INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_80195C50);
INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_801963D8);
#else
extern EInit g_EInitParticle;
extern EInit g_EInitInteractable;
extern EInit g_EInitCerberos;
extern EInit D_us_8018046C;
extern EInit D_us_80180478;

typedef struct {
    /* 0x7C */ s32 : 32;
    /* 0x80 */ u16 timer;
    /* 0x82 */ s16 : 16;
    /* 0x84 */ u8 backOff; // set when the player gets too close
    /* 0x85 */ u8 fireballIdx;
    /* 0x86 */ u8 count;
} ET_Cerberus;
#define CERBERUS_EXT(e) ((ET_Cerberus*)&(e)->ext)

typedef struct {
    /* 0x7C */ Primitive* prim;
} ET_CerberusMgr;
#define CERBERUS_MGR_EXT(e) ((ET_CerberusMgr*)&(e)->ext)

// arena floor sensor used when landing from a jump
static s16 D_us_801806D8[] = {0x00, 0x20, 0x00, 0x04, 0x08, -4, -16, 0x00};
// palettes of the three fire breaths
static u16 D_us_801806E8[] = {0x2E4, 0x2EA, 0x2E9, 0x000};
// fire breath spawn offsets, one per head
static s16 D_us_801806F0[][2] = {{-24, 1}, {-23, -20}, {-13, -8}};

static u8 D_us_801806FC[] = {
    0x12, 0x03, 0x10, 0x02, 0x16, 0x01, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00};
static u8 D_us_80180708[] = {
    0x12, 0x03, 0x06, 0x04, 0x04, 0x05, 0x06, 0x06, 0x12, 0x07,
    0x06, 0x06, 0x04, 0x05, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00};
static u8 D_us_8018071C[] = {
    0x02, 0x07, 0x02, 0x06, 0x02, 0x05, 0x02, 0x04,
    0x02, 0x03, 0x02, 0x02, 0xFF, 0x00, 0x00, 0x00};
static u8 D_us_8018072C[] = {
    0x03, 0x03, 0x04, 0x08, 0x04, 0x09, 0x03, 0x0A,
    0x05, 0x0B, 0x01, 0x0C, 0x01, 0x0D, 0x00, 0x00};
static u8 D_us_8018073C[] = {
    0x10, 0x03, 0x02, 0x0A, 0x18, 0x0B, 0x02, 0x0C, 0x03, 0x0D, 0xFF, 0x00};
static u8 D_us_80180748[] = {
    0x08, 0x11, 0x05, 0x12, 0x02, 0x13, 0x02, 0x14, 0x20, 0x15, 0xFF, 0x00};
static u8 D_us_80180754[] = {
    0x08, 0x03, 0x06, 0x16, 0x06, 0x17, 0x1A, 0x11, 0xFF, 0x00, 0x00, 0x00};
static u8 D_us_80180760[] = {
    0x08, 0x11, 0x06, 0x17, 0x06, 0x16, 0x08, 0x03, 0xFF, 0x00, 0x00, 0x00};
static u8 D_us_8018076C[] = {
    0x08, 0x03, 0x06, 0x16, 0x06, 0x17, 0x40, 0x18, 0xFF, 0x00, 0x00, 0x00};
static u8 D_us_80180778[] = {
    0x04, 0x03, 0x04, 0x16, 0x04, 0x17, 0x1A, 0x18,
    0x03, 0x17, 0x01, 0x16, 0xFF, 0x00, 0x00, 0x00};
static u8 D_us_80180788[] = {
    0x01, 0x16, 0x02, 0x17, 0x10, 0x18, 0x06, 0x17,
    0x06, 0x16, 0x08, 0x03, 0xFF, 0x00, 0x00, 0x00};
static u8 D_us_80180798[] = {
    0x03, 0x19, 0x05, 0x1A, 0x07, 0x1B, 0x08, 0x1C, 0x00, 0x00, 0x00, 0x00};
static u8 D_us_801807A4[] = {
    0x03, 0x1D, 0x05, 0x1E, 0x07, 0x1F, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00};
static u8 D_us_801807B0[] = {
    0x03, 0x21, 0x05, 0x22, 0x07, 0x23, 0x08, 0x24, 0x00, 0x00, 0x00, 0x00};
static u8* D_us_801807BC[] = {D_us_80180798, D_us_801807A4, D_us_801807B0};
static u8 D_us_801807C8[] = {
    0x02, 0x01, 0x02, 0x02, 0x02, 0x03, 0x02, 0x04, 0x02, 0x05, 0x02, 0x06,
    0x02, 0x07, 0x02, 0x08, 0x02, 0x09, 0x01, 0x0A, 0x01, 0x0B, 0x01, 0x0C,
    0x01, 0x0D, 0xFF, 0x00};

// hitbox offset x, offset y, width, height
static s8 D_us_801807E4[][4] = {
    {0, 0, 0, 0},    {5, 8, 28, 22},  {4, 8, 18, 24},  {5, 11, 27, 20},
    {5, 16, 28, 16}, {-4, 1, 12, 3},  {-4, 0, 12, 3},  {-4, 0, 12, 4},
    {-5, 0, 12, 4},  {-5, 1, 12, 4},  {-4, 1, 12, 4},  {-5, 0, 11, 2},
    {-4, -1, 12, 3}, {-5, 0, 12, 3},
};
// hitbox index for each animCurFrame
static u8 D_us_8018081C[] = {
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x01, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x06, 0x06, 0x09, 0x0A,
    0x0B, 0x0C, 0x07, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// D_us_80180848
s32 g_CerberusMgrState = 0;

static u8 D_us_8018084C[] = {
    0x03, 0x01, 0x03, 0x02, 0x03, 0x03, 0x03, 0x04, 0x03, 0x05, 0x03, 0x06,
    0x03, 0x07, 0x03, 0x08, 0x03, 0x09, 0x03, 0x0A, 0x03, 0x0B, 0x03, 0x0C,
    0x03, 0x0D, 0xFF, 0x00, 0x03, 0x01, 0x03, 0x02, 0x03, 0x03, 0x03, 0x04,
    0x03, 0x05, 0x03, 0x06, 0x03, 0x07, 0x03, 0x08, 0x03, 0x09, 0x03, 0x0A,
    0x03, 0x0B, 0x03, 0x0C, 0x03, 0x0D, 0x00, 0x00};
static u8 D_us_80180884[] = {
    0x02, 0x01, 0x02, 0x02, 0x02, 0x03, 0x02, 0x04, 0x02, 0x05, 0x02,
    0x06, 0x02, 0x07, 0x02, 0x08, 0x02, 0x09, 0x02, 0x0A, 0x02, 0x0B,
    0x02, 0x0C, 0x02, 0x0D, 0x02, 0x0E, 0x00, 0x00, 0x00, 0x00};

typedef struct {
    u16 animSet;
    u16 unk5A;
    u16 palette;
    u16 blendMode;
    u8* frames;
} CerberusFlameDef;

static CerberusFlameDef D_us_801808A4[] = {
    {ANIMSET_OVL(4), 0x50, 0x01, BLEND_TRANSP | BLEND_ADD, D_us_8018084C},
    {ANIMSET_OVL(3), 0x53, 0x03, BLEND_TRANSP | BLEND_ADD, D_us_80180884},
    {ANIMSET_OVL(4), 0x50, 0x03, BLEND_TRANSP | BLEND_ADD, D_us_8018084C},
};

// Snaps the boss to the arena floor once it falls low enough. Same routine
// as rbo0's copy, with bo7's higher floor (0xD0 vs 0xE0).
bool func_us_80194338_from_rbo0(s16* offsets) {
    s32 posY;

    offsets++;
    posY = g_CurrentEntity->posY.i.hi + *offsets + g_Tilemap.scrollY.i.hi;
    posY = 0xD0 - posY;
    if (posY <= 0) {
        g_CurrentEntity->posY.i.hi += posY;
        g_CurrentEntity->velocityX = 0;
        g_CurrentEntity->velocityY = 0;
        return true;
    }
    return false;
}

static void CerberusSpitFireball(Entity* self) {
    Entity* entity;

    PlaySfxPositional(SFX_B07_STOMP);
    PlaySfxPositional(SFX_WARG_ATTACK);
    entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
    if (entity != NULL) {
        CreateEntityFromEntity(E_UNK_1D, self, entity);
        entity->facingLeft = self->facingLeft;
        if (self->facingLeft) {
            entity->posX.i.hi += 0x18;
        } else {
            entity->posX.i.hi -= 0x18;
        }
        entity->params = CERBERUS_EXT(self)->fireballIdx;
        entity->zPriority = self->zPriority + 1;
    }
}

// Cerberus
void func_us_80194D3C(Entity* self) {
    Entity* entity;
    s32 posX;
    s32 side;
    s32 absX;
    s32 i;
    s8* hitbox;

    if (self->flags & FLAG_DEAD) {
        if (self->step != 8) {
            SetStep(8);
        }
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCerberos);
        self->hitboxState = 0;
        SetStep(2);
        // fallthrough
    case 2: // wait for the fight to start
        AnimateEntity(D_us_801806FC, self);
        if (g_CerberusMgrState & CERBERUS_MGR_FIGHT_ON) {
            self->hitboxState = 3;
            SetStep(3);
        }
        break;

    case 3: // walk around, then pick the next attack
        if (!self->step_s) {
            CERBERUS_EXT(self)->timer = 0x40;
            self->step_s++;
        }
        if (!AnimateEntity(D_us_80180708, self)) {
            PlaySfxPositional(SFX_WARG_GROWL);
        }
        if (GetDistanceToPlayerX() < 0x68) {
            CERBERUS_EXT(self)->backOff = 1;
        }
        if (GetDistanceToPlayerX() > 0x80) {
            CERBERUS_EXT(self)->backOff = 0;
        }
        if (GetDistanceToPlayerX() > 0x30) {
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        }
        MoveEntity();
        if (self->facingLeft == CERBERUS_EXT(self)->backOff) {
            self->velocityX = FIX(-0.5);
        } else {
            self->velocityX = FIX(0.5);
        }
        if (--CERBERUS_EXT(self)->timer) {
            break;
        }
        posX = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
        SetStep(10);
        if (self->facingLeft) {
            posX = 0x200 - posX;
        }
        side = (GetSideToPlayer() & 1) ^ 1;
        if (posX < 0xD0 && self->facingLeft == side) {
            SetStep(9);
        }
        absX = posX;
        if (absX < 0) {
            absX = -absX;
        }
        if (absX > 0x50 && self->facingLeft != side) {
            SetStep(9);
        }
        if (posX > 0x140 && self->facingLeft == side) {
            SetStep(6);
        }
        if (posX > 0x180 && self->facingLeft == side) {
            SetStep(4);
        }
        if ((g_Player.status & PLAYER_STATUS_BAT_FORM) ||
            (PLAYER.posY.i.hi < 0xA0 &&
             (g_Player.vram_flag & TOUCHING_GROUND))) {
            SetStep(5);
        }
        break;

    case 11: // dash
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_8018071C, self)) {
                if (self->facingLeft) {
                    self->velocityX = FIX(-6);
                } else {
                    self->velocityX = FIX(6);
                }
                CERBERUS_EXT(self)->timer = 0x40;
                self->step_s++;
            }
            break;
        case 1:
            MoveEntity();
            self->velocityX -= self->velocityX >> 6;
            if (!--CERBERUS_EXT(self)->timer) {
                SetStep(3);
            }
            break;
        }
        break;

    case 5: // breathe fire from the three heads
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_80180754, self)) {
                SetSubStep(1);
            }
            break;
        case 1:
            if (!AnimateEntity(D_us_80180748, self)) {
                PlaySfxPositional(SFX_WARG_ATTACK);
                PlaySfxPositional(SFX_SHAFT_FIRE_ATTACK);
                CERBERUS_EXT(self)->timer = 0x80;
                SetSubStep(2);
            }
            break;
        case 2:
            if (!(CERBERUS_EXT(self)->timer & 3)) {
                for (i = 0; i < 3; i++) {
                    entity = AllocEntity(&g_Entities[128], &g_Entities[224]);
                    if (entity == NULL) {
                        continue;
                    }
                    CreateEntityFromEntity(E_UNK_1E, self, entity);
                    if (self->facingLeft) {
                        entity->posX.i.hi -= D_us_801806F0[i][0];
                    } else {
                        entity->posX.i.hi += D_us_801806F0[i][0];
                    }
                    entity->params = i;
                    entity->posY.i.hi += D_us_801806F0[i][1];
                    entity->zPriority = self->zPriority + 1;
                    entity->facingLeft = self->facingLeft;
                }
            }
            if (!--CERBERUS_EXT(self)->timer) {
                self->step_s++;
            }
            break;
        case 3:
            if (AnimateEntity(D_us_80180760, self)) {
                break;
            }
            SetStep(3);
            break;
        }
        break;

    case 10: // spit a single fireball
        if (!AnimateEntity(D_us_8018073C, self)) {
            SetStep(3);
        }
        if (!self->poseTimer && self->pose == 3) {
            CerberusSpitFireball(self);
        }
        break;

    case 9: // jump towards the facing direction
    case 4: // jump away from the facing direction
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_80180778, self)) {
                SetSubStep(1);
            }
            break;
        case 1:
            self->velocityY = FIX(-6);
            if (self->step == 9) {
                if (self->facingLeft) {
                    self->velocityX = FIX(-2.5);
                } else {
                    self->velocityX = FIX(2.5);
                }
            } else {
                if (self->facingLeft) {
                    self->velocityX = FIX(2.5);
                } else {
                    self->velocityX = FIX(-2.5);
                }
            }
            self->step_s++;
            // fallthrough
        case 2:
            MoveEntity();
            self->velocityY += FIX(0.21875);
            if (func_us_80194338_from_rbo0(D_us_801806D8) & 1) {
                PlaySfxPositional(SFX_START_SLAM_B);
                g_api.ShakeCamera(1);
                self->step_s++;
            }
            break;
        case 3:
            if (!AnimateEntity(D_us_80180788, self)) {
                SetStep(3);
            }
            break;
        }
        break;

    case 6: // spit a volley of fireballs
        switch (self->step_s) {
        case 0:
            CERBERUS_EXT(self)->count = 8;
            self->step_s++;
            // fallthrough
        case 1:
            if (!AnimateEntity(D_us_8018072C, self)) {
                if (!--CERBERUS_EXT(self)->count) {
                    SetStep(3);
                }
            }
            if (!self->poseTimer && self->pose == 5) {
                CerberusSpitFireball(self);
                CERBERUS_EXT(self)->fireballIdx++;
                if (CERBERUS_EXT(self)->fireballIdx >= 3) {
                    CERBERUS_EXT(self)->fireballIdx = 0;
                }
            }
            break;
        }
        break;

    case 8: // death
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            g_CerberusMgrState |= CERBERUS_MGR_DEAD;
            PlaySfxPositional(SFX_WARG_DEATH_HOWL);
            self->step_s++;
            // fallthrough
        case 1:
            if (!(g_Timer & 7)) {
                PlaySfxPositional(SFX_EXPLODE_B);
                entity = AllocEntity(&g_Entities[192], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_20, self, entity);
                    entity->params = 1;
                    entity->zPriority = self->zPriority + 1;
                    entity->posX.i.hi += (Random() & 0x3F) - 0x20;
                    entity->posY.i.hi += (Random() & 0x1F) - 0xC;
                }
            }
            if (!AnimateEntity(D_us_8018076C, self)) {
                PlaySfxPositional(SFX_BOSS_LARGE_FLAMES);
                CERBERUS_EXT(self)->timer = 0x50;
                self->step_s++;
            }
            break;
        case 2:
            entity = AllocEntity(&g_Entities[192], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_20, self, entity);
                entity->params = 2;
                entity->zPriority = self->zPriority + 1;
                posX = entity->posX.i.hi + (Random() & 0x3F) - 0x20;
                if (self->facingLeft) {
                    entity->posX.i.hi = posX - 0xC;
                } else {
                    entity->posX.i.hi = posX + 0xC;
                }
                entity->posY.i.hi += 0x18;
            }
            if (!--CERBERUS_EXT(self)->timer) {
                self->animCurFrame = 0;
                self->step_s++;
            }
            break;
        case 3:
            g_CerberusMgrState |= CERBERUS_MGR_DEATH_DONE;
            self->step_s++;
            break;
        case 4:
            break;
        }
        break;

    case 0xFF:
        FntPrint("charal %x\n", self->animCurFrame);
        if (g_pads[1].pressed & PAD_SQUARE) {
            if (self->params) {
                break;
            }
            self->animCurFrame++;
            self->params |= 1;
        } else {
            self->params = 0;
        }
        if (g_pads[1].pressed & PAD_CIRCLE) {
            if (self->step_s) {
                break;
            }
            self->animCurFrame--;
            self->step_s |= 1;
        } else {
            self->step_s = 0;
        }
        break;
    }

    // keep Cerberus inside the arena
    posX = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
    if (self->velocityX < 0) {
        if (posX < 0x68) {
            self->posX.i.hi = 0x68 - g_Tilemap.scrollX.i.hi;
        }
    } else if (posX > 0x1A0) {
        self->posX.i.hi = 0x1A0 - g_Tilemap.scrollX.i.hi;
    }

    hitbox = D_us_801807E4[D_us_8018081C[self->animCurFrame]];
    self->hitboxOffX = *hitbox++;
    self->hitboxOffY = *hitbox++;
    self->hitboxWidth = *hitbox++;
    self->hitboxHeight = *hitbox++;
}

// fireball spat by a single head
void func_us_801959E0(Entity* self) {
    Entity* entity;

    if (g_CerberusMgrState & CERBERUS_MGR_DEAD) {
        DestroyEntity(self);
        return;
    }
    if (self->flags & FLAG_DEAD) {
        entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 1;
        }
        DestroyEntity(self);
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_8018046C);
        if (self->facingLeft) {
            self->velocityX = FIX(6);
        } else {
            self->velocityX = FIX(-6);
        }
        // fallthrough
    case 1:
        MoveEntity();
        AnimateEntity(D_us_801807BC[self->params], self);
        break;
    }
}

// fire breath, leaves a trail of flames behind
void func_us_80195AF0(Entity* self) {
    Entity* entity;
    s16 angle;

    if (g_CerberusMgrState & CERBERUS_MGR_DEAD) {
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        angle = -0x680;
        InitializeEntity(D_us_80180478);
        self->drawFlags = ENTITY_SCALEX | ENTITY_ROTATE;
        self->scaleX = 0x80;
        self->rotate = -0x280;
        self->palette = D_us_801806E8[self->params];
        if (self->facingLeft) {
            angle = -0x180;
        }
        self->velocityX = (rcos(angle) * FIX(6)) >> 0xC;
        self->velocityY = (rsin(angle) * FIX(6)) >> 0xC;
    }
    MoveEntity();
    if (!AnimateEntity(D_us_801807C8, self)) {
        DestroyEntity(self);
    }
    if (!self->poseTimer && self->pose == 8) {
        entity = AllocEntity(&g_Entities[128], &g_Entities[224]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_UNK_1E, self, entity);
            entity->params = self->params;
            entity->facingLeft = self->facingLeft;
        }
    }
}

// Cerberus fight manager and the burning background of the arena
void func_us_80195C50(Entity* self) {
    DRAWENV drawEnv;
    RECT clipRect;
    Primitive* prim;
    Entity* entity;
    s32 primIndex;
    s32 i;
    s16 x;
    s16 y;
    s16 angle;
    s32 brightness;
    s32 posX;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        self->flags |= FLAG_UNK_10000;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 7);
        if (primIndex == -1) {
            self->step = 0;
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->flags |= FLAG_HAS_PRIMS;
        if (g_api.func_800EDB08((POLY_GT4*)prim) != NULL) {
            prim->type = PRIM_ENV;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
            g_api.func_800EDB08((POLY_GT4*)prim);
        } else {
            prim = NULL;
        }
        if (prim == NULL) {
            g_api.FreePrimitives(primIndex);
            self->step = 0;
            self->flags &= ~FLAG_HAS_PRIMS;
            return;
        }
        prim->type = PRIM_ENV;
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;

        // red glow, drawn off-screen and stretched over the background
        CERBERUS_MGR_EXT(self)->prim = prim;
        prim->type = PRIM_G4;
        prim->r0 = prim->g0 = prim->b0 = 0;
        prim->x1 = prim->x3 = 0x42;
        prim->y2 = prim->y3 = 0xB0;
        prim->r2 = 0xFF;
        prim->g2 = prim->b2 = 0;
        prim->x0 = prim->x2 = 0;
        prim->y0 = prim->y1 = 0;
        prim->priority = 0xF;
        prim->drawMode = DRAW_DEFAULT;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r2);
        prim = prim->next;

        x = 0x40;
        for (i = 0; i < 4; i++) {
            prim->tpage = 0x110;
            if (i & 1) {
                prim->u0 = prim->u2 = 0x3F;
                prim->u1 = prim->u3 = 0;
            } else {
                prim->u0 = prim->u2 = 0;
                prim->u1 = prim->u3 = 0x3F;
            }
            prim->x0 = prim->x2 = i << 6;
            prim->priority = 0x13;
            prim->v0 = prim->v1 = 0x10;
            prim->v2 = prim->v3 = 0xF0;
            prim->x1 = prim->x3 = x;
            prim->y0 = prim->y1 = 0x10;
            prim->y2 = prim->y3 = 0xF0;
            prim->drawMode = DRAW_DEFAULT;
            prim = prim->next;
            x += 0x40;
        }
        g_api.PlaySfx(SFX_BOSS_LARGE_FLAMES);
        // fallthrough
    case 1:
        posX = PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi;
        if ((u32)(posX - 0x41) < 0x17F) {
            g_CerberusMgrState |= CERBERUS_MGR_PLAYER_IN;
            self->step++;
        }
        if (!(g_Timer % 95)) {
            g_api.PlaySfx(SFX_BOSS_LARGE_FLAMES);
        }
        break;
    case 2:
        if (!(g_Timer % 95)) {
            g_api.PlaySfx(SFX_BOSS_LARGE_FLAMES);
        }
        posX = PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi;
        if ((u32)(posX - 0x81) < 0xFF) {
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_CERBERUS_DEFEAT, TIMEATTACK_SET_VISITED);
            stopMusicFlag = 1;
            currentMusicId = 0x31D;
            g_CerberusMgrState |= CERBERUS_MGR_FIGHT_ON;
            self->step++;
        }
        break;
    case 3:
        if (!g_api.func_80131F68()) {
            stopMusicFlag = 0;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        // fallthrough
    case 4:
        if (g_CerberusMgrState & CERBERUS_MGR_DEAD) {
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_CERBERUS_DEFEAT, TIMEATTACK_SET_RECORD);
            g_api.PlaySfx(SET_UNK_90);
            currentMusicId = 0x319;
            self->step++;
        }
        break;
    case 5:
        if (g_CerberusMgrState & CERBERUS_MGR_DEATH_DONE) {
            self->step++;
        }
        break;
    case 6:
        x = 0x100 - g_Tilemap.scrollX.i.hi;
        y = 0x80 - g_Tilemap.scrollY.i.hi;
        entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_LIFE_UP_SPAWN, self, entity);
            entity->posX.i.hi = x;
            entity->posY.i.hi = y;
            entity->params = 5;
            stopMusicFlag = 1;
            currentMusicId = 0x319;
            g_CerberusMgrState |= CERBERUS_MGR_REWARD;
            self->step++;
        }
        // fallthrough
    case 7:
        if (!g_api.func_80131F68()) {
            stopMusicFlag = 0;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        break;
    }

    // render the glow into the off-screen area at (0, 256)
    prim = &g_PrimBuf[self->primIndex];
    drawEnv = g_CurrentBuffer->draw;
    drawEnv.isbg = 1;
    drawEnv.r0 = drawEnv.g0 = drawEnv.b0 = 0;
    clipRect.x = 0;
    clipRect.y = 0x100;
    clipRect.w = 0x7F;
    clipRect.h = 0xFF;
    drawEnv.clip = clipRect;
    drawEnv.ofs[0] = 0;
    drawEnv.ofs[1] = 0x100;
    SetDrawEnv(PRIM_DR_ENV(prim), &drawEnv);
    prim->priority = 0xE;
    prim->drawMode = DRAW_DEFAULT;
    prim = prim->next;
    prim->priority = 0x11;
    prim->drawMode = DRAW_UNK_800;

    prim = CERBERUS_MGR_EXT(self)->prim;
    angle = self->rotate + 0x100;
    self->rotate = angle;
    if (g_CerberusMgrState & CERBERUS_MGR_DEAD) {
        PrimDecreaseBrightness(prim, 4);
    } else {
        brightness = (rcos(angle) << 7) >> 0xC;
        if (brightness < 0) {
            brightness = -brightness;
        }
        prim->g2 = brightness;
        LOW(prim->r3) = LOW(prim->r2);
    }
    if (!(g_CerberusMgrState & CERBERUS_MGR_DEAD) && !(g_Timer & 3)) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromCurrentEntity(E_UNK_20, entity);
            entity->posX.i.hi = (Random() & 0x3F) + 0xA;
            entity->posY.i.hi = 0xB0;
            entity->zPriority = 0x10;
            entity->params = 0;
        }
    }
}

// flames rising from the arena floor and from the dying Cerberus
void func_us_801963D8(Entity* self) {
    CerberusFlameDef* def;
    s32 params;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        params = self->params & 0xF;
        def = &D_us_801808A4[params];
        self->palette = def->palette + 0x2E0;
        self->blendMode = def->blendMode;
        self->animSet = def->animSet;
        self->unk5A = def->unk5A;
        self->step = params + 1;
        self->ext.deathFlames.frames = (AnimateEntityFrame*)def->frames;
        if (self->params & 0xFF00) {
            self->zPriority = (self->params & 0xFF00) >> 8;
        }
        if (self->params & 0xF0) {
            self->palette = PAL_FLAG(PAL_UNK_19F);
            self->blendMode = BLEND_TRANSP;
            self->facingLeft = true;
        }
        break;
    case 1:
        if (!self->step_s) {
            self->drawFlags = ENTITY_OPACITY;
            self->opacity = 0x50;
            self->flags &= ~FLAG_POS_CAMERA_LOCKED;
            self->facingLeft = Random() & 1;
            self->velocityX = (Random() << 8) - FIX(0.5);
            self->velocityY = FIX(-1.5);
            self->ext.deathFlames.unk8C = -(Random() * 0x10) - 0x1000;
            self->step_s++;
        }
        MoveEntity();
        self->velocityY += self->ext.deathFlames.unk8C;
        self->opacity--;
        if (!AnimateEntity((u8*)self->ext.deathFlames.frames, self)) {
            DestroyEntity(self);
        }
        break;
    case 2:
        if (!self->step_s) {
            self->velocityY = FIX(-0.25);
            self->step_s++;
        }
        MoveEntity();
        if (!AnimateEntity((u8*)self->ext.deathFlames.frames, self)) {
            DestroyEntity(self);
        }
        break;
    case 3:
        if (!self->step_s) {
            self->drawFlags = ENTITY_OPACITY;
            self->opacity = 0xC0;
            self->facingLeft = Random() & 1;
            self->velocityX = (Random() << 8) - FIX(0.5);
            self->velocityY = FIX(-0.75);
            self->ext.deathFlames.unk8C = -(Random() * 0x10) - 0x4000;
            self->step_s++;
        }
        MoveEntity();
        self->velocityY += self->ext.deathFlames.unk8C;
        self->opacity--;
        if (!AnimateEntity((u8*)self->ext.deathFlames.frames, self)) {
            DestroyEntity(self);
        }
        break;
    }
}
#endif
