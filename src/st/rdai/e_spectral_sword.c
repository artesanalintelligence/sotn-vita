// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rdai.h"

#include "../step_towards.h"

// The RDAI Spectral Sword commands six spears and four shields, which are
// spawned right after it in the entity array.
#define NUM_SPEARS 6
#define NUM_SHIELDS 4

// Shared ext layout of the Spectral Sword and its weapons. The offsets of the
// fields read across entities match the PSX ones; the primitive pointer is
// kept after them so it can grow to 64 bits on PC.
typedef struct {
    /* 0x00 */ u32 unused0;
    /* 0x04 */ u16 timer;
    /* 0x06 */ u16 closeTimer;  // sword: time spent too close to the player
    /* 0x08 */ u8 side;         // sword: side of the room it is guarding
    /* 0x09 */ u8 unused9;
    /* 0x0A */ u8 spearCount;   // sword: spears still alive
    /* 0x0B */ u8 shieldCount;  // sword: shields still alive
    /* 0x0C */ f32 targetX;     // weapons: position set by the sword
    /* 0x10 */ f32 targetY;
    /* 0x14 */ s16 angle;       // weapons: attack direction
    /* 0x16 */ s16 unused16;
    /* 0x18 */ u8 command;      // weapons: 1 attack, 2 keep angle, 4 delayed
    /* 0x19 */ u8 unused19;
    /* 0x1A */ u16 delay;       // sword: next command; weapons: attack delay
    /* 0x1C */ u32 unused1C;
    /* 0x20 */ Primitive* prim; // sword: movement target
} RdaiSpectralSwordExt;

#define SSEXT(entity) ((RdaiSpectralSwordExt*)&(entity)->ext)

extern EInit g_EInitSpectralSwordRDAI;
extern EInit g_EInitSpectralSwordAura;
extern EInit g_EInitSpear;
extern EInit g_EInitShield;
extern u8 D_us_80181914[];
extern AnimateEntityFrame D_us_8018191C[];
extern AnimateEntityFrame D_us_80181924[];
extern AnimateEntityFrame D_us_8018192C[];

#ifndef NON_MATCHING
INCLUDE_ASM("st/rdai/nonmatchings/e_spectral_sword", func_us_801C0C44);
#else
void func_us_801C0C44(Entity* self) {
    Primitive* prim;
    Entity* entity;
    s32 i;
    s32 count;
    s32 dx, dy;
    s32 dist;
    s32 baseX, baseY;
    s32 offset;
    s16 angle;
    s16 perpAngle;
    s16 targetX, targetY;

    if ((self->flags & FLAG_DEAD) && self->step < 8) {
        SetStep(8);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitSpectralSwordRDAI);
        self->animCurFrame = 1;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->hitboxOffX = 0;
        self->hitboxOffY = 4;
        i = g_api.AllocPrimitives(PRIM_TILE, 1);
        if (i == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[i];
        self->primIndex = i;
        SSEXT(self)->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        prim->u0 = prim->v0 = 2;
        PGREY(prim, 0) = 192;
        prim->priority = 192;
        prim->drawMode = DRAW_HIDE | DRAW_UNK02;
        entity = self + 1;
        for (i = 0; i < NUM_SPEARS; i++, entity++) {
            CreateEntityFromCurrentEntity(E_UNK_24, entity);
            entity->params = i | (D_us_80181914[i] << 8);
        }
        for (i = 0; i < NUM_SHIELDS; i++, entity++) {
            CreateEntityFromCurrentEntity(E_UNK_25, entity);
            entity->params = i;
        }
        SetStep(2);
        SSEXT(self)->closeTimer = 0x40;
        SSEXT(self)->delay = 0x40;
        SSEXT(self)->spearCount = NUM_SPEARS;
        SSEXT(self)->shieldCount = NUM_SHIELDS;
        break;

    case 2:
        if (GetDistanceToPlayerX() < 0x60) {
            SetStep(3);
        }
        break;

    case 3:
        SetStep(6);
        SSEXT(self)->side = GetSideToPlayer() & 1;
        break;

    case 6:
        prim = SSEXT(self)->prim;
        if (!self->step_s) {
            if (SSEXT(self)->side) {
                angle = Random() * 2 + 0x100;
            } else {
                angle = Random() * 2 + 0x500;
            }
            dx = (rcos(angle) * 0x60) >> 12;
            dy = -(rsin(angle) * 0x60) >> 12;
            targetX = PLAYER.posX.i.hi + dx;
            targetY = PLAYER.posY.i.hi + dy;
            prim->x0 = targetX;
            prim->y0 = targetY;
            angle = ratan2((s16)(targetY - self->posY.i.hi),
                           (s16)(targetX - self->posX.i.hi));
            self->velocityX = rcos(angle) * 12;
            self->velocityY = rsin(angle) * 12;
            SSEXT(self)->timer = 0x80;
            self->step_s++;
        }
        MoveEntity();
        dx = abs(prim->x0 - self->posX.i.hi);
        dy = abs(prim->y0 - self->posY.i.hi);
        if ((dx < 8 && dy < 8) || !--SSEXT(self)->timer) {
            self->step_s = 0;
        }
        dx = (s16)PLAYER.posX.i.hi - self->posX.i.hi;
        if (SSEXT(self)->side) {
            dx = -dx;
        }
        if (dx > 0x80) {
            self->velocityY = 0;
            self->velocityX = SSEXT(self)->side ? FIX(-0.75) : FIX(0.75);
        }
        if (dx < 0x40) {
            // backs away; switches side after being too close for a while
            self->velocityY = 0;
            self->velocityX = SSEXT(self)->side ? FIX(0.75) : FIX(-0.75);
            if (!--SSEXT(self)->closeTimer) {
                SetStep(5);
            }
        } else {
            SSEXT(self)->closeTimer = 0x40;
        }
        if (SSEXT(self)->spearCount && !--SSEXT(self)->delay) {
            switch (Random() & 3) {
            default:
                // a random spear attacks on its own
                SSEXT(self)->delay = 0x100;
                count = (Random() % SSEXT(self)->spearCount) + 1;
                entity = self + 1;
                for (i = 0; i < NUM_SPEARS;) {
                    if (!(entity->flags & FLAG_DEAD)) {
                        count--;
                    }
                    i++;
                    if (!count) {
                        break;
                    }
                    entity++;
                }
                SSEXT(entity)->command = 1;
                break;
            case 2:
                // all the spears attack at once in the same direction
                SSEXT(self)->delay = 0x180;
                angle = GetAngleBetweenEntities(self, &PLAYER);
                entity = self + 1;
                for (i = 0; i < NUM_SPEARS; i++, entity++) {
                    SSEXT(entity)->angle = angle;
                    SSEXT(entity)->command = 3;
                }
                break;
            case 3:
                // the spears attack one after another
                SSEXT(self)->delay = 0x280;
                offset = 0x40;
                entity = self + 1;
                for (i = 0; i < NUM_SPEARS; i++, entity++) {
                    if (!(entity->flags & FLAG_DEAD)) {
                        SSEXT(entity)->delay = offset;
                        offset += 0x40;
                        SSEXT(entity)->command = 5;
                    }
                }
                break;
            }
        }
        break;

    case 5:
        // jump to the other side of the room
        switch (self->step_s) {
        case 0:
            angle = ratan2(0x40 - self->posY.i.hi, 0x80 - self->posX.i.hi);
            self->velocityX = rcos(angle) * 0x30;
            self->velocityY = rsin(angle) * 0x30;
            SSEXT(self)->side ^= 1;
            self->step_s++;
            break;
        case 1:
            MoveEntity();
            if (SSEXT(self)->side) {
                if (self->posX.i.hi > 0x80) {
                    self->velocityX = FIX(3);
                    self->step_s++;
                }
            } else if (self->posX.i.hi < 0x80) {
                self->velocityX = FIX(-3);
                self->step_s++;
            }
            break;
        case 2:
            MoveEntity();
            self->velocityY += FIX(0.125);
            dx = (s16)PLAYER.posX.i.hi - self->posX.i.hi;
            if (SSEXT(self)->side) {
                dx = -dx;
            }
            if (dx > 0x70) {
                SetStep(6);
            }
            break;
        }
        break;

    case 8:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            entity = self + 1;
            for (i = 0; i < NUM_SPEARS + NUM_SHIELDS; i++, entity++) {
                entity->flags |= FLAG_DEAD;
            }
            self->rotate = 0;
            self->drawFlags |= ENTITY_ROTATE;
            PlaySfxPositional(0x697);
            self->step_s++;
            break;
        case 1:
            MoveEntity();
            self->velocityY += FIX(0.09375);
            self->rotate -= 4;
            if (!(g_Timer & 7)) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_EXPLOSION, self, entity);
                    entity->params = 3;
                }
            }
            if (self->posY.i.hi > 0x160) {
                self->animCurFrame = 0;
                self->step_s++;
            }
            break;
        case 2:
            count = 0;
            entity = self + 1;
            for (i = 0; i < NUM_SPEARS + NUM_SHIELDS; i++, entity++) {
                if (!entity->animCurFrame) {
                    count++;
                }
            }
            if (count == NUM_SPEARS + NUM_SHIELDS) {
                entity = self;
                for (i = 0; i < NUM_SPEARS + NUM_SHIELDS + 1; i++, entity++) {
                    DestroyEntity(entity);
                }
                return;
            }
            break;
        }
        break;
    }

    if (self->flags & FLAG_DEAD) {
        return;
    }

    // Line the spears up perpendicular to the player direction
    dx = (s16)PLAYER.posX.i.hi - self->posX.i.hi;
    dy = (s16)PLAYER.posY.i.hi - self->posY.i.hi;
    angle = ratan2(dy, dx);
    perpAngle = angle + 0x400;
    dist = SquareRoot0(dx * dx + dy * dy);
    baseX = self->posX.val;
    baseY = self->posY.val;
    entity = self + 1;
    offset = -0x30;
    for (i = 0; i < NUM_SPEARS; i++, entity++) {
        SSEXT(entity)->targetX.val = baseX + offset * rcos(perpAngle) * 16;
        SSEXT(entity)->targetY.val = baseY + offset * rsin(perpAngle) * 16;
        offset += 0x10;
        if (!offset) {
            offset = 0x10;
        }
    }

    // The shields stand between the Spectral Sword and the player
    dist /= 2;
    if (dist < 0x20) {
        dist = 0x20;
    }
    if (dist > 0x48) {
        dist = 0x48;
    }
    baseX += dist * rcos(angle) * 16;
    baseY += dist * rsin(angle) * 16;
    offset = (SSEXT(self)->shieldCount >> 1) * -0x18;
    for (i = 0; i < NUM_SHIELDS; i++, entity++) {
        if (!(entity->flags & FLAG_DEAD)) {
            SSEXT(entity)->angle = angle;
            SSEXT(entity)->targetX.val =
                baseX + offset * rcos(perpAngle) * 16;
            SSEXT(entity)->targetY.val =
                baseY + offset * rsin(perpAngle) * 16;
            offset += 0x18;
        }
    }

    if (!(self->flags & 0xF)) {
        if (g_Timer & 1) {
            self->palette = PAL_SPECTRAL_SWORD;
        } else {
            self->palette = PAL_SPECTRAL_SWORD + 1;
        }
    }
    if (!(g_Timer & 0xF)) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_UNK_23, self, entity);
            entity->ext.et_801CEB08.unk80 = self;
            entity->zPriority = self->zPriority + 1;
        }
    }
}
#endif

#include "../e_spectral_sword_aura.h"

#ifndef NON_MATCHING
INCLUDE_ASM("st/rdai/nonmatchings/e_spectral_sword", func_us_801C17E8);

INCLUDE_ASM("st/rdai/nonmatchings/e_spectral_sword", func_us_801C1DE8);
#else
// Spear: hovers next to the Spectral Sword and charges when commanded
void func_us_801C17E8(Entity* self) {
    Entity* entity;
    s32 dx, dy;
    s32 dist;
    s32 hit;
    s32 radius;
    s16 angle;
    u8 index;

    if ((self->flags & FLAG_DEAD) && self->step < 8) {
        SetStep(8);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitSpear);
        self->drawFlags = ENTITY_ROTATE;
        if (self->params & 0x100) {
            self->animCurFrame = 4;
        } else {
            self->animCurFrame = 5;
        }
        SetStep(2);
        break;

    case 2:
        MoveEntity();
        dx = (s16)SSEXT(self)->targetX.i.hi - self->posX.i.hi;
        dy = (s16)SSEXT(self)->targetY.i.hi - self->posY.i.hi;
        dist = SquareRoot0(dx * dx + dy * dy);
        if (dist > 0x38) {
            dist = 0x38;
        }
        angle = ratan2(dy, dx);
        self->velocityX = dist * rcos(angle);
        self->velocityY = dist * rsin(angle);
        StepTowards(
            &self->rotate, (s16)(GetAngleBetweenEntities(self, &PLAYER) - 0x400),
            0x10);
        if (SSEXT(self)->command) {
            if (!(SSEXT(self)->command & 4) || !--SSEXT(self)->delay) {
                SetStep(4);
            }
        }
        break;

    case 4:
        switch (self->step_s) {
        case 0:
            // pull back before the charge, and tell a shield to cover
            if (!(SSEXT(self)->command & 2)) {
                SSEXT(self)->angle = GetAngleBetweenEntities(self, &PLAYER);
            }
            self->velocityX = rcos((s16)(SSEXT(self)->angle - 0x800)) << 5;
            index = self->params;
            self->velocityY = rsin((s16)(SSEXT(self)->angle - 0x800)) << 5;
            entity = self - index + NUM_SPEARS + (index * NUM_SHIELDS) / 6;
            SSEXT(entity)->command = (SSEXT(self)->command & 2) ? 2 : 1;
            self->step_s++;
            break;
        case 1:
            MoveEntity();
            self->velocityX -= self->velocityX / 16;
            self->velocityY -= self->velocityY / 16;
            if (abs(self->velocityX) < FIX(0.125) &&
                abs(self->velocityY) < FIX(0.125)) {
                self->step_s++;
            }
            StepTowards(
                &self->rotate,
                (s16)(ratan2(self->velocityY, self->velocityX) - 0x400), 8);
            break;
        case 2:
            angle = SSEXT(self)->angle;
            if (StepTowards(&self->rotate, (s16)(angle - 0x400), 0x20)) {
                self->velocityX = rcos(angle) << 7;
                self->velocityY = rsin(angle) << 7;
                SSEXT(self)->timer = 0x30;
                PlaySfxPositional(0x60A);
                self->step_s++;
            }
            break;
        case 3:
            MoveEntity();
            hit = GetSideToPlayer();
            if (abs(self->velocityY) < abs(self->velocityX)) {
                hit &= 1;
                if (self->velocityX < 0) {
                    hit ^= 1;
                }
            } else {
                hit &= 2;
                if (self->velocityY < 0) {
                    hit ^= 2;
                }
            }
            if (hit || !--SSEXT(self)->timer) {
                SSEXT(self)->command = 0;
                SetStep(2);
            }
            break;
        }
        break;

    case 8:
        switch (self->step_s) {
        case 0:
            entity = self - ((self->params & 0xFF) + 1);
            SSEXT(entity)->spearCount--;
            self->hitboxState = 0;
            PlaySfxPositional(0x683);
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            self->velocityY += FIX(0.125);
            if (!(g_Timer & 7)) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_EXPLOSION, self, entity);
                    entity->params = 1;
                }
            }
            if (self->posY.i.hi > 0x140) {
                self->animCurFrame = 0;
                self->step_s++;
            }
            break;
        case 2:
            self->animCurFrame = 0;
            break;
        }
        break;
    }

    angle = self->rotate + 0x400;
    radius = 0x16;
    if (self->params & 0x100) {
        radius = 0x18;
    }
    self->hitboxOffX = (radius * rcos(angle)) >> 12;
    self->hitboxOffY = (radius * rsin(angle)) >> 12;
}

// Shield: stands between the Spectral Sword and the player
void func_us_801C1DE8(Entity* self) {
    Entity* entity;
    AnimateEntityFrame* anim;
    s32 dx, dy;
    s32 dist;
    s16 angle;

    if ((self->flags & FLAG_DEAD) && self->step < 8) {
        SetStep(8);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitShield);
        self->animCurFrame = 7;
        self->zPriority += 2;
        SetStep(3);
        break;

    case 2:
        // unused
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        dist = GetDistanceToPlayerX();
        self->animCurFrame = 8;
        if (dist < 0x40) {
            self->animCurFrame = 7;
        }
        if (dist < 0x20) {
            self->animCurFrame = 6;
        }
        break;

    case 3:
        MoveEntity();
        if (self->animCurFrame != 8) {
            AnimateEntity(D_us_8018192C, self);
        }
        dx = (s16)SSEXT(self)->targetX.i.hi - self->posX.i.hi;
        dy = (s16)SSEXT(self)->targetY.i.hi - self->posY.i.hi;
        dist = SquareRoot0(dx * dx + dy * dy);
        if (dist > 0x38) {
            dist = 0x38;
        }
        angle = ratan2(dy, dx);
        self->velocityX = dist * rcos(angle);
        self->velocityY = dist * rsin(angle);
        if (self->animCurFrame != 6) {
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        }
        if (SSEXT(self)->command) {
            SetStep(4);
        }
        break;

    case 4:
        switch (self->step_s) {
        case 0:
            self->velocityX = rcos((s16)(SSEXT(self)->angle - 0x800)) << 5;
            self->velocityY = rsin((s16)(SSEXT(self)->angle - 0x800)) << 5;
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            anim = D_us_8018191C;
            if (SSEXT(self)->command & 2) {
                anim = D_us_80181924;
            }
            if (!AnimateEntity(anim, self)) {
                SSEXT(self)->timer = 0x20;
                self->step_s++;
            }
            break;
        case 2:
            if (!--SSEXT(self)->timer) {
                SSEXT(self)->command = 0;
                SetStep(3);
            }
            break;
        }
        break;

    case 8:
        switch (self->step_s) {
        case 0:
            entity = self - (self->params + NUM_SPEARS + 1);
            SSEXT(entity)->shieldCount--;
            self->hitboxState = 0;
            PlaySfxPositional(0x60F);
            PlaySfxPositional(0x683);
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            self->velocityY += FIX(0.125);
            if (!(g_Timer & 7)) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_EXPLOSION, self, entity);
                    entity->params = 1;
                }
            }
            if (self->posY.i.hi > 0x140) {
                self->animCurFrame = 0;
                self->step_s++;
            }
            break;
        case 2:
            self->animCurFrame = 0;
            break;
        }
        break;
    }
}
#endif
