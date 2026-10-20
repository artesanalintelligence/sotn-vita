// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rlib.h"

#ifndef NON_MATCHING
INCLUDE_ASM("st/rlib/nonmatchings/unk_2E4B4", func_us_801AE4B4);
#else
// Moves *value towards target by step, returns true once it is reached
s32 func_us_801AE4B4(s16* value, s16 target, s16 step) {
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

// Tin Man body, its arm (E_UNK_20) and the pieces it breaks into (E_UNK_23)
// share the same entity extension
typedef struct {
    /* 0x7C */ s32 : 32;
    /* 0x80 */ u16 timer;
    /* 0x82 */ s16 armAngle;
    /* 0x84 */ u8 hideArm;
    /* 0x85 */ u8 armBroken;
    /* 0x86 */ u8 hasGravity;
} ET_Tinman;
#define TINMAN(entity) ((ET_Tinman*)&(entity)->ext)

typedef struct {
    s16 timer;
    u8 hasGravity;
    s32 velocityX;
    s32 velocityY;
} TinmanPiece;

extern EInit g_EInitTinman;
extern EInit D_us_80180670;
extern EInit D_us_8018067C;
extern EInit D_us_80180688;
extern s16 D_us_80181820[];
extern s16 D_us_80181830[];
extern u8 D_us_80181838[];
extern u8 D_us_80181844[];
extern u8 D_us_80181880[];
extern u8 D_us_801818A8[];
extern u8 D_us_801818B8[];
extern TinmanPiece D_us_801818D0[];

#ifndef NON_MATCHING
INCLUDE_ASM("st/rlib/nonmatchings/unk_2E4B4", func_us_801AE534);
#else
void func_us_801AE534(Entity* self) {
    Entity* entity;
    s32 velocityCapped;
    s32 spawnSteam;
    s32 i;

    if (self->step &&
        self->hitPoints < g_api.enemyDefs[ENEMY_TIN_MAN].hitPoints / 4 &&
        self->step < 5) {
        SetStep(5);
    }
    if (self->hitFlags & 3) {
        PlaySfxPositional(SFX_UNK_RLIB_73D);
    }
    if ((self->flags & FLAG_DEAD) && self->step != 7) {
        SetStep(7);
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitTinman);
        self->hitboxOffY = 9;
        CreateEntityFromCurrentEntity(E_UNK_20, self + 1);
        self[1].zPriority = self->zPriority + 1;
        /* fallthrough */
    case 1:
        if (UnkCollisionFunc3(D_us_80181820) & 1) {
            SetStep(2);
        }
        break;

    case 2:
        if (GetDistanceToPlayerX() < 0x60) {
            SetStep(3);
        }
        break;

    case 3:
        AnimateEntity(D_us_80181838, self);
        velocityCapped = false;
        TINMAN(self)->hideArm = false;
        if (self->facingLeft) {
            self->velocityX += FIX(0.03125);
            if (self->velocityX > FIX(1.5)) {
                self->velocityX = FIX(1.5);
                velocityCapped = true;
            }
        } else {
            self->velocityX -= FIX(0.03125);
            if (self->velocityX < FIX(-1.5)) {
                self->velocityX = FIX(-1.5);
                velocityCapped = true;
            }
        }
        if (velocityCapped) {
            spawnSteam = !(Random() & 0x1F);
            if (self->poseTimer) {
                self->poseTimer--;
            }
        } else {
            spawnSteam = !(g_Timer & 0xF);
        }
        if (spawnSteam) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_22, self, entity);
                entity->ext.et_801AEFE0.parent = self;
                entity->facingLeft = self->facingLeft;
                entity->zPriority = self->zPriority + 1;
            }
        }
        if (UnkCollisionFunc2(D_us_80181830) & 0x60) {
            self->posX.val -= self->velocityX;
            self->velocityX = -self->velocityX;
        }
        if (GetDistanceToPlayerX() < 0x50) {
            TINMAN(self)->armAngle = (TINMAN(self)->armAngle - 0x100) & 0xFFF;
        } else {
            func_us_801AE4B4(&TINMAN(self)->armAngle, 0, 0x80);
        }
        if ((GetSideToPlayer() & 1) == self->facingLeft &&
            GetDistanceToPlayerX() > 0x30) {
            SetStep(4);
        }
        break;

    case 4:
        self->velocityX -= self->velocityX / 32;
        if (UnkCollisionFunc2(D_us_80181830) & 0x60) {
            self->posX.val -= self->velocityX;
            self->velocityX = -self->velocityX;
        }
        switch (self->step_s) {
        case 0:
            TINMAN(self)->hideArm = false;
            if (func_us_801AE4B4(&TINMAN(self)->armAngle, 0, 0x80)) {
                self->step_s++;
            }
            break;
        case 1:
            TINMAN(self)->hideArm = true;
            if (!AnimateEntity(D_us_801818B8, self)) {
                TINMAN(self)->hideArm = false;
                self->animCurFrame = 1;
                self->facingLeft ^= 1;
                SetStep(3);
            }
            break;
        }
        break;

    case 5:
        switch (self->step_s) {
        case 0:
            if (GetSideToPlayer() & 1) {
                self->velocityX = FIX(1);
            } else {
                self->velocityX = FIX(-1);
            }
            self->velocityY = FIX(-2);
            self->drawFlags = ENTITY_ROTATE;
            self->step_s++;
            /* fallthrough */
        case 1:
            func_us_801AE4B4(&self->rotate, 0x80, 0x10);
            if (UnkCollisionFunc3(D_us_80181820) & 1) {
                self->step_s++;
            }
            break;
        case 2:
            if (func_us_801AE4B4(&self->rotate, 0, 0x10)) {
                TINMAN(self)->armBroken = true;
                self->drawFlags = ENTITY_DEFAULT;
                self->rotate = 0;
                self->step_s++;
            }
            break;
        case 3:
            if (!AnimateEntity(D_us_80181844, self)) {
                self->hitboxWidth = 15;
                self->hitboxHeight = 17;
                self->hitboxOffX = 2;
                self->hitboxOffY = 15;
                TINMAN(self)->timer = Random() & 0xF;
                SetStep(6);
            }
            break;
        }
        break;

    case 6:
        AnimateEntity(D_us_80181880, self);
        if (!(TINMAN(self)->timer & 7)) {
            PlaySfxPositional(SFX_EXPLODE_E);
        }
        TINMAN(self)->timer++;
        if (!(TINMAN(self)->timer & 3)) {
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_21, self, entity);
                entity->facingLeft = self->facingLeft;
                entity->posY.i.hi =
                    entity->posY.i.hi - 7 - ((TINMAN(self)->timer & 7) >> 1);
                if (self->facingLeft) {
                    entity->posX.i.hi -= 2;
                } else {
                    entity->posX.i.hi += 2;
                }
            }
        }
        break;

    case 7:
        UnkCollisionFunc3(D_us_80181820);
        func_us_801AE4B4(&self->rotate, 0, 0x20);
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            if (TINMAN(self)->armBroken) {
                self->step_s = 2;
            } else {
                self->step_s = 1;
            }
            break;
        case 1:
            if (!AnimateEntity(D_us_80181844, self)) {
                self->step_s++;
            }
            break;
        case 2:
            for (i = 0; i < 11; i++) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_23, self, entity);
                    entity->params = i + 0x3B;
                    entity->facingLeft = self->facingLeft;
                }
            }
            PlaySfxPositional(SFX_FLEA_ARMOR_EXPLODE);
            DestroyEntity(self);
            break;
        }
        break;

    case 0xFF:
#include "../pad2_anim_debug.h"
    }
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("st/rlib/nonmatchings/unk_2E4B4", func_us_801AED4C);
#else
// Tin Man arm, follows the body in the previous entity slot
void func_us_801AED4C(Entity* self) {
    Entity* body;
    Entity* entity;
    s16 angle;
    s32 i;

    body = self - 1;
    self->posX.val = body->posX.val;
    self->facingLeft = body->facingLeft;
    self->posY.val = body->posY.val;
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180670);
        self->drawFlags = ENTITY_ROTATE;
        /* fallthrough */
    case 1:
        if (TINMAN(body)->hideArm == true) {
            self->hitboxState = 0;
            self->animCurFrame = 0;
        } else {
            self->animCurFrame = 5;
            self->rotate = TINMAN(body)->armAngle;
            self->hitboxState = 1;
            angle = self->rotate + 0x200;
            self->hitboxOffX = (rcos(angle) * 7) >> 10;
            self->hitboxOffY = (rsin(angle) * 7) >> 10;
            if (self->rotate >= 0xF00) {
                PlaySfxPositional(SFX_WHIP_TWIRL_SWISH);
            }
        }
        if (TINMAN(body)->armBroken || (body->flags & FLAG_DEAD)) {
            self->hitboxState = 0;
            self->step++;
        }
        break;

    case 2:
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_UNK_23, self, entity);
            entity->facingLeft = self->facingLeft;
            entity->rotate = self->rotate;
            entity->params = 5;
            entity->zPriority = self->zPriority;
        }
        for (i = 0; i < 8; i++) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_23, self, entity);
                entity->params = 0;
                entity->zPriority = self->zPriority;
            }
        }
        PlaySfxPositional(SFX_WALL_DEBRIS_C);
        self->hitboxWidth = 15;
        self->hitboxHeight = 11;
        self->hitboxOffX = 23;
        self->hitboxOffY = -5;
        self->parent = self - 1;
        self->animCurFrame = 0;
        self->hitboxState = body->hitboxState;
        self->attackElement = body->attackElement;
        self->attack = body->attack;
        self->step++;
        break;

    case 3:
        if (body->entityId != E_UNK_1F || (body->flags & FLAG_DEAD)) {
            DestroyEntity(self);
        }
        break;
    }
}
#endif

extern EInit D_us_80180688;
extern AnimationFrame D_us_80181888;

void func_us_801AEFE0(Entity* self) {
    Entity* parent;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180688);
        self->blendMode = BLEND_TRANSP;
        PlaySfxPositional(SFX_NOISE_SWEEP_DOWN_B);
        /* fall through */
    case 1:
        parent = self->ext.et_801AEFE0.parent;
        self->posX.val = parent->posX.val;
        self->posY.val = parent->posY.val;
        if (AnimateEntity(&D_us_80181888, self) == 0) {
            DestroyEntity(self);
        }
        break;
    }
}

extern EInit D_us_8018067C;
extern AnimationFrame D_us_8018189C;

void func_us_801D8D44_from_no4(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_8018067C);
        if (self->facingLeft) {
            self->velocityX = FIX(6);
        } else {
            self->velocityX = FIX(-6);
        }
        /* fall through */
    case 1:
        MoveEntity();
        AnimateEntity(&D_us_8018189C, self);
        if (self->flags & FLAG_DEAD) {
            DestroyEntity(self);
        }
        break;
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("st/rlib/nonmatchings/unk_2E4B4", func_us_801AF11C);
#else
// Tin Man pieces: params 0 is a small falling fragment, params 1-0x3A a
// spinning piece and 0x3B onwards the body parts it breaks into on death
void func_us_801AF11C(Entity* self) {
    Entity* entity;
    TinmanPiece* piece;
    s32 speed;
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180688);
        if (!self->params) {
            self->step = 1;
            speed = (Random() & 0x1F) + 0x10;
            angle = Random() * 6 + 0x900;
            self->velocityX = speed * rcos(angle);
            self->velocityY = speed * rsin(angle);
            break;
        }
        self->animCurFrame = self->params;
        if (self->params >= 0x3B) {
            piece = &D_us_801818D0[self->params - 0x3B];
            self->velocityX = piece->velocityX;
            if (self->facingLeft) {
                self->velocityX = -piece->velocityX;
            }
            self->velocityY = piece->velocityY;
            TINMAN(self)->timer = piece->timer;
            self->step = 3;
            TINMAN(self)->hasGravity = piece->hasGravity;
            break;
        }
        self->drawFlags = ENTITY_ROTATE;
        self->step = 2;
        break;

    case 1:
        AnimateEntity(D_us_801818A8, self);
        MoveEntity();
        self->velocityY += FIX(0.09375);
        break;

    case 2:
        MoveEntity();
        self->velocityY += FIX(0.09375);
        self->rotate += 0x80;
        break;

    case 3:
        switch (self->step_s) {
        case 0:
            if (!--TINMAN(self)->timer) {
                TINMAN(self)->timer = 0x20;
                self->step_s++;
            }
            break;
        case 1:
            MoveEntity();
            if (TINMAN(self)->hasGravity) {
                self->velocityY += FIX(0.125);
            }
            if (!--TINMAN(self)->timer) {
                PlaySfxPositional(SFX_EXPLODE_B);
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_EXPLOSION, self, entity);
                    entity->params = 1;
                    entity->posY.i.hi += 16;
                }
                DestroyEntity(self);
            }
            break;
        }
        break;
    }
}
#endif
