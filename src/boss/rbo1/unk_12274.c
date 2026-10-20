// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo1.h"

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", EntityBreakable);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_801923A8);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_80192C5C);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_80192F84);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_801936FC);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_80193C2C);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_80193E24);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_80194108);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_8019ED80_from_rbo2);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", polarPlacePartsWithAngvel);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_801CDD00);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_801CDD80);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_801CDF1C);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_801CE1E8);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_801CE228);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", polarPlacePartsList);

// decompiled in src/boss/bo1/e_explosion_flame.c
INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_8019D260_from_rcen);

INCLUDE_ASM("boss/rbo1/nonmatchings/unk_12274", func_us_801947E4);
#else
// The functions below are not matching yet, they are only built for the PC
// port. The data they use still lives in the PSX assembly data segments, so
// it is defined here as well.

// Beelzebub (the body, its limbs, the hanging chains and the flies) and its
// effects share this layout. The fields used by the giant brothers helpers
// are kept at the same position as in `ET_GurkhaHammer`.
typedef struct {
    /* 0x7C */ struct Primitive* prim;
    /* 0x80 */ s16 timer;
    /* 0x82 */ s16 angle;
    /* 0x84 */ union {
        s32 prevHitPoints; // body and limbs
        s16 nLinks[7];     // chains: number of links of each chain
    } u84;
    /* 0x94 */ s32 unk94;
    /* 0x98 */ s32 unk98;
    /* 0x9C */ s16 rotate;
    /* 0x9E */ s16 length;
    /* 0xA0 */ struct Entity* parent;
    /* 0xA4 */ s16 unkA4;
    /* 0xA6 */ s16 rotVel;
    /* 0xA8 */ u8 unkA8;
    /* 0xA9 */ u8 unkA9;
    /* 0xAA */ u16 unkAA;
    /* 0xAC */ s16 angleOffset;
    /* 0xAE */ s16 unkAE;
    /* 0xB0 */ u16 unkB0[2];
    /* 0xB4 */ u16 unkB4[2];
} ET_Beelzebub;

// same layout as `ET_ExplosionPuffOpaque` with the extra field used by
// the floating smoke variant
typedef struct {
    /* 0x7C */ s32 : 32;
    /* 0x80 */ u8* anim;
    /* 0x84 */ s16 angle;
    /* 0x86 */ s16 : 16;
    /* 0x88 */ u8 puffStyle;
    /* 0x89 */ u8 speed;
    /* 0x8A */ u16 : 16;
    /* 0x8C */ s32 accel;
    /* 0x90 */ s32 accelY;
} ET_BeelzebubPuff;

STATIC_ASSERT(sizeof(ET_Beelzebub) <= sizeof(Ext), "ext too large");
STATIC_ASSERT(sizeof(ET_BeelzebubPuff) <= sizeof(Ext), "ext too large");

#define BZ(entity) ((ET_Beelzebub*)&(entity)->ext)
#define PUFF(entity) ((ET_BeelzebubPuff*)&(entity)->ext)

typedef struct {
    s16 idx;    // limb entity, relative to the body
    s16 parent; // entity the limb is attached to, relative to the body
    s16 length;
    s16 animCurFrame;
    s16 zOffset;
    s16 angleOffset;
} BeelzebubLimbInit;

typedef struct {
    s16 idx; // entity the chain is attached to, relative to the body
    s16 dx;  // attach point, rotated with the entity
    s16 dy;
    s16 x; // where the chain hangs from, in room coordinates
    s16 y;
} BeelzebubChainInit;

extern EInit g_EInitBreakable;
extern EInit g_EInitParticle;
extern EInit g_EInitInteractable;
extern EInit g_EInitBeezelbub;
extern EInit g_EInitBeezelbub801804A0;
extern EInit D_us_801804AC;
extern EInit D_us_801804B8;
extern EInit D_us_801804C4;
extern EInit D_us_801804D0;
extern s32 g_BossMgrState;
extern s16* sprites_rbo1_2[];

void EntityExplosion(Entity* self);

// EntityBreakable
static u8 anim_1[] = {4, 1, 4, 2, 0, 0, 0, 0};
static u8 anim_2[] = {4, 0, 4, 0, 0, 0, 0, 0};
static u8* g_eBreakableAnimations[8] = {anim_1, anim_2};
static u8 g_eBreakableHitboxes[8] = {8, 8};
static u8 g_eBreakableExplosionTypes[8] = {0, 0};
static u16 g_eBreakableanimSets[8] = {ANIMSET_DRA(3), ANIMSET_DRA(3)};
static u8 blend_modes[16] = {
    BLEND_QUARTER | BLEND_TRANSP,
    BLEND_TRANSP | BLEND_ADD,
};

#include "../../st/e_breakable.h"

// shared hit points of the body and its limbs
static s32 D_us_80180630 = 0;
// number of limbs destroyed so far
static s32 D_us_80180634 = 0;
// chains that already played their breaking sound
static s32 D_us_80180638 = 0;

static BeelzebubLimbInit D_us_8018063C[] = {
    {0x02, 0x00, -0x30, 0x12, -2, 0x200},
    {0x01, 0x00, 0x18, 0x1B, 2, 0x100},
    {0x04, 0x00, 0x1C, 0x00, 2, 0x600},
    {0x05, 0x04, 0x24, 0x17, -1, 0x3E0},
    {0x06, 0x05, 0x2C, 0x18, 1, 0x020},
    {0x07, 0x00, 0x30, 0x00, 2, -0x4C0},
    {0x08, 0x07, 0x38, 0x19, -1, 0x000},
    {0x09, 0x08, 0x28, 0x1A, 1, 0x000},
    {0x0A, 0x01, 0x40, 0x1C, 1, 0x200},
    {0x0B, 0x0A, 0x40, 0x1D, 2, -0x100},
    {0x0C, 0x01, 0x40, 0x1E, 1, 0x000},
    {0x0D, 0x0C, 0x40, 0x1F, 2, 0x000},
    {0},
};

// hit points threshold to destroy each limb
static s16 D_us_801806D8[] = {
    0, 300, 0, 100, 500, 0, 200, 600, 400, 800, 700, 900,
};

static s16 D_us_801806F0[] = {
    2, 4, 5, 6, 7, 8, 9, 1, 10, 11, 12, 13, 0, 0,
};
static s16 D_us_8018070C[] = {
    2, 4, 7, 5, 6, 8, 9, 1, 10, 11, 12, 13, 0, 0,
};

static s16 D_us_80180728[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static s16 D_us_80180740[] = {
    -0x40, 0, 0, 0x80, 0x100, -0x100, 0x100, -0x40, -0x40, 0x80, 0, -0x20,
};
static s16 D_us_80180758[] = {
    -0x60, 0, 0, 0xA0, 0x130, -0x40, 0x180, -0x48, -0x20, 0xA0, -0x20, -0x40,
};
static unkStr_801CDD80 D_us_80180770[] = {
    {0x10, D_us_80180728}, {0x40, D_us_80180740}, {0x30, D_us_80180758},
    {0x20, D_us_80180740}, {0x80, D_us_80180728}, {0, NULL},
};

static s16 D_us_801807A0[] = {
    -0x10, 0, 0, -0x10, -0x20, -0x10, 0x20, 0, -0x10, -0x10, 0x10, -0x10,
};
static s16 D_us_801807B8[] = {
    8, 0, 0, 8, -0x28, 8, 0x10, 0, 8, -8, 0, 8,
};
static unkStr_801CDD80 D_us_801807D0[] = {
    {0x10, D_us_80180728},
    {0x10, D_us_801807A0},
    {0x10, D_us_80180728},
    {0x08, D_us_801807B8},
    {0, NULL},
};

static BeelzebubChainInit D_us_801807F8[] = {
    {0x02, -0x16, -0x2E, 0x250, 0x3C}, {0x00, -0x12, -0x32, 0x220, 0x3C},
    {0x00, 0x3E, -0x20, 0x2C0, 0x3C},  {0x05, -0x12, -0x24, 0x1C0, 0x3C},
    {0x01, -0x28, -0x1C, 0x170, 0x3C}, {0x0A, -0x22, -0x18, 0x150, 0x3C},
    {0x0C, 0x1A, -0x34, 0x320, 0x3C},
};

static u8 D_us_80180840[] = {
    0x10, 0x14, 0x10, 0x15, 0x10, 0x16, 0x10, 0x15, 0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_8018084C[] = {0x10, 0x12, 0x10, 0x13, 0x00, 0x00, 0x00, 0x00};
static u8 D_us_80180854[] = {
    0x02, 0x01, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_80180860[] = {0x04, 0x04, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00};
static u8 D_us_80180868[] = {0x04, 0x06, 0x04, 0x07, 0x00, 0x00, 0x00, 0x00};
static u8 D_us_80180870[] = {0x02, 0x08, 0x02, 0x09, 0x00, 0x00, 0x00, 0x00};
static u8 D_us_80180878[] = {
    0x04, 0x0A, 0x04, 0x0B, 0x04, 0x0C, 0x04, 0x0D, 0x04, 0x0E,
    0x04, 0x0F, 0x04, 0x10, 0x04, 0x11, 0x00, 0x00, 0x00, 0x00,
};

// hitboxes: offset x, offset y, width, height
static s8 D_us_8018088C[][4] = {
    {0x00, 0x00, 0x00, 0x00}, {0x00, -0x04, 0x14, 0x0F},
    {-0x2C, -0x20, 0x00, 0x00}, {-0x04, -0x04, 0x00, 0x00},
    {0x00, -0x01, 0x04, 0x04}, {-0x05, -0x03, 0x10, 0x15},
    {0x09, -0x0A, 0x24, 0x14}, {0x10, -0x05, 0x18, 0x0C},
    {-0x03, 0x00, 0x05, 0x1E}, {0x00, -0x15, 0x05, 0x17},
    {0x00, -0x02, 0x05, 0x21}, {-0x01, -0x01, 0x17, 0x12},
    {0x06, -0x0A, 0x0F, 0x13}, {-0x05, 0x03, 0x06, 0x32},
    {0x02, -0x19, 0x08, 0x1E}, {0x00, -0x04, 0x08, 0x30},
    {-0x04, -0x08, 0x00, 0x00},
};
// hitbox index of each animation frame
static u8 D_us_801808D0[] = {
    0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x00, 0x00, 0x00,
};

static u8 D_us_801808F4[] = {
    0x03, 0x01, 0x03, 0x02, 0x03, 0x03, 0x03, 0x04, 0x03, 0x05,
    0x03, 0x06, 0x03, 0x07, 0x03, 0x08, 0x03, 0x09, 0x03, 0x0A,
    0x03, 0x0B, 0x03, 0x0C, 0x03, 0x0D, 0xFF, 0x00,
};
static u8 D_us_80180910[] = {
    0x02, 0x01, 0x02, 0x02, 0x02, 0x03, 0x02, 0x04, 0x02, 0x05, 0x02, 0x06,
    0x02, 0x07, 0x02, 0x08, 0x02, 0x09, 0x02, 0x0A, 0x02, 0x0B, 0x02, 0x0C,
    0x02, 0x0D, 0x02, 0x0E, 0xFF, 0x00, 0x00, 0x00,
};
static u8 D_us_80180930[] = {
    0x03, 0x01, 0x03, 0x02, 0x03, 0x03, 0x03, 0x04, 0x03, 0x05,
    0x03, 0x06, 0x03, 0x07, 0x03, 0x08, 0x03, 0x09, 0x03, 0x0A,
    0x03, 0x0B, 0x03, 0x0C, 0x03, 0x0D, 0xFF, 0x00,
};
static EntityConfig D_us_8018094C[] = {
    {ANIMSET_DRA(14), 0x79, 0, BLEND_TRANSP | BLEND_ADD, D_us_801808F4},
    {ANIMSET_OVL(4), 0x54, 0, BLEND_NO, D_us_80180910},
    {ANIMSET_OVL(4), 0x54, 3, BLEND_TRANSP | BLEND_ADD, D_us_80180910},
    {ANIMSET_OVL(5), 0x55, 3, BLEND_TRANSP | BLEND_ADD, D_us_80180930},
    {ANIMSET_OVL(5), 0x55, 3, BLEND_TRANSP | BLEND_ADD, D_us_80180930},
};

void func_801CE1E8(s32 step);
void func_us_8019D260_from_rcen(void);
void polarPlacePartsList(s16* offsets);
void polarPlacePartsWithAngvel(s16* entOffsets);
void func_801CDF1C(s16 entIndices[], unkStr_801CDD80* arg1, s32 arg2);

static void SetHitbox(Entity* self) {
    s8* hitbox = D_us_8018088C[D_us_801808D0[self->animCurFrame]];

    self->hitboxOffX = hitbox[0];
    self->hitboxOffY = hitbox[1];
    self->hitboxWidth = hitbox[2];
    self->hitboxHeight = hitbox[3];
}

// Beelzebub's body
void func_us_801923A8(Entity* self) {
    BeelzebubLimbInit* limb;
    Entity* prev;
    Entity* entity;
    s16* offsets;
    s32 count;
    s32 i;

    FntPrint("boss_life %x\n", D_us_80180630);
    if ((self->flags & FLAG_DEAD) && self->step != 4) {
        func_801CE1E8(4);
    }

    switch (self->step) {
    case 0:
        prev = self;
        InitializeEntity(g_EInitBeezelbub);
        self->animCurFrame = 0x14;
        D_us_80180634 = 0;
        BZ(self)->u84.prevHitPoints = self->hitPoints;
        D_us_80180630 = self->hitPoints;
        SetHitbox(self);
        for (i = 0, limb = D_us_8018063C; limb->idx; limb++, i++) {
            entity = self + limb->idx;
            CreateEntityFromCurrentEntity(E_UNK_17, entity);
            BZ(entity)->length = limb->length;
            BZ(entity)->parent = self + limb->parent;
            entity->params = i;
            BZ(entity)->angleOffset = limb->angleOffset;
            entity->nextPart = prev;
            prev = entity;
            self[limb->idx].zPriority = self->zPriority + limb->zOffset;
        }
        self->nextPart = prev;
        CreateEntityFromCurrentEntity(E_UNK_18, self + 24);
        BZ(self + 24)->parent = self;
        func_801CE1E8(1);
        // fallthrough

    case 1:
        polarPlacePartsList(D_us_801806F0);
        if (g_BossMgrState & BOSS_MGR_FIGHT_ON) {
            func_801CE1E8(2);
        }
        break;

    case 2:
    case 3:
        if (!BZ(self)->timer) {
            g_api.PlaySfx(SFX_BEELZEBUB_FLY_BUZZ);
            count = D_us_80180634 + 2;
            BZ(self)->timer = 0x180;
            for (i = 0; i < count; i++) {
                entity = AllocEntity(&g_Entities[128], &g_Entities[152]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_19, entity);
                    entity->params = i & 1;
                    BZ(entity)->parent = self;
                    entity->posY.i.hi = (i / 2) << 6;
                    entity->rotate = (0x1000 / count) * i;
                    if (i & 1) {
                        entity->posX.i.hi = self->posX.i.hi + 0xC0;
                    } else {
                        entity->posX.i.hi = self->posX.i.hi - 0xC0;
                    }
                    entity->zPriority = self->zPriority;
                }
            }
        }
        BZ(self)->timer--;
        switch (self->step) {
        case 2:
            AnimateEntity(D_us_80180840, self);
            func_801CDF1C(D_us_8018070C, D_us_801807D0, 0);
            polarPlacePartsWithAngvel(D_us_8018070C);
            polarPlacePartsList(D_us_801806F0);
            if (self->hitFlags & 3) {
                g_api.PlaySfx(SFX_BEELZEBUB_PAIN);
                func_801CE1E8(3);
            }
            break;

        case 3:
            func_801CDF1C(D_us_8018070C, D_us_80180770, 0);
            polarPlacePartsWithAngvel(D_us_8018070C);
            polarPlacePartsList(D_us_801806F0);
            if (!BZ(self)->unkB4[0] && !BZ(self)->unkB0[0]) {
                func_801CE1E8(2);
            }
            break;
        }
        break;

    case 4:
        switch (self->step_s) {
        case 0:
            D_us_80180630 = -1;
            self->hitboxState = 0;
            g_BossMgrState |= BOSS_MGR_DEFEAT;
            self->step_s++;
            self->drawFlags |= ENTITY_ROTATE;
            // fallthrough

        case 1:
            MoveEntity();
            self->velocityY += FIX(1.0 / 64);
            self->rotate++;
            if (!(g_Timer & 7)) {
                PlaySfxPositional(SFX_EXPLODE_A);
            }
            if (!(g_Timer & 3)) {
                entity = AllocEntity(&g_Entities[80], &g_Entities[248]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_1D, self, entity);
                    entity->posY.i.hi -= Random() & 0xF;
                    entity->posX.i.hi = entity->posX.i.hi - 0x20 +
                                        (Random() & 0x3F);
                    entity->zPriority = self->zPriority + 1;
                }
                func_us_8019D260_from_rcen();
            }
            if (self->posY.i.hi + (g_Tilemap.scrollY.i.hi + 0x18) > 0x1C0) {
                BZ(self)->timer = 0x80;
                PlaySfxPositional(SFX_BOSS_LARGE_FLAMES);
                self->step_s++;
            }
            break;

        case 2:
            if (!(g_Timer & 7)) {
                PlaySfxPositional(SFX_EXPLODE_A);
                for (i = 0; i < 12; i++) {
                    entity = AllocEntity(&g_Entities[80], &g_Entities[248]);
                    if (entity != NULL) {
                        CreateEntityFromEntity(E_UNK_1D, self, entity);
                        entity->rotate = ((Random() & 0x7F) << 4) - 0x400;
                        entity->params = 3;
                        entity->zPriority = self->zPriority + 1;
                    }
                }
            }
            if (!--BZ(self)->timer) {
                BZ(self)->timer = 0x40;
                self->step_s++;
            }
            break;

        case 3:
            if (BZ(self)->timer < 0x30) {
                self->animCurFrame = 0;
            }
            entity = AllocEntity(&g_Entities[80], &g_Entities[248]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1D, self, entity);
                entity->params = 4;
                entity->zPriority = self->zPriority + 1;
                entity->posY.i.hi += 0x10;
            }
            if (!--BZ(self)->timer) {
                self->step_s++;
            }
            break;

        case 4:
            g_BossMgrState |= BOSS_MGR_TERMINATED;
            break;
        }
        break;

    case 0xFF:
        FntPrint("charal %x\n", self->animCurFrame);
        if (g_pads[1].pressed & 0x80) {
            if (self->params) {
                break;
            }
            self->animCurFrame++;
            self->params |= 1;
        } else {
            self->params = 0;
        }
        if (g_pads[1].pressed & 0x20) {
            if (!self->step_s) {
                self->animCurFrame--;
                self->step_s |= 1;
            }
        } else {
            self->step_s = 0;
        }
        break;
    }

    for (offsets = D_us_801806F0; *offsets; offsets++) {
        BZ(self + *offsets)->unkA8 = 0;
    }

    if (!(self->flags & FLAG_DEAD)) {
        if (D_us_80180630 < 0) {
            D_us_80180630 = 1;
        }
        if (D_us_80180630 != BZ(self)->u84.prevHitPoints && self->step != 3) {
            func_801CE1E8(3);
        }
        D_us_80180630 += self->hitPoints - BZ(self)->u84.prevHitPoints;
        self->hitPoints = D_us_80180630;
        BZ(self)->u84.prevHitPoints = self->hitPoints;
    }
}

// Beelzebub's limbs
void func_us_80192C5C(Entity* self) {
    Entity* entity;
    s32 delta;
    s32 i;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitBeezelbub801804A0);
        BZ(self)->u84.prevHitPoints = self->hitPoints;
        self->animCurFrame = D_us_8018063C[self->params].animCurFrame;
        self->drawFlags |= ENTITY_ROTATE;
        SetHitbox(self);
        for (i = 0; i < 3; i++) {
            entity = AllocEntity(&g_Entities[216], &g_Entities[248]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1A, self, entity);
                BZ(entity)->parent = self;
                entity->zPriority = self->zPriority;
            }
        }
        break;

    case 1:
        self->rotate = BZ(self)->rotate;
        if (!(Random() & 0x3F)) {
            entity = AllocEntity(&g_Entities[112], &g_Entities[128]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1B, self, entity);
                BZ(entity)->parent = self;
                entity->zPriority = self->zPriority + 1;
                entity->posX.i.hi = entity->posX.i.hi - 8 + (Random() & 0xF);
                entity->posY.i.hi = entity->posY.i.hi - 8 + (Random() & 0xF);
            }
        }
        delta = self->hitPoints - BZ(self)->u84.prevHitPoints;
        BZ(self)->u84.prevHitPoints = self->hitPoints;
        D_us_80180630 += delta;
        if (D_us_80180630 < D_us_801806D8[self->params] &&
            ((BZ(self)->parent->flags & FLAG_DEAD) || (self->hitFlags & 3))) {
            D_us_80180634++;
            SetStep(2);
        }
        break;

    case 2:
        entity = AllocEntity(&g_Entities[192], &g_Entities[248]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_UNK_1C, self, entity);
            entity->params = self->animCurFrame;
            entity->zPriority = self->zPriority;
            entity->rotate = self->rotate;
        }
        self->animCurFrame = 0;
        self->hitboxState = 0;
        self->flags |= FLAG_DEAD;
        self->step++;
        return;

    case 3:
        return;
    }

    if (!self->params) {
        AnimateEntity(D_us_8018084C, self);
    }
}

// chains that keep Beelzebub hanging from the ceiling
void func_us_80192F84(Entity* self) {
    BeelzebubChainInit* chain;
    Primitive* prim;
    Entity* part;
    s16* nLinks;
    s32 primIndex;
    s32 used;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    s32 bodyX;
    s32 bodyY;
    s32 accX;
    s32 accY;
    s32 sinA;
    s32 cosA;
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = (s16)g_api.func_800EDB58(PRIM_SPRT | 0x10, 0x100);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        BZ(self)->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        while (prim != NULL) {
            prim->tpage = 0x12;
            prim->clut = 0x200;
            prim->u0 = 0xD0;
            prim->v0 = 0x70;
            prim->v1 = 8;
            prim->u1 = 8;
            prim->drawMode = DRAW_HIDE;
            prim->priority = self->zPriority;
            prim = prim->next;
        }
        break;

    case 1:
        nLinks = BZ(self)->u84.nLinks;
        for (i = 0, chain = D_us_801807F8; i < 7; i++, chain++) {
            part = BZ(self)->parent + chain->idx;
            angle = part->rotate;
            sinA = rsin(angle);
            cosA = rcos(angle);
            x = (chain->x - g_Tilemap.scrollX.i.hi) -
                (((chain->dx * cosA - chain->dy * sinA) >> 12) +
                 part->posX.i.hi);
            y = (chain->y - g_Tilemap.scrollY.i.hi) -
                (((chain->dx * sinA + chain->dy * cosA) >> 12) +
                 part->posY.i.hi);
            nLinks[i] = SquareRoot0(x * x + y * y) / 6 + 2;
        }
        self->step++;
        // fallthrough

    case 2:
        nLinks = BZ(self)->u84.nLinks;
        prim = BZ(self)->prim;
        used = 0;
        for (i = 0, chain = D_us_801807F8; i < 7; i++, chain++) {
            part = BZ(self)->parent + chain->idx;
            if (part->flags & FLAG_DEAD) {
                // the limb is gone, let the chain fall down
                if (!((D_us_80180638 >> i) & 1)) {
                    g_api.PlaySfx(SFX_RIC_WHIP_RATTLE_A);
                    D_us_80180638 |= 1 << i;
                }
                x = (chain->x - g_Tilemap.scrollX.i.hi) << 8;
                y = (chain->y - g_Tilemap.scrollY.i.hi) << 8;
                for (j = 0; j < nLinks[i]; j++) {
#ifdef VERSION_PC
                    if (prim == NULL) {
                        break;
                    }
#endif
                    dx = ((prim->x0 << 8) | prim->x1) - x;
                    dy = ((prim->y0 << 8) | prim->y1) - y + 0x800;
                    angle = ratan2(dy, dx);
                    if (dx * dx + dy * dy > 0x240000) {
                        dx = (rcos(angle) * 6) >> 4;
                        dy = (rsin(angle) * 6) >> 4;
                    }
                    x += dx;
                    y += dy;
                    prim->x1 = x & 0xFF;
                    prim->y1 = y & 0xFF;
                    prim->drawMode = DRAW_UNK02;
                    prim->x0 = x >> 8;
                    prim->y0 = y >> 8;
                    if ((u32)(s16)(x >> 8) > 0x100 ||
                        (u32)((s16)(y >> 8) - 0x10) > 0xE0) {
                        prim->drawMode = DRAW_UNK02 | DRAW_HIDE;
                    }
                    prim = prim->next;
                }
            } else {
                angle = part->rotate;
                sinA = rsin(angle);
                cosA = rcos(angle);
                x = chain->x - g_Tilemap.scrollX.i.hi;
                y = chain->y - g_Tilemap.scrollY.i.hi;
                bodyX = ((chain->dx * cosA - chain->dy * sinA) >> 12) +
                        part->posX.i.hi;
                bodyY = ((chain->dx * sinA + chain->dy * cosA) >> 12) +
                        part->posY.i.hi;
                angle = ratan2(bodyY - y, bodyX - x);
                sinA = rsin(angle);
                cosA = rcos(angle);
                accX = 0;
                accY = sinA * 6;
                for (j = 0; j < nLinks[i]; j++, accY += sinA * 6) {
#ifdef VERSION_PC
                    if (prim == NULL) {
                        break;
                    }
#endif
                    accX += cosA * 6;
                    dx = x + (accX >> 12);
                    dy = y + (accY >> 12);
                    prim->drawMode = DRAW_UNK02;
                    prim->x0 = dx;
                    prim->y0 = dy;
                    if ((u32)dx > 0x100 || (u32)(dy - 0x10) > 0xE0) {
                        prim->drawMode = DRAW_UNK02 | DRAW_HIDE;
                    }
                    if (bodyY < dy) {
                        prim->drawMode |= DRAW_HIDE;
                    }
                    used++;
                    prim = prim->next;
                }
            }
        }
        FntPrint("used_work %x\n", used);
#ifdef VERSION_PC
        if (prim == NULL) {
            break;
        }
#endif
        while (prim->next != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        prim->tpage = 0x12;
        prim->clut = 0x200;
        prim->y0 = 0;
        prim->x0 = 0;
        prim->drawMode = DRAW_UNK02;
        break;
    }
}

static s32 GetCirclePoint(Entity* self, s16* outX, s16* outY) {
    Entity* parent = BZ(self)->parent;
    s16 angle = self->rotate;
    s16 posX = parent->posX.i.hi;
    s16 posY = parent->posY.i.hi;

    *outX = posX + ((rcos(angle) * 0x60) >> 12);
    *outY = posY + ((rsin(angle) * 0x60) >> 12);
    return 0;
}

// flies circling around Beelzebub, then diving towards the player
void func_us_801936FC(Entity* self) {
    Entity* entity;
    s16 angle;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    s32 dist;
    s16 cx;
    s16 cy;

    self->rotate += 0x10;
    if (g_BossMgrState & BOSS_MGR_DEFEAT) {
        self->flags |= FLAG_DEAD;
    }
    if (self->flags & FLAG_DEAD) {
        PlaySfxPositional(SFX_QUICK_STUTTER_EXPLODE_A);
        entity = AllocEntity(&g_Entities[192], &g_Entities[248]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 3;
            entity->zPriority = self->zPriority;
        }
        DestroyEntity(self);
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804AC);
        self->animCurFrame = 0;
        self->hitboxState = 0;
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        if (self->params) {
            self->step = 3;
            self->zPriority += 6;
        } else {
            self->step = 2;
            self->zPriority -= 6;
        }
        break;

    case 2:
        // fly closer, growing from the background
        AnimateEntity(D_us_80180860, self);
        GetCirclePoint(self, &cx, &cy);
        x = cx;
        y = cy;
        dx = x - self->posX.i.hi;
        dy = y - self->posY.i.hi;
        angle = ratan2(dy, dx);
        dist = SquareRoot0(dx * dx + dy * dy) - 3;
        self->posX.i.hi = x - ((dist * rcos(angle)) >> 12);
        self->posY.i.hi = y - ((dist * rsin(angle)) >> 12);
        self->scaleX = 0x100 - dist;
        if (self->scaleX < 0x40) {
            self->scaleX = 0x40;
        }
        self->scaleY = self->scaleX;
        if (dist < 2) {
            self->drawFlags = ENTITY_DEFAULT;
            self->zPriority += 0xC;
            SetStep(4);
        }
        break;

    case 3:
        // fly closer, shrinking from the foreground
        AnimateEntity(D_us_80180868, self);
        GetCirclePoint(self, &cx, &cy);
        x = cx;
        y = cy;
        dx = x - self->posX.i.hi;
        dy = y - self->posY.i.hi;
        angle = ratan2(dy, dx);
        dist = SquareRoot0(dx * dx + dy * dy) - 3;
        self->posX.i.hi = x - ((dist * rcos(angle)) >> 12);
        self->posY.i.hi = y - ((dist * rsin(angle)) >> 12);
        self->scaleX = dist + 0x100;
        if (self->scaleX > 0x1C0) {
            self->scaleX = 0x1C0;
        }
        self->scaleY = self->scaleX;
        if (dist < 2) {
            self->drawFlags = ENTITY_DEFAULT;
            SetStep(4);
        }
        break;

    case 4:
        if (!self->step_s) {
            BZ(self)->timer = 0x80;
            self->hitboxState = 3;
            self->step_s++;
        }
        if (!AnimateEntity(D_us_80180854, self)) {
            self->facingLeft = GetSideToPlayer() & 1;
        }
        GetCirclePoint(self, &cx, &cy);
        self->posX.i.hi = cx;
        self->posY.i.hi = cy;
        if (!--BZ(self)->timer) {
            SetStep(5);
        }
        break;

    case 5:
        angle = GetAngleBetweenEntities(self, &PLAYER);
        self->velocityX = (rcos(angle) * FIX(2.5)) >> 12;
        self->velocityY = (rsin(angle) * FIX(2.5)) >> 12;
        if (self->velocityX > 0) {
            self->facingLeft = 0;
        } else {
            self->facingLeft = 1;
        }
        self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
        self->step++;
        // fallthrough

    case 6:
        MoveEntity();
        AnimateEntity(D_us_80180854, self);
        break;
    }
}

// flies buzzing around a limb
void func_us_80193C2C(Entity* self) {
    Entity* parent;
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804C4);
        BZ(self)->angle = Random() << 4;
        self->pose = Random() & 1;
        self->poseTimer = Random() & 1;
        Random();
        self->zPriority = 0x92;
        // fallthrough

    case 1:
        parent = BZ(self)->parent;
        if (parent->flags & FLAG_DEAD) {
            parent = BZ(parent)->parent;
            BZ(self)->parent = parent;
        }
        self->rotate = ratan2(parent->posY.i.hi - self->posY.i.hi,
                              parent->posX.i.hi - self->posX.i.hi);
        BZ(self)->timer = (Random() & 0x1F) + 0x10;
        self->step++;
        // fallthrough

    case 2:
        AnimateEntity(D_us_80180870, self);
        MoveEntity();
        angle = LimitAngleChange(0x18, BZ(self)->angle, self->rotate);
        self->velocityX = rcos(angle) * 0x18;
        self->velocityY = rsin(angle) * 0x18;
        BZ(self)->angle = angle;
        if (!--BZ(self)->timer) {
            self->step--;
        }
        if (g_BossMgrState & BOSS_MGR_DEFEAT) {
            self->step = 3;
        }
        break;

    case 3:
        MoveEntity();
        self->velocityY = FIX(-1.5);
        if (self->posY.i.hi < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

// maggots crawling on a limb, then falling to the floor
void func_us_80193E24(Entity* self) {
    Entity* parent;
    s16 angle;
    s32 x;

    if (g_BossMgrState & BOSS_MGR_DEFEAT) {
        self->flags |= FLAG_DEAD;
    }
    if (self->flags & FLAG_DEAD) {
        self->step = 0;
        self->drawFlags = ENTITY_DEFAULT;
        self->pfnUpdate = EntityExplosion;
        self->params = 0;
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804B8);
        self->animCurFrame = 0xA;
        self->drawFlags = ENTITY_ROTATE;
        self->zPriority = 0x90;
        // fallthrough

    case 1:
        parent = BZ(self)->parent;
        if (parent->flags & FLAG_DEAD) {
            self->step = 2;
        }
        if (!self->step_s) {
            BZ(self)->angle = ratan2(parent->posY.i.hi - self->posY.i.hi,
                                     parent->posX.i.hi - self->posX.i.hi);
            BZ(self)->timer = (Random() & 0xF) + 4;
            self->step_s++;
        }
        AnimateEntity(D_us_80180878, self);
        MoveEntity();
        angle = LimitAngleChange(0x20, self->rotate, BZ(self)->angle);
        self->velocityX = rcos(angle) * 4;
        self->velocityY = rsin(angle) * 4;
        self->rotate = angle;
        if (!--BZ(self)->timer) {
            self->step_s = 0;
            if (!(Random() & 0x7F)) {
                self->step = 2;
            }
        }
        break;

    case 2:
        self->flags |= FLAG_DESTROY_IF_BARELY_OUT_OF_CAMERA;
        MoveEntity();
        self->velocityY += FIX(0.03125);
        if (self->posY.i.hi + g_Tilemap.scrollY.i.hi + 1 > 0x1C0) {
            self->drawFlags = ENTITY_DEFAULT;
            self->velocityX = 0;
            self->velocityY = 0;
            self->posY.i.hi = 0x1BF - g_Tilemap.scrollY.i.hi;
            self->facingLeft = GetSideToPlayer() & 1;
            self->step++;
        }
        break;

    case 3:
        AnimateEntity(D_us_80180878, self);
        MoveEntity();
        if (self->facingLeft) {
            self->velocityX = FIX(-0.25);
        } else {
            self->velocityX = FIX(0.25);
        }
        x = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
        if (self->velocityX > 0) {
            if (x > 0x3D0) {
                x = 0;
            }
        } else if (x < 0xC0) {
            x = 0;
        }
        if (!x) {
            DestroyEntity(self);
        }
        break;
    }
}

// destroyed limb falling to the floor
void func_us_80194108(Entity* self) {
    Entity* entity;
    s16* frame;
    s32 bottom;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804D0);
        self->drawFlags = ENTITY_ROTATE;
        self->animCurFrame = self->params;
        BZ(self)->rotVel = (Random() & 3) + 2;
        PlaySfxPositional(SFX_BO1_UNK_7C7);
        // fallthrough

    case 1:
        MoveEntity();
        self->velocityY += FIX(1.0 / 16);
        self->rotate += BZ(self)->rotVel;
        // the sprite part offset and height give the bottom of the limb
        frame = sprites_rbo1_2[self->animCurFrame];
#ifdef VERSION_PC
        // the invisible joints use frame 0, which has no sprite: the PSX
        // reads the (zeroed) memory at address 0 instead
        if (frame == NULL) {
            static s16 empty_frame[6] = {0};
            frame = empty_frame;
        }
#endif
        bottom = frame[3] + (u16)frame[5];
        if (!(g_Timer & 7)) {
            entity = AllocEntity(&g_Entities[192], &g_Entities[248]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1D, self, entity);
                entity->posY.i.hi += frame[3] + (s16)frame[5] / 2;
                entity->params = 1;
                entity->posX.i.hi = entity->posX.i.hi - 8 + (Random() & 0xF);
                entity->zPriority = self->zPriority + 1;
            }
        }
        if (bottom + (self->posY.i.hi + g_Tilemap.scrollY.i.hi) > 0x1C0) {
            entity = AllocEntity(&g_Entities[192], &g_Entities[248]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_EXPLOSION, self, entity);
                entity->params = 3;
                entity->posY.i.hi += frame[3] + (s16)frame[5] / 2;
            }
            func_us_8019D260_from_rcen();
            DestroyEntity(self);
        }
        break;
    }
}

// similar to polarPlacePart, with an additional per-limb angle offset
void func_us_8019ED80_from_rbo2(Entity* self) {
    Entity* src;
    s16 angle;

    angle = BZ(self)->rotate + BZ(self)->angleOffset;
    if (g_CurrentEntity->facingLeft) {
        angle = -angle;
    }
    src = BZ(self)->parent;
    self->posX.val = src->posX.val;
    self->posY.val = src->posY.val;
    self->posX.val -= BZ(self)->length * rsin(angle) * 16;
    BZ(self)->unkA8 |= 1;
    self->posY.val += BZ(self)->length * rcos(angle) * 16;
}

void polarPlacePartsWithAngvel(s16* entOffsets) {
    Entity* ent;

    while (*entOffsets) {
        if (*entOffsets != 0xFF) {
            ent = g_CurrentEntity + *entOffsets;
            BZ(ent)->rotate += BZ(ent)->rotVel;
            func_us_8019ED80_from_rbo2(ent);
        }
        entOffsets++;
    }
}

void func_801CDD00(Entity* entity, s16 arg1, s16 arg2) {
    s16 temp_t0 = arg1 - BZ(entity)->rotate;

    if (temp_t0 > 0x800) {
        temp_t0 = temp_t0 - 0x1000;
    }
    if (temp_t0 < -0x800) {
        temp_t0 = temp_t0 + 0x1000;
    }
    temp_t0 = temp_t0 / arg2;
    BZ(entity)->rotVel = temp_t0;
    BZ(entity)->unkA4 = arg1;
}

void func_801CDD80(s16* entOffsets, unkStr_801CDD80* arg1) {
    Entity* ent;
    s16* ptr = arg1->unk4;

    while (*entOffsets) {
        if (*entOffsets != 0xFF) {
            ent = g_CurrentEntity + *entOffsets;
            func_801CDD00(ent, *ptr, arg1->unk0);
        }
        ptr++;
        entOffsets++;
    }
}

void func_801CDF1C(s16 entIndices[], unkStr_801CDD80* arg1, s32 arg2) {
    arg1 += BZ(g_CurrentEntity)->unkB0[arg2];

    if (!BZ(g_CurrentEntity)->unkB4[arg2]) {
        func_801CDD80(entIndices, arg1);
        BZ(g_CurrentEntity)->unkB4[arg2] = arg1->unk0;
    }
    if (!--BZ(g_CurrentEntity)->unkB4[arg2]) {
        arg1++;
        if (!arg1->unk0) {
            BZ(g_CurrentEntity)->unkB0[arg2] = 0;
        } else {
            ++BZ(g_CurrentEntity)->unkB0[arg2];
        }
    }
}

void func_801CE1E8(s32 step) {
    s32 i;

    g_CurrentEntity->step = step;
    g_CurrentEntity->step_s = 0;
    g_CurrentEntity->pose = 0;
    g_CurrentEntity->poseTimer = 0;
#ifdef VERSION_PC
    // the original loop writes out of bounds of both arrays (see
    // giantbro_helpers_2.h), ending up clearing `unkB8` too
    for (i = 0; i < 2; i++) {
        BZ(g_CurrentEntity)->unkB0[i] = 0;
        BZ(g_CurrentEntity)->unkB4[i] = 0;
    }
    g_CurrentEntity->unkB8 = NULL;
#else
    for (i = 0; i < 4; i++) {
        BZ(g_CurrentEntity)->unkB0[i] = 0;
        BZ(g_CurrentEntity)->unkB4[i] = 0;
    }
#endif
}

void func_801CE228(void) {
    s32 i;

    // same out of bounds clearing as func_801CE1E8
    for (i = 0; i < 2; i++) {
        BZ(g_CurrentEntity)->unkB0[i] = 0;
        BZ(g_CurrentEntity)->unkB4[i] = 0;
    }
    g_CurrentEntity->unkB8 = NULL;
}

void polarPlacePartsList(s16* offsets) {
    Entity* entity;

    while (*offsets) {
        entity = g_CurrentEntity + *offsets;
        if (!BZ(entity)->unkA8) {
            func_us_8019ED80_from_rbo2(entity);
        }
        offsets++;
    }
}

void func_us_8019D260_from_rcen(void) {
    Entity* puff;
    s32 rand3 = Random() & 3;
    s16 initAngle = ((Random() & 0xF) << 8) - ROT(180);
    s32 i;

    for (i = 0; i < 6; i++) {
        puff = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (puff != NULL) {
            CreateEntityFromEntity(E_UNK_1D, g_CurrentEntity, puff);
            PUFF(puff)->speed = 6 - i;
            PUFF(puff)->puffStyle = rand3;
            puff->params = 2;
            PUFF(puff)->angle = initAngle;
            puff->zPriority = g_CurrentEntity->zPriority + 1;
        }
    }
}

// similar to EntityExplosionPuffOpaque
void func_us_801947E4(Entity* self) {
    EntityConfig* obj;
    s32 params;
    s32 speed;
    s16 rotate;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        params = self->params & 0xF;
        obj = &D_us_8018094C[params];
        self->palette = obj->palette + 0x2E0;
        self->blendMode = obj->blendMode;
        self->animSet = obj->animSet;
        self->unk5A = obj->unk5A;
        self->step = params + 1;
        PUFF(self)->anim = obj->animData;
        if (self->params & 0xFF00) {
            self->zPriority = (self->params & 0xFF00) >> 8;
        }
        if (self->params & 0xF0) {
            self->palette = PAL_FLAG(0x19F);
            self->blendMode = BLEND_TRANSP;
            self->facingLeft = 1;
        }
        return;

    case 1:
        MoveEntity();
        self->velocityY = FIX(-1);
        break;

    case 2:
        break;

    case 3:
        if (!self->step_s) {
            self->drawFlags |= ENTITY_ROTATE;
            switch (PUFF(self)->puffStyle) {
            case 1:
                if (PUFF(self)->speed > 3) {
                    PUFF(self)->speed -= 3;
                    PUFF(self)->angle -= ROT(180);
                }
                break;

            case 2:
                PUFF(self)->angle += PUFF(self)->speed * 0xC0;
                break;
            }
            PUFF(self)->angle &= 0xFFF;
            self->rotate = PUFF(self)->angle;
            speed = PUFF(self)->speed * 0x140 / 28;
            self->velocityX = speed * rsin(PUFF(self)->angle);
            self->velocityY = -(speed * rcos(PUFF(self)->angle));
            self->step_s++;
        }
        if (self->pose > 12) {
            self->velocityX -= self->velocityX / 4;
            self->velocityY -= self->velocityY / 4;
        }
        MoveEntity();
        break;

    case 4:
        if (!self->step_s) {
            self->drawFlags = ENTITY_OPACITY | ENTITY_ROTATE;
            self->opacity = 0x80;
            self->facingLeft = Random() & 1;
            rotate = self->rotate;
            if (self->facingLeft) {
                rotate = -rotate;
            }
            self->velocityX = rsin(rotate) * 0x28;
            self->velocityY = -(rcos(rotate) * 0x28);
            PUFF(self)->accel = (Random() << 4) + 0x1000;
            self->step_s++;
        }
        MoveEntity();
        rotate = self->rotate;
        self->opacity--;
        if (self->facingLeft) {
            rotate = -rotate;
        }
        self->velocityX += (rsin(rotate) * PUFF(self)->accel) >> 0xC;
        self->velocityY += (-PUFF(self)->accel * rcos(rotate)) >> 0xC;
        break;

    case 5:
        if (!self->step_s) {
            self->drawFlags = ENTITY_OPACITY;
            self->opacity = 0x80;
            self->facingLeft = Random() & 1;
            self->velocityX = (Random() << 9) - FIX(1);
            self->velocityY = FIX(-2.5);
            PUFF(self)->accelY = -(Random() << 4) - 0x1000;
            self->step_s++;
        }
        MoveEntity();
        self->velocityY += PUFF(self)->accelY;
        self->opacity--;
        break;

    default:
        return;
    }

    if (!AnimateEntity(PUFF(self)->anim, self)) {
        DestroyEntity(self);
    }
}
#endif
