// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rdai.h"

#ifndef NON_MATCHING
INCLUDE_ASM("st/rdai/nonmatchings/e_black_panther", func_us_801C4B2C);

INCLUDE_ASM("st/rdai/nonmatchings/e_black_panther", func_us_801C4C4C);

INCLUDE_ASM("st/rdai/nonmatchings/e_black_panther", func_us_801C5AA0);

INCLUDE_ASM("st/rdai/nonmatchings/e_black_panther", EntityBlackPanther);
#else
// The Black Panther dashes along the floor; when the player attacks it while
// facing it, it melts into a shadow (made of horizontal slices), slides
// forward and rises up again.

// Only the head of the primitive list is stored; the others are reached by
// walking the list, so the layout stays the same with 64-bit pointers.
typedef struct {
    /* 0x00 */ Primitive* prim;
    /* 0x04 */ u16 timer;
    /* 0x06 */ u8 sliceState;
    /* 0x07 */ u8 shadowState;
    /* 0x08 */ u8 flashState;
    /* 0x09 */ u8 unused9;
    /* 0x0A */ u8 hurtTimer;
    /* 0x0B */ u8 shadowTimer;
    /* 0x0C */ u8 flashTimer;
    /* 0x0D */ u8 blink;
} RdaiBlackPantherExt;

#define BPEXT(entity) ((RdaiBlackPantherExt*)&(entity)->ext)

#define NUM_SLICES 8
#define PRIM_SHADOW 8
#define PRIM_FLASH 9
#define PRIM_ORB 10
#define PRIM_AURA 12

extern EInit g_EInitBlackPanther;
extern s16 D_us_80181AC0[];
extern s16 D_us_80181ACC[];
extern s16 D_us_80181ADC[];
extern AnimateEntityFrame D_us_80181AE4[];
extern AnimateEntityFrame D_us_80181AF8[];

static Primitive* BlackPantherPrim(Entity* self, s32 index) {
    Primitive* prim = BPEXT(self)->prim;
    while (index--) {
        prim = prim->next;
    }
    return prim;
}

static void BlackPantherSetSlice(Entity* self, Primitive* prim) {
    prim->tpage = 0x12;
    prim->clut = 0x164;
    PGREY(prim, 0) = 0x80;
    PGREY(prim, 1) = 0x80;
    PGREY(prim, 2) = 0x80;
    PGREY(prim, 3) = 0x80;
    if (self->facingLeft) {
        prim->x0 = self->posX.i.hi - 0x30;
    } else {
        prim->x0 = self->posX.i.hi - 0x10;
    }
    prim->x1 = prim->x0 + 0x40;
    prim->x2 = prim->x0;
    prim->x3 = prim->x1;
}

// Pulls the lower corners of each slice halfway towards the next one,
// so the slices bend like a melting body
static void func_us_801C4B2C(void) {
    Primitive* prim;
    Primitive* next;
    s32 midX, midY;
    s32 i;

    prim = BPEXT(g_CurrentEntity)->prim;
    for (i = 0; i < NUM_SLICES - 1; i++) {
        next = prim->next;
        midX = (next->x2 + prim->x0) / 2;
        midY = (next->y2 + prim->y0) / 2;
        prim->x2 = next->x0 = (prim->x2 - midX) / 8 + midX;
        prim->y2 = next->y0 = (prim->y2 - midY) / 8 + midY;
        midX = (next->x3 + prim->x1) / 2;
        midY = (next->y3 + prim->y1) / 2;
        prim->x3 = next->x1 = (prim->x3 - midX) / 8 + midX;
        prim->y3 = next->y1 = (prim->y3 - midY) / 8 + midY;
        prim = next;
    }
}

// Melts into the shadow
static void func_us_801C4C4C(void) {
    Entity* self = g_CurrentEntity;
    RdaiBlackPantherExt* ext = BPEXT(self);
    Primitive* prim;
    Primitive* last;
    s32 primIndex;
    s32 i;
    s16 y;
    u8 v0, v2;

    switch (self->step_s) {
    case 0:
        self->hitboxState = 0;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 20);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        ext->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        v0 = 0xB5;
        v2 = 0xB8;
        for (y = -7; y < 9; y += 2) {
            BlackPantherSetSlice(self, prim);
            if (self->facingLeft) {
                prim->u0 = prim->u2 = 0x40;
                prim->u1 = prim->u3 = 0;
            } else {
                prim->u0 = prim->u2 = 0;
                prim->u1 = prim->u3 = 0x40;
            }
            prim->v0 = prim->v1 = v0;
            prim->v2 = prim->v3 = v2;
            prim->y0 = prim->y1 = self->posY.i.hi + y - 3;
            prim->y2 = prim->y3 = prim->y0 + 4;
            prim->drawMode = DRAW_UNK02;
            prim->priority = self->zPriority + 2;
            prim = prim->next;
            v0 += 2;
            v2 += 2;
        }

        // shadow
        BlackPantherSetSlice(self, prim);
        prim->u0 = prim->u2 = 0;
        prim->u1 = prim->u3 = 0x40;
        prim->v0 = prim->v1 = 0xEE;
        prim->v2 = prim->v3 = 0xEF;
        prim->y0 = prim->y1 = prim->y2 = prim->y3 = self->posY.i.hi + 8;
        prim->drawMode = DRAW_HIDE | DRAW_UNK02;
        prim->priority = self->zPriority + 3;
        prim = prim->next;

        // flash
        BlackPantherSetSlice(self, prim);
        prim->u0 = prim->u2 = 0x40;
        prim->u1 = prim->u3 = 0x7F;
        prim->v0 = prim->v1 = 0xC8;
        prim->v2 = prim->v3 = 0xC9;
        prim->y0 = prim->y1 = prim->y2 = prim->y3 = self->posY.i.hi + 8;
        prim->drawMode = DRAW_HIDE | DRAW_UNK02;
        prim->priority = self->zPriority + 2;
        prim = prim->next;

        // orb
        UnkPolyFunc2(prim);
        prim->tpage = 0x12;
        prim->clut = 0x164;
        prim->u0 = prim->u2 = 0x5C;
        prim->u1 = prim->u3 = 0x64;
        prim->v0 = prim->v1 = 0xC0;
        prim->v2 = prim->v3 = 0xC8;
        if (self->facingLeft) {
            prim->next->x1 = self->posX.i.hi - 0x12;
        } else {
            prim->next->x1 = self->posX.i.hi + 0x12;
        }
        prim->next->y0 = self->posY.i.hi - 0x28;
        LOW(prim->next->u0) = 0;
        LOW(prim->next->r1) = 0;
        LOH(prim->next->r2) = 8;
        LOH(prim->next->b2) = 8;
        prim->next->b3 = 0x80;
        prim->drawMode = DRAW_HIDE | DRAW_UNK02;
        prim->priority = self->zPriority + 2;
        prim = prim->next->next;

        // aura that follows the shadow
        UnkPolyFunc2(prim);
        prim->tpage = 0x12;
        prim->clut = 0x164;
        if (self->facingLeft) {
            prim->u0 = prim->u2 = 0x3C;
            prim->u1 = prim->u3 = 0;
        } else {
            prim->u0 = prim->u2 = 0;
            prim->u1 = prim->u3 = 0x3C;
        }
        prim->v0 = prim->v1 = 0xD0;
        prim->v2 = prim->v3 = 0xDD;
        if (self->facingLeft) {
            prim->next->x1 = self->posX.i.hi - 0x12;
        } else {
            prim->next->x1 = self->posX.i.hi + 0x12;
        }
        prim->next->y0 = self->posY.i.hi + 2;
        LOW(prim->next->u0) = 0;
        LOW(prim->next->r1) = 0;
        LOH(prim->next->r2) = 2;
        LOH(prim->next->b2) = 1;
        prim->next->b3 = 0x80;
        prim->drawMode = DRAW_HIDE | DRAW_UNK02;
        prim->priority = self->zPriority + 2;

        PlaySfxPositional(0x672);
        ext->timer = 0x10;
        self->animCurFrame = 0;
        self->step_s++;
        break;

    case 1:
        if (!--ext->timer) {
            ext->sliceState = 0;
            ext->shadowState = 0;
            ext->flashState = 0;
            ext->unused9 = 0;
            ext->hurtTimer = 0;
            ext->shadowTimer = 0x24;
            ext->flashTimer = 0x30;
            ext->blink = 0;
            self->step_s++;
        }
        break;

    case 2:
        ext->timer++;
        switch (ext->sliceState) {
        case 0:
            // squash the slices upwards
            func_us_801C4B2C();
            prim = ext->prim;
            prim->y0 -= 3;
            prim->y1 -= 3;
            if (ext->timer & 1) {
                prim->x0++;
                prim->x1--;
            }
            if (prim->x1 - prim->x0 < 0x20) {
                ext->timer = 0xC;
                ext->sliceState++;
            }
            prim = prim->next;
            for (i = 1; i < 6; i++) {
                prim->x2 += 2;
                prim->x3 -= 2;
                prim = prim->next;
            }
            break;
        case 1:
            // then drop them down
            func_us_801C4B2C();
            prim = ext->prim;
            prim->y0 += 6;
            prim->y1 += 6;
            prim = prim->next;
            for (i = 1; i < 6; i++) {
                prim->x2++;
                prim->x3--;
                prim = prim->next;
            }
            if (ext->timer & 1) {
                last = BlackPantherPrim(self, NUM_SLICES - 1);
                last->x2 += 1;
                last->x3 -= 2;
            }
            prim = ext->prim;
            if (self->posY.i.hi + 8 < prim->y0) {
                for (i = 0; i < NUM_SLICES; i++) {
                    prim->drawMode |= DRAW_HIDE;
                    prim = prim->next;
                }
            }
            break;
        }

        prim = BlackPantherPrim(self, PRIM_SHADOW);
        switch (ext->shadowState) {
        case 0:
            if (!--ext->shadowTimer) {
                prim->drawMode &= ~DRAW_HIDE;
                ext->shadowTimer = 0xF;
                ext->shadowState++;
            }
            break;
        case 1:
            prim->v2 = prim->v3 = prim->v2 + 1;
            prim->y0 = prim->y1 = prim->y0 - 2;
            if (!(ext->timer & 3)) {
                prim->x0--;
                prim->x1++;
            }
            if (!--ext->shadowTimer) {
                ext->shadowState++;
            }
            break;
        case 2:
            if (!(ext->timer % 3)) {
                prim->v2 = prim->v3 = prim->v2 - 1;
            }
            prim->y0 = prim->y1 = prim->y0 + 1;
            if (prim->y2 < prim->y0) {
                prim->drawMode |= DRAW_HIDE;
                ext->shadowState = 8;
            }
            break;
        }

        prim = BlackPantherPrim(self, PRIM_FLASH);
        switch (ext->flashState) {
        case 0:
            if (!--ext->flashTimer) {
                prim->drawMode &= ~DRAW_HIDE;
                ext->flashState++;
            }
            break;
        case 1:
            prim->v2 = prim->v3 = prim->v2 + 4;
            prim->y0 = prim->y1 = prim->y0 - 4;
            if (prim->v2 >= 0xF9) {
                prim = BlackPantherPrim(self, PRIM_ORB);
                prim->drawMode &= ~DRAW_HIDE;
                LOW(prim->next->r1) = 0xFFFF0000;
                prim = BlackPantherPrim(self, PRIM_AURA);
                prim->drawMode &= ~DRAW_HIDE;
                ext->flashState++;
            }
            break;
        case 2:
            prim->y0 = prim->y1 = prim->y0 + 1;
            if (prim->y0 == prim->y3) {
                ext->timer = 1;
                ext->blink = 0;
                prim->drawMode |= DRAW_HIDE;
                self->step_s++;
            }
            prim = BlackPantherPrim(self, PRIM_ORB);
            UnkPrimHelper(prim);
            LOW(prim->next->r1) += 0x1000;
            prim = BlackPantherPrim(self, PRIM_AURA);
            UnkPrimHelper(prim);
            if (LOH(prim->next->b2) < 0xD) {
                LOH(prim->next->b2)++;
            }
            if (LOH(prim->next->r2) < 0x3C) {
                LOH(prim->next->r2) += 2;
            }
            break;
        }
        break;

    case 3:
        prim = BlackPantherPrim(self, PRIM_AURA);
        ext->timer++;
        UnkPrimHelper(prim);
        if (!(ext->timer & 3)) {
            ext->blink ^= 1;
        }
        if (LOH(prim->next->b2) < 0xD) {
            LOH(prim->next->b2)++;
        }
        if (LOH(prim->next->r2) < 0x3C) {
            LOH(prim->next->r2)++;
        }
        prim = BlackPantherPrim(self, PRIM_ORB);
        UnkPrimHelper(prim);
        LOW(prim->next->r1) += 0x1000;
        if (self->posY.i.hi + 6 < prim->next->y0) {
            prim->drawMode |= DRAW_HIDE;
            self->step_s++;
        }
        break;
    }
}

// Rises from the shadow
static void func_us_801C5AA0(void) {
    Entity* self = g_CurrentEntity;
    RdaiBlackPantherExt* ext = BPEXT(self);
    Primitive* prim;
    u16 facingLeft;
    s32 i;
    u8 v0, v2;
    u16 palette;

    switch (self->step_s) {
    case 0:
        facingLeft = self->facingLeft;
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        if (facingLeft != self->facingLeft) {
            if (self->facingLeft) {
                self->posX.i.hi += 0x18;
            } else {
                self->posX.i.hi -= 0x18;
            }
        }
        v0 = 0xB5;
        v2 = 0xB6;
        prim = ext->prim;
        for (i = 0; i < NUM_SLICES; i++) {
            BlackPantherSetSlice(self, prim);
            if (self->facingLeft) {
                prim->u0 = prim->u2 = 0x40;
                prim->u1 = prim->u3 = 0;
            } else {
                prim->u0 = prim->u2 = 0;
                prim->u1 = prim->u3 = 0x40;
            }
            prim->v0 = prim->v1 = v0;
            prim->v2 = prim->v3 = v2;
            prim->y0 = prim->y1 = prim->y2 = prim->y3 = self->posY.i.hi + 8;
            prim->drawMode = DRAW_UNK02;
            prim->priority = self->zPriority + 2;
            prim = prim->next;
            v0 += 2;
            v2 += 2;
        }
        func_us_801C4B2C();
        PlaySfxPositional(0x672);
        ext->shadowTimer = 8;
        self->step_s++;
        break;

    case 1:
        prim = BlackPantherPrim(self, PRIM_AURA);
        prim->y0 -= 2;
        prim->y1 -= 2;
        if (!--ext->shadowTimer) {
            ext->shadowTimer = 0x10;
            self->step_s++;
        }
        break;

    case 2:
        prim = BlackPantherPrim(self, PRIM_AURA);
        prim->y0--;
        prim->y1--;
        prim = ext->prim;
        prim->y0 -= 4;
        prim->y1 -= 4;
        func_us_801C4B2C();
        if (!--ext->shadowTimer) {
            ext->shadowTimer = 0xF;
            self->step_s++;
        }
        break;

    case 3:
        prim = BlackPantherPrim(self, PRIM_AURA);
        prim->y0 += 2;
        prim->y1 += 2;
        if (self->posY.i.hi < prim->y0) {
            prim->drawMode |= DRAW_HIDE;
        }
        prim = ext->prim;
        prim->y0 -= 2;
        prim->y1 -= 2;
        func_us_801C4B2C();
        if (!--ext->shadowTimer) {
            ext->shadowTimer = 0xF;
            self->step_s++;
        }
        break;

    case 4:
        prim = BlackPantherPrim(self, PRIM_AURA);
        prim->y0 += 2;
        prim->y1 += 2;
        if (self->posY.i.hi < prim->y0) {
            prim->drawMode |= DRAW_HIDE;
        }
        prim = ext->prim;
        prim->y0 += 2;
        prim->y1 += 2;
        func_us_801C4B2C();
        if (self->posY.i.hi - 6 < prim->y0) {
            self->step_s++;
        }
        break;

    case 5:
        // back to the sprite, flashing for a while
        palette = self->palette;
        self->animCurFrame = 1;
        ext->hurtTimer = 0x18;
        self->palette = PAL_FLAG(0x164);
        self->hitEffect = palette;
        g_api.FreePrimitives(self->primIndex);
        self->flags &= ~FLAG_HAS_PRIMS;
        self->step_s++;
        break;

    case 6:
        if (!--ext->hurtTimer) {
            self->hitboxState = 3;
            self->palette = self->hitEffect;
            self->step_s++;
        }
        break;
    }
}

void EntityBlackPanther(Entity* self) {
    Collider collider;
    Primitive* prim;
    s16 x;

    if ((self->flags & FLAG_DEAD) && self->step < 0x10) {
        self->hitboxState = 0;
        SetStep(0x10);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitBlackPanther);
        self->hitboxOffX = 2;
        self->hitboxOffY = -4;
        self->animCurFrame = 1;
        // fallthrough
    case 1:
        if (UnkCollisionFunc3(D_us_80181ACC) & 1) {
            self->step++;
        }
        break;

    case 2:
        self->animCurFrame = 1;
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        if (GetDistanceToPlayerX() < 0x70) {
            self->step++;
        }
        break;

    case 3:
        // run
        UnkCollisionFunc2(D_us_80181ADC);
        if (self->facingLeft) {
            self->velocityX = FIX(2.5);
        } else {
            self->velocityX = FIX(-2.5);
        }
        if (!AnimateEntity(D_us_80181AE4, self)) {
            PlaySfxPositional(0x649);
            if (CheckColliderOffsets(D_us_80181AC0, self->facingLeft ^ 1) !=
                1) {
                SetStep(4);
            }
        }
        if (PLAYER.facingLeft != (GetSideToPlayer() & 1) &&
            GetDistanceToPlayerX() < 0x50 && (g_Player.status & 0x1C00)) {
            SetStep(7);
        }
        x = self->facingLeft ? self->posX.i.hi + 0x18 : self->posX.i.hi - 0x18;
        g_api.CheckCollision(x, self->posY.i.hi, &collider, 0);
        if (collider.effects) {
            SetStep(6);
        }
        break;

    case 4:
        // jump over a gap
        switch (self->step_s) {
        case 0:
            self->velocityX = self->facingLeft ? FIX(2) : FIX(-2);
            self->velocityY = FIX(-3.5);
            self->step_s++;
            break;
        case 1:
            AnimateEntity(D_us_80181AF8, self);
            if (UnkCollisionFunc3(D_us_80181ACC) & 1) {
                self->step_s++;
            }
            x = self->facingLeft ? self->posX.i.hi + 0x18
                                 : self->posX.i.hi - 0x18;
            g_api.CheckCollision(x, self->posY.i.hi, &collider, 0);
            if (collider.effects) {
                SetStep(6);
            }
            break;
        case 2:
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            SetStep(3);
            break;
        }
        break;

    case 5:
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 9;
            self->velocityX = 0;
            self->velocityY = FIX(-4);
            self->step_s++;
            break;
        case 1:
            if (UnkCollisionFunc3(D_us_80181ACC) & 1) {
                self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
                SetStep(3);
            }
            break;
        }
        break;

    case 6:
        // bounce back from a wall
        if (!self->step_s) {
            self->animCurFrame = 0xB;
            self->velocityX = self->facingLeft ? FIX(-2.5) : FIX(2.5);
            self->velocityY = FIX(-3);
            self->rotate = 0;
            self->drawFlags |= ENTITY_ROTATE;
            self->step_s++;
        }
        self->rotate += 0x100;
        if (UnkCollisionFunc3(D_us_80181ACC) & 1) {
            self->drawFlags &= ~ENTITY_ROTATE;
            SetStep(3);
        } else {
            self->velocityY -= FIX(1.0 / 16);
        }
        break;

    case 7:
        func_us_801C4C4C();
        if (self->step_s == 4) {
            SetStep(8);
        }
        break;

    case 8:
        // slide forward as a shadow
        if (!self->step_s) {
            self->velocityX = self->facingLeft ? FIX(4) : FIX(-4);
            BPEXT(self)->timer = 0x30;
            self->step_s++;
            break;
        }
        UnkCollisionFunc2(D_us_80181ADC);
        prim = BlackPantherPrim(self, PRIM_AURA);
        if (self->facingLeft) {
            prim->next->x1 = self->posX.i.hi - 0x12;
        } else {
            prim->next->x1 = self->posX.i.hi + 0x12;
        }
        prim->next->y0 = self->posY.i.hi + 2;
        UnkPrimHelper(prim);
        if (!--BPEXT(self)->timer) {
            SetStep(9);
        }
        break;

    case 9:
        func_us_801C5AA0();
        if (self->step_s == 7) {
            SetStep(3);
        }
        break;

    case 0x10:
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 0xB;
            self->rotate = 0;
            self->drawFlags |= ENTITY_ROTATE;
            self->velocityX = self->facingLeft ? FIX(2) : FIX(-2);
            self->velocityY = FIX(-3.5);
            self->step_s++;
            break;
        case 1:
            self->rotate -= 0x80;
            if (UnkCollisionFunc3(D_us_80181ACC) & 1) {
                self->step_s++;
            }
            break;
        case 2:
            prim = (Primitive*)AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (prim != NULL) {
                CreateEntityFromEntity(E_EXPLOSION, self, (Entity*)prim);
                ((Entity*)prim)->params = 2;
            }
            PlaySfxPositional(0x65E);
            DestroyEntity(self);
            return;
        }
        break;

    case 0x20:
#include "../pad2_anim_debug.h"
        break;
    }

    if (self->animCurFrame == 1) {
        self->hitboxOffX = 9;
        self->hitboxOffY = 2;
        self->hitboxWidth = 0x11;
        self->hitboxHeight = 4;
    }
    if (self->animCurFrame == 2) {
        self->hitboxOffX = 5;
        self->hitboxOffY = -5;
        self->hitboxWidth = 0x10;
        self->hitboxHeight = 0xA;
    }
    if (self->animCurFrame >= 3) {
        self->hitboxOffX = 4;
        self->hitboxOffY = -0xC;
        self->hitboxWidth = 0x17;
        self->hitboxHeight = 6;
    }
    if (self->animCurFrame >= 6) {
        self->hitboxOffX = -2;
        self->hitboxOffY = -4;
        self->hitboxWidth = 0xB;
        self->hitboxHeight = 0xB;
    }
}
#endif
