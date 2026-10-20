// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo8.h"

void func_801CE3FC(s16* offsets) {
    Entity* entity;
    s32 i;

    for (i = 0; i < 4; i++) {
        entity = g_CurrentEntity + offsets[i];
        polarPlacePart(entity);
    }
    offsets += 4;

    while (*offsets) {
        if (*offsets != 0xFF) {
            entity = g_CurrentEntity + *offsets;
            polarPlacePart(entity);
        }
        offsets++;
    }
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo8/nonmatchings/unk_15868", func_us_80195938);
#else
// Updates the triangle that fills the gap between Galamoth's head, back and
// tail.
void func_us_80195938(void) {
    Primitive* prim;
    s16 angle;

    prim = g_CurrentEntity->ext.GH_Props.prim;
    prim->x0 = g_CurrentEntity[1].posX.i.hi;
    prim->y0 = g_CurrentEntity[1].posY.i.hi;
    prim->x1 = g_CurrentEntity[12].posX.i.hi;
    prim->y1 = g_CurrentEntity[12].posY.i.hi;
    prim->x2 = g_CurrentEntity[15].posX.i.hi;
    prim->y2 = g_CurrentEntity[15].posY.i.hi;
    angle = g_CurrentEntity[15].ext.GH_Props.rotate;
    if (g_CurrentEntity->facingLeft) {
        prim->x2 -= (rcos(angle) * 11) >> 12;
    } else {
        prim->x2 += (rcos(angle) * 11) >> 12;
    }
    prim->y2 += (rsin(angle) * 10) >> 12;
    prim->drawMode &= ~DRAW_COLORS;
    prim->clut = g_CurrentEntity->palette & 0xFFF;
    if (g_CurrentEntity->facingLeft) {
        if (prim->x1 < prim->x2) {
            prim->drawMode |= DRAW_COLORS;
        }
    } else if (prim->x2 < prim->x1) {
        prim->drawMode |= DRAW_COLORS;
    }
}
#endif

static void func_801D0B40(void) {
    Entity* ent;
    Entity* parent;

    ent = g_CurrentEntity + 7;
    parent = ent->ext.GH_Props.parent;
    ent->ext.GH_Props.rotate = parent->ext.GH_Props.rotate + 0x100;

    ent = g_CurrentEntity + 11;
    parent = ent->ext.GH_Props.parent;
    ent->ext.GH_Props.rotate = parent->ext.GH_Props.rotate + 0x180;
}

extern s32 abs(s32 value);

#include "func_us_80195ad8.h"

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo8/nonmatchings/unk_15868", func_us_80195D80);

INCLUDE_ASM("boss/rbo8/nonmatchings/unk_15868", func_us_80197B1C);
#else
extern EInit g_EInitGalamothLvl50;
extern EInit g_EInitGalamothLvl0;
extern s32 D_us_80180C0C;
extern s16 D_us_80180C10[];
extern GalamothHitbox D_us_80180C88[];
extern giantBroBodyPartsInit D_us_80180CD0[];
extern s16 D_us_80180D70[];
extern s16 D_us_80180D90[];
extern s16 D_us_80180DB0[];
extern s16 D_us_80180DD0[];
extern s16 D_us_80180DE8[];
extern s16 D_us_80180E00[];
extern s16 D_us_80180E10[];
extern s16 D_us_80180F28[];
extern s16 D_us_801811D8[];
extern unkStr_801CDD80 D_us_80180EBC[];
extern unkStr_801CDD80 D_us_80180ECC[];
extern unkStr_801CDD80 D_us_80180F10[];
extern unkStr_801CDD80 D_us_80180F40[];
extern unkStr_801CDD80 D_us_80181014[];
extern unkStr_801CDD80 D_us_801810E8[];
extern unkStr_801CDD80 D_us_80181120[];
extern unkStr_801CDD80 D_us_801811A8[];
extern unkStr_801CDD80 D_us_80181208[];
extern unkStr_801CDD80 D_us_80181258[];
extern unkStr_801CDD80 D_us_801812B0[];
extern unkStr_801CDD80 D_us_801812C8[];
extern unkStr_801CDD80 D_us_80181318[];
extern unkStr_801CDD80 D_us_80181378[];
extern unkStr_801CDD80 D_us_801813B0[];
extern unkStr_801CDD80 D_us_801814A8[];
extern unkStr_801CDD80 D_us_801814C8[];
extern unkStr_801CDD80 D_us_80181518[];
extern unkStr_801CDD80 D_us_80181588[];
extern unkStr_801CDD80 D_us_801815DC[];
extern unkStr_801CDD80 D_us_801816D4[];
extern unkStr_801CDD80 D_us_801816EC[];
extern unkStr_801CDD80 D_us_80181714[];
extern unkStr_801CDD80 D_us_80181734[];
extern unkStr_801CDD80 D_us_80181744[];
extern u16 PrizeDrops[];

// Galamoth's fight progress flags (bit 0: fight started, bit 1: defeated,
// bit 2: death animation over) live in the same word as PrizeDrops. All the
// flags fit in the low half of that little-endian word.
#define GALAMOTH_FLAGS PrizeDrops[0]

s32 func_us_801955F8(Entity* self, s32 facingLeft);

// Galamoth
void func_us_80195D80(Entity* self) {
    Collider collider;
    Entity* entity;
    Entity* prev;
    Primitive* prim;
    giantBroBodyPartsInit* parts;
    s16* list;
    s16 angle;
    s16 primIndex;

    FntPrint("step %x\n", self->step);
    FntPrint("step_s %x\n", self->step_s);
    if ((self->step & 1) && (self->hitFlags & 3)) {
        func_801CE1E8(0x18);
    }
    if ((self->flags & FLAG_DEAD) && self->step < 0x20) {
        for (list = D_us_80180D70; *list; list++) {
            self[*list].hitboxState = 0;
        }
        self->hitboxState = 0;
        D_us_80180C0C = 1;
        if (self[16].ext.et_8019921C.hitFloor ||
            self[13].ext.et_8019921C.hitFloor) {
            func_801CE1E8(0x20);
        }
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitGalamothLvl50);
        self->animCurFrame = 7;
        self->hitboxOffX = D_us_80180C88[7].offsetX;
        self->hitboxOffY = D_us_80180C88[7].offsetY;
        self->hitboxWidth = D_us_80180C88[7].width;
        self->hitboxHeight = D_us_80180C88[7].height;
        primIndex = g_api.AllocPrimitives(PRIM_GT3, 1);
        if (primIndex == -1) {
            self->step = 0;
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->ext.GH_Props.prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        prim->tpage = 0x12;
        prim->clut = 0x200;
        prim->v0 = 2;
        prim->u1 = 0xD1;
        prim->v1 = prim->v2 = 0x2B;
        prim->r0 = prim->g0 = prim->b0 = 0x10;
        prim->u0 = prim->u2 = 0xF9;
        prim->r1 = prim->g1 = prim->b1 = 0x40;
        prim->r2 = prim->g2 = prim->b2 = 0x40;
        prim->drawMode = DRAW_UNK02;
        prim->priority = self->zPriority;
        // fallthrough
    case 1:
        if (UnkCollisionFunc3(D_us_80180C10) & 1) {
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            self->step++;
        }
        break;

    case 2:
        prev = self;
        for (parts = D_us_80180CD0; parts->eArrayOffset; parts++) {
            entity = self + parts->eArrayOffset;
            CreateEntityFromCurrentEntity(E_UNK_17, entity);
            entity->ext.GH_Props.length = parts->length;
            entity->ext.GH_Props.parent = self + parts->eArrayParentOffset;
            entity->params = parts->params;
            entity->nextPart = prev;
            prev = entity;
            entity->parent = self;
            entity->zPriority = self->zPriority + parts->zOffset;
        }
        self->nextPart = prev;
        self->parent = NULL;
        self->step++;
        // fallthrough
    case 3:
        func_801CE228();
        GALAMOTH(self)->unk84 = 0;
        func_801CE1E8(4);
        break;

    case 4:
        list = D_us_80180DB0;
        GALAMOTH(self)->unk8B = 0;
        switch (self->step_s) {
        case 0:
            func_801CDF1C(list, D_us_80180EBC, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            polarPlacePartsList(D_us_80180D70);
            self->step_s++;
            break;
        case 1:
            if (self->hitFlags || (GALAMOTH_FLAGS & 1)) {
                self->step_s++;
            }
            break;
        case 2:
            GALAMOTH_FLAGS |= 1;
            self->step_s++;
            break;
        case 3:
            func_801CDF1C(list, D_us_80180ECC, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            polarPlacePartsList(D_us_80180D70);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                func_us_80195AD8();
            }
            break;
        }
        break;

    case 5:
    case 7:
        if (self->step == 5) {
            list = D_us_80180DE8;
            GALAMOTH(self)->unk8B = 0;
            if (GALAMOTH(self)->unk84 == 1) {
                list = D_us_80180DD0;
            }
            func_801CDF1C(list, D_us_80181378, 0);
        } else {
            GALAMOTH(self)->unk8B = 0;
            if (!self->step_s) {
                GALAMOTH(self)->unkB0[0] = 1;
                self->step_s++;
            }
            list = D_us_80180E10;
            if (GALAMOTH(self)->unk84 == 1) {
                list = D_us_80180E00;
            }
            func_801CDF1C(list, D_us_801813B0, 0);
        }
        func_801CDE10(list);
        func_801D0A00(list);
        func_801D0B40();
        polarPlacePartsList(D_us_80180D70);
        if (GALAMOTH(self)->unkB0[0] >= 2) {
            // check when the foot that is walking reaches the floor
            collider.unk18 = 0x1D;
            entity = self + list[3];
            func_us_801955A0(entity, &collider);
            if (entity->ext.et_8019921C.hitFloor) {
                g_api.PlaySfx(0x647);
                entity->posY.i.hi += collider.unk18;
                GALAMOTH(self)->unk84 ^= 1;
                func_801CE228();
                if (self->step == 5) {
                    if (func_us_801955F8(entity, self->facingLeft)) {
                        func_801CE1E8(7);
                    } else {
                        func_us_80195AD8();
                    }
                } else {
                    if (func_us_801955F8(entity, self->facingLeft ^ 1)) {
                        func_801CE1E8(5);
                    } else {
                        func_us_80195AD8();
                    }
                }
            }
        }
        break;

    case 18:
        list = D_us_80180DB0;
        GALAMOTH(self)->unk8B = 0;
        if (GALAMOTH(self)->unk84 == 1) {
            list = D_us_80180D90;
        }
        func_801CDF1C(list, D_us_80181014, 0);
        func_801CDE10(list);
        func_801D0A00(list);
        polarPlacePartsList(D_us_80180D70);
        if (GALAMOTH(self)->unkB0[0] == 3 && !GALAMOTH(self)->unkB4[0]) {
            GALAMOTH(self)->unk84 ^= 1;
        }
        if (GALAMOTH(self)->unkB0[0] == 6 && !GALAMOTH(self)->unkB4[0]) {
            g_api.ShakeCamera(1);
            g_api.PlaySfx(0x63D);
        }
        if (!GALAMOTH(self)->unkB0[0] && !GALAMOTH(self)->unkB4[0]) {
            func_us_80195AD8();
        }
        break;

    case 10:
        list = D_us_80180DE8;
        if (GALAMOTH(self)->unk84 == 1) {
            list = D_us_80180DD0;
        }
        switch (self->step_s) {
        case 0:
            self->step_s++;
            break;
        case 1:
            func_801CDF1C(list, D_us_801814A8, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                self->step_s++;
            }
            break;
        case 2:
            if (self->facingLeft) {
                self->velocityX = FIX(3);
            } else {
                self->velocityX = FIX(-3);
            }
            self->velocityY = FIX(-7);
            func_801CE228();
            self->step_s++;
            // fallthrough
        case 3:
            MoveEntity();
            self->velocityY += FIX(0.25);
            func_801CDF1C(list, D_us_801814C8, 0);
            func_801CDE10(list);
            func_801CE3FC(list);
            collider.unk18 = 0x1D;
            entity = self + list[3];
            func_us_801955A0(entity, &collider);
            if (entity->ext.et_8019921C.hitFloor) {
                entity->posY.i.hi += collider.unk18;
                GALAMOTH(self)->unk84 ^= 1;
                func_801CE228();
                self->step_s = 1;
                func_us_801955F8(entity, self->facingLeft);
                func_801CE1E8(7);
            }
            break;
        }
        func_801D0B40();
        polarPlacePartsList(D_us_80180D70);
        break;

    case 26:
        list = D_us_80180DB0;
        GALAMOTH(self)->unk8B = 1;
        if (GALAMOTH(self)->unk84 == 1) {
            list = D_us_80180D90;
        }
        func_801CDF1C(list, D_us_80181588, 0);
        func_801CDE10(list);
        func_801D0A00(list);
        func_801D0B40();
        polarPlacePartsList(D_us_80180D70);
        GALAMOTH(self + list[5])->unk88 = 1;
        if (!GALAMOTH(self)->unkB0[0] && !GALAMOTH(self)->unkB4[0]) {
            GALAMOTH(self)->unk8B = 0;
            GALAMOTH(self + list[5])->unk88 = 0;
            func_us_80195AD8();
        }
        if (GALAMOTH(self)->unkB0[0] == 3 && !GALAMOTH(self)->unkB4[0]) {
            g_api.PlaySfx(0x7C8);
        }
        break;

    case 9:
        switch (self->step_s) {
        case 0:
            list = D_us_80180DB0;
            if (GALAMOTH(self)->unk84 == 1) {
                list = D_us_80180D90;
            }
            func_801CDF1C(list, D_us_80180F10, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            if (!GALAMOTH(self)->unkB0[0] && !GALAMOTH(self)->unkB4[0]) {
                GALAMOTH(self)->unk8A = 4;
                self->step_s++;
            }
            break;
        case 1:
            // aim the head towards the player
            angle = ratan2(self[3].posX.i.hi - PLAYER.posX.i.hi,
                           PLAYER.posY.i.hi - self[3].posY.i.hi);
            if (self->facingLeft) {
                angle = -angle;
            }
            if (angle > 0x700) {
                angle = 0x700;
            }
            if (angle < 0x200) {
                angle = 0x200;
            }
            func_801CDD00(self + 8, angle, 12);
            func_801CDD00(self + 10, angle, 12);
            func_801CDD00(self + 11, angle, 12);
            angle -= 0x400;
            func_801CDD00(self + 2, angle, 12);
            func_801CDD00(self + 3, angle / 2, 12);
            GALAMOTH(self)->timer = 12;
            self->step_s++;
            // fallthrough
        case 2:
            polarPlacePartsWithAngvel(D_us_80180F28);
            if (!--GALAMOTH(self)->timer) {
                self->step_s++;
            }
            break;
        case 3:
            func_801CDF1C(D_us_80180F28, D_us_80180F40, 0);
            func_801CDE10(D_us_80180F28);
            if (!GALAMOTH(self)->unkB0[0] && !GALAMOTH(self)->unkB4[0]) {
                if (--GALAMOTH(self)->unk8A) {
                    self->step_s = 1;
                } else {
                    func_us_80195AD8();
                }
            }
            break;
        }
        polarPlacePartsList(D_us_80180D70);
        break;

    case 11:
        list = D_us_80180DE8;
        GALAMOTH(self)->unk8B = 0;
        if (GALAMOTH(self)->unk84 == 1) {
            list = D_us_80180DD0;
        }
        func_801CDF1C(list, D_us_80181518, 0);
        func_801CDE10(list);
        func_801D0A00(list);
        func_801D0B40();
        polarPlacePartsList(D_us_80180D70);
        if (!GALAMOTH(self)->unkB4[0]) {
            self->facingLeft ^= 1;
            func_us_80195AD8();
        }
        break;

    case 12:
        list = D_us_80180DB0;
        if (GALAMOTH(self)->unk84 == 1) {
            list = D_us_80180D90;
        }
        switch (self->step_s) {
        case 0:
            GALAMOTH(self)->unk8B = 0;
            func_801CDF1C(list, D_us_801810E8, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            GALAMOTH(self)->timer = 0x20;
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                self->step_s++;
            }
            // fallthrough
        case 1:
            if (g_Timer % 5 == 0) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_18, entity);
                    entity->ext.et_801980E4.source = self + 7;
                }
            }
            if (!--GALAMOTH(self)->timer) {
                GALAMOTH(self)->timer = 0x100;
                GALAMOTH(self)->unk8B = 1;
                self->step_s++;
            }
            break;
        case 2:
            if (!(GALAMOTH(self)->timer & 0x1F)) {
                g_api.PlaySfx(0x655);
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_19, self + 7, entity);
                }
            }
            if (!--GALAMOTH(self)->timer) {
                GALAMOTH(self)->unk8B = 0;
                self->step_s++;
            }
            break;
        case 3:
            func_801CDF1C(list, D_us_80181120, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                func_us_80195AD8();
            }
            break;
        }
        polarPlacePartsList(D_us_80180D70);
        break;

    case 14:
        list = D_us_80180DB0;
        if (GALAMOTH(self)->unk84 == 1) {
            list = D_us_80180D90;
        }
        switch (self->step_s) {
        case 0:
            GALAMOTH(self)->unk8B = 0;
            func_801CDF1C(list, D_us_801811A8, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            GALAMOTH(self)->timer = 0x20;
            if (!GALAMOTH(self)->unkB4[0]) {
                if (!GALAMOTH(self)->unkB0[0]) {
                    self->step_s++;
                }
                if (!GALAMOTH(self)->unkB4[0] &&
                    GALAMOTH(self)->unkB0[0] == 3) {
                    g_api.PlaySfx(0x6C6);
                }
            }
            // fallthrough
        case 1:
            if (g_Timer % 5 == 0) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_18, entity);
                    entity->ext.et_801980E4.source = self + 7;
                }
            }
            if (!--GALAMOTH(self)->timer) {
                g_api.PlaySfx(0x80F);
                GALAMOTH(self)->unk8B = 1;
                GALAMOTH(self)->timer = 0x100;
                self->step_s++;
            }
            break;
        case 2:
            func_801CDF1C(D_us_801811D8, D_us_80181208, 0);
            polarPlacePartsWithAngvel(D_us_801811D8);
            if (!(GALAMOTH(self)->timer & 0xF)) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_1A, self + 7, entity);
                    entity->facingLeft = self->facingLeft;
                }
            }
            if (!(GALAMOTH(self)->timer & 0x1F)) {
                g_api.PlaySfx(0x80F);
            }
            if (!--GALAMOTH(self)->timer) {
                func_801CE228();
                GALAMOTH(self)->timer = 0x80;
                self->step_s++;
            }
            break;
        case 3:
            func_801CDF1C(D_us_801811D8, D_us_80181208, 0);
            polarPlacePartsWithAngvel(D_us_801811D8);
            if (!(GALAMOTH(self)->timer & 0x1F)) {
                g_api.PlaySfx(0x80F);
            }
            if (!--GALAMOTH(self)->timer) {
                func_801CE228();
                GALAMOTH(self)->unk8B = 0;
                self->step_s++;
            }
            break;
        case 4:
            func_801CDF1C(list, D_us_80181258, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                func_us_80195AD8();
            }
            break;
        }
        polarPlacePartsList(D_us_80180D70);
        break;

    case 24:
        list = D_us_80180DB0;
        if (GALAMOTH(self)->unk84 == 1) {
            list = D_us_80180D90;
        }
        func_801CDF1C(list, D_us_801815DC, 0);
        func_801CDE10(list);
        func_801D0A00(list);
        polarPlacePartsList(D_us_80180D70);
        GALAMOTH(self)->unk8B = 1;
        if (!GALAMOTH(self)->unkB0[0] && !GALAMOTH(self)->unkB4[0]) {
            GALAMOTH(self)->unk8B = 0;
            func_us_80195AD8();
        }
        break;

    case 22:
        list = D_us_80180DB0;
        if (GALAMOTH(self)->unk84 == 1) {
            list = D_us_80180D90;
        }
        switch (self->step_s) {
        case 0:
            GALAMOTH(self)->unk8B = 0;
            func_801CDF1C(list, D_us_801812B0, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            if (g_Timer % 5 == 0) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_18, entity);
                    entity->ext.et_801980E4.source = self + 7;
                }
            }
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                self->step_s++;
            }
            break;
        case 1:
            // the lightning bolts thrown between the two ends of the body
            entity = self + 20;
            DestroyEntity(entity);
            CreateEntityFromCurrentEntity(E_UNK_1C, entity);
            GALAMOTH(entity)->parent = self + 7;
            entity->params = 0;
            entity->facingLeft = self->facingLeft;
            entity = self + 21;
            DestroyEntity(entity);
            CreateEntityFromCurrentEntity(E_UNK_1C, entity);
            GALAMOTH(entity)->parent = self + 11;
            entity->params = 1;
            entity->facingLeft = self->facingLeft;
            g_api.PlaySfx(0x80F);
            self->step_s++;
            // fallthrough
        case 2:
            if (g_Timer % 5 == 0) {
                g_api.PlaySfx(0x80F);
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_18, entity);
                    entity->ext.et_801980E4.source = self + 7;
                }
            }
            func_801CDF1C(list, D_us_801812C8, 0);
            func_801CDE10(list);
            if (++GALAMOTH(self)->timer & 1) {
                self[1].ext.GH_Props.rotate += 0x18;
            } else {
                self[1].ext.GH_Props.rotate -= 0x18;
            }
            func_801D0A00(list);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                self[1].ext.GH_Props.rotate = 0;
                GALAMOTH(self)->unk8B = 1;
                self->step_s++;
            }
            break;
        case 3:
            func_801CDF1C(list, D_us_80181318, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            if (GALAMOTH(self)->unkB4[0] && GALAMOTH(self)->unkB0[0] == 1) {
                GALAMOTH(self + 21)->unk88 = 1;
                GALAMOTH(self + 20)->unk88 = 1;
            }
            if (!(g_Timer & 0x1F)) {
                g_api.PlaySfx(0x665);
            }
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                g_api.PlaySfx(0x653);
                self->step_s++;
            }
            break;
        case 4:
            GALAMOTH(self)->unk8B = 0;
            func_us_80195AD8();
            break;
        }
        polarPlacePartsList(D_us_80180D70);
        break;

    case 32:
        // defeated
        list = D_us_80180DB0;
        GALAMOTH(self)->unk8B = 1;
        if (GALAMOTH(self)->unk84 == 1) {
            list = D_us_80180D90;
        }
        if (++GALAMOTH(self)->timer & 1) {
            self[1].ext.GH_Props.rotate += 0x18;
        } else {
            self[1].ext.GH_Props.rotate -= 0x18;
        }
        if (!(g_Timer & 0x1F)) {
            g_api.PlaySfx(0x655);
        }
        switch (self->step_s) {
        case 0:
            self[2].step = 0x20;
            GALAMOTH(self + 2)->unk8D = 1;
            self[2].step_s = 0;
            self->step_s = 2;
            GALAMOTH_FLAGS |= 2;
            break;
        case 1:
            func_801CDF1C(list, D_us_801816D4, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            polarPlacePartsList(D_us_80180D70);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                GALAMOTH(self)->unk84 = 1;
                self->step_s++;
            }
            break;
        case 2:
            func_801CDF1C(list, D_us_801816EC, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            polarPlacePartsList(D_us_80180D70);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                g_api.PlaySfx(0x7C5);
                g_api.PlaySfx(0x7C7);
                GALAMOTH(self + 2)->unk8C = 1;
                self[4].step = 0x20;
                GALAMOTH(self + 4)->unk8D = 1;
                self[6].step = 0x20;
                self[7].step = 0x20;
                self->step_s++;
            }
            break;
        case 3:
            func_801CDF1C(list, D_us_80181714, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            polarPlacePartsList(D_us_80180D70);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                g_api.PlaySfx(0x7C5);
                g_api.PlaySfx(0x7C7);
                GALAMOTH(self + 4)->unk8C = 1;
                GALAMOTH(self + 6)->unk8C = 1;
                GALAMOTH(self + 7)->unk8C = 1;
                entity = self + list[2];
                entity->step = 0x20;
                GALAMOTH(entity)->unk8D = 1;
                GALAMOTH(entity)->unk8E = 1;
                self[list[3]].step = 0x20;
                self[list[5]].step = 0x20;
                self->step_s++;
            }
            break;
        case 4:
            func_801CDF1C(list, D_us_80181734, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            polarPlacePartsList(D_us_80180D70);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                g_api.PlaySfx(0x7C5);
                g_api.PlaySfx(0x7C7);
                GALAMOTH(self + list[2])->unk8C = 1;
                GALAMOTH(self + list[3])->unk8C = 1;
                GALAMOTH(self + list[5])->unk8C = 1;
                self[8].step = 0x20;
                self[10].step = 0x20;
                self[11].step = 0x20;
                GALAMOTH(self + 8)->unk8D = 1;
                GALAMOTH(self + 8)->unk8E = 1;
                self->ext.GH_Props.prim->drawMode = DRAW_HIDE;
                self->step_s++;
            }
            break;
        case 5:
            func_801CDF1C(list, D_us_80181744, 0);
            func_801CDE10(list);
            func_801D0A00(list);
            polarPlacePartsList(D_us_80180D70);
            if (!GALAMOTH(self)->unkB4[0] && !GALAMOTH(self)->unkB0[0]) {
                g_api.PlaySfx(0x7C5);
                GALAMOTH(self + 8)->unk8C = 1;
                GALAMOTH(self + 10)->unk8C = 1;
                GALAMOTH(self + 11)->unk8C = 1;
                self->step_s++;
            }
            break;
        case 6:
            for (list = D_us_80180D70; *list; list++) {
                self[*list].drawFlags |= ENTITY_OPACITY;
            }
            self[3].step = 0x20;
            self[1].step = 0x20;
            self[12].step = 0x20;
            self->opacity = 0x80;
            GALAMOTH(self + 1)->unk8D = 1;
            GALAMOTH(self + 1)->unk8E = 1;
            self->drawFlags |= ENTITY_OPACITY;
            self->step_s++;
            // fallthrough
        case 7:
            self->opacity -= 2;
            for (list = D_us_80180D70; *list; list++) {
                self[*list].opacity = self->opacity;
            }
            if (!self->opacity) {
                self->step_s++;
            }
            break;
        case 8:
            self->palette = PAL_FLAG(0x20A);
            self->step_s++;
            // fallthrough
        case 9:
            if (!(GALAMOTH(self)->timer & 7)) {
                self->palette++;
            }
            self->posY.i.hi++;
            for (list = D_us_80180D70; *list; list++) {
                self[*list].palette = self->palette;
                self[*list].posY.val += FIX(0.25);
            }
            if (self->palette > PAL_FLAG(0x216)) {
                self->step_s++;
            }
            break;
        case 10:
            for (list = D_us_80180D70; *list; list++) {
                DestroyEntity(&self[*list]);
            }
            g_api.PlaySfx(0x654);
            GALAMOTH_FLAGS |= 4;
            DestroyEntity(self);
            return;
        case 11:
            return;
        }
        break;
    }

    func_us_80195938();
    for (list = D_us_80180D70; *list; list++) {
        entity = &self[*list];
        entity->ext.GH_Props.unkA8 = 0;
        entity->facingLeft = self->facingLeft;
    }
    D_8006C384.y = GALAMOTH(self)->unkB0[0];
    D_8006C38C.y = GALAMOTH(self)->unkB4[0];
}

// Galamoth body parts
void func_us_80197B1C(Entity* self) {
    Collider collider;
    Entity* entity;
    s32 length;
    s16 angle;
    s8 distance;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitGalamothLvl50);
        if (self->params == 9) {
            InitializeEntity(g_EInitGalamothLvl0);
        }
        if (self->params == 3 || self->params == 0x10) {
            self->hitboxState = 1;
        }
        self->animCurFrame = self->params;
        self->drawFlags |= ENTITY_ROTATE;
        self->hitboxWidth = D_us_80180C88[self->animCurFrame].width;
        self->hitboxHeight = D_us_80180C88[self->animCurFrame].height;
        self->step = 1;
        break;
    case 1:
        angle = GALAMOTH(self)->rotate;
        self->rotate = angle;
        distance = D_us_80180C88[self->animCurFrame].offsetY;
        self->hitboxOffX = (-distance * rsin(angle)) >> 12;
        self->hitboxOffY = (distance * rcos(angle)) >> 12;
        if (GALAMOTH(self)->unk89) {
            GALAMOTH(self)->unk89 = 0;
            self->facingLeft ^= 1;
            self->rotate = -self->rotate;
        }
        break;
    case 0x20:
        switch (self->step_s) {
        case 0:
            if (!(g_Timer & 7)) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(GALAMOTH(self)->unk8D
                                               ? E_MINOTAUR_SPIT_LIQUID
                                               : E_UNK_20,
                                           self, entity);
                    entity->zPriority = self->zPriority + 1;
                    entity->rotate = self->rotate;
                    entity->facingLeft = self->facingLeft;
                    length = -GALAMOTH(self)->length;
                    angle = GALAMOTH(self)->rotate;
                    if (self->facingLeft) {
                        angle = -angle;
                    }
                    entity->posX.val -= length * rsin(angle) * 16;
                    entity->posY.val += length * rcos(angle) * 16;
                    if (GALAMOTH(self)->unk8E) {
                        entity->rotate += 0x800;
                    }
                }
            }
            self->rotate = GALAMOTH(self)->rotate;
            if (GALAMOTH(self)->unk8C) {
                self->step_s++;
            }
            break;
        case 1:
            entity = AllocEntity(&g_Entities[64], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1F, self, entity);
                entity->params = self->animCurFrame;
                entity->facingLeft = self->facingLeft;
                entity->rotate = self->rotate;
            }
            self->animCurFrame = 0;
            if (GALAMOTH(self)->unk8D) {
                self->step_s++;
            } else {
                self->step_s += 2;
            }
            // fallthrough
        case 2:
            if (!(g_Timer & 3)) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(
                        E_MINOTAUR_SPIT_LIQUID, self, entity);
                    entity->zPriority = self->zPriority + 1;
                    entity->rotate = GALAMOTH(self)->rotate;
                    entity->facingLeft = self->facingLeft;
                    length = -GALAMOTH(self)->length;
                    angle = GALAMOTH(self)->rotate;
                    if (self->facingLeft) {
                        angle = -angle;
                    }
                    entity->posX.val -= length * rsin(angle) * 16;
                    entity->posY.val += length * rcos(angle) * 16;
                    if (GALAMOTH(self)->unk8E) {
                        entity->rotate += 0x800;
                    }
                }
            }
            break;
        }
        break;
    }

    switch (self->animCurFrame) {
    case 5:
    case 12:
        // feet
        collider.unk18 = 0x1D;
        func_us_801955A0(self, &collider);
        break;
    case 6:
    case 13:
        // toes follow the floor contact of the foot they are attached to
        switch (GALAMOTH(self)->parent->ext.et_8019921C.hitFloor) {
        case 2:
            GALAMOTH(self)->rotate = 0x200;
            break;
        case 4:
            GALAMOTH(self)->rotate = -0x200;
            break;
        case 1:
            GALAMOTH(self)->rotate = 0;
            break;
        }
        if (GALAMOTH(self)->unk88) {
            angle = self->rotate;
            self->hitboxOffX += -(rsin(angle) * 16) >> 12;
            self->hitboxOffY += (rcos(angle) * 16) >> 12;
            self->attack = g_api.enemyDefs[ENEMY_GALAMOTH_UNK_CA].attack;
            self->attackElement =
                g_api.enemyDefs[ENEMY_GALAMOTH_UNK_CA].attackElement;
        } else {
            self->attack = g_api.enemyDefs[ENEMY_GALAMOTH].attack;
            self->attackElement = g_api.enemyDefs[ENEMY_GALAMOTH].attackElement;
        }
        FntPrint("attack %x\n", self->attackElement);
        break;
    case 9:
    case 17:
        // the head opens its mouth while attacking
        if (GALAMOTH(self - 2)->unk8B) {
            self->animCurFrame = 17;
        } else {
            self->animCurFrame = 9;
        }
        break;
    }
}
#endif

extern EInit g_EInitGalamothLvl50;
extern s32 D_us_80180C0C;

void func_us_801980E4(Entity* self) {
    Entity* source;

    if (D_us_80180C0C != 0) {
        DestroyEntity(self);
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitGalamothLvl50);
        self->hitboxState = 0;
        self->animCurFrame = 0x10;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = 0x100;
        self->drawFlags |= ENTITY_OPACITY;
        self->opacity = 0xC0;
        self->drawFlags |= ENTITY_ROTATE;
        /* fall through */
    case 1:
        if (g_Timer & 1) {
            self->palette = PAL_FLAG(0x206);
        } else {
            self->palette = PAL_FLAG(0x207);
        }
        source = self->ext.et_801980E4.source;
        self->facingLeft = source->facingLeft;
        self->rotate = source->rotate;
        self->posX.i.hi = source->posX.i.hi;
        self->posY.i.hi = source->posY.i.hi;
        self->scaleX = self->scaleY += 0x20;
        self->opacity -= 8;
        if (!self->opacity) {
            DestroyEntity(self);
        }
        break;
    }
}

extern EInit D_us_80180A8C;
extern u8 D_us_80180C20[];

void func_us_80198210(Entity* self) {
    Entity* entities;
    s16 angle;

    if ((self->flags & FLAG_DEAD) || D_us_80180C0C != 0) {
        self->velocityX = 0;
        self->velocityY = 0;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180A8C);
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        entities = g_Entities;
        angle = GetAngleBetweenEntities(self, entities);
        self->velocityX = rcos(angle) * 0x140;
        self->velocityY = rsin(angle) * 0x140;
        /* fall through */

    case 1:
        AnimateEntity(D_us_80180C20, self);
        MoveEntity();
        self->velocityX -= self->velocityX / 0x10;
        self->velocityY -= self->velocityY / 0x10;
        if (abs(self->velocityX) < 0x4000 && abs(self->velocityY) < 0x4000) {
            self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
            self->scaleX = self->scaleY = 0x100;
            self->hitboxState = 0;
            self->step++;
        }
        break;

    case 2:
        AnimateEntity(D_us_80180C20, self);
        self->scaleX = self->scaleY -= 0x10;
        if (!self->scaleX) {
            DestroyEntity(self);
        }
        break;
    }
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo8/nonmatchings/unk_15868", func_us_801983EC);

INCLUDE_ASM("boss/rbo8/nonmatchings/unk_15868", func_us_80198964);

INCLUDE_ASM("boss/rbo8/nonmatchings/unk_15868", func_us_8019C7B8_from_rcen);
#else
extern EInit g_EInitInteractable;

// Lightning that crawls along the floor, similar to EntityShaftLightning
void func_us_801983EC(Entity* self) {
    Primitive* prim;
    Primitive* next;
    Entity* entity;
    s32 posX;
    s32 posY;
    s32 i;
    u8 closeToTarget;
    s16 angle;
    s16 boltAngle;
    s16 primIndex;

    if (D_us_80180C0C) {
        DestroyEntity(self);
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        GALAMOTH_BOLT(self)->offset = -0x80;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x28);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        GALAMOTH_BOLT(self)->prim = prim;
        GALAMOTH_BOLT(self)->current = prim;
        self->flags |= FLAG_HAS_PRIMS;
        i = 0;
        while (prim != NULL) {
            prim->r0 = prim->g0 = prim->b0 = 0x50;
            prim->tpage = 0x1A;
            prim->clut = 0x194;
            prim->u0 = prim->u1 = (i * 0x10) + 0x90;
            prim->u2 = prim->u3 = (i * 0x10) + 0xA0;
            prim->v0 = prim->v2 = 0xD0;
            prim->v1 = prim->v3 = 0xC0;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->drawMode = DRAW_HIDE;
            prim->priority = self->zPriority - 1;
            prim = prim->next;
            if (++i >= 6) {
                i = 0;
            }
        }
        // fallthrough
    case 1:
        prim = GALAMOTH_BOLT(self)->prim;
        prim->x0 = self->posX.i.hi;
        prim->y0 = self->posY.i.hi;
        prim->x1 = prim->x0;
        prim->y1 = prim->y1 - 0x10;
        prim->x2 = self->posX.i.hi;
        prim->y2 = self->posY.i.hi;
        prim->x3 = prim->x2;
        prim->y3 = prim->y2 - 0x10;
        GALAMOTH_BOLT(self)->current = prim;
        GALAMOTH_BOLT(self)->angle = 0xC00;
        GALAMOTH_BOLT(self)->segmentsBeforeAim = 0;
        self->step++;
        // fallthrough
    case 2:
        for (i = 0; i < 6; i++) {
            prim = GALAMOTH_BOLT(self)->current;
            boltAngle = GALAMOTH_BOLT(self)->angle;
            if (self->facingLeft) {
                posX = self->posX.i.hi + GALAMOTH_BOLT(self)->offset;
            } else {
                posX = self->posX.i.hi - GALAMOTH_BOLT(self)->offset;
            }
            posY = 0x1C0 - g_Tilemap.scrollY.i.hi - prim->y2;
            posX -= prim->x2;
            if (posY < 0) {
                // the bolt reached the floor
                prim->y3 = prim->y2 = 0x1C0 - g_Tilemap.scrollY.i.hi;
                self->step = 3;
                return;
            }
            closeToTarget = false;
            if (abs(posX) < 0x40) {
                closeToTarget = abs(posY) < 0x40;
            }
            if (!GALAMOTH_BOLT(self)->segmentsBeforeAim) {
                GALAMOTH_BOLT(self)->segmentsBeforeAim = 4;
                if (closeToTarget) {
                    GALAMOTH_BOLT(self)->segmentsBeforeAim = 2;
                }
                angle = ratan2(-posY, posX) - boltAngle;
                if (angle > 0x800) {
                    angle -= 0x1000;
                }
                if (angle < -0x800) {
                    angle += 0x1000;
                }
                if (!closeToTarget) {
                    angle /= 4;
                } else {
                    angle /= 2;
                }
                GALAMOTH_BOLT(self)->angleStep = angle;
            }
            boltAngle += GALAMOTH_BOLT(self)->angleStep;
            if (!closeToTarget) {
                boltAngle += 0xC0 - ((Random() & 3) << 7);
            }
            boltAngle &= 0xFFF;
            next = prim->next;
            if (next == NULL) {
                self->step = 3;
                return;
            }
            LOW(next->x0) = LOW(prim->x2);
            LOW(next->x1) = LOW(prim->x3);
            GALAMOTH_BOLT(self)->angle = boltAngle;
            GALAMOTH_BOLT(self)->current = next;
            next->x2 = next->x0 + ((rcos(boltAngle) * 12) >> 12);
            next->y2 = next->y0 - ((rsin(boltAngle) * 12) >> 12);
            angle = boltAngle - 0x400;
            next->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;
            next->x3 = next->x2 + ((rcos(angle) * 16) >> 12);
            next->y3 = next->y2 - ((rsin(angle) * 16) >> 12);
            posX = next->x3 - PLAYER.posX.i.hi;
            posY = next->y3 - PLAYER.posY.i.hi;
            if (abs(posX) < 12 && abs(posY) < 12) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_1D, entity);
                    entity->posX.i.hi = next->x3;
                    entity->posY.i.hi = next->y3;
                }
            }
            GALAMOTH_BOLT(self)->segmentsBeforeAim--;
        }
        break;
    case 3:
        prim = GALAMOTH_BOLT(self)->prim;
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        GALAMOTH_BOLT(self)->offset += 8;
        self->step = 1;
        if (GALAMOTH_BOLT(self)->offset > 0x80) {
            DestroyEntity(self);
        }
        break;
    }
}

// The two ends of the lightning thrown between Galamoth's head and tail
void func_us_80198964(Entity* self) {
    Entity* entity;
    u16 i;

    if (GALAMOTH(self)->parent->entityId != E_UNK_17 || D_us_80180C0C) {
        DestroyEntity(self);
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        if (!self->params) {
            for (i = 0; i < 6; i++) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_1B, entity);
                    GALAMOTH_BOLT(entity)->parent = self;
                    entity->params = i;
                    entity->facingLeft = self->facingLeft;
                }
            }
        }
        // fallthrough
    case 1:
        self->posX.i.hi = GALAMOTH(self)->parent->posX.i.hi;
        self->posY.i.hi = GALAMOTH(self)->parent->posY.i.hi;
        if (GALAMOTH(self)->unk88) {
            self->step++;
        }
        break;
    case 2:
        if (self->facingLeft) {
            self->velocityX = FIX(6);
        } else {
            self->velocityX = FIX(-6);
        }
        self->velocityY = 0;
        self->step++;
        // fallthrough
    case 3:
        MoveEntity();
        self->velocityX += self->velocityX / 128;
        if ((u32)(self->posX.i.hi + g_Tilemap.scrollX.i.hi + 0x40) > 0x280) {
            DestroyEntity(self);
        }
        break;
    }
}

// Lightning thrown from one end of the body to the other
void func_us_8019C7B8_from_rcen(Entity* self) {
    Primitive* prim;
    Primitive* next;
    Entity* entity;
    s32 posX;
    s32 posY;
    s32 i;
    u8 closeToTarget;
    s16 angle;
    s16 boltAngle;
    s16 primIndex;

    if (D_us_80180C0C) {
        DestroyEntity(self);
        return;
    }

    entity = GALAMOTH_BOLT(self)->parent;
    self->posX.i.hi = entity->posX.i.hi;
    self->posY.i.hi = entity->posY.i.hi;
    GALAMOTH_BOLT(self)->unk88 = GALAMOTH(entity)->unk88;
    if (entity->entityId != E_UNK_1C) {
        DestroyEntity(self);
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x10);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        GALAMOTH_BOLT(self)->prim = prim;
        GALAMOTH_BOLT(self)->current = prim;
        self->flags |= FLAG_HAS_PRIMS;
        i = 0;
        while (prim != NULL) {
            prim->r0 = prim->g0 = prim->b0 = 0x50;
            prim->tpage = 0x1A;
            prim->clut = 0x194;
            prim->u0 = prim->u1 = (i * 0x10) + 0x90;
            prim->u2 = prim->u3 = (i * 0x10) + 0xA0;
            prim->v0 = prim->v2 = 0xD0;
            prim->v1 = prim->v3 = 0xC0;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->drawMode = DRAW_HIDE;
            prim->priority = self->zPriority - 1;
            prim = prim->next;
            if (++i >= 6) {
                i = 0;
            }
        }
        // fallthrough
    case 1:
        prim = GALAMOTH_BOLT(self)->prim;
        prim->x0 = self->posX.i.hi;
        prim->y0 = self->posY.i.hi;
        prim->x1 = prim->x0;
        prim->y1 = prim->y1 - 0x10;
        prim->x2 = self->posX.i.hi;
        prim->y2 = self->posY.i.hi;
        prim->x3 = prim->x2;
        prim->y3 = prim->y2 - 0x10;
        GALAMOTH_BOLT(self)->current = prim;
        GALAMOTH_BOLT(self)->angle = Random() * 0x10;
        if (GALAMOTH_BOLT(self)->unk88) {
            // the bolts fan out while the two ends fly apart
            if (self->facingLeft) {
                GALAMOTH_BOLT(self)->angle = 0x100;
                GALAMOTH_BOLT(self)->angle = (self->params << 7) + 0x100;
                GALAMOTH_BOLT(self)->angle += Random() * 2;
            } else {
                GALAMOTH_BOLT(self)->angle = 0x700;
                GALAMOTH_BOLT(self)->angle = 0x700 - (self->params << 7);
                GALAMOTH_BOLT(self)->angle -= Random() * 2;
            }
        }
        while (prim != NULL) {
            prim->r0 = prim->g0 = prim->b0 = 0x50;
            if (GALAMOTH_BOLT(self)->unk88) {
                prim->r0 = prim->g0 = prim->b0 = 0x50 - (self->params * 8);
            }
            prim->drawMode = DRAW_HIDE;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim = prim->next;
        }
        GALAMOTH_BOLT(self)->segmentsBeforeAim = 0;
        self->step++;
        // fallthrough
    case 2:
        // the other end is the entity right after the parent
        closeToTarget = false;
        while (true) {
            prim = GALAMOTH_BOLT(self)->current;
            boltAngle = GALAMOTH_BOLT(self)->angle;
            entity = GALAMOTH_BOLT(self)->parent + 1;
            posX = entity->posX.i.hi - prim->x2;
            posY = entity->posY.i.hi - prim->y2;
            if (abs(posX) < 0x10 && abs(posY) < 0x10) {
                break;
            }
            if (!GALAMOTH_BOLT(self)->unk88) {
                closeToTarget = false;
                if (abs(posX) < 0x20) {
                    closeToTarget = abs(posY) < 0x20;
                }
            }
            if (!GALAMOTH_BOLT(self)->segmentsBeforeAim) {
                GALAMOTH_BOLT(self)->segmentsBeforeAim = 4;
                if (closeToTarget) {
                    GALAMOTH_BOLT(self)->segmentsBeforeAim = 2;
                }
                angle = ratan2(-posY, posX) - boltAngle;
                if (angle > 0x800) {
                    angle -= 0x1000;
                }
                if (angle < -0x800) {
                    angle += 0x1000;
                }
                if (GALAMOTH_BOLT(self)->unk88) {
                    GALAMOTH_BOLT(self)->segmentsBeforeAim = 0x10;
                    angle /= 8;
                } else if (!closeToTarget) {
                    angle /= 4;
                } else {
                    angle /= 2;
                }
                GALAMOTH_BOLT(self)->angleStep = angle;
            }
            boltAngle += GALAMOTH_BOLT(self)->angleStep;
            if (!closeToTarget && !GALAMOTH_BOLT(self)->unk88) {
                boltAngle += 0x120 - ((Random() & 3) * 0xC0);
            }
            boltAngle &= 0xFFF;
            next = prim->next;
            if (next == NULL) {
                break;
            }
            LOW(next->x0) = LOW(prim->x2);
            LOW(next->x1) = LOW(prim->x3);
            GALAMOTH_BOLT(self)->angle = boltAngle;
            GALAMOTH_BOLT(self)->current = next;
            next->x2 = next->x0 + ((rcos(boltAngle) * 12) >> 12);
            next->y2 = next->y0 - ((rsin(boltAngle) * 12) >> 12);
            angle = boltAngle - 0x400;
            next->x3 = next->x2 + ((rcos(angle) * 16) >> 12);
            next->y3 = next->y2 - ((rsin(angle) * 16) >> 12);
            posX = next->x3 - PLAYER.posX.i.hi;
            posY = next->y3 - PLAYER.posY.i.hi;
            if (abs(posX) < 12 && abs(posY) < 12) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_1D, entity);
                    entity->posX.i.hi = next->x3;
                    entity->posY.i.hi = next->y3;
                }
            }
            next->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;
            GALAMOTH_BOLT(self)->segmentsBeforeAim--;
        }
        self->step = 1;
        break;
    case 3:
        prim = GALAMOTH_BOLT(self)->prim;
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        self->step = 1;
        break;
    }
}
#endif

extern EInit D_us_80180A98;

void func_us_801991D4(Entity* self) {
    if (!self->step) {
        InitializeEntity(D_us_80180A98);
        return;
    }
    DestroyEntity(self);
}

void func_us_8019921C(Entity* self) {
    s32 args[9];

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitGalamothLvl50);
        self->animCurFrame = self->params;
        self->drawFlags = ENTITY_ROTATE;
        /* fall through */
    case 1:
        MoveEntity();
        self->rotate -= ROT(2.8125);
        self->velocityY += FIX(0.125);
        if (self->facingLeft) {
            self->velocityX = FIX(0.5);
        } else {
            self->velocityX = FIX(-0.5);
        }
        args[6] = 0x20;
        func_us_801955A0(self, args);
        if (self->ext.et_8019921C.hitFloor) {
            self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
            self->zPriority = 0x5F;
        }
        break;
    }
}

extern EInit D_us_80180AA4;
extern AnimationFrame D_us_80180C4C[];

void EntityMinotaurSpitLiquid(Entity* self) {
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180AA4);
        if (self->facingLeft) {
            self->rotate = -self->rotate;
        }
        self->facingLeft = 0;
        self->rotate += 0x100 - Random() * 2;

        angle = self->rotate;
        self->velocityX = rsin(angle) * 0x28;
        self->velocityY = rcos(angle) * -0x28;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->drawFlags =
            ENTITY_OPACITY | ENTITY_ROTATE | ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = 0x20;
        self->scaleY = 0x100;
        self->opacity = 0x80;
        /* fall through */

    case 1:
        MoveEntity();
        self->scaleX += 0xC;
        self->scaleY += 2;
        self->opacity -= 1;
        if (!AnimateEntity(D_us_80180C4C, self)) {
            DestroyEntity(self);
        }
        break;
    }
}

extern EInit D_us_80180AB0;
extern u8 D_us_80180C68[];

void func_us_8019943C(Entity* self) {
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180AB0);
        if (self->facingLeft) {
            self->rotate = -self->rotate;
        }
        self->facingLeft = 0;
        self->rotate += 0x100 - Random() * 2;

        angle = self->rotate;
        self->velocityX = rsin(angle) * 0x20;
        self->velocityY = rcos(angle) * -0x20;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        /* fall through */
    case 1:
        MoveEntity();
        self->posY.i.hi--;
        if (AnimateEntity(D_us_80180C68, self) == 0) {
            DestroyEntity(self);
        }
        break;
    }
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo8/nonmatchings/unk_15868", func_us_8019953C);

INCLUDE_ASM("boss/rbo8/nonmatchings/unk_15868", func_us_801BA164_from_cat);

INCLUDE_ASM("boss/rbo8/nonmatchings/unk_15868", func_us_80199A58);
#else
extern EInit g_EInitParticle;
extern EInit D_us_80180A68;
extern Point32 D_us_80181758[];
extern u16 D_us_801817A8[];
extern u16 D_us_801817B0[];
extern s32 D_us_801817B8;

// Boss fight manager: spawns Galamoth, handles the music and the life up
void func_us_8019953C(Entity* self) {
    Entity* entity;
    s16 posX;
    s16 posY;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        CreateEntityFromCurrentEntity(E_UNK_16, &g_Entities[80]);
        g_Entities[80].posX.i.hi = 0x190 - g_Tilemap.scrollX.i.hi;
        g_Entities[80].posY.i.hi = 0x1A3 - g_Tilemap.scrollY.i.hi;
        // fallthrough
    case 1:
        if (PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi > 0x100) {
            GALAMOTH_FLAGS |= 1;
        }
        if (GALAMOTH_FLAGS & 1) {
            D_us_801817B8 = 1;
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_GALAMOTH_DEFEAT, TIMEATTACK_SET_VISITED);
            stopMusicFlag = 1;
            currentMusicId = 0x334;
            self->step++;
        }
        break;
    case 2:
    case 6:
        if (!g_api.func_80131F68()) {
            stopMusicFlag = 0;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        break;
    case 3:
        if (GALAMOTH_FLAGS & 2) {
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_GALAMOTH_DEFEAT, TIMEATTACK_SET_RECORD);
            g_api.PlaySfx(0x90);
            currentMusicId = 0x303;
            self->step++;
        }
        break;
    case 4:
        if (GALAMOTH_FLAGS & 4) {
            self->step++;
        }
        break;
    case 5:
        posX = 0x100 - g_Tilemap.scrollX.i.hi;
        posY = 0x180 - g_Tilemap.scrollY.i.hi;
        entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_LIFE_UP_SPAWN, self, entity);
            entity->params = 5;
            entity->posX.i.hi = posX;
            entity->posY.i.hi = posY;
            stopMusicFlag = 1;
            D_us_801817B8 = 0;
            currentMusicId = 0x303;
            self->step++;
        }
        break;
    }
}

// Debris falling from the ceiling, based on CAT's func_us_801BA164
void func_us_801BA164_from_cat(Entity* self) {
    Primitive* prim;
    s32 i;
    s32 temp;
    s16 posX;
    s16 posY;
    s16 primIndex;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        self->ext.et_801BA164.unk80 = 0x40;
        primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, 8);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->ext.et_801BA164.prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        for (i = 0; prim->next != NULL; i++, prim = prim->next) {
            prim->u0 = 2;
            prim->v0 = 2;
            prim->x0 = self->posX.i.hi;
            prim->r0 = 0x60;
            prim->g0 = 0x40;
            prim->b0 = 0x60;
            prim->y0 = self->posY.i.hi;
            LOW(prim->r2) = D_us_80181758[i].x;
            LOW(prim->x2) = D_us_80181758[i].y;
            if (self->params & 0x10) {
                temp = LOW(prim->r2);
                LOW(prim->r2) = LOW(prim->x2);
                LOW(prim->x2) = -temp;
            }
            if (self->params & 1) {
                LOW(prim->r2) = -LOW(prim->r2);
            }
            LOW(prim->r1) = 0;
            LOW(prim->x1) = 0;
            prim->drawMode = DRAW_DEFAULT;
            prim->priority = self->zPriority;
        }
        prim->u0 = prim->v0 = 0;
        prim->x0 = prim->y0 = 0;
        prim->drawMode = DRAW_UNK02;
        prim->priority = self->zPriority;
        // fallthrough
    case 1:
        posX = self->posX.i.hi;
        posY = self->posY.i.hi;
        prim = self->ext.et_801BA164.prim;
        while (prim->next != NULL) {
            LOW(prim->r1) =
                (LOH(prim->b1) << 0x10) + LOH(prim->r1) + LOW(prim->r2);
            temp = (prim->y1 << 0x10) + prim->x1 + LOW(prim->x2);
            LOW(prim->x2) += 0x1000;
            LOW(prim->x1) = temp;
            prim->x0 = posX + LOH(prim->b1);
            prim->y0 = posY + prim->y1;
            prim = prim->next;
        }
        prim->u0 = prim->v0 = 0;
        prim->x0 = prim->y0 = 0;
        prim->drawMode = DRAW_UNK02;
        prim->priority = self->zPriority;
        if (!--self->ext.et_801BA164.unk80) {
            DestroyEntity(self);
        }
        break;
    }
}

// Ceiling blocks, based on CAT's func_us_801BA388
void func_us_80199A58(Entity* self) {
    Collider collider;
    Entity* entity;
    s32 i;
    s32 tilePos;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180A68);
        self->animCurFrame = self->params + 0x10;
        if (self->params) {
            self->posX.i.hi = 7 - g_Tilemap.scrollX.i.hi;
            self->posY.i.hi = 0x141 - g_Tilemap.scrollY.i.hi;
        } else {
            self->posX.i.hi = 0x1F7 - g_Tilemap.scrollX.i.hi;
            self->posY.i.hi = 0x41 - g_Tilemap.scrollY.i.hi;
        }
        tilePos = self->params ? 0x320 : 0x13F;
        for (i = 0; i < 4; i++, tilePos -= 0x20) {
            g_Tilemap.fg[tilePos] = D_us_801817B0[i];
        }
        self->zPriority = 0x5F;
        // fallthrough
    case 1:
        if (D_us_801817B8) {
            self->step++;
        }
        break;
    case 2:
        self->posY.val += self->velocityY;
        self->velocityY += FIX(0.5);
        g_api.CheckCollision(
            self->posX.i.hi, (s16)(self->posY.i.hi + 0x20), &collider, 0);
        if (collider.effects) {
            self->posY.i.hi += collider.unk18;
            self->velocityY = -self->velocityY;
            self->velocityY /= 4;
            if (!(D_us_801817B8 & 0x10)) {
                D_us_801817B8 |= 0x10;
            }
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_INTENSE_EXPLOSION, self, entity);
                entity->params = 0x10;
                entity->posX.i.hi += (Random() & 7) * 2 - 8;
                entity->posY.i.hi += 0x20;
            }
            if (self->velocityY >= -0x1FFF) {
                self->step++;
            }
        }
        break;
    case 3:
        tilePos = self->params ? 0x320 : 0x13F;
        for (i = 0; i < 4; i++, tilePos -= 0x20) {
            g_Tilemap.fg[tilePos] = D_us_801817A8[i];
        }
        self->step++;
        break;
    case 4:
        if (!D_us_801817B8) {
            self->step++;
        }
        break;
    case 5:
        tilePos = self->params ? 0x320 : 0x13F;
        for (i = 0; i < 4; i++, tilePos -= 0x20) {
            g_Tilemap.fg[tilePos] = D_us_801817B0[i];
        }
        self->step++;
        break;
    case 6:
        self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
        self->posY.i.hi--;
        break;
    }
}
#endif
