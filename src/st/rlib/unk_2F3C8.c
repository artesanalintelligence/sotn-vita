// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rlib.h"

#ifndef NON_MATCHING
INCLUDE_ASM("st/rlib/nonmatchings/unk_2F3C8", func_us_801AF3C8);
#else
// Moves *value towards target by step, returns true once it is reached
s32 func_us_801AF3C8(s16* value, s16 target, s16 step) {
    s32 diff = *value - target;

    if (abs(diff) < step) {
        *value = target;
        return true;
    }
    if (*value > target) {
        *value = *value - step;
    }
    if (*value < target) {
        *value = *value + step;
    }
    return false;
}
#endif

// Scarecrow body (E_UNK_24) and its four limbs (E_UNK_25), which are
// spawned in the entity slots right after the body
typedef struct {
    /* 0x7C */ s32 : 32;
    /* 0x80 */ u16 timer;
    /* 0x82 */ s16 : 16;
    /* 0x84 */ s32 prevVelocityX;
    /* 0x88 */ s32 prevVelocityY;
} ET_Scarecrow;
#define SCARECROW(entity) ((ET_Scarecrow*)&(entity)->ext)

typedef struct {
    s16 animCurFrame;
    s16 zPriority;
    s16 offsetX;
    s16 offsetY;
    s16 minRotate;
    s16 maxRotate;
} ScarecrowLimb;

extern EInit g_EInitScarecrow;
extern EInit D_us_801806A0;
extern ScarecrowLimb D_us_80181954[];
extern s16 D_us_80181984[];

#ifndef NON_MATCHING
INCLUDE_ASM("st/rlib/nonmatchings/unk_2F3C8", func_us_801AF448);
#else
void func_us_801AF448(Entity* self) {
    Collider collider;
    Entity* entity;
    Entity* limb;
    s32 sideToPlayer;
    s32 distance;
    s32 floorOffset;
    s32 radius;
    s32 offsetX;
    s32 offsetY;
    s16 angle;
    s32 i;

    if ((self->flags & FLAG_DEAD) && self->step != 3) {
        SetStep(3);
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitScarecrow);
        self->drawFlags = ENTITY_ROTATE;
        self->animCurFrame = 2;
        self->hitboxWidth = 12;
        self->hitboxHeight = 8;
        self->hitboxOffY = -6;
        if (!(Random() & 0xF)) {
            self->params |= 1;
        }
        if (self->params) {
            // limbless variant
            self->animCurFrame = 7;
            self->hitboxWidth = 9;
            self->hitboxHeight = 12;
            self->hitboxOffY = -6;
            self->palette--;
            break;
        }
        limb = self + 1;
        for (i = 0; i < 4; i++, limb++) {
            CreateEntityFromEntity(E_UNK_25, self, limb);
            limb->params = i;
            limb->nextPart = limb - 1;
            limb->parent = self;
        }
        self->nextPart = self + 4;
        /* fallthrough */
    case 1:
        switch (self->step_s) {
        case 0:
            self->velocityY = FIX(-2.5);
            if (self->facingLeft) {
                self->velocityX = FIX(1.5);
            } else {
                self->velocityX = FIX(-1.5);
            }
            self->step_s++;
            /* fallthrough */
        case 1:
            MoveEntity();
            self->velocityY += FIX(0.125);
            if (self->velocityY > 0) {
                func_us_801AF3C8(&self->rotate, 0x100, 8);
            } else {
                func_us_801AF3C8(&self->rotate, -0x100, 0x10);
            }
            g_api.CheckCollision(
                self->posX.i.hi, self->posY.i.hi + 48, &collider, 0);
            if (!(collider.effects & EFFECT_SOLID)) {
                break;
            }
            self->step_s = 0;
            PlaySfxPositional(SFX_STOMP_HARD_D);
            floorOffset = collider.unk18;
            g_api.CheckCollision(
                self->posX.i.hi, self->posY.i.hi + 40, &collider, 0);
            if (collider.effects & EFFECT_SOLID) {
                self->facingLeft ^= 1;
                break;
            }
            self->posY.i.hi += floorOffset;
            sideToPlayer = (GetSideToPlayer() & 1) ^ 1;
            distance = GetDistanceToPlayerX();
            if (sideToPlayer != self->facingLeft && distance > 0x40) {
                self->facingLeft ^= 1;
                if (!(Random() & 3)) {
                    SetStep(2);
                }
            }
            break;
        }
        break;

    case 2:
        if (!self->step_s) {
            self->velocityX = 0;
            self->velocityY = 0;
            SCARECROW(self)->timer = (Random() & 0x1F) + 0x20;
            self->step_s++;
        }
        i = UnkCollisionFunc2(D_us_80181984);
        if (self->facingLeft) {
            self->velocityX += FIX(0.0625);
        } else {
            self->velocityX -= FIX(0.0625);
        }
        if (i == 0xFF) {
            SCARECROW(self)->timer = 1;
        }
        if (!--SCARECROW(self)->timer) {
            self->facingLeft = GetSideToPlayer() & 1;
            SetStep(1);
        }
        break;

    case 3:
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        radius = 0x30;
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 3;
        }
        angle = self->rotate + 0x400;
        if (self->facingLeft) {
            angle = 0x800 - angle;
        }
        for (i = 0; i < 8; i++) {
            offsetX = (radius * rcos(angle)) >> 12;
            offsetY = (radius * rsin(angle)) >> 12;
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            radius -= 8;
            if (entity != NULL) {
                CreateEntityFromEntity(E_EXPLODE_PUFF_OPAQUE, self, entity);
                entity->params = 3;
                entity->rotate = self->rotate;
                entity->posY.i.hi += offsetY;
                entity->posX.i.hi += offsetX;
            }
        }
        PlaySfxPositional(SFX_FIRE_SHOT);
        DestroyEntity(self);
        break;

    case 0xFF:
#include "../pad2_anim_debug.h"
    }
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("st/rlib/nonmatchings/unk_2F3C8", func_us_801AF9E8);
#else
// Scarecrow limb: params is the limb index, the body is params + 1 slots back
void func_us_801AF9E8(Entity* self) {
    Entity* body;
    ScarecrowLimb* limb;
    s32 offsetX;
    s32 offsetY;
    s32 sine;
    s32 cosine;
    s32 deltaX;
    s32 deltaY;
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801806A0);
        limb = &D_us_80181954[self->params];
        self->drawFlags |= ENTITY_ROTATE;
        body = self - (self->params + 1);
        self->animCurFrame = limb->animCurFrame;
        self->zPriority = body->zPriority + limb->zPriority;
        SCARECROW(self)->prevVelocityX = body->velocityX;
        SCARECROW(self)->prevVelocityY = body->velocityY;
        /* fallthrough */
    case 1:
        body = self - (self->params + 1);
        self->posX.i.hi = body->posX.i.hi;
        self->posY.i.hi = body->posY.i.hi;
        self->facingLeft = body->facingLeft;
        limb = &D_us_80181954[self->params];
        self->palette = body->palette;
        offsetX = limb->offsetX;
        angle = body->rotate;
        offsetY = limb->offsetY;
        sine = rsin(angle);
        cosine = rcos(angle);
        deltaX = (offsetX * cosine - offsetY * sine) >> 12;
        deltaY = (offsetX * sine + offsetY * cosine) >> 12;
        if (self->facingLeft) {
            self->posX.i.hi -= deltaX;
        } else {
            self->posX.i.hi += deltaX;
        }
        self->posY.i.hi += deltaY;

        // swing the limb according to how the body accelerates
        deltaX = body->velocityX - SCARECROW(self)->prevVelocityX;
        if (self->facingLeft) {
            deltaX = -deltaX;
        }
        deltaY = SCARECROW(self)->prevVelocityY - body->velocityY;
        self->rotate += (deltaX >> 8) + (deltaY >> 8);
        func_us_801AF3C8(&self->rotate, 0, 0x10);
        if (self->rotate > limb->maxRotate) {
            self->rotate = limb->maxRotate;
        }
        if (self->rotate < limb->minRotate) {
            self->rotate = limb->minRotate;
        }
        SCARECROW(self)->prevVelocityX = body->velocityX;
        SCARECROW(self)->prevVelocityY = body->velocityY;
        if (body->entityId != E_UNK_24) {
            DestroyEntity(self);
        }
        break;
    }
}
#endif

extern EInit g_EInitSchmoo;
extern AnimationFrame D_us_8018198C;
extern AnimationFrame D_us_80181998;

void func_us_801AFC88(Entity* self) {
    Entity* entity;
    s32 sideToPlayer;
    s32 distanceToPlayer;

    if (self->flags & FLAG_DEAD) {
        PlaySfxPositional(SFX_EXPLODE_B);
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 1;
        }
        DestroyEntity(self);
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitSchmoo);
        self->drawFlags = ENTITY_ROTATE;
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        /* fall through */
    case 1:
        AnimateEntity(&D_us_8018198C, self);
        MoveEntity();
        sideToPlayer = GetSideToPlayer();

        if (self->facingLeft) {
            self->velocityX += FIX(0.046875);
            if (self->velocityX > FIX(3)) {
                self->velocityX = FIX(3);
            }
        } else {
            self->velocityX -= FIX(0.046875);
            if (self->velocityX < FIX(-3)) {
                self->velocityX = FIX(-3);
            }
        }

        if (sideToPlayer & 2) {
            self->velocityY -= FIX(0.046875);
            if (self->velocityY < FIX(-1.5)) {
                self->velocityY = FIX(-1.5);
            }
        } else {
            self->velocityY += FIX(0.046875);
            if (self->velocityY > FIX(1.5)) {
                self->velocityY = FIX(1.5);
            }
        }

        sideToPlayer = (sideToPlayer & 1) ^ 1;
        distanceToPlayer = GetDistanceToPlayerX();
        if (sideToPlayer != self->facingLeft && distanceToPlayer >= 0x59) {
            SetStep(2);
        }
        break;

    case 2:
        MoveEntity();
        switch (self->step_s) {
        case 0:
            self->velocityX -= self->velocityX >> 4;
            self->velocityY -= self->velocityY >> 4;
            if (abs(self->velocityX) < FIX(0.25)) {
                self->step_s++;
            }
            break;

        case 1:
            self->velocityX -= self->velocityX >> 8;
            self->velocityY -= self->velocityY >> 8;
            if (AnimateEntity(&D_us_80181998, self) == 0) {
                SetStep(1);
            }
            if (!self->poseTimer && self->pose == 2) {
                self->facingLeft ^= 1;
            }
            break;
        }
        break;
    }

    self->rotate = -abs(self->velocityX >> 8);
}
