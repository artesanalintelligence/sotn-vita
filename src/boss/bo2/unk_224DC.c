// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"

extern u8* g_eBreakableAnimations[];
extern u8 g_eBreakableHitboxes[];
extern u8 g_eBreakableExplosionTypes[];
extern u16 g_eBreakableanimSets[];
extern u8 blend_modes[];
extern EInit g_EInitInteractable;
extern s32 D_us_80180AB0;
extern EInit g_EInitMinotaurus;
extern s32 D_us_80180AAC;
extern s16 D_us_801806C8[];
extern s16 D_us_801806D0[];
extern s8 D_us_80180880[][4];
extern u8 D_us_80180908[];
extern u8 D_us_801806E0[];
extern u8 D_us_801806F4[];
extern u8 D_us_80180704[];
extern u8 D_us_80180714[];
extern u8 D_us_80180724[];
extern u8 D_us_80180734[];
extern u8 D_us_8018073C[];
extern u8 D_us_80180750[];
extern u8 D_us_80180770[];
extern u8 D_us_8018077C[];
extern u8 D_us_80180790[];
extern u8 D_us_80180798[];
extern u8 D_us_801807AC[];
extern u8 D_us_801807B4[];
extern u8 D_us_801807D0[];
extern u8 D_us_801807F4[];
extern u8 D_us_801807FC[];
extern u8 D_us_80180808[];
extern u8 D_us_80180810[];
extern u8 D_us_80180818[];
extern u8 D_us_80180824[];
extern u8 D_us_80180850[];
extern u8 D_us_80180858[];

#include "../../st/e_breakable.h"

void func_us_801A2610(void) {
    s32 posX;

    posX = g_CurrentEntity->posX.i.hi + g_Tilemap.scrollX.i.hi;
    if (g_CurrentEntity->velocityX > 0) {
        posX += 0x10;
        if (posX > 0x1B0) {
            g_CurrentEntity->posX.i.hi = 0x1A0 - g_Tilemap.scrollX.i.hi;
        }
    }
    if (g_CurrentEntity->velocityX < 0) {
        posX -= 0x10;
        if (posX < 0x50) {
            g_CurrentEntity->posX.i.hi = 0x60 - g_Tilemap.scrollX.i.hi;
        }
    }
}

void func_us_801A269C(Entity* self) {
    Entity* entity;
    s32 i;
    s8* ptr;
    s32 playerVelocityX;
    s32 arrived;

    if (self->flags & FLAG_DEAD && self->step < 0x40) {
        SetStep(0x40);
    }

    FntPrint("mino_step %x\n", self->step);
    FntPrint("mino_set_step %x\n", self->ext.minotaur.nextStep);
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitMinotaurus);
        self->hitboxState = 0;
        entity = self + 1;
        CreateEntityFromCurrentEntity(E_MINOTAUR_ATTACK_HITBOX, entity);
        // fallthrough
    case 1:
        AnimateEntity(D_us_801806F4, self);
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        UnkCollisionFunc3(D_us_801806D0);
        switch (self->step_s) {
        case 0:
            self->palette = PAL_FLAG(PAL_FILL_WHITE);
            self->blendMode = BLEND_SUB | BLEND_TRANSP;
            self->opacity = 0;
            self->drawFlags |= ENTITY_OPACITY;
            self->step_s++;
            // fallthrough
        case 1:
            self->opacity += 4;
            if (self->opacity == 0x80) {
                self->palette = g_EInitMinotaurus[3];
                self->blendMode = BLEND_NO;
                self->opacity = 0;
                self->step_s++;
            }
            break;
        case 2:
            self->opacity += 4;
            if (self->opacity == 0x80) {
                self->drawFlags = ENTITY_DEFAULT;
                D_us_80180AB0 = 1;
                self->step_s++;
            }
            break;
        case 3:
            if (D_us_80180AAC) {
                self->hitboxState = 3;
                SetStep(2);
            }
            break;
        }
        break;
    case 2:
        if (!self->step_s) {
            if (!self->ext.minotaur.moveTimer) {
                self->ext.minotaur.moveTimer = 0x20;
            }
            self->ext.minotaur.isReady = false;
            self->step_s++;
        }

        AnimateEntity(D_us_801806F4, self);
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        if (self->ext.minotaur.moveTimer) {
            self->ext.minotaur.moveTimer--;
        } else {
            self->ext.minotaur.isReady = true;
        }

        if (self->ext.minotaur.deathPuffPosX) {
            self->ext.minotaur.isReady = false;
            SetStep(7);
        } else if (self->ext.minotaur.hasNextStep) {
            self->ext.minotaur.hasNextStep = false;
            self->ext.minotaur.isReady = false;
            SetStep(self->ext.minotaur.nextStep);
        } else {
            entity = &PLAYER;
            playerVelocityX = entity->velocityX;
            if (self->facingLeft) {
                playerVelocityX = -playerVelocityX;
            }
            if (self->facingLeft == entity->facingLeft) {
                if (GetDistanceToPlayerX() > 0x48 &&
                    GetDistanceToPlayerX() < 0x60 &&
                    g_Player.status & PLAYER_STATUS_UNK400 &&
                    playerVelocityX > 0) {
                    self->ext.minotaur.isReady = false;
                    SetStep(0x12);
                }
                if (self->facingLeft == entity->facingLeft &&
                    g_Player.status & PLAYER_STATUS_SUBWPN) {
                    self->ext.minotaur.isReady = false;
                    SetStep(0x10);
                }
            }
        }
        break;
    case 7:
        if (!self->step_s) {
            i = self->ext.minotaur.deathPuffPosX;
            if (self->posX.i.hi > PLAYER.posX.i.hi) {
                i += PLAYER.posX.i.hi;
                i += g_Tilemap.scrollX.i.hi;
                if (i > 0x190) {
                    i = 0x190;
                }
            } else {
                i = PLAYER.posX.i.hi - i;
                i += g_Tilemap.scrollX.i.hi;
                if (i < 0x70) {
                    i = 0x70;
                }
            }
            self->ext.minotaur.deathPuffPosX = i;
            i = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
            i = self->ext.minotaur.deathPuffPosX - i;
            if (i > 0) {
                self->velocityX = FIX(1.0);
            } else {
                self->velocityX = FIX(-1.0);
            }
            self->step_s++;
        }
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        AnimateEntity(D_us_801806E0, self);
        UnkCollisionFunc2(D_us_801806C8);
        arrived = false;
        i = self->posX.i.hi + g_Tilemap.scrollX.i.hi -
            self->ext.minotaur.deathPuffPosX;
        if (self->velocityX > 0) {
            if (i > 0) {
                arrived = true;
            }
        } else if (i < 0) {
            arrived = true;
        }
        if (arrived) {
            self->ext.minotaur.deathPuffPosX = 0;
            SetStep(2);
        }
        break;
    case 0xB:
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_80180724, self)) {
                self->step_s++;
            }
            break;
        case 1:
            (self - 3)->ext.werewolf.signal = true;
            self->ext.minotaur.timer = 0x40;
            PlaySfxPositional(SFX_MINOTAUR_BREATH_ATTACK);
            self->step_s++;
            // fallthrough
        case 2:
            if (!(g_Timer & 3)) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(
                        E_MINOTAUR_SPIT_LIQUID, self, entity);
                    if (self->facingLeft) {
                        entity->posX.i.hi += 0x26;
                    } else {
                        entity->posX.i.hi -= 0x26;
                    }
                    entity->posY.i.hi -= 4;
                    entity->facingLeft = self->facingLeft;
                    entity->rotate = ROT(-112.5);
                    entity->zPriority = self->zPriority + 1;
                }
            }
            if (!--self->ext.minotaur.timer) {
                SetStep(7);
            }
            break;
        }
        break;
    case 0xC:
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_80180734, self)) {
                (self - 3)->ext.werewolf.signal = true;
                SetSubStep(1);
            }
            break;
        case 1:
            if (self->facingLeft) {
                self->velocityX = FIX(4.5);
            } else {
                self->velocityX = FIX(-4.5);
            }
            self->velocityY = 0;
            self->animCurFrame = 0x19;
            PlaySfxPositional(SFX_SCRAPE_A);
            self->step_s++;
            // fallthrough
        case 2:
            if (UnkCollisionFunc2(D_us_801806C8) & 0xF0) {
                self->ext.minotaur.timer = 1;
                self->step_s++;
            }
            break;
        case 3:
            AnimateEntity(D_us_8018073C, self);
            if (!--self->ext.minotaur.timer) {
                SetSubStep(4);
            }
            break;
        case 4:
            SetStep(7);
            break;
        }
        break;
    case 0xE:
        if (!AnimateEntity(D_us_80180750, self)) {
            SetStep(7);
        }
        if (!self->poseTimer && self->pose == 8) {
            for (i = 0; i < 8; i++) {
                entity = AllocEntity(
                    &g_Entities[224], &g_Entities[TOTAL_ENTITY_COUNT]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_MINOTAUR_FIREBALL, self, entity);
                    entity->posY.i.hi += 0x28;
                    entity->facingLeft = self->facingLeft;
                    entity->params = i;
                }
            }
        }
        break;
    case 0x22:
        switch (self->step_s) {
        case 0:
            entity = self - 3;
            self->ext.minotaur.signal = false;
            if (self->posX.i.hi > entity->posX.i.hi) {
                self->facingLeft = 0;
            } else {
                self->facingLeft = 1;
            }
            if (entity->flags & FLAG_DEAD) {
                SetStep(0x12);
                return;
            }
            self->step_s++;
            // fallthrough
        case 1:
            if (!AnimateEntity(D_us_80180770, self)) {
                SetSubStep(2);
            }
            break;
        case 2:
            if (self->ext.minotaur.signal) {
                PlaySfxPositional(SFX_MINOTAUR_ATTACK);
                self->step_s++;
            }
            if ((self - 3)->flags & FLAG_DEAD) {
                SetSubStep(3);
                return;
            }
            break;
        case 3:
            if (!AnimateEntity(D_us_8018077C, self)) {
                SetStep(7);
            }
            break;
        }
        break;
    case 0x10:
        if (!AnimateEntity(D_us_80180790, self)) {
            SetStep(2);
        }
        break;
    case 0x12:
        if (!self->step_s) {
            self->velocityX = 0;
            self->velocityY = 0;
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            self->step_s++;
        }
        UnkCollisionFunc2(D_us_801806C8);
        if (!AnimateEntity(D_us_80180798, self)) {
            SetStep(2);
        }
        if ((self - 3)->flags & FLAG_DEAD) {
            self->velocityX -= self->velocityX / 8;
            if (!self->poseTimer && self->pose == 7) {
                PlaySfxPositional(SFX_MINOTAUR_ATTACK);
                if (self->facingLeft) {
                    self->velocityX = FIX(4.0);
                } else {
                    self->velocityX = FIX(-4.0);
                }
            }
        }
        break;
    case 8:
        switch (self->step_s) {
        case 0:
            self->velocityY = FIX(-6.5);
            if (self->facingLeft) {
                self->velocityX = FIX(3.0);
            } else {
                self->velocityX = FIX(-3.0);
            }
            self->ext.minotaur.signal = false;
            self->step_s++;
            // fallthrough
        case 1:
            AnimateEntity(D_us_80180704, self);
            UnkCollisionFunc3(D_us_801806D0);
            if (self->velocityY > 0) {
                PlaySfxPositional(SFX_BOSS_WING_FLAP);
                SetSubStep(2);
            }
            break;
        case 2:
            AnimateEntity(D_us_80180714, self);
            if (UnkCollisionFunc3(D_us_801806D0) & 1) {
                if (!((self - 3)->flags & FLAG_DEAD) &&
                    (self - 3)->ext.werewolf.signal) {
                    if (self->posX.i.hi > (self - 3)->posX.i.hi) {
                        self->facingLeft = 0;
                    } else {
                        self->facingLeft = 1;
                    }
                    SetSubStep(3);
                } else {
                    SetStep(7);
                }
            }
            break;
        case 3:
            if (!AnimateEntity(D_us_80180850, self)) {
                SetSubStep(4);
            }
            break;
        case 4:
            if (self->ext.minotaur.signal) {
                if (!AnimateEntity(D_us_80180858, self)) {
                    SetStep(7);
                }
            }
            break;
        }
        break;
    case 0x14:
        if (!AnimateEntity(D_us_801807AC, self)) {
            SetStep(0x18);
        }
        break;
    case 0x18:
        if (!AnimateEntity(D_us_801807B4, self)) {
            SetStep(0x16);
        }
        break;
    case 0x16:
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_801807D0, self)) {
                SetSubStep(1);
            }
            if (!self->poseTimer && self->pose == 5) {
                PlaySfxPositional(SFX_UNK_BO2_789);
            }
            if (!self->poseTimer && self->pose == 3) {
                (self + 1)->ext.minotaur.axeThrown = true;
                self->ext.minotaur.axeThrown = false;
                self->ext.minotaur.attack = self->attack;
                self->attack *= 2;
            }
            break;
        case 1:
            if (self->facingLeft) {
                self->velocityX = FIX(2.5);
            } else {
                self->velocityX = FIX(-2.5);
            }
            self->velocityY = 0;
            self->step_s++;
            // fallthrough
        case 2:
            AnimateEntity(D_us_801807F4, self);
            if (UnkCollisionFunc2(D_us_801806C8) & 0xF0) {
                (self - 3)->ext.werewolf.signal = true;
                SetSubStep(3);
                break;
            }
            if (GetDistanceToPlayerX() < 8) {
                (self - 3)->ext.werewolf.signal = true;
                PlaySfxPositional(SFX_MINOTAURUS_JUMP_ATTACK);
                SetSubStep(3);
            }
            break;
        case 3:
            if (!AnimateEntity(D_us_801807FC, self)) {
                self->velocityX = 0;
                self->velocityY = FIX(-7.0);
                SetSubStep(4);
            }
            break;
        case 4:
            if (self->velocityY < 0) {
                AnimateEntity(D_us_80180808, self);
            } else {
                AnimateEntity(D_us_80180810, self);
            }
            if (UnkCollisionFunc3(D_us_801806D0) & 1) {
                PlaySfxPositional(SFX_STOMP_HARD_A);
                (self + 1)->ext.minotaur.axeThrown = false;
                self->attack = self->ext.minotaur.attack;
                SetSubStep(5);
            }
            break;
        case 5:
            AnimateEntity(D_us_80180818, self);
            if (self->ext.minotaur.axeThrown) {
                SetStep(7);
            }
            break;
        }
        break;
    case 0x21:
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 0x2B;
            self->step_s++;
            break;
        case 1:
            self->facingLeft ^= 1;
            self->step_s++;
            break;
        case 2:
            self->animCurFrame = 1;
            SetStep(7);
            break;
        }
        break;
    case 0x40:
        self->ext.minotaur.isReady = true;
        (self - 3)->ext.werewolf.signal = true;
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            self->velocityX = 0;
            self->velocityY = 0;
            self->step_s++;
            // fallthrough
        case 1:
            if (UnkCollisionFunc3(D_us_801806D0) & 1) {
                self->ext.minotaur.deathPuffPosX = -0x10;
                self->ext.minotaur.timer = 0;
                PlaySfxPositional(SFX_EXPLODE_A);
                self->step_s++;
            }
            break;
        case 2:
            if (!AnimateEntity(D_us_80180824, self)) {
                self->step_s = 3;
            }
            self->ext.minotaur.timer &= 1;
            // fallthrough
        case 3:
            if (self->ext.minotaur.timer & 1) {
                entity = AllocEntity(
                    &g_Entities[224], &g_Entities[TOTAL_ENTITY_COUNT]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_MINOTAUR_DEATH_PUFF, self, entity);
                    entity->posX.i.hi += self->ext.minotaur.deathPuffPosX;
                    entity->posY.i.hi += 0x20;
                    entity->zPriority = self->zPriority + 1;
                    entity->params = abs(self->ext.minotaur.deathPuffPosX) +
                                     (self->ext.minotaur.timer >> 1);
                }
                self->ext.minotaur.deathPuffPosX += 8;
                if (self->ext.minotaur.deathPuffPosX > 0x10) {
                    self->ext.minotaur.deathPuffPosX = -0x10;
                }
            }
            if (!(g_Timer & 0x1F)) {
                PlaySfxPositional(SFX_FIREBALL_SHOT_B);
            }
            if (self->ext.minotaur.timer++ > 0x40) {
                self->step_s++;
            }
            break;
        case 4:
            self->animCurFrame = 0;
            self->ext.minotaur.deathPuffPosX = 0;
            break;
        }
        break;
    case 0xFF:
#include "../../st/pad2_anim_debug.h"
    }

    ptr = *D_us_80180880;
    ptr += D_us_80180908[self->animCurFrame] * 4;
    self->hitboxOffX = *ptr++;
    self->hitboxOffY = *ptr++;
    self->hitboxWidth = *ptr++;
    self->hitboxHeight = *ptr++;
    func_us_801A2610();
}

#include "e_minotaur.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo2/nonmatchings/unk_224DC", func_us_801A3E04);
#else
// Attack patterns the fight controller cycles through. Each entry gives
// the position (relative to the player) both bosses walk to before starting
// the paired attack steps.
typedef struct {
    s32 minotaurPosX;
    s32 minotaurStep;
    s32 werewolfPosX;
    s32 werewolfStep;
} BossPattern;
extern BossPattern D_us_80180AB4[];
extern s32 D_us_80180B74;
extern s32 g_CutsceneFlags;

// Minotaur & Werewolf fight controller. The Werewolf is spawned right after
// it and the Minotaur four slots after it. The current attack pattern index
// is kept in ext.werewolf.moveAway.
void func_us_801A3E04(Entity* self) {
    Entity* minotaur;
    Entity* werewolf;
    Entity* entity;
    Primitive* prim;
    BossPattern* pattern;
    s16 scrollX;
    s16 scrollY;
    s16 posY;
    s32 primIndex;
    u8 index;

    scrollX = g_Tilemap.scrollX.i.hi;
    scrollY = g_Tilemap.scrollY.i.hi;
    werewolf = self + 1;
    FntPrint("set_step %x\n", self->step);
    minotaur = self + 4;
    FntPrint("boss_go %x\n", D_us_80180AAC);
    if (self->step >= 5 && self->step < 9 && werewolf->flags & FLAG_DEAD &&
        minotaur->flags & FLAG_DEAD && self->step != 8) {
        SetStep(8);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex == -1) {
            self->step = 0;
            return;
        }
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.werewolf.prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        prim->r0 = prim->g0 = prim->b0 = 0;
        prim->x1 = prim->x3 = 0x100;
        prim->y2 = prim->y3 = 0x100;
        prim->priority = 0x1F0;
        prim->x0 = prim->x2 = 0;
        prim->y0 = prim->y1 = 0;
        prim->drawMode = DRAW_HIDE;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        self->ext.werewolf.moveAway = 0;
        // fallthrough
    case 1:
        if (g_CastleFlags[RICHTER_CS_AFTER_M_AND_W] || g_PlayableCharacter ||
            g_DemoMode != Demo_None) {
            if (GetDistanceToPlayerX() < 0x10) {
                D_us_80180B74 = 1;
                stopMusicFlag = false;
                currentMusicId = 0x31D;
                g_api.PlaySfx(0x31D);
                SetStep(2);
                return;
            }
        } else if (g_CutsceneFlags & 0x10) {
            SetStep(2);
            return;
        }
        break;
    case 2:
        switch (self->step_s) {
        case 0:
            entity = &g_Entities[112];
            CreateEntityFromCurrentEntity(E_UNK_28, entity);
            posY = 0xD0 - scrollY;
            entity->posX.i.hi = 0x150 - scrollX;
            entity->posY.i.hi = posY;
            entity->params = 0;
            entity = &g_Entities[113];
            CreateEntityFromCurrentEntity(E_UNK_28, entity);
            entity->posX.i.hi = 0xA0 - scrollX;
            entity->posY.i.hi = posY;
            entity->params = 1;
            self->ext.werewolf.timer = 0x80;
            self->step_s++;
            // fallthrough
        case 1:
            if (!--self->ext.werewolf.timer) {
                SetStep(3);
                return;
            }
            break;
        }
        break;
    case 3:
        CreateEntityFromCurrentEntity(E_MINOTAUR, minotaur);
        CreateEntityFromCurrentEntity(E_WEREWOLF, werewolf);
        minotaur->posX.i.hi = 0x160 - scrollX;
        minotaur->posY.i.hi = 0xA8 - scrollY;
        werewolf->posX.i.hi = 0xA0 - scrollX;
        werewolf->posY.i.hi = 0xB5 - scrollY;
        g_api.TimeAttackController(
            TIMEATTACK_EVENT_MINOTAUR_WEREWOLF_DEFEAT, TIMEATTACK_SET_VISITED);
        SetStep(4);
        // fallthrough
    case 4:
        switch (self->step_s) {
        case 0:
            self->ext.werewolf.timer = 0x100;
            self->step_s++;
            // fallthrough
        case 1:
            if (--self->ext.werewolf.timer) {
                return;
            }
            self->step_s++;
            break;
        case 2:
            if (g_CastleFlags[RICHTER_CS_AFTER_M_AND_W] ||
                g_PlayableCharacter || g_DemoMode != Demo_None) {
                D_us_80180AAC = 1;
            }
            SetStep(5);
            return;
        }
        break;
    case 5:
        pattern = &D_us_80180AB4[self->ext.werewolf.moveAway];
        if (!(minotaur->flags & FLAG_DEAD)) {
            minotaur->ext.minotaur.deathPuffPosX = pattern->minotaurPosX;
            minotaur->ext.minotaur.nextStep = pattern->minotaurStep;
        }
        if (!(werewolf->flags & FLAG_DEAD)) {
            werewolf->ext.minotaur.deathPuffPosX = pattern->werewolfPosX;
            werewolf->ext.minotaur.nextStep = pattern->werewolfStep;
        }
        SetStep(6);
        return;
    case 6:
        if (minotaur->ext.minotaur.isReady && werewolf->ext.minotaur.isReady) {
            pattern = &D_us_80180AB4[self->ext.werewolf.moveAway];
            if (pattern->werewolfStep == 0xE && pattern->minotaurStep == 0xC &&
                abs(minotaur->posX.i.hi - werewolf->posX.i.hi) < 0x40) {
                // too close to charge at each other: breath + flame spin
                minotaur->ext.minotaur.nextStep = 0xB;
                werewolf->ext.minotaur.nextStep = 0x18;
            }
            werewolf->ext.minotaur.hasNextStep = true;
            minotaur->ext.minotaur.hasNextStep = true;
            SetStep(7);
            return;
        }
        break;
    case 7:
        index = self->ext.werewolf.moveAway;
        self->ext.werewolf.moveAway = index + 1;
        pattern = &D_us_80180AB4[self->ext.werewolf.moveAway];
        if (pattern->werewolfStep == 0x18 &&
            (minotaur->posX.i.hi - PLAYER.posX.i.hi) *
                    (werewolf->posX.i.hi - PLAYER.posX.i.hi) <
                0) {
            // the player stands between both bosses
            self->ext.werewolf.moveAway = index + 2;
            pattern++;
        }
        if (pattern->minotaurPosX == -1) {
            self->ext.werewolf.moveAway = 0;
        }
        SetStep(5);
        return;
    case 8:
        switch (self->step_s) {
        case 0:
            g_api.PlaySfx(SET_UNK_92);
            self->ext.werewolf.timer = 0x100;
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_MINOTAUR_WEREWOLF_DEFEAT,
                TIMEATTACK_SET_RECORD);
            self->step_s++;
            // fallthrough
        case 1:
            if (!--self->ext.werewolf.timer) {
                self->step_s++;
                if (!g_CastleFlags[RICHTER_CS_AFTER_M_AND_W] &&
                    !g_PlayableCharacter && g_DemoMode == Demo_None) {
                    CreateEntityFromCurrentEntity(
                        E_CUTSCENE_DIALOGUE, &g_Entities[200]);
                    g_Entities[200].params = 1;
                    CreateEntityFromCurrentEntity(E_UNK_26, &g_Entities[96]);
                    g_Entities[96].params = 1;
                    return;
                }
                self->step_s++;
            }
            break;
        case 2:
            if (g_CutsceneFlags & 0x1000) {
                self->step_s++;
            }
            break;
        case 3:
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_LIFE_UP_SPAWN, self, entity);
                stopMusicFlag = true;
                D_us_80180B74 = 0;
                entity->posX.i.hi = 0x100 - g_Tilemap.scrollX.i.hi;
                entity->params = 2;
                entity->posY.i.hi = 0x80 - g_Tilemap.scrollY.i.hi;
                currentMusicId = 0x323;
                self->step_s++;
            }
            break;
        case 4:
            if (!g_api.func_80131F68()) {
                stopMusicFlag = false;
                g_api.PlaySfx(currentMusicId);
                self->step_s++;
            }
            break;
        }
        break;
    }
}
#endif

void func_us_801A460C(Entity* self) {
    DRAWENV drawEnv;
    RECT clipRect;
    RECT unused;
    Primitive* prim;
    DR_ENV* dr_env;
    Entity* entity;
    s32 primIndex;
    s32 left;
    s32 right;
    s32 floorY;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 2);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->ext.et_801A460C.prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        dr_env = g_api.func_800EDB08((POLY_GT4*)prim);
        if (dr_env == NULL) {
            DestroyEntity(self);
            return;
        }
        prim->type = PRIM_ENV;
        drawEnv = g_CurrentBuffer->draw;
        clipRect.w = 0x100;
        clipRect.y = 0x14;
        drawEnv.isbg = 0;
        drawEnv.r0 = 0;
        drawEnv.g0 = 0;
        drawEnv.b0 = 0;
        drawEnv.dtd = 0;
        drawEnv.ofs[0] = 0;
        clipRect.x = 0;
        clipRect.h = 0xCF;
        drawEnv.clip = clipRect;
        SetDrawEnv(dr_env, &drawEnv);
        prim->priority = self->params * 4 + 0x9F;
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
        dr_env = g_api.func_800EDB08((POLY_GT4*)prim);
        if (dr_env == NULL) {
            DestroyEntity(self);
            return;
        }
        prim->type = PRIM_ENV;
        prim->drawMode = DRAW_UNK_800 | DRAW_HIDE;
        prim->priority = self->params * 4 + 0xA1;
        self->ext.et_801A460C.halfWidth = 0;
    case 1:
        switch (self->step_s) {
        case 0:
            if (++self->ext.et_801A460C.timer & 1) {
                self->ext.et_801A460C.halfWidth++;
            }
            if (!(g_Timer & 0xF)) {
                g_api.PlaySfx(SFX_FIREBALL_SHOT_B);
            }
            if (self->ext.et_801A460C.halfWidth >= 0x1E) {
                self->step_s++;
            }
            break;
        case 1:
            if (D_us_80180AB0) {
                self->ext.et_801A460C.timer = 0x80;
                self->step_s++;
            }
            if (!(g_Timer & 0xF)) {
                g_api.PlaySfx(SFX_FIREBALL_SHOT_B);
            }
            break;
        case 2:
            if (!--self->ext.et_801A460C.timer) {
                DestroyEntity(self);
                return;
            }
            break;
        }

        left = self->posX.i.hi - self->ext.et_801A460C.halfWidth;
        right = self->ext.et_801A460C.halfWidth + self->posX.i.hi;
        prim = self->ext.et_801A460C.prim;
        floorY = 0xD0 - g_Tilemap.scrollY.i.hi;
        if (left < 0) {
            left = 0;
        }
        if (left > 0x100) {
            left = 0x100;
        }
        if (right < 0) {
            right = 0;
        }
        if (right > 0x100) {
            right = 0x100;
        }
        dr_env = *(DR_ENV**)&prim->r1;
        drawEnv = g_CurrentBuffer->draw;
        clipRect.w = right - left;
        clipRect.y = 0x14;
        drawEnv.isbg = 0;
        clipRect.h = floorY - 0x15;
        clipRect.x = drawEnv.ofs[0] + left;
        drawEnv.clip = clipRect;
        SetDrawEnv(dr_env, &drawEnv);
        prim->drawMode = DRAW_DEFAULT;
        prim = prim->next;
        prim->drawMode = DRAW_UNK_800;

        if (!D_us_80180AB0 && !(g_Timer & 1)) {
            if (Random() & 1) {
                entity = AllocEntity(&g_Entities[144], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_29, entity);
                    entity->zPriority = self->params * 4 + 0xA0;
                    entity->posX.i.hi =
                        self->posX.i.hi - self->ext.et_801A460C.halfWidth / 2;
                    entity->posX.i.hi -= Random() & 0xF;
                    entity->posY.i.hi = 0xD0;
                    entity->rotate = Random() + 0x40;
                }
            } else {
                entity = AllocEntity(&g_Entities[144], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_29, entity);
                    entity->zPriority = self->params * 4 + 0xA0;
                    entity->posX.i.hi =
                        self->posX.i.hi + self->ext.et_801A460C.halfWidth / 2;
                    entity->posX.i.hi += Random() & 0xF;
                    entity->posY.i.hi = 0xD0;
                    left = Random();
                    entity->rotate = -0x40 - left;
                }
            }
        }
        break;
    }
}

#include "e_unk_29.h"
