// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B37D0);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B385C);

#include "e_werewolf_attack_hitbox.h"

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B503C);

#include "e_werewolf_after_image.h"

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B52FC);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", EntityWerewolfDeathFlames);
#else
// The Werewolf works in tandem with the Minotaur, which always sits three
// entity slots after it. Both share the ET_Minotaur extension layout, but the
// signal the Minotaur raises on the Werewolf is written through
// ext.werewolf.signal, so it is read back through the same member here.
#define MINOTAUR(self) ((self) + 3)

extern EInit g_EInitWerewolfARE;
extern EInit D_us_80180578;
extern EInit D_us_80180584;
extern EInit g_EInitInteractable;
extern s32 D_us_80180AAC;
extern s32 D_us_80180AB0;
extern s16* sprites_bo2_3[];

extern s16 D_us_80181850[]; // sensors
extern s16 D_us_80181858[]; // sensors_ground
extern u8 D_us_80181868[];  // anim_idle
extern u8 D_us_80181874[];  // anim_walk
extern u8 D_us_80181884[];  // anim_punch
extern u8 D_us_801818A4[];  // anim_jump_back
extern u8 D_us_801818AC[];  // anim_energy_wave
extern u8 D_us_801818BC[];  // anim_slide_attack
extern u8 D_us_801818D0[];
extern u8 D_us_801818D8[];
extern u8 D_us_801818E0[]; // anim_charge_init
extern u8 D_us_801818EC[]; // anim_charge_attack
extern u8 D_us_801818FC[]; // anim_death_pain
extern u8 D_us_80181908[]; // anim_death_melt
extern u8 D_us_80181930[]; // anim_spin_attack
extern s8 D_us_80181958[][4];
extern u8 D_us_801819F8[];
extern SVECTOR D_us_80181ACC[];

// Keeps the Werewolf inside the arena walls
void func_us_801B37D0(void) {
    s32 posX;

    posX = g_CurrentEntity->posX.i.hi + g_Tilemap.scrollX.i.hi;
    if (g_CurrentEntity->velocityX > 0) {
        posX += 8;
        if (posX > 0x1B0) {
            g_CurrentEntity->posX.i.hi = 0x1A8 - g_Tilemap.scrollX.i.hi;
        }
    }
    if (g_CurrentEntity->velocityX < 0) {
        posX -= 8;
        if (posX < 0x50) {
            g_CurrentEntity->posX.i.hi = 0x58 - g_Tilemap.scrollX.i.hi;
        }
    }
}

static void SpawnSpinAfterImage(Entity* self) {
    Entity* entity;

    entity = AllocEntity(&g_Entities[224], &g_Entities[TOTAL_ENTITY_COUNT]);
    if (entity != NULL) {
        CreateEntityFromEntity(
            E_WEREWOLF_SPIN_ATTACK_AFTER_IMAGE, self, entity);
        entity->facingLeft = self->facingLeft;
        entity->rotate = self->rotate;
        entity->params = self->animCurFrame;
    }
}

void func_us_801B385C(Entity* self) {
    Entity* entity;
    s8* ptr;
    s32 posX;
    s32 arrived;
    s16 angle;

    FntPrint("wolf_step %x\n", self->step);
    FntPrint("wolf_set_step %x\n", self->ext.minotaur.nextStep);
    if (self->flags & FLAG_DEAD && self->step < 0x20) {
        SetStep(0x20);
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitWerewolfARE);
        self->hitboxState = 0;
        self->zPriority += 8;
        entity = self + 1;
        CreateEntityFromCurrentEntity(E_WEREWOLF_ATTACK_HITBOX, entity);
        // fallthrough
    case 1:
        AnimateEntity(D_us_80181868, self);
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        UnkCollisionFunc3(D_us_80181858);
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
                self->blendMode = BLEND_NO;
                self->opacity = 0;
                self->palette = g_EInitWerewolfARE[3];
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
        AnimateEntity(D_us_80181868, self);
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        if (self->ext.minotaur.moveTimer) {
            self->ext.minotaur.moveTimer--;
        } else {
            self->ext.minotaur.isReady = true;
        }

        if (self->ext.minotaur.deathPuffPosX) {
            self->ext.minotaur.isReady = false;
            SetStep(4);
        } else if (self->ext.minotaur.hasNextStep) {
            self->ext.minotaur.isReady = false;
            self->ext.minotaur.hasNextStep = false;
            SetStep(self->ext.minotaur.nextStep);
        } else {
            entity = &PLAYER;
            posX = entity->velocityX;
            if (self->facingLeft) {
                posX = -posX;
            }
            if (self->facingLeft == entity->facingLeft &&
                GetDistanceToPlayerX() < 0x38 && posX > 0) {
                self->ext.minotaur.isReady = false;
                SetStep(6);
            }
        }
        break;
    case 4:
        // walk to the X position requested by the fight controller
        if (!self->step_s) {
            posX = self->ext.minotaur.deathPuffPosX;
            if (self->posX.i.hi > PLAYER.posX.i.hi) {
                posX += PLAYER.posX.i.hi;
                posX += g_Tilemap.scrollX.i.hi;
                if (posX > 0x190) {
                    posX = 0x190;
                }
            } else {
                posX = PLAYER.posX.i.hi - posX;
                posX += g_Tilemap.scrollX.i.hi;
                if (posX < 0x70) {
                    posX = 0x70;
                }
            }
            self->ext.minotaur.deathPuffPosX = posX;
            posX = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
            posX = self->ext.minotaur.deathPuffPosX - posX;
            if (posX > 0) {
                self->velocityX = FIX(1.125);
            } else {
                self->velocityX = FIX(-1.125);
            }
            self->step_s++;
        }
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        AnimateEntity(D_us_80181874, self);
        UnkCollisionFunc2(D_us_80181850);
        arrived = false;
        posX = self->posX.i.hi + g_Tilemap.scrollX.i.hi -
               self->ext.minotaur.deathPuffPosX;
        if (self->velocityX > 0) {
            if (posX > 0) {
                arrived = true;
            }
        } else if (posX < 0) {
            arrived = true;
        }
        if (arrived) {
            self->ext.minotaur.deathPuffPosX = 0;
            SetStep(2);
        }
        break;
    case 6:
        if (!AnimateEntity(D_us_80181884, self)) {
            SetStep(2);
        }
        if (!self->poseTimer && self->pose == 6) {
            PlaySfxPositional(SFX_WHIP_TWIRL_SWISH);
        }
        break;
    case 8:
        switch (self->step_s) {
        case 0:
            posX = MINOTAUR(self)->posX.i.hi - self->posX.i.hi;
            if (self->facingLeft == MINOTAUR(self)->facingLeft &&
                abs(posX) < 0x20) {
                self->ext.werewolf.signal = true;
            } else {
                self->ext.werewolf.signal = false;
            }
            MINOTAUR(self)->ext.minotaur.signal =
                self->ext.werewolf.signal ^ 1;
            self->step_s++;
            // fallthrough
        case 1:
            if (!AnimateEntity(D_us_801818A4, self)) {
                self->animCurFrame = 0x2F;
                self->velocityX = FIX(-2.0);
                if (self->facingLeft) {
                    self->velocityX = FIX(2.0);
                }
                self->velocityY = FIX(-2.0);
                SetSubStep(3);
            }
            break;
        case 3:
            if (UnkCollisionFunc3(D_us_80181858) & 1) {
                self->step_s++;
            }
            break;
        case 4:
            if (!AnimateEntity(D_us_801818AC, self)) {
                SetStep(4);
            }
            if (!self->poseTimer && self->pose == 4) {
                PlaySfxPositional(SFX_EXPLODE_F);
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_1D, self, entity);
                    entity->ext.werewolf.prim = (Primitive*)MINOTAUR(self);
                    entity->facingLeft = self->facingLeft;
                    entity->posY.i.hi -= 0x10;
                    entity->ext.werewolf.signal = self->ext.werewolf.signal;
                }
            }
            break;
        }
        break;
    case 0xA:
        if (!self->step_s) {
            self->velocityX = 0;
            self->velocityY = 0;
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            self->step_s++;
        }
        UnkCollisionFunc2(D_us_80181850);
        if (!AnimateEntity(D_us_801818BC, self)) {
            SetStep(2);
        }
        self->velocityX -= self->velocityX / 16;
        if (!self->poseTimer && self->pose == 3) {
            PlaySfxPositional(SFX_SCRAPE_B);
            if (self->facingLeft) {
                self->velocityX = FIX(4.0);
            } else {
                self->velocityX = FIX(-4.0);
            }
        }
        break;
    case 0xC:
        if (!self->step_s) {
            if (!AnimateEntity(D_us_801818D0, self)) {
                self->ext.minotaur.timer = 0x80;
                self->step_s++;
            }
        } else {
            AnimateEntity(D_us_801818D8, self);
            if (!--self->ext.minotaur.timer) {
                SetStep(0xE);
            }
        }
        break;
    case 0xE:
        if (self->flags & 0xF) {
            self->blendMode = BLEND_NO;
        } else if (g_Timer & 1) {
            self->blendMode = BLEND_TRANSP | BLEND_ADD;
        } else {
            self->blendMode = BLEND_SUB | BLEND_TRANSP | BLEND_ADD;
        }
        switch (self->step_s) {
        case 0:
            self->velocityX = 0;
            self->velocityY = 0;
            self->ext.werewolf.signal = false;
            self->step_s++;
            // fallthrough
        case 1:
            if (self->ext.werewolf.signal) {
                self->step_s++;
            }
            break;
        case 2:
            if (!AnimateEntity(D_us_801818E0, self)) {
                SetSubStep(3);
            }
            if (!self->poseTimer && self->pose == 1) {
                if (self->facingLeft) {
                    self->velocityX = FIX(6.0);
                } else {
                    self->velocityX = FIX(-6.0);
                }
                self->ext.minotaur.timer = 0x30;
            }
            break;
        case 3:
            UnkCollisionFunc2(D_us_80181850);
            self->velocityX -= self->velocityX / 64;
            self->animCurFrame = 0x2B;
            if (!--self->ext.minotaur.timer) {
                SetSubStep(4);
            }
            break;
        case 4:
            UnkCollisionFunc2(D_us_80181850);
            self->velocityX -= self->velocityX / 16;
            if (!AnimateEntity(D_us_801818EC, self)) {
                self->blendMode = BLEND_NO;
                SetStep(4);
            }
            break;
        }
        break;
    case 0x12:
        switch (self->step_s) {
        case 0:
            if (self->facingLeft) {
                self->velocityX = FIX(3.5);
            } else {
                self->velocityX = FIX(-3.5);
            }
            self->ext.minotaur.timer = 0x20;
            self->animCurFrame = 0x17;
            self->velocityY = 0;
            self->drawFlags |= ENTITY_ROTATE;
            self->step_s++;
            // fallthrough
        case 1:
            UnkCollisionFunc2(D_us_80181850);
            self->rotate -= ROT(22.5);
            if (!(g_Timer & 8)) {
                PlaySfxPositional(SFX_WEREWOLF_SPIN_ATTACK);
            }
            if (!(self->ext.minotaur.timer & 3)) {
                SpawnSpinAfterImage(self);
            }
            if (!--self->ext.minotaur.timer) {
                self->step_s = 2;
            }
            break;
        case 2:
            self->drawFlags = ENTITY_DEFAULT;
            SetStep(4);
            break;
        }
        break;
    case 0x16:
        // jump attack combined with the Minotaur
        switch (self->step_s) {
        case 0:
            self->ext.werewolf.signal = false;
            if (MINOTAUR(self)->posX.i.hi < self->posX.i.hi) {
                self->facingLeft = 0;
            } else {
                self->facingLeft = 1;
            }
            if (self->facingLeft) {
                self->velocityX = FIX(3.5);
            } else {
                self->velocityX = FIX(-3.5);
            }
            self->velocityY = 0;
            if (MINOTAUR(self)->flags & FLAG_DEAD) {
                SetStep(0xA);
                return;
            }
            self->step_s++;
            // fallthrough
        case 1:
            if (!AnimateEntity(D_us_80181930, self)) {
                self->animCurFrame = 0x17;
                self->drawFlags |= ENTITY_ROTATE;
                self->step_s++;
            }
            break;
        case 2:
            UnkCollisionFunc2(D_us_80181850);
            self->rotate -= ROT(22.5);
            if (!(g_Timer & 7)) {
                PlaySfxPositional(SFX_WEREWOLF_SPIN_ATTACK);
            }
            if (!(self->ext.minotaur.timer & 3)) {
                SpawnSpinAfterImage(self);
            }
            posX = self->posX.i.hi - MINOTAUR(self)->posX.i.hi;
            if (self->facingLeft) {
                posX = -posX;
            }
            if (posX < 0x20) {
                MINOTAUR(self)->ext.minotaur.signal = true;
                self->rotate = 0;
                self->step_s++;
            }
            if (MINOTAUR(self)->flags & FLAG_DEAD) {
                SetSubStep(7);
            }
            break;
        case 3:
            self->velocityY = FIX(-8.5);
            self->velocityX = (PLAYER.posX.val - self->posX.val) / 64;
            self->animCurFrame = 8;
            if (self->velocityX > 0) {
                self->facingLeft = 1;
            } else {
                self->facingLeft = 0;
            }
            self->step_s++;
            break;
        case 4:
            MoveEntity();
            self->velocityY += FIX(0.25);
            if (self->posY.i.hi + g_Tilemap.scrollY.i.hi < 0x2C) {
                self->posY.i.hi = 0x2C - g_Tilemap.scrollY.i.hi;
                self->ext.minotaur.timer = 8;
                self->step_s++;
            }
            break;
        case 5:
            self->animCurFrame = 0x17;
            self->ext.minotaur.timer--;
            self->rotate -= ROT(33.75);
            if (!self->ext.minotaur.timer) {
                self->ext.minotaur.timer = 0xC;
                self->step_s++;
            }
            break;
        case 6:
            self->animCurFrame = 0x1B;
            self->rotate = 0;
            if (!--self->ext.minotaur.timer) {
                angle = GetAngleBetweenEntities(self, &PLAYER);
                self->velocityX = rcos(angle) * 0x50;
                self->velocityY = rsin(angle) * 0x50;
                self->ext.minotaur.attack = self->attack;
                self->attack *= 2;
                self->step_s++;
            }
            break;
        case 7:
            self->animCurFrame = 0x17;
            self->rotate -= ROT(33.75);
            if (!(g_Timer & 7)) {
                PlaySfxPositional(SFX_WEREWOLF_SPIN_ATTACK);
            }
            if (UnkCollisionFunc3(D_us_80181858) & 1) {
                PlaySfxPositional(SFX_STOMP_HARD_B);
                self->ext.minotaur.timer = 0x10;
                self->attack = self->ext.minotaur.attack;
                self->step_s++;
            }
            if (!(self->ext.minotaur.timer & 3)) {
                SpawnSpinAfterImage(self);
            }
            break;
        case 8:
            self->drawFlags = ENTITY_DEFAULT;
            self->animCurFrame = 0xC;
            if (!--self->ext.minotaur.timer) {
                SetStep(4);
            }
            break;
        }
        break;
    case 0x18:
        // spin through the Minotaur's flame breath
        switch (self->step_s) {
        case 0:
            self->ext.werewolf.signal = false;
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            self->step_s++;
            // fallthrough
        case 1:
            if (self->ext.werewolf.signal) {
                self->step_s++;
            }
            break;
        case 2:
            if (!AnimateEntity(D_us_80181930, self)) {
                self->animCurFrame = 0x17;
                self->drawFlags |= ENTITY_ROTATE;
                if (self->facingLeft) {
                    self->velocityX = FIX(3.5);
                } else {
                    self->velocityX = FIX(-3.5);
                }
                self->ext.minotaur.timer = 0x40;
                self->step_s++;
            }
            break;
        case 3:
            UnkCollisionFunc2(D_us_80181850);
            self->rotate -= ROT(22.5);
            if (!(g_Timer & 7)) {
                PlaySfxPositional(SFX_WEREWOLF_SPIN_ATTACK);
            }
            if (!(self->ext.minotaur.timer & 3)) {
                SpawnSpinAfterImage(self);
            }
            if (!(MINOTAUR(self)->flags & FLAG_DEAD)) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(
                        E_MINOTAUR_SPIT_LIQUID, self, entity);
                    entity->rotate = self->rotate;
                    if (self->facingLeft) {
                        angle = -self->rotate;
                    } else {
                        angle = self->rotate;
                    }
                    entity->posX.val += rsin(angle) * 0xC0;
                    entity->posY.val -= rcos(angle) * 0xC0;
                    entity->facingLeft = self->facingLeft;
                    entity->zPriority = self->zPriority + 1;
                }
            }
            if (!--self->ext.minotaur.timer) {
                self->step_s++;
            }
            break;
        case 4:
            self->drawFlags = ENTITY_DEFAULT;
            SetStep(4);
            break;
        }
        break;
    case 0x14:
        // spinning dive onto the player
        if (self->flags & 0xF) {
            self->blendMode = BLEND_NO;
        } else if (g_Timer & 1) {
            self->blendMode = BLEND_TRANSP | BLEND_ADD;
        } else {
            self->blendMode = BLEND_SUB | BLEND_TRANSP | BLEND_ADD;
        }
        switch (self->step_s) {
        case 0:
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            self->ext.werewolf.signal = false;
            self->step_s++;
            // fallthrough
        case 1:
            if (!AnimateEntity(D_us_80181930, self)) {
                self->step_s++;
            }
            break;
        case 2:
            self->velocityY = FIX(-4.0);
            self->velocityX = 0;
            self->drawFlags = ENTITY_ROTATE;
            self->step_s++;
            // fallthrough
        case 3:
            MoveEntity();
            self->rotate -= ROT(22.5);
            if (!(g_Timer & 7)) {
                PlaySfxPositional(SFX_WEREWOLF_SPIN_ATTACK);
            }
            if (g_Timer & 1) {
                self->animCurFrame = 0x18;
            } else {
                self->animCurFrame = 0x19;
            }
            if (!(self->ext.minotaur.timer & 3)) {
                SpawnSpinAfterImage(self);
            }
            if (self->posY.i.hi + g_Tilemap.scrollY.i.hi < 0x28) {
                self->step_s++;
            }
            break;
        case 4:
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            if (self->facingLeft) {
                self->velocityX = FIX(2.5);
            } else {
                self->velocityX = FIX(-2.5);
            }
            self->velocityY = 0;
            self->step_s++;
            // fallthrough
        case 5:
            MoveEntity();
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            if (self->facingLeft) {
                self->velocityX = FIX(2.5);
            } else {
                self->velocityX = FIX(-2.5);
            }
            self->rotate -= ROT(22.5);
            if (!(g_Timer & 7)) {
                PlaySfxPositional(SFX_WEREWOLF_SPIN_ATTACK);
            }
            if (g_Timer & 1) {
                self->animCurFrame = 0x18;
            } else {
                self->animCurFrame = 0x19;
            }
            if (self->ext.werewolf.signal) {
                self->animCurFrame = 0xA;
                self->drawFlags = ENTITY_DEFAULT;
                self->velocityX = 0;
                self->velocityY = FIX(2.0);
                self->step_s++;
            }
            break;
        case 6:
            if (UnkCollisionFunc3(D_us_80181858) & 1) {
                self->animCurFrame = 0xB;
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_1F, self, entity);
                    entity->posY.i.hi += 0x1B;
                    entity->zPriority = self->zPriority - 1;
                }
                PlaySfxPositional(SFX_FIREBALL_SHOT_A);
                self->ext.minotaur.timer = 0x40;
                self->step_s++;
            }
            break;
        case 7:
            if (!--self->ext.minotaur.timer) {
                self->blendMode = BLEND_NO;
                SetStep(4);
            }
            break;
        }
        break;
    case 0x20:
        self->ext.minotaur.isReady = true;
        MINOTAUR(self)->ext.minotaur.signal = true;
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            self->rotate = 0;
            self->velocityX = 0;
            self->velocityY = 0;
            self->step_s++;
            // fallthrough
        case 1:
            if (UnkCollisionFunc3(D_us_80181858) & 1) {
                self->step_s++;
            }
            // fallthrough
        case 2:
            if (!AnimateEntity(D_us_801818FC, self)) {
                PlaySfxPositional(SFX_EXPLODE_A);
                entity = AllocEntity(
                    &g_Entities[224], &g_Entities[TOTAL_ENTITY_COUNT]);
                if (entity != NULL) {
                    CreateEntityFromEntity(
                        E_WEREWOLF_DEATH_FLAMES, self, entity);
                    entity->posY.i.hi += 0x1B;
                    entity->zPriority = self->zPriority;
                }
                SetSubStep(3);
            }
            break;
        case 3:
            if (!AnimateEntity(D_us_80181908, self)) {
                SetSubStep(4);
            }
            break;
        case 4:
            self->ext.minotaur.deathPuffPosX = 0;
            self->animCurFrame = 0;
            break;
        }
        break;
    case 0xFF:
#include "../../st/pad2_anim_debug.h"
    }

    ptr = *D_us_80181958;
    ptr += D_us_801819F8[self->animCurFrame] * 4;
    self->hitboxOffX = *ptr++;
    self->hitboxOffY = *ptr++;
    self->hitboxWidth = *ptr++;
    self->hitboxHeight = *ptr++;
    func_us_801B37D0();
}

#include "e_werewolf_attack_hitbox.h"

// Energy wave thrown by the Werewolf; it can be bounced back by the Minotaur
// (ext.werewolf.prim holds the Minotaur entity here)
void func_us_801B503C(Entity* self) {
    Entity* minotaur;
    s32 velocityX;
    s32 posX;
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180578);
        self->animCurFrame = 0x39;
        if (self->facingLeft) {
            self->velocityX = FIX(4.5);
        } else {
            self->velocityX = FIX(-4.5);
        }
        minotaur = (Entity*)self->ext.werewolf.prim;
        self->velocityY = 0;
        velocityX = minotaur->velocityX;
        if (self->facingLeft) {
            velocityX = -velocityX;
        }
        if (velocityX > 0 || minotaur->flags & FLAG_DEAD) {
            self->ext.werewolf.signal = false;
            minotaur->ext.minotaur.signal = true;
        }
        // fallthrough
    case 1:
        MoveEntity();
        self->velocityY -= FIX(5.0 / 64.0);
        angle = ratan2(-self->velocityY, abs(self->velocityX));
        if (angle > 0x200) {
            self->animCurFrame = 0x3A;
        }
        if (angle > 0x280) {
            self->animCurFrame = 0x3B;
        }
        if (angle > 0x300) {
            self->animCurFrame = 0x3C;
        }
        if (angle > 0x380) {
            self->animCurFrame = 0x3D;
        }
        if (self->ext.werewolf.signal) {
            PlaySfxPositional(SFX_ANIME_SWORD_B);
            minotaur = (Entity*)self->ext.werewolf.prim;
            posX = self->posX.i.hi - minotaur->posX.i.hi;
            if (abs(posX) < 0x10) {
                minotaur->ext.minotaur.signal = true;
                self->velocityY = FIX(2.0);
                self->animCurFrame = 0x39;
                self->facingLeft ^= 1;
                self->velocityX = self->velocityX * -2;
                self->step++;
            }
        }
        break;
    case 2:
        MoveEntity();
        break;
    }
}

#include "e_werewolf_after_image.h"

// Fire pillar left on the ground by the Werewolf's dive
void func_us_801B52FC(Entity* self) {
    Entity* entity;
    Primitive* prim;
    s32 primIndex;
    s32 i;
    s32 offsetX;
    s32 speed;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180584);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 3);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->ext.prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        while (prim != NULL) {
            prim->tpage = 0x16;
            prim->clut = PAL_WEREWOLF + 1;
            prim->u0 = prim->u2 = 0x80;
            prim->u1 = prim->u3 = 0xA0;
            prim->v0 = prim->v1 = 0;
            prim->v2 = prim->v3 = 0;
            if (!self->params) {
                prim->x0 = prim->x2 = self->posX.i.hi - 0x18;
                prim->x1 = prim->x3 = self->posX.i.hi + 0x18;
            } else {
                prim->x0 = prim->x2 = self->posX.i.hi - 0xC;
                prim->x1 = prim->x3 = self->posX.i.hi + 0xC;
            }
            prim->y0 = prim->y1 = prim->y2 = prim->y3 = self->posY.i.hi;
            prim->drawMode = DRAW_UNK02;
            prim->priority = self->zPriority;
            prim = prim->next;
        }
        if (!self->params) {
            // two smaller pillars on the sides
            offsetX = -0x24;
            for (i = -1; i < 2; i += 2) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_1F, self, entity);
                    entity->params = 1;
                    entity->posX.i.hi += offsetX;
                }
                offsetX += 0x48;
            }
        }
        // fallthrough
    case 1:
        prim = self->ext.prim;
        speed = 4;
        if (self->params) {
            speed = 2;
        }
        prim->v2 = prim->v3 = prim->v3 + speed;
        prim->y0 = prim->y1 = prim->y1 - speed * 2;
        if (prim->v2 - prim->v0 >= 0x40) {
            self->step++;
        }
        break;
    case 2:
        prim = self->ext.prim;
        speed = 4;
        if (self->params) {
            speed = 2;
        }
        prim->y0 = prim->y1 = prim->y1 + speed;
        prim->v2 = prim->v3 = prim->v3 - speed / 2;
        if (prim->y2 - prim->y0 <= 0) {
            self->step++;
        }
        break;
    case 3:
        DestroyEntity(self);
        // the original code keeps updating the (now cleared) entity below,
        // which reads through a NULL primitive pointer
        return;
    }

    prim = self->ext.prim;
    self->hitboxWidth = (abs(prim->x0 - prim->x1) - 8) / 2;
    self->hitboxHeight = (abs(prim->y0 - prim->y2) * 3) / 10;
    self->hitboxOffY = 0xE - self->hitboxHeight;
    if (g_Timer & 1) {
        prim->u0 = prim->u2 = 0x80;
        prim->u1 = prim->u3 = 0xA0;
    } else {
        prim->u0 = prim->u2 = 0xA0;
        prim->u1 = prim->u3 = 0x80;
    }
}

Primitive* FindFirstUnkPrim2(Primitive* prim, u8 index);
void EntityWerewolfDeathFlames(Entity* self) {
    long otz;
    SVECTOR rot;
    VECTOR trans;
    MATRIX matrix;
    Primitive* prim;
    s16* ptr;
    s32 posY;
    s32 i;
    SVECTOR* sVec;
    s32 color;
    s32 primIndex;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x80);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->ext.werewolf.prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        while (prim != NULL) {
            prim->tpage = 0x17;
            prim->clut = 0x223;
            prim->drawMode = DRAW_HIDE;
            prim->priority = self->zPriority;
            prim = prim->next;
        }
        self->ext.werewolf.timer = 0x140;
        // fallthrough
    case 1:
        if (!--self->ext.werewolf.timer) {
            DestroyEntity(self);
            return;
        }
        if (!(self->ext.werewolf.timer & 0x1F)) {
            PlaySfxPositional(SFX_FIREBALL_SHOT_B);
        }

        if (!(self->ext.werewolf.timer & 3)) {
            color = self->ext.werewolf.timer;
            self->ext.werewolf.unk9C -= 0x20;
            if (color > 0x80) {
                color = 0x80;
            }
            for (i = 0; i < 3; i++) {
                prim = FindFirstUnkPrim2(self->ext.werewolf.prim, 2);
                if (prim != NULL) {
                    UnkPolyFunc2(prim);
                    prim->next->x2 = 0;
                    prim->next->y2 = 0;
                    prim->next->x1 =
                        self->ext.werewolf.unk9C + (i * 0x555) + Random();
                    prim->next->y1 = 0;
                    PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) =
                        PGREY(prim, 3) = color;
                    prim->next->drawMode = DRAW_HIDE;
                }
            }
        }

        SetGeomScreen(0x200);
        SetGeomOffset(self->posX.i.hi, self->posY.i.hi);
        prim = self->ext.werewolf.prim;
        sVec = D_us_80181ACC;
        posY = self->posY.i.hi + 8;
        while (prim != NULL) {
            if (prim->p3 == 8) {
                trans.vx = 0;
                trans.vy = prim->next->y1;
                trans.vz = 0x200;
                TransMatrix(&matrix, &trans);
                SetTransMatrix(&matrix);
                rot.vx = 0;
                rot.vy = prim->next->x1;
                rot.vz = 0x180;
                RotMatrix(&rot, &matrix);
                SetRotMatrix(&matrix);
                gte_ldv3c(sVec);
                gte_rtpt();
                gte_stsxy3_gt3(prim);
                gte_ldv0(&sVec[3]);
                gte_rtps();
                gte_stsxy((long*)&prim->x3);
                gte_avsz4();
#ifdef VERSION_US
                gte_stszotz(otz);
#else
                gte_stszotz(&otz);
#endif
                if (otz > 0x80) {
                    prim->priority = self->zPriority - 1;
                } else {
                    prim->priority = self->zPriority + 1;
                }
                if (posY < prim->y2) {
                    prim->y2 = posY;
                }
                if (posY < prim->y3) {
                    prim->y3 = posY;
                }
                prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                 DRAW_UNK02 | DRAW_TRANSP;
            }
            prim = prim->next;
        }

        prim = self->ext.werewolf.prim;
        while (prim != NULL) {
            if (prim->p3 == 8) {
                prim->next->y1 -= 3;
                prim->next->x1 += 0x30;
                prim->r0 -= 2;
                if (!prim->r0) {
                    UnkPolyFunc0(prim);
                    continue;
                }
                prim->g0 = prim->b0 = prim->r0;
                PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3) = prim->r0;
                if (!prim->next->y2) {
                    prim->next->x2++;
                    if (prim->next->x2 > 0xD) {
                        UnkPolyFunc0(prim);
                        continue;
                    }
                    ptr = sprites_bo2_3[prim->next->x2];
                    ptr += 8;
                    prim->u0 = prim->u2 = *ptr++;
                    prim->v0 = prim->v1 = *ptr++;
                    prim->u1 = prim->u3 = *ptr++;
                    prim->v2 = prim->v3 = *ptr++;
                    prim->next->y2 = 3;
                } else {
                    prim->next->y2--;
                }
            }
            prim = prim->next;
        }
        break;
    }
}
#endif
