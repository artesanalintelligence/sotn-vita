// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rdai.h"

#include "../e_spear_guard_collision.h"

#include "../approach_s16.h"

// Private ext fields of the archer parts that have no struct yet
#define EXT_U8(self, offset) (*(u8*)((u8*)&(self)->ext + (offset)))
#define EXT_S16(self, offset) (*(s16*)((u8*)&(self)->ext + (offset)))

extern EInit g_EInitRdaiUnk1F;
extern EInit D_us_80180884;
extern EInit D_us_80180890;
extern AnimateEntityFrame D_us_801818B4[];
extern AnimateEntityFrame D_us_801818BC[];
extern AnimateEntityFrame D_us_801818C8[];
extern AnimateEntityFrame D_us_801818D4[];
extern AnimateEntityFrame D_us_801818DC[];
extern AnimateEntityFrame D_us_801818E4[];
extern AnimateEntityFrame D_us_801818F4[];
extern s16** spriteBanks[];

#ifndef NON_MATCHING
INCLUDE_ASM("st/rdai/nonmatchings/e_archer", func_us_801BF830);
#else
// The cart the archer rides: rolls back and forth and owns the archer parts
void func_us_801BF830(Entity* self) {
    Entity* entity;
    AnimateEntityFrame* anim;
    s32 i;
    s32 ret;

    if (self->step && (self + 4)->entityId != E_UNK_1C && self->step != 3) {
        SetStep(3);
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitRdaiUnk1F);
        entity = self + 1;
        for (i = 0; i < 3; i++, entity++) {
            CreateEntityFromEntity(E_UNK_1F, self, entity);
            entity->params = i;
        }
        CreateEntityFromEntity(E_UNK_1C, self, self + 4);
        SetStep(1);
        break;

    case 1:
        if (!self->step_s) {
            self->ext.rdaiArcher.timer = 0x80;
            self->step_s++;
        }
        if (!EXT_U8(self, 8)) {
            self->velocityX += FIX(1.0 / 64);
            if (self->velocityX > FIX(0.75)) {
                self->velocityX = FIX(0.75);
            }
        } else {
            self->velocityX -= FIX(1.0 / 64);
            if (self->velocityX < FIX(-0.75)) {
                self->velocityX = FIX(-0.75);
            }
        }
        AnimateEntity(D_us_801818B4, self);
        SpearGuardCheckCollision();
        if (!(g_Timer & 7)) {
            PlaySfxPositional(0x76A);
        }
        if (!--self->ext.rdaiArcher.timer) {
            SetStep(2);
        }
        break;

    case 2:
        switch (self->step_s) {
        case 0:
            // slow down, then turn around
            SpearGuardCheckCollision();
            self->velocityX -= self->velocityX >> 8;
            if (abs(self->velocityX) < FIX(0.5)) {
                self->velocityX = 0;
                EXT_U8(self, 8) ^= 1;
                self->step_s++;
            }
            break;
        case 2:
            if (!EXT_U8(self, 8)) {
                self->velocityX += FIX(1.0 / 64);
                if (self->velocityX > FIX(0.75)) {
                    self->velocityX = FIX(0.75);
                }
            } else {
                self->velocityX -= FIX(1.0 / 64);
                if (self->velocityX < FIX(-0.75)) {
                    self->velocityX = FIX(-0.75);
                }
            }
            // fallthrough
        case 1:
            SpearGuardCheckCollision();
            anim = D_us_801818BC;
            if (!EXT_U8(self, 8)) {
                anim = D_us_801818C8;
            }
            ret = AnimateEntity(anim, self);
            if (self->pose >= 3) {
                self->step_s = 2;
            }
            if (!ret) {
                SetStep(1);
            }
            break;
        }
        break;

    case 3:
        PlaySfxPositional(0x6CA);
        for (i = 0; i < 9; i++) {
            if (i == 7) {
                continue;
            }
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_21, self, entity);
                entity->params = i + 1;
            }
        }
        PreventEntityFromRespawning(self);
        DestroyEntity(self);
        break;

    case 0xFF:
#include "../pad2_anim_debug.h"
        break;
    }
}
#endif

// Child parts share one implementation; only the fixed EInit address is local.
#include "../e_rdai_unk1f.h"

#include "../e_rdai_archer.h"

#ifndef NON_MATCHING
INCLUDE_ASM("st/rdai/nonmatchings/e_archer", func_us_801C0240);

INCLUDE_ASM("st/rdai/nonmatchings/e_archer", func_us_801C0528);

INCLUDE_ASM("st/rdai/nonmatchings/e_archer", func_us_801C0898);
#else
static void ArcherSpawnArrow(Entity* self) {
    Entity* entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
    if (entity != NULL) {
        CreateEntityFromEntity(E_UNK_20, self, entity);
        entity->ext.et_801AEFE0.parent = self;
        entity->zPriority = self->zPriority - 1;
    }
}

// Archer body parts; params 1 is the bow, which spawns the arrows
void func_us_801C0240(Entity* self) {
    Entity* archer;
    u8 mode;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitRdaiUnk1F);
        self->drawFlags = ENTITY_ROTATE;
        self->hitboxState = 0;
        self->step = self->params;
        if (self->params == 1) {
            ArcherSpawnArrow(self);
        }
        break;

    case 1:
        archer = self - 1;
        self->posX.i.hi = archer->posX.i.hi;
        self->posY.i.hi = archer->posY.i.hi;
        self->rotate = archer->rotate;
        self->zPriority = archer->zPriority + 3;
        mode = self->ext.rdaiArcher.mode;
        if (mode == 1) {
            AnimateEntity(D_us_801818DC, self);
            if (self->pose == 1) {
                PlaySfxPositional(0x6C9);
            }
        } else if (mode == 3) {
            AnimateEntity(D_us_801818F4, self);
            if (self->pose == 6) {
                ArcherSpawnArrow(self);
            }
        } else {
            self->animCurFrame = mode == 2 ? 0x1B : 0x19;
            self->pose = 0;
            self->poseTimer = 0;
        }
        self->ext.rdaiArcher.mode = archer->ext.rdaiArcher.mode;
        if (archer->entityId != E_UNK_1C) {
            DestroyEntity(self);
        }
        break;

    case 2:
        archer = self - 2;
        self->animCurFrame = 0x1C;
        self->posX.i.hi = archer->posX.i.hi;
        self->zPriority = archer->zPriority - 1;
        self->rotate = archer->rotate;
        self->posY.i.hi = archer->posY.i.hi;
        if (archer->entityId != E_UNK_1C) {
            DestroyEntity(self);
        }
        break;

    case 3:
        archer = self - 3;
        self->posX.i.hi = archer->posX.i.hi;
        self->posY.i.hi = archer->posY.i.hi;
        self->zPriority = archer->zPriority + 1;
        self->rotate = archer->rotate;
        mode = self->ext.rdaiArcher.mode;
        if (mode == 2) {
            AnimateEntity(D_us_801818E4, self);
        } else if (mode == 1) {
            AnimateEntity(D_us_801818D4, self);
            if (archer->ext.rdaiArcher.mode == 2) {
                self->pose = 0;
                self->poseTimer = 0;
                self->ext.rdaiArcher.mode = 2;
            }
        } else {
            self->animCurFrame = 0xB;
            self->pose = 0;
            self->poseTimer = 0;
        }
        self->ext.rdaiArcher.mode = archer->ext.rdaiArcher.mode;
        if (archer->entityId != E_UNK_1C) {
            DestroyEntity(self);
        }
        break;
    }
}

// Arrow: follows the bow until shot, then flies; sticks to the player on hit
// and breaks in two pieces (params 1 and 2) when destroyed
void func_us_801C0528(Entity* self) {
    Entity* entity;
    Entity* bow;
    s32 i;
    s32 dist;
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180884);
        self->animCurFrame = 0x16;
        self->hitboxState = 0;
        self->drawFlags |= ENTITY_ROTATE;
        if (self->params) {
            self->animCurFrame += self->params;
            self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA |
                           FLAG_DESTROY_IF_BARELY_OUT_OF_CAMERA |
                           FLAG_UNK_00200000 | FLAG_UNK_2000;
            if (self->params == 1) {
                self->velocityX = FIX(0.5);
            } else {
                self->velocityX = FIX(-0.5);
            }
            self->step = 4;
            break;
        }
        // fallthrough
    case 1:
        bow = self->ext.et_801AEFE0.parent;
        self->posX.i.hi = bow->posX.i.hi;
        self->posY.i.hi = bow->posY.i.hi;
        angle = bow->rotate;
        self->rotate = angle;
        dist = 0x18;
        if (bow->animCurFrame == 0x1A) {
            dist = 0xE;
        }
        if (bow->animCurFrame == 0x1B) {
            dist = 8;
        }
        self->posX.i.hi += (dist * rcos(angle)) >> 12;
        self->posY.i.hi += (dist * rsin(angle)) >> 12;
        if (bow->entityId != E_UNK_1D) {
            DestroyEntity(self);
            break;
        }
        if (bow->ext.rdaiArcher.mode == 2) {
            self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
            self->velocityX = (rcos(angle) * FIX(5)) >> 12;
            self->velocityY = (rsin(angle) * FIX(5)) >> 12;
            self->hitboxState = 3;
            self->hitboxOffX = rcos(angle) >> 9;
            self->hitboxOffY = rsin(angle) >> 9;
            self->hitboxWidth = self->hitboxHeight = 4;
            self->step = 2;
        }
        break;

    case 2:
        MoveEntity();
        if (self->flags & FLAG_DEAD) {
            for (i = 0; i < 2; i++) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_20, self, entity);
                    entity->params = i + 1;
                    entity->rotate = self->rotate;
                }
            }
            DestroyEntity(self);
            break;
        }
        if (self->hitFlags & 0x80) {
            EXT_S16(self, 0x18) = PLAYER.posX.i.hi - self->posX.i.hi;
            EXT_S16(self, 0x1A) = PLAYER.posY.i.hi - self->posY.i.hi;
            self->ext.rdaiArcher.timer = 0x20;
            self->hitboxState = 0;
            self->step++;
        }
        break;

    case 3:
        self->posX.i.hi = PLAYER.posX.i.hi - EXT_S16(self, 0x18);
        self->posY.i.hi = PLAYER.posY.i.hi - EXT_S16(self, 0x1A);
        if (!--self->ext.rdaiArcher.timer) {
            DestroyEntity(self);
        }
        break;

    case 4:
        MoveEntity();
        self->velocityY += FIX(0.125);
        if (self->params == 1) {
            self->rotate += 0x20;
        } else {
            self->rotate -= 0x20;
        }
        break;
    }
}

static void ArcherDebrisExplode(Entity* self) {
    Entity* entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
    if (entity != NULL) {
        CreateEntityFromEntity(E_EXPLOSION, self, entity);
        entity->posX.i.hi += EXT_S16(self, 0x18);
        entity->params = 1;
        entity->posY.i.hi += EXT_S16(self, 0x1A);
    }
    DestroyEntity(self);
}

// Debris of the archer and its cart, params is the sprite frame
void func_us_801C0898(Entity* self) {
    Entity* entity;
    s16* sprite;
    s16 x, y;
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180890);
        self->animCurFrame = self->params;
        sprite = spriteBanks[3][self->params];
        x = sprite[2] + sprite[4] / 2;
        y = sprite[3] + sprite[5] / 2;
        EXT_S16(self, 0x18) = x;
        EXT_S16(self, 0x1A) = y;
        self->ext.rdaiArcher.timer = (Random() & 0xF) + 8;
        angle = ratan2(y, x);
        self->velocityX = (rcos(angle) << 17) >> 12;
        self->velocityY = (rsin(angle) << 17) >> 12;
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 1;
            entity->posX.i.hi += x;
            entity->posY.i.hi += y;
        }
        if (self->params >= 0x13) {
            self->step = 2;
        }
        break;

    case 1:
        MoveEntity();
        self->velocityY += FIX(0.125);
        if (!--self->ext.rdaiArcher.timer) {
            PlaySfxPositional(0x657);
            ArcherDebrisExplode(self);
        }
        break;

    case 2:
        switch (self->params) {
        case 19:
            if (!self->step_s) {
                self->velocityY = FIX(-2.5);
                self->ext.rdaiArcher.timer = 0x10;
                self->step_s++;
            }
            MoveEntity();
            self->velocityY += FIX(0.125);
            if (!--self->ext.rdaiArcher.timer) {
                ArcherDebrisExplode(self);
            }
            break;
        case 20:
            if (!self->step_s) {
                self->ext.rdaiArcher.timer = 0x18;
                self->velocityX = 0;
                self->velocityY = 0;
                self->step_s++;
            }
            MoveEntity();
            self->velocityY += FIX(0.03125);
            if (!--self->ext.rdaiArcher.timer) {
                ArcherDebrisExplode(self);
            }
            break;
        case 21:
            // step_s is never advanced, so this keeps resetting the fall
            if (!self->step_s) {
                self->ext.rdaiArcher.timer = 0x40;
                self->velocityY = 0;
            }
            MoveEntity();
            self->velocityY += FIX(0.1875);
            break;
        }
        break;
    }
}
#endif
