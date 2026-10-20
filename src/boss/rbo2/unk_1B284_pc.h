// SPDX-License-Identifier: AGPL-3.0-or-later
// Death (inverted castle) boss logic. Non-matching C used by the PC build in
// place of the assembly; the PSX build still uses the INCLUDE_ASM blocks.

// Death's first form, its scythe balls and the skulls fired by them
typedef struct {
    /* 0x7C */ Primitive* prim;
    /* 0x80 */ s16 timer;
    /* 0x82 */ s16 angle;
    /* 0x84 */ s32 speed;
    /* 0x88 */ u8 phase2;      // first form: 2nd attack pattern unlocked
    /* 0x89 */ u8 recallBalls; // first form: balls come back to Death
    /* 0x8A */ u8 ready;       // ball: reached its position
    /* 0x8B */ u8 ballsOpen;   // first form: balls open up
    /* 0x8C */ u8 unk8C;
    /* 0x8D */ u8 ballsThrow; // first form: balls are thrown at the player
    /* 0x90 */ u8 brightness;
    /* 0x94 */ s16 orbitAngle;
    /* 0x98 */ s16 spread;
    /* 0x9C */ struct Entity* parent;
    /* 0xA0 */ struct Entity* other;
} ET_RDeath;

// Death's second form: the body and every jointed body part
typedef struct {
    /* 0x7C */ u32 : 32;
    /* 0x80 */ s16 timer;
    /* 0x82 */ s16 angle;
    /* 0x84 */ s16 orbitAngle;
    /* 0x90 */ u8 scytheBack;
    /* 0x9C */ s16 rotate;
    /* 0x9E */ s16 length;
    /* 0xA0 */ struct Entity* parent;
    /* 0xA4 */ s16 unkA4;
    /* 0xA6 */ s16 rotVel;
    /* 0xA8 */ u8 unkA8;
    /* 0xAC */ s16 angleOffset;
    /* 0xB0 */ u16 unkB0[2];
    /* 0xB4 */ u16 unkB4[2];
} ET_RDeathPart;

// The scythe thrown by Death's second form
typedef struct {
    /* 0x7C */ u32 : 32;
    /* 0x80 */ s16 timer;
    /* 0x82 */ s16 angle;
    /* 0x8C */ s32 speed;
    /* 0x9C */ struct Entity* parent;
} ET_RDeathScythe;

STATIC_ASSERT(sizeof(ET_RDeath) <= sizeof(Ext), "ext too big");
STATIC_ASSERT(sizeof(ET_RDeathPart) <= sizeof(Ext), "ext too big");
STATIC_ASSERT(sizeof(ET_RDeathScythe) <= sizeof(Ext), "ext too big");

#define DEATH(e) ((ET_RDeath*)&(e)->ext)
#define PART(e) ((ET_RDeathPart*)&(e)->ext)
#define SCYTHE(e) ((ET_RDeathScythe*)&(e)->ext)

typedef struct {
    s16 index;  // entity offset from the body
    s16 parent; // entity offset of the joint the part is attached to
    s16 length;
    s16 frame;
    s16 zOffset;
    s16 angle;
} DeathPartDef;

typedef struct {
    /* 0x0 */ u16 animSet;
    /* 0x2 */ u16 unk5A;
    /* 0x4 */ u16 palette;
    /* 0x6 */ u16 blendMode;
    /* 0x8 */ u8* anim;
} EInitUnk22;

extern EInit g_EInitBreakable;
extern EInit g_EInitParticle;
extern EInit g_EInitDeath1;
extern EInit g_EInitDeath2;
extern EInit D_us_801804A0;
extern EInit D_us_801804AC;
extern EInit D_us_801804B8;
extern EInit D_us_801804C4;
extern EInit D_us_801804DC;
extern EInit D_us_801804E8;
// the prize drop table doubles as Death's idle animation
extern u16 PrizeDrops[];
#define ANIM_DEATH_IDLE ((u8*)PrizeDrops)

void CreateEntityFromEntity(u16 entityId, Entity* source, Entity* entity);
void CreateEntityFromCurrentEntity(u16 entityId, Entity* entity);
void MoveEntity();
void SetStep(u8 step);
void SetSubStep(u8 step_s);
void PlaySfxPositional(s16 sfxId);
u16 LimitAngleChange(u16 delta, u16 base, u16 target);
#include "../../st/approach_s16.h"

s32 g_BossMgrState = 0;



// EntityBreakable data
static u8 anim_1[] = {4, 1, 4, 2, 0, 0, 0, 0};
static u8 anim_2[] = {4, 7, 4, 8, 4, 9, 0, 0};
static u8 anim_3[] = {4, 29, 4, 30, 4, 31, 0, 0};
static u8 anim_4[] = {4, 1, 4, 2, 0, 0, 0, 0};
static u8 anim_5[] = {4, 1, 4, 2, 0, 0, 0, 0};
static u8 anim_6[] = {4, 1, 4, 2, 0, 0, 0, 0};
static u8 anim_7[] = {4, 1, 4, 2, 0, 0, 0, 0};
static u8 anim_8[] = {4, 1, 4, 2, 0, 0, 0, 0, 4, 1, 4, 2, 0, 0, 0, 0};
static u8* g_eBreakableAnimations[8] = {
    anim_1, anim_2, anim_3, anim_4, anim_5, anim_6, anim_7, anim_8};
static u8 g_eBreakableHitboxes[8] = {8, 8, 8, 8, 8, 8, 8, 8};
static u8 g_eBreakableExplosionTypes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
static u16 g_eBreakableanimSets[8] = {
    ANIMSET_DRA(3), ANIMSET_OVL(1), ANIMSET_OVL(1), ANIMSET_DRA(3),
    ANIMSET_DRA(3), ANIMSET_DRA(3), ANIMSET_DRA(3), ANIMSET_DRA(3)};
static u8 blend_modes[8] = {0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

#include "../../st/e_breakable.h"

// Death data
static u8 D_us_8018067C[] = {
    0x05, 0x07, 0x04, 0x08, 0x03, 0x09, 0x05, 0x0A, 0x04, 0x0B, 0x06, 0x0C,
    0x04, 0x0D, 0x04, 0x0E, 0x01, 0x0F, 0x04, 0x10, 0x18, 0x0F, 0xFF, 0x00,
};
static u8 D_us_80180694[] = {
    0x03, 0x0A, 0x03, 0x0B, 0x04, 0x0C, 0x04, 0x0D, 0x03, 0x0E, 0x01, 0x0F,
    0x01, 0x10, 0x20, 0x0F, 0xFF, 0x00,
};
static u8 D_us_801806A8[] = {
    0x02, 0x0F, 0x02, 0x11, 0x02, 0x12, 0x00, 0x00,
};
static u8 D_us_801806B0[] = {
    0x10, 0x0A, 0x06, 0x08, 0x05, 0x07, 0xFF, 0x00,
};
static u8 D_us_801806B8[] = {
    0x02, 0x01, 0x06, 0x13, 0x07, 0x14, 0x08, 0x15, 0x09, 0x16, 0x06, 0x17,
    0x06, 0x18, 0x07, 0x19, 0x07, 0x1A, 0x08, 0x1B, 0x04, 0x1C, 0xFF, 0x00,
};
static u8 D_us_801806D0[] = {
    0x04, 0x02, 0x04, 0x4A, 0x04, 0x4B, 0x04, 0x4C, 0x04, 0x4D, 0x04, 0x4E,
    0xFF, 0x00,
};
static u8 D_us_801806E0[] = {
    0x09, 0x1D, 0x04, 0x1E, 0x04, 0x1F, 0x04, 0x20, 0x03, 0x21, 0x03, 0x22,
    0x03, 0x23, 0x03, 0x24, 0x03, 0x25, 0x04, 0x26, 0x08, 0x27, 0xFF, 0x00,
};
static u8 D_us_801806F8[] = {
    0x03, 0x25, 0x03, 0x28, 0x03, 0x29, 0x03, 0x2A, 0x03, 0x1F, 0x04, 0x1E,
    0x05, 0x1D, 0xFF, 0x00,
};
static u8 D_us_80180708[] = {
    0x04, 0x2B, 0x04, 0x2C, 0x04, 0x2D, 0x04, 0x2E, 0x04, 0x2F, 0x04, 0x31,
    0x04, 0x30, 0xFF, 0x00,
};
static u8 D_us_80180718[] = {
    0x02, 0x30, 0x01, 0x31, 0x02, 0x32, 0x01, 0x31, 0x02, 0x33, 0x01, 0x31,
    0x02, 0x34, 0x01, 0x31, 0x00, 0x00,
};
static u8 D_us_8018072C[] = {
    0x01, 0x33, 0x04, 0x31, 0x01, 0x34, 0x04, 0x31, 0x01, 0x30, 0x04, 0x31,
    0x04, 0x35, 0x04, 0x36, 0x04, 0x37, 0x04, 0x38, 0x10, 0x39, 0xFF, 0x00,
};
static u8 D_us_80180744[] = {
    0x10, 0x39, 0x04, 0x38, 0x04, 0x37, 0x04, 0x36, 0x04, 0x35, 0x04, 0x31,
    0x01, 0x30, 0x04, 0x31, 0x01, 0x34, 0x04, 0x31, 0x01, 0x33, 0xFF, 0x00,
};
static u8 D_us_8018075C[] = {
    0x04, 0x30, 0x04, 0x3A, 0x04, 0x3B, 0x04, 0x3C, 0x04, 0x3D, 0x04, 0x3E,
    0x04, 0x3F, 0x08, 0x40, 0x08, 0x50, 0xFF, 0x00,
};
static u8 D_us_80180770[] = {
    0x05, 0x41, 0x05, 0x42, 0x05, 0x43, 0x05, 0x44, 0x05, 0x45, 0x05, 0x46,
    0x05, 0x47, 0x05, 0x48, 0x05, 0x49, 0x00, 0x00,
};
static s32 D_us_80180784 = 0; // a scythe throw is requested
static s32 D_us_80180788 = 0; // the scythe swing can hit
static DeathPartDef D_us_8018078C[] = {
    {1, 0, -24, 1, 1, -256}, {3, 1, 24, 7, 2, 0},   {4, 3, 18, 8, 3, 0},
    {5, 4, 0, 10, 4, 0},     {6, 1, 24, 11, -1, 0}, {7, 6, 18, 12, -2, 0},
    {8, 7, 0, 14, -3, 0},    {9, 0, 26, 3, 1, 0},   {10, 9, 18, 4, 2, 0},
    {11, 10, 12, 5, 3, 0},   {12, 11, 0, 6, 4, 0},  {0, 0, 0, 0, 0, 0},
};
static s16 D_us_8018081C[] = {1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0};
static s16 D_us_80180834[] = {1, 3, 4, 6, 7, 5, 8, 0};
static s16 D_us_80180844[] = {1, 3, 4, 6, 7, 5, 8, 9, 10, 11, 12, 0};
static s16 D_us_8018085C[] = {
    -256, -256, 384, 384, 512, 384, 512, 256, 384, 512, 896, 0};
static s16 D_us_80180874[] = {
    -320, 256, 256, 448, 576, 384, 768, 320, 448, 576, 960, 0};
static s16 D_us_8018088C[] = {
    0, -256, 384, 0, 576, 512, 768, -64, 256, 384, 576, 0};
static unkStr_801CDD80 D_us_801808A4[] = {
    {0x20, D_us_8018085C},
    {0x40, D_us_80180874},
    {0x30, D_us_8018088C},
    {0, NULL},
};
static s16 D_us_801808C4[] = {-128, -64, 512, -128, 384, 512, 768, 0};
static s16 D_us_801808D4[] = {32, -320, 256, 128, 704, 576, 896, 0};
static unkStr_801CDD80 D_us_801808E4[] = {
    {0x10, D_us_8018088C}, {0x18, D_us_801808C4}, {0x10, D_us_8018088C},
    {0x40, D_us_801808D4}, {0, NULL},
};
static s16 D_us_8018090C[] = {256, 1024, 1664, 1152, 1792, 1792, 2304, 0};
static s16 D_us_8018091C[] = {384, 1536, 1792, 1920, 2176, 1856, 2432, 0};
static s16 D_us_8018092C[] = {-384, 1024, 1024, 1024, 1024, 1024, 1024, 0};
static s16 D_us_8018093C[] = {-768, 256, 384, 384, 512, 384, 512, 0};
static s16 D_us_8018094C[] = {-704, 288, 448, 448, 640, 384, 512, 0};
static unkStr_801CDD80 D_us_8018095C[] = {
    {0x20, D_us_8018090C}, {0x20, D_us_8018091C}, {0x04, D_us_8018092C},
    {0x02, D_us_8018093C}, {0x20, D_us_8018094C}, {0x40, D_us_8018088C},
    {0, NULL},
};
static s16 D_us_80180994[] = {256, 1536, 2048, -128, 896, 2048, 1024, 0};
static s16 D_us_801809A4[] = {512, 1664, 2304, 192, 896, 2688, 1024, 0};
static s16 D_us_801809B4[] = {-384, 768, 1152, -512, 512, 1024, 1024, 0};
static s16 D_us_801809C4[] = {-448, 896, 1216, -384, 576, 1152, 1024, 0};
static s16 D_us_801809D4[] = {-128, -256, 512, 64, 384, 640, 512, 0};
static unkStr_801CDD80 D_us_801809E4[] = {
    {0x20, D_us_80180994}, {0x18, D_us_801809A4}, {0x04, D_us_801809B4},
    {0x20, D_us_801809C4}, {0x40, D_us_801809D4}, {0, NULL},
};
static s16 D_us_80180A14[] = {-384, -256, 256, -384, 512, 384, 640, 0};
static s16 D_us_80180A24[] = {-448, -256, 320, -384, 448, 384, 640, 0};
static unkStr_801CDD80 D_us_80180A34[] = {
    {0x06, D_us_80180A14},
    {0x08, D_us_80180A24},
    {0x10, D_us_8018088C},
    {0, NULL},
};
static s16 D_us_80180A54[] = {
    384, -256, 128, 0, 0, 192, 0, 0, -64, -128, -128, 0};
static s16 D_us_80180A6C[] = {
    512, -256, -256, -128, -128, -256, -128, 0, -64, -128, -256, 0};
static unkStr_801CDD80 D_us_80180A84[] = {
    {0x06, D_us_80180A54},
    {0x40, D_us_80180A6C},
    {0, NULL},
};
static s16 D_us_80180A9C[] = {9, 10, 11, 12, 0, 0};
static s16 D_us_80180AA8[] = {256, 288, 32, -192};
static s16 D_us_80180AB0[] = {-128, 0, 384, 768};
static unkStr_801CDD80 D_us_80180AB8[] = {
    {0x40, D_us_80180AA8},
    {0x40, D_us_80180AB0},
    {0, NULL},
};
static u8 D_us_80180AEC[] = {
    0x03, 0x01, 0x03, 0x02, 0x03, 0x03, 0x03, 0x04, 0x03, 0x05, 0x03, 0x06,
    0x03, 0x07, 0x03, 0x08, 0x03, 0x09, 0x03, 0x0A, 0x03, 0x0B, 0x03, 0x0C,
    0x03, 0x0D, 0xFF, 0x00,
};
static u8 D_us_80180B24[] = {
    0x02, 0x01, 0x02, 0x02, 0x02, 0x03, 0x02, 0x04, 0x02, 0x05, 0x02, 0x06,
    0x02, 0x07, 0x02, 0x08, 0x02, 0x09, 0x02, 0x0A, 0x02, 0x0B, 0x02, 0x0C,
    0x02, 0x0D, 0x02, 0x0E, 0xFF, 0x00,
};
static EInitUnk22 D_us_80180B44[] = {
    {ANIMSET_OVL(6), 0x58, 4, BLEND_ADD | BLEND_TRANSP, D_us_80180B24},
    {ANIMSET_OVL(5), 0x59, 8, BLEND_ADD | BLEND_TRANSP, D_us_80180AEC},
};

static s16 func_us_8019A98C_from_rcen(s16 step, s16 current, s16 target) {
    s32 cur;
    s16 diff;
    s16 wrapped;

    cur = current & 0xFFF;
    diff = target - cur;
    wrapped = diff;
    if (diff > 0x800) {
        wrapped = diff - 0x1000;
    }
    if (diff < -0x800) {
        wrapped = diff + 0x1000;
    }
    if (step < abs(wrapped)) {
        // the direction comes from the unwrapped difference
        if (diff < 0) {
            return cur - step;
        }
        return cur + step;
    }
    return target;
}

static void func_us_8019B430(void) {
    Entity* entity;
    s32 x;

    if (!(g_Timer & 0xF)) {
        entity = AllocEntity(&g_Entities[160], &g_Entities[168]);
        if (entity != NULL) {
            CreateEntityFromCurrentEntity(E_UNK_17, entity);
            x = rand() & 0xFF;
            entity->posY.i.hi = (rand() & 0x3F) + 0x20;
            entity->posX.i.hi = x;
        }
    }
}

static void DeathSetPrimColors(Primitive* prim, u8 brightness) {
    prim->r0 = prim->g0 = prim->b0 = brightness;
    LOW(prim->r1) = LOW(prim->r0);
    LOW(prim->r2) = LOW(prim->r0);
    LOW(prim->r3) = LOW(prim->r0);
    prim = prim->next;
    prim->r0 = prim->g0 = prim->b0 = 0;
    LOW(prim->r2) = LOW(prim->r0);
    prim->r1 = prim->g1 = prim->b1 = brightness;
    LOW(prim->r3) = LOW(prim->r1);
    prim = prim->next;
    prim->r0 = prim->g0 = prim->b0 = brightness;
    prim->r1 = prim->g1 = prim->b1 = 0;
    LOW(prim->r2) = LOW(prim->r0);
    LOW(prim->r3) = LOW(prim->r1);
}

// Death, first form
void func_us_8019B52C(Entity* self) {
    Primitive* prim;
    Entity* ball;
    s32 primIndex;
    s32 x, y;
    s32 dx, dy;
    s32 count;
    s16 angle;
    s16 hp;

    if (self->flags & FLAG_DEAD) {
        if (self->step != 9) {
            SetStep(9);
        }
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitDeath1);
        self->hitboxOffY = -5;
        self->hitboxState = 2;
        primIndex = g_api.AllocPrimitives(PRIM_TILE, 3);
        if (primIndex == -1) {
            self->step = 0;
            break;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        DEATH(self)->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        while (prim != NULL) {
            prim->v0 = prim->u0 = 0;
            prim->drawMode = DRAW_HIDE | DRAW_UNK02;
            prim->priority = self->zPriority;
            prim = prim->next;
        }
        SetStep(1);
        // fallthrough
    case 1:
        AnimateEntity(ANIM_DEATH_IDLE, self);
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        self->hitboxState = 0;
        if (g_BossMgrState & BOSS_MGR_DEATH_FIGHT_ON) {
            self->hitboxState = 2;
            SetStep(2);
        }
        break;
    case 2:
        if (self->hitFlags & 3) {
            SetStep(8);
            break;
        }
        switch (self->step_s) {
        case 0:
            DEATH(self)->timer = ((Random() & 3) << 5) + 0x20;
            x = PLAYER.posX.i.hi;
            y = PLAYER.posY.i.hi;
            if (x < self->posX.i.hi) {
                x -= 0x60;
            } else {
                x += 0x60;
            }
            y -= Random() & 0x3F;
            // the invisible primitive stores the destination point
            prim = DEATH(self)->prim;
            prim->x0 = x - self->posX.i.hi;
            prim->y0 = y - self->posY.i.hi;
            DEATH(self)->angle = rand() & 0xFFF;
            DEATH(self)->speed = FIX(1.75);
            self->step_s++;
            // fallthrough
        case 1:
            AnimateEntity(ANIM_DEATH_IDLE, self);
            MoveEntity();
            prim = DEATH(self)->prim;
            angle = func_us_8019A98C_from_rcen(
                0x30, DEATH(self)->angle,
                ratan2(prim->y0 - self->posY.i.hi, prim->x0 - self->posX.i.hi));
            self->velocityX = (rcos(angle) * DEATH(self)->speed) >> 12;
            self->velocityY = (rsin(angle) * DEATH(self)->speed) >> 12;
            DEATH(self)->angle = angle;
            if (DEATH(self)->speed > 0xE000) {
                DEATH(self)->speed -= DEATH(self)->speed >> 4;
            }
            func_us_8019B430();
            if (--DEATH(self)->timer == 0) {
                self->step_s--;
            }
            hp = g_api.enemyDefs[ENEMY_DEATH].hitPoints;
            if (!DEATH(self)->phase2) {
                if (self->hitPoints < hp - hp / 4) {
                    SetStep(3);
                }
            } else if (self->hitPoints < hp / 2) {
                SetStep(4);
            }
            break;
        }
        break;
    case 3:
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_8018067C, self)) {
                SetSubStep(1);
            }
            break;
        case 1:
            ball = self + 1;
            CreateEntityFromEntity(E_UNK_18, self, ball);
            ball->params = 0;
            DEATH(ball)->parent = self;
            ball->posX.i.hi -= 0x28;
            ball->zPriority = self->zPriority + 1;
            ball->posY.i.hi -= 0x1C;
            ball = self + 2;
            CreateEntityFromEntity(E_UNK_18, self, ball);
            ball->params = 1;
            DEATH(self)->timer = 0x80;
            DEATH(ball)->parent = self;
            ball->posX.i.hi += 0x28;
            ball->zPriority = self->zPriority + 1;
            self->step_s++;
            ball->posY.i.hi -= 0x1C;
            // fallthrough
        case 2:
            if (!(DEATH(self)->timer & 0xF)) {
                PlaySfxPositional(0x6E2);
            }
            AnimateEntity(D_us_801806A8, self);
            if (--DEATH(self)->timer == 0) {
                SetSubStep(3);
            }
            break;
        case 3:
            if (!AnimateEntity(D_us_80180694, self)) {
                PlaySfxPositional(0x7F2);
                DEATH(self)->phase2 = 1;
                self->step_s++;
            }
            break;
        case 4:
            if (!AnimateEntity(D_us_801806B0, self)) {
                SetStep(2);
            }
            break;
        }
        break;
    case 4:
        switch (self->step_s) {
        case 0:
            DEATH(self)->recallBalls = 1;
            self->velocityX = 0;
            self->velocityY = 0;
            self->step_s++;
            // fallthrough
        case 1:
            if (!AnimateEntity(D_us_8018067C, self)) {
                PlaySfxPositional(0x7EF);
                SetSubStep(2);
            }
            break;
        case 2:
            AnimateEntity(D_us_801806A8, self);
            if (DEATH(self + 1)->ready && DEATH(self + 2)->ready) {
                DEATH(self)->ballsOpen = 1;
                SetStep(5);
            }
            break;
        }
        break;
    case 5:
        AnimateEntity(D_us_801806A8, self);
        MoveEntity();
        DEATH(self)->orbitAngle += 0x40;
        self->velocityY = (rsin(DEATH(self)->orbitAngle) * 0xC000) >> 12;
        if (self->posY.i.hi < 0x68) {
            self->velocityY += FIX(1.0 / 16);
        } else {
            self->velocityY -= FIX(1.0 / 16);
        }
        if (self->posX.i.hi < 0x80) {
            if (self->velocityX < 0xE000) {
                self->velocityX += 0x800;
            }
        } else if (self->velocityX > -0xE000) {
            self->velocityX -= 0x800;
        }
        if (self->hitPoints <
            (s16)(g_api.enemyDefs[ENEMY_DEATH].hitPoints / 3)) {
            SetStep(6);
        }
        break;
    case 6:
        switch (self->step_s) {
        case 0:
            g_BossMgrState |= 4;
            self->step_s++;
            // fallthrough
        case 1:
            AnimateEntity(D_us_801806A8, self);
            if (DEATH(self + 1)->ready && DEATH(self + 2)->ready) {
                DEATH(self)->unk8C = 1;
                DEATH(self)->timer = 0x40;
                PlaySfxPositional(0x7F1);
                self->step_s++;
            }
            break;
        case 2:
            AnimateEntity(D_us_801806A8, self);
            if (--DEATH(self)->timer == 0) {
                DEATH(self)->orbitAngle = 0x200;
                SetStep(7);
            }
            break;
        }
        break;
    case 7:
        switch (self->step_s) {
        case 0:
            AnimateEntity(ANIM_DEATH_IDLE, self);
            if (DEATH(self + 1)->ready && DEATH(self + 2)->ready) {
                SetSubStep(1);
            }
            break;
        case 1:
            if (!AnimateEntity(D_us_801806B8, self)) {
                DEATH(self)->ballsThrow = 1;
                DEATH(self)->timer = 0x10;
                SetSubStep(2);
            }
            if (self->pose == 4) {
                PlaySfxPositional(0x7F0);
            }
            break;
        case 2:
            DEATH(self)->ballsThrow = 0;
            if (--DEATH(self)->timer == 0) {
                SetSubStep(0);
            }
            break;
        }
        MoveEntity();
        angle = DEATH(self)->orbitAngle;
        x = PLAYER.posX.i.hi + ((rcos(angle) * 0x60) >> 12);
        y = PLAYER.posY.i.hi - ((rsin(angle) * 0x60) >> 12);
        angle = ratan2(y - self->posY.i.hi, x - self->posX.i.hi);
        DEATH(self)->speed = FIX(1.75);
        angle = func_us_8019A98C_from_rcen(0x30, DEATH(self)->angle, angle);
        self->velocityX = (rcos(angle) * DEATH(self)->speed) >> 12;
        self->velocityY = (rsin(angle) * DEATH(self)->speed) >> 12;
        DEATH(self)->angle = angle;
        x = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
        if (x > 0x180) {
            DEATH(self)->orbitAngle = 0x600;
        }
        if (x < 0x80) {
            DEATH(self)->orbitAngle = 0x200;
        }
        break;
    case 8:
        if (!self->step_s) {
            DEATH(self)->timer = 0x40;
            if (GetSideToPlayer() & 1) {
                self->velocityX = FIX(4);
            } else {
                self->velocityX = FIX(-4);
            }
            self->velocityY = FIX(-4);
            if (Random() & 1) {
                PlaySfxPositional(0x7F7);
            } else {
                PlaySfxPositional(0x7F8);
            }
            x = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
            if (x > 0x180) {
                self->velocityX = FIX(-7);
            }
            if (x < 0x80) {
                self->velocityX = FIX(7);
            }
            self->step_s++;
        }
        AnimateEntity(D_us_801806D0, self);
        MoveEntity();
        self->velocityX -= self->velocityX >> 5;
        self->velocityY =
            self->velocityY - (self->velocityY >> 5) + FIX(1.0 / 16);
        if (--DEATH(self)->timer == 0) {
            SetStep(2);
        }
        break;
    case 9:
        switch (self->step_s) {
        case 0:
            prim = DEATH(self)->prim;
            self->hitboxState = 0;
            g_BossMgrState |= 8;
            prim->x0 = 0x90;
            prim->y0 = 0x68;
            PlaySfxPositional(0x7F3);
            self->step_s++;
            // fallthrough
        case 1:
            AnimateEntity(ANIM_DEATH_IDLE, self);
            MoveEntity();
            prim = DEATH(self)->prim;
            dx = prim->x0 - self->posX.i.hi;
            dy = prim->y0 - self->posY.i.hi;
            angle = ratan2(dy, dx);
            self->velocityX = rcos(angle) * 16;
            self->velocityY = rsin(angle) * 16;
            if (SquareRoot0(dx * dx + dy * dy) < 8) {
                self->step_s++;
            }
            break;
        case 2:
            // light beams spreading from Death
            prim = DEATH(self)->prim;
            DEATH(self)->brightness = 0x40;
            DEATH(self)->spread = 0;
            self->opacity = 0x80;
            self->drawFlags |= ENTITY_OPACITY;
            prim->type = PRIM_G4;
            prim->x0 = prim->x2 = self->posX.i.hi;
            prim->y0 = prim->y1 = 0;
            prim->y2 = prim->y3 = 0x100;
            prim->x1 = prim->x3 = self->posX.i.hi;
            prim->priority = 0xC0;
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 |
                             DRAW_TRANSP;
            prim = prim->next;
            prim->type = PRIM_G4;
            prim->x0 = prim->x2 = self->posX.i.hi;
            prim->y0 = prim->y1 = 0;
            prim->y2 = prim->y3 = 0x100;
            prim->x1 = prim->x3 = self->posX.i.hi;
            prim->priority = 0xC0;
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 |
                             DRAW_TRANSP;
            prim = prim->next;
            prim->type = PRIM_G4;
            prim->x0 = prim->x2 = self->posX.i.hi;
            prim->y0 = prim->y1 = 0;
            prim->y2 = prim->y3 = 0x100;
            prim->x1 = prim->x3 = self->posX.i.hi;
            prim->priority = 0xC0;
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 |
                             DRAW_TRANSP;
            DeathSetPrimColors(DEATH(self)->prim, DEATH(self)->brightness);
            DEATH(self)->timer = 0x20;
            PlaySfxPositional(0x636);
            self->step_s++;
            // fallthrough
        case 3:
            prim = DEATH(self)->prim->next;
            prim->x0 = prim->x2 = prim->x2 - 2;
            prim = prim->next;
            prim->x1 = prim->x3 = prim->x3 + 2;
            if (self->posX.i.hi == prim->x3 - 0x30) {
                PlaySfxPositional(0x7F4);
                self->step_s++;
            }
            break;
        case 4:
            count = 0;
            DEATH(self)->spread += 4;
            x = (s16)(self->posX.i.hi - DEATH(self)->spread);
            if (x < 0) {
                count = 1;
                x = 0;
            }
            prim = DEATH(self)->prim;
            prim->x0 = prim->x2 = x;
            prim = prim->next;
            prim->x0 = prim->x2 = x - 0x30;
            prim->x1 = prim->x3 = x;
            x = (s16)(self->posX.i.hi + DEATH(self)->spread);
            if (x > 0x100) {
                count++;
                x = 0x100;
            }
            prim = DEATH(self)->prim;
            prim->x1 = prim->x3 = x;
            prim = prim->next->next;
            prim->x0 = prim->x2 = x;
            prim->x1 = prim->x3 = x + 0x30;
            if (self->opacity) {
                self->opacity -= 8;
            }
            if (DEATH(self)->brightness < 0xF0) {
                DEATH(self)->brightness += 8;
            }
            DeathSetPrimColors(DEATH(self)->prim, DEATH(self)->brightness);
            if (count == 2) {
                self->step_s++;
            }
            break;
        case 5:
            for (prim = DEATH(self)->prim; prim != NULL; prim = prim->next) {
                prim->drawMode = DRAW_HIDE;
            }
            // the second form takes over
            CreateEntityFromEntity(E_UNK_1B, self, &g_Entities[96]);
            self->step_s++;
            break;
        case 6:
            self->animCurFrame = 0;
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
            if (self->step_s == 0) {
                self->animCurFrame--;
                self->step_s |= 1;
            }
        } else {
            self->step_s = 0;
        }
        break;
    }

    y = self->posY.i.hi + g_Tilemap.scrollY.i.hi;
    x = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
    if (self->velocityX < 0) {
        if (x < 0x20) {
            self->posX.i.hi = 0x20 - g_Tilemap.scrollX.i.hi;
        }
    } else if (x > 0x1E0) {
        self->posX.i.hi = 0x1E0 - g_Tilemap.scrollX.i.hi;
    }
    if (self->velocityY < 0) {
        if (y < 0x50) {
            self->posY.i.hi = 0x50 - g_Tilemap.scrollY.i.hi;
        }
    } else if (y > 0xE0) {
        self->posY.i.hi = 0xE0 - g_Tilemap.scrollY.i.hi;
    }
}

// Small sickles raining down during the first form
void func_us_8019C718(Entity* self) {
    s32 x, y;
    s16 angle;

    if (g_BossMgrState & 8) {
        self->flags |= FLAG_DEAD;
    }
    if (self->flags & FLAG_DEAD) {
        if (self->step != 3) {
            PlaySfxPositional(0x60F);
            self->hitboxState = 0;
            SetStep(3);
        }
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804A0);
        self->facingLeft = Random() & 1;
        self->hitboxState = 0;
        // fallthrough
    case 1:
        if (!AnimateEntity(D_us_801806E0, self)) {
            self->hitboxState = 3;
            self->animCurFrame = 0x4F;
            self->drawFlags = ENTITY_ROTATE;
            SetStep(2);
        }
        break;
    case 2:
        if (!self->step_s) {
            x = PLAYER.posX.i.hi - 0x20 + (Random() & 0x3F);
            y = PLAYER.posY.i.hi - 0x20 + (Random() & 0x3F);
            angle = ratan2(y - self->posY.i.hi, x - self->posX.i.hi);
            self->velocityX = rcos(angle) << 4;
            self->velocityY = rsin(angle) << 4;
            PlaySfxPositional(0x689);
            self->step_s++;
        }
        MoveEntity();
        self->rotate += 0xC0;
        break;
    case 3:
        if (!AnimateEntity(D_us_801806F8, self)) {
            DestroyEntity(self);
        }
        break;
    }
}

// The two balls holding Death's scythes during the first form
void func_us_8019C924(Entity* self) {
    Entity* entity;
    Entity* parent;
    s32 x, y, dist;
    s32 ret;
    s16 angle;

    FntPrint("ball_step %x\n", self->step);
    if (g_BossMgrState & 8) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 3;
        }
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804AC);
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        self->scaleX = self->scaleY = 0x80;
        self->hitboxState = 0;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        PlaySfxPositional(0x6DE);
        // fallthrough
    case 1:
        if (!AnimateEntity(D_us_80180708, self)) {
            self->step++;
        }
        // fallthrough
    case 2:
        self->scaleX += 4;
        if (self->scaleX > 0x100) {
            self->hitboxState = 1;
            self->drawFlags = ENTITY_DEFAULT;
            SetStep(3);
        }
        self->scaleY = self->scaleX;
        return;
    case 3:
        AnimateEntity(D_us_80180718, self);
        if (DEATH(DEATH(self)->parent)->phase2) {
            DEATH(self)->angle = 0x400;
            SetStep(4);
        }
        return;
    case 4:
        AnimateEntity(D_us_80180718, self);
        MoveEntity();
        x = PLAYER.posX.i.hi - 0x20 + (Random() & 0x3F);
        y = PLAYER.posY.i.hi - 0x20 + (Random() & 0x3F);
        angle = LimitAngleChange(
            0x40, DEATH(self)->angle,
            ratan2(y - self->posY.i.hi, x - self->posX.i.hi));
        self->velocityX = (rcos(angle) * 0x7000) >> 12;
        self->velocityY = (rsin(angle) * 0x7000) >> 12;
        DEATH(self)->angle = angle;
        if (DEATH(DEATH(self)->parent)->recallBalls) {
            SetStep(5);
        }
        return;
    case 5:
        AnimateEntity(D_us_80180718, self);
        MoveEntity();
        parent = DEATH(self)->parent;
        if (self->params) {
            x = parent->posX.i.hi + 0x20;
        } else {
            x = parent->posX.i.hi - 0x20;
        }
        y = (parent->posY.i.hi - 0x1C) - self->posY.i.hi;
        x -= self->posX.i.hi;
        angle = ratan2(y, x);
        self->velocityX = (rcos(angle) * 0x1C000) >> 12;
        self->velocityY = (rsin(angle) * 0x1C000) >> 12;
        if (SquareRoot0(x * x + y * y) < 4) {
            DEATH(self)->ready = 1;
            self->step++;
        }
        break;
    case 6:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            AnimateEntity(D_us_80180718, self);
            if (DEATH(DEATH(self)->parent)->ballsOpen) {
                DEATH(self)->ready = 0;
                SetSubStep(1);
            }
            break;
        case 1:
            if (!AnimateEntity(D_us_8018072C, self)) {
                SetStep(7);
            }
            break;
        }
        break;
    case 7:
        switch (self->step_s) {
        case 0:
            entity = AllocEntity(&g_Entities[176], &g_Entities[184]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_19, self, entity);
                DEATH(entity)->parent = self;
                if (self->params) {
                    DEATH(entity)->other = self - 1;
                } else {
                    DEATH(entity)->other = self + 1;
                }
                entity->facingLeft = self->params;
                entity->zPriority = self->zPriority + 1;
                DEATH(self)->unk8C = 0;
                PlaySfxPositional(0x6E6);
                self->step_s++;
            }
            break;
        case 1:
            DEATH(self)->timer = 0x60;
            self->step_s++;
            break;
        case 2:
            if (--DEATH(self)->timer == 0) {
                self->step_s = 0;
            }
            break;
        }
        if (g_Timer & 1) {
            self->animCurFrame = 0x39;
        } else {
            self->animCurFrame = 0;
        }
        MoveEntity();
        parent = DEATH(self)->parent;
        if (self->params) {
            x = parent->posX.i.hi + 0x50;
        } else {
            x = parent->posX.i.hi - 0x50;
        }
        x -= self->posX.i.hi;
        y = (parent->posY.i.hi - 0x1C) - self->posY.i.hi;
        angle = ratan2(y, x);
        self->velocityX = (rcos(angle) * 0xE000) >> 12;
        self->velocityY = (rsin(angle) * 0xE000) >> 12;
        if (SquareRoot0(x * x + y * y) < 3) {
            self->velocityX = 0;
            self->velocityY = 0;
        }
        if (g_BossMgrState & 4) {
            SetStep(8);
        }
        break;
    case 8:
        switch (self->step_s) {
        case 0:
            ret = AnimateEntity(D_us_80180744, self);
            MoveEntity();
            parent = DEATH(self)->parent;
            if (self->params) {
                x = parent->posX.i.hi + 0x20;
            } else {
                x = parent->posX.i.hi - 0x20;
            }
            y = (parent->posY.i.hi - 0x1C) - self->posY.i.hi;
            x -= self->posX.i.hi;
            angle = ratan2(y, x);
            self->velocityX = (rcos(angle) * 0x1C000) >> 12;
            self->velocityY = (rsin(angle) * 0x1C000) >> 12;
            if (SquareRoot0(x * x + y * y) < 4 && !ret) {
                DEATH(self)->ready = 1;
                SetSubStep(1);
            }
            break;
        case 1:
            AnimateEntity(D_us_80180718, self);
            if (DEATH(DEATH(self)->parent)->unk8C) {
                self->facingLeft = self->params;
                SetSubStep(2);
            }
            break;
        case 2:
            if (!AnimateEntity(D_us_8018075C, self)) {
                self->attackElement =
                    g_api.enemyDefs[ENEMY_DEATH_UNK_168].attackElement;
                self->attack = g_api.enemyDefs[ENEMY_DEATH_UNK_168].attack;
                self->hitboxWidth =
                    g_api.enemyDefs[ENEMY_DEATH_UNK_168].hitboxWidth;
                self->hitboxState = 1;
                self->blendMode = BLEND_NO;
                self->drawFlags = ENTITY_ROTATE;
                self->hitboxHeight =
                    g_api.enemyDefs[ENEMY_DEATH_UNK_168].hitboxHeight;
                SetStep(9);
            }
            break;
        }
        break;
    case 9:
        switch (self->step_s) {
        case 0:
            DEATH(self)->ready = 1;
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            func_801CDC80(&self->rotate, 0, 0x40);
            parent = DEATH(self)->parent;
            if (self->params) {
                x = parent->posX.i.hi + 0x20;
            } else {
                x = parent->posX.i.hi - 0x20;
            }
            x -= self->posX.i.hi;
            y = (parent->posY.i.hi - 0x10) - self->posY.i.hi;
            angle = ratan2(y, x);
            self->velocityX = (rcos(angle) * 0xE000) >> 12;
            self->velocityY = (rsin(angle) * 0xE000) >> 12;
            if (SquareRoot0(x * x + y * y) < 3) {
                self->velocityX = 0;
                self->velocityY = 0;
            }
            if (DEATH(DEATH(self)->parent)->ballsThrow) {
                DEATH(self)->ready = 0;
                self->step_s++;
            }
            break;
        case 2:
            DEATH(self)->angle =
                ratan2(PLAYER.posY.i.hi - self->posY.i.hi,
                       PLAYER.posX.i.hi - self->posX.i.hi);
            DEATH(self)->speed = FIX(6);
            self->step_s++;
            // fallthrough
        case 3:
            MoveEntity();
            self->rotate -= 0x100;
            if (!(self->rotate & 0xF00)) {
                PlaySfxPositional(0x7FA);
            }
            angle = DEATH(self)->angle;
            self->velocityX = (rcos(angle) * DEATH(self)->speed) >> 12;
            self->velocityY = (rsin(angle) * DEATH(self)->speed) >> 12;
            DEATH(self)->speed -= DEATH(self)->speed >> 4;
            if (DEATH(self)->speed < 0x4000) {
                DEATH(self)->timer = 0x20;
                self->step_s++;
            }
            if (!(g_Timer & 3)) {
                entity = AllocEntity(&g_Entities[208], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_1A, self, entity);
                    entity->params = self->animCurFrame;
                    entity->facingLeft = self->facingLeft;
                    entity->rotate = self->rotate;
                }
            }
            break;
        case 4:
            x = (u16)self->rotate - 0x200;
            self->rotate = x & 0xFFF;
            if (!(x & 0xF00)) {
                PlaySfxPositional(0x7FA);
            }
            if (--DEATH(self)->timer == 0) {
                self->step_s++;
            }
            break;
        case 5:
            MoveEntity();
            func_801CDC80(&self->rotate, 0, 0x40);
            parent = DEATH(self)->parent;
            if (self->params) {
                x = parent->posX.i.hi + 0x20;
            } else {
                x = parent->posX.i.hi - 0x20;
            }
            x -= self->posX.i.hi;
            y = (parent->posY.i.hi - 0x10) - self->posY.i.hi;
            angle = ratan2(y, x);
            dist = SquareRoot0(x * x + y * y);
            if (dist > 0x20) {
                angle = LimitAngleChange(0x60, DEATH(self)->angle, angle);
            }
            self->velocityX = (rcos(angle) * 0x30000) >> 12;
            self->velocityY = (rsin(angle) * 0x30000) >> 12;
            DEATH(self)->angle = angle;
            if (dist < 8) {
                self->step_s = 0;
            }
            break;
        }
        break;
    }
}

// Skull flying from one ball to the other
void func_us_8019D4CC(Entity* self) {
    Entity* entity;
    Entity* other;
    s32 x, y, dist;
    s16 angle;

    FntPrint("skull_step %x\n", self->step);
    if (g_BossMgrState & 8) {
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
        InitializeEntity(D_us_801804B8);
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->scaleX = self->scaleY = 0;
        // fallthrough
    case 1:
        other = DEATH(self)->parent;
        self->posX.i.hi = other->posX.i.hi;
        self->posY.i.hi = other->posY.i.hi;
        AnimateEntity(D_us_80180770, self);
        self->scaleX += 0xC;
        if (self->scaleX > 0x100) {
            self->hitboxState = 1;
            DEATH(self)->timer = 0x80;
            self->drawFlags = ENTITY_DEFAULT;
            DEATH(self)->angle = 0x400;
            self->step++;
        }
        self->scaleY = self->scaleX;
        if (g_BossMgrState & 4) {
            DestroyEntity(self);
        }
        return;
    case 2:
        AnimateEntity(D_us_80180770, self);
        MoveEntity();
        angle = LimitAngleChange(0x20, DEATH(self)->angle,
                                 ratan2(PLAYER.posY.i.hi - self->posY.i.hi,
                                        PLAYER.posX.i.hi - self->posX.i.hi));
        self->velocityX = (rcos(angle) * 0x30000) >> 12;
        self->velocityY = (rsin(angle) * 0x30000) >> 12;
        DEATH(self)->angle = angle;
        DEATH(self)->timer--;
        x = PLAYER.posX.i.hi - self->posX.i.hi;
        if (self->facingLeft) {
            x = -x;
        }
        if (!DEATH(self)->timer || x < 0 || (self->hitFlags & 0x80)) {
            self->step++;
        }
        break;
    case 3:
        AnimateEntity(D_us_80180770, self);
        MoveEntity();
        other = DEATH(self)->other;
        x = other->posX.i.hi - self->posX.i.hi;
        y = other->posY.i.hi - self->posY.i.hi;
        angle = ratan2(y, x);
        dist = SquareRoot0(x * x + y * y);
        if (dist > 0x20) {
            angle = LimitAngleChange(0x60, DEATH(self)->angle, angle);
        }
        self->velocityX = (rcos(angle) * 0x30000) >> 12;
        self->velocityY = (rsin(angle) * 0x30000) >> 12;
        DEATH(self)->angle = angle;
        if (dist < 4) {
            self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
            self->scaleX = self->scaleY = 0x100;
            self->hitboxState = 0;
            self->step++;
        }
        break;
    case 4:
        AnimateEntity(D_us_80180770, self);
        other = DEATH(self)->other;
        self->posX.i.hi = other->posX.i.hi;
        self->scaleX -= 0x10;
        self->scaleY = self->scaleX;
        self->posY.i.hi = other->posY.i.hi;
        if (self->scaleX == 0) {
            DEATH(other)->unk8C = 1;
            DestroyEntity(self);
            return;
        }
        if (g_BossMgrState & 4) {
            DestroyEntity(self);
        }
        return;
    case 5:
        self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
        MoveEntity();
        return;
    default:
        return;
    }

    // steps 2 and 3 leave a trail behind
    if (!(g_Timer & 3)) {
        entity = AllocEntity(&g_Entities[208], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_UNK_1A, self, entity);
            entity->params = self->animCurFrame;
            entity->facingLeft = self->facingLeft;
        }
    }
    if (g_BossMgrState & 4) {
        SetStep(5);
    }
}

// Afterimage of the balls and the skulls
void func_us_8019D950(Entity* self) {
    if (g_BossMgrState & 8) {
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        InitializeEntity(D_us_801804C4);
        self->drawFlags = ENTITY_OPACITY;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->opacity = 0x60;
        self->animCurFrame = self->params;
        if (self->rotate) {
            self->drawFlags |= ENTITY_ROTATE;
        }
    }
    self->opacity -= 8;
    if (!self->opacity) {
        DestroyEntity(self);
    }
}

static void func_801CE1E8(s32 step);
static void polarPlacePartsList(s16* offsets);
static void polarPlacePartsWithAngvel(s16* offsets);
static void func_801CDF1C(s16* entOffsets, unkStr_801CDD80* arg1, s32 arg2);

static void DeathTurnOrApproach(Entity* self) {
    s32 facing = (GetSideToPlayer() & 1) ^ 1;

    if (facing == self->facingLeft && GetDistanceToPlayerX() > 0x50) {
        func_801CE1E8(4);
    } else {
        func_801CE1E8(3);
    }
}

// Death, second form (the body; the limbs are func_us_8019E558)
void func_us_8019DA04(Entity* self) {
    DeathPartDef* def;
    Entity* part;
    Entity* prev;
    Entity* entity;
    s16* offsets;
    s32 x, y;
    s32 i;
    s32 facing;
    s16 angle;

    if (self->flags & FLAG_DEAD) {
        if (self->step != 6) {
            func_801CE1E8(6);
        }
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitDeath2);
        prev = self;
        self->animCurFrame = 2;
        self->hitboxHeight = 0xC;
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        for (def = D_us_8018078C, i = 0; def->index; def++, i++) {
            part = self + def->index;
            CreateEntityFromCurrentEntity(E_UNK_1C, part);
            PART(part)->length = def->length;
            PART(part)->parent = self + def->parent;
            part->params = i;
            PART(part)->angleOffset = def->angle;
            part->nextPart = prev;
            prev = part;
            part->parent = self;
            part->zPriority = self->zPriority + def->zOffset;
        }
        self->parent = NULL;
        self->nextPart = prev;
        polarPlacePartsList(D_us_8018081C);
        PART(self)->orbitAngle = 0x200;
        func_801CE1E8(1);
        // fallthrough
    case 1:
        func_801CDF1C(D_us_80180844, D_us_801808A4, 0);
        polarPlacePartsWithAngvel(D_us_80180844);
        if (!PART(self)->unkB4[0] && !PART(self)->unkB0[0]) {
            func_801CE1E8(2);
        }
        break;
    case 2:
        if (!self->step_s) {
            PART(self)->timer = ((Random() & 3) << 5) + 0x40;
            self->step_s++;
        }
        MoveEntity();
        facing = (GetSideToPlayer() & 1) ^ 1;
        if (self->facingLeft != facing) {
            if (self->facingLeft) {
                self->posX.i.hi += 0xC;
            } else {
                self->posX.i.hi -= 0xC;
            }
            self->facingLeft = facing;
        }
        angle = PART(self)->orbitAngle;
        x = PLAYER.posX.i.hi + ((rcos(angle) * 0x60) >> 12);
        y = PLAYER.posY.i.hi - ((rsin(angle) * 0x60) >> 12);
        angle = func_us_8019A98C_from_rcen(
            0x30, PART(self)->angle,
            ratan2(y - self->posY.i.hi, x - self->posX.i.hi));
        self->velocityX = rcos(angle) << 4;
        self->velocityY = rsin(angle) << 4;
        PART(self)->angle = angle;
        x = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
        if (x > 0x180) {
            PART(self)->orbitAngle = 0x600;
        }
        if (x < 0x80) {
            PART(self)->orbitAngle = 0x200;
        }
        func_801CDF1C(D_us_80180834, D_us_801808E4, 0);
        polarPlacePartsWithAngvel(D_us_80180834);
        func_801CDF1C(D_us_80180A9C, D_us_80180AB8, 1);
        polarPlacePartsWithAngvel(D_us_80180A9C);
        polarPlacePartsList(D_us_8018081C);
        if (!PART(self)->timer) {
            DeathTurnOrApproach(self);
        } else {
            PART(self)->timer--;
            if (self->hitFlags & 3) {
                func_801CE1E8(5);
            }
        }
        break;
    case 3:
        if (!self->step_s) {
            self->velocityX = 0;
            self->velocityY = 0;
            self->step_s++;
        }
        MoveEntity();
        self->velocityY -= self->velocityY / 16;
        func_801CDF1C(D_us_80180834, D_us_8018095C, 0);
        polarPlacePartsWithAngvel(D_us_80180834);
        func_801CDF1C(D_us_80180A9C, D_us_80180AB8, 1);
        polarPlacePartsWithAngvel(D_us_80180A9C);
        polarPlacePartsList(D_us_8018081C);
        if ((u32)(PART(self)->unkB0[0] - 2) < 3) {
            D_us_80180788 = 1;
        } else {
            D_us_80180788 = 0;
        }
        if (!PART(self)->unkB4[0]) {
            if (PART(self)->unkB0[0] == 2) {
                self->velocityY = FIX(4);
                PlaySfxPositional(0x66C);
            }
            if (!PART(self)->unkB4[0] && !PART(self)->unkB0[0]) {
                func_801CE1E8(2);
            }
        }
        break;
    case 4:
        switch (self->step_s) {
        case 0:
            func_801CDF1C(D_us_80180834, D_us_801809E4, 0);
            polarPlacePartsWithAngvel(D_us_80180834);
            if (!PART(self)->unkB4[0]) {
                if (!PART(self)->unkB0[0]) {
                    self->step_s++;
                }
                if (!PART(self)->unkB4[0] && PART(self)->unkB0[0] == 3) {
                    D_us_80180784 = 1;
                }
            }
            break;
        case 1:
            if (!D_us_80180784) {
                func_801CE1E8(2);
            }
            break;
        }
        func_801CDF1C(D_us_80180A9C, D_us_80180AB8, 1);
        polarPlacePartsWithAngvel(D_us_80180A9C);
        polarPlacePartsList(D_us_8018081C);
        break;
    case 5:
        if (!self->step_s) {
            if (GetSideToPlayer() & 1) {
                self->velocityX = FIX(2);
            } else {
                self->velocityX = FIX(-2);
            }
            self->velocityY = 0;
            self->step_s++;
        }
        MoveEntity();
        self->velocityX -= self->velocityX >> 4;
        func_801CDF1C(D_us_80180834, D_us_80180A34, 0);
        polarPlacePartsWithAngvel(D_us_80180834);
        polarPlacePartsList(D_us_8018081C);
        if (!PART(self)->unkB4[0] && !PART(self)->unkB0[0]) {
            func_801CE1E8(2);
            DeathTurnOrApproach(self);
        }
        break;
    case 6:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            g_BossMgrState |= BOSS_MGR_DEATH_DEFEAT;
            if (GetSideToPlayer() & 1) {
                self->velocityX = FIX(2);
            } else {
                self->velocityX = FIX(-2);
            }
            self->velocityY = 0;
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            self->velocityX -= self->velocityX >> 4;
            func_801CDF1C(D_us_80180834, D_us_80180A34, 0);
            polarPlacePartsWithAngvel(D_us_80180834);
            polarPlacePartsList(D_us_8018081C);
            if (!PART(self)->unkB4[0] && !PART(self)->unkB0[0]) {
                self->step_s++;
            }
            break;
        case 2:
            func_801CDF1C(D_us_80180844, D_us_80180A84, 0);
            polarPlacePartsWithAngvel(D_us_80180844);
            if (!PART(self)->unkB4[0] && !PART(self)->unkB0[0]) {
                PART(self)->timer = 0x40;
                self->velocityX = 0;
                g_BossMgrState |= 0x20;
                self->step_s++;
            }
            break;
        case 3:
            if (--PART(self)->timer == 0) {
                PART(self)->timer = 0x80;
                self->step_s++;
                g_api.PlaySfx(0x7C5);
            }
            break;
        case 4:
            entity = AllocEntity(&g_Entities[64], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1F, self, entity);
                entity->params = 1;
                entity->posY.i.hi = 0xF0;
                entity->posX.i.hi = entity->posX.i.hi - 0x10 + (rand() & 0x1F);
                entity->zPriority =
                    self->zPriority - ((PART(self)->timer >> 4) - 8);
            }
            if (--PART(self)->timer == 0) {
                self->animCurFrame = 0;
                self->hitboxState = 0;
                g_BossMgrState |= BOSS_MGR_DEATH_REWARD_READY;
                self->step_s++;
            }
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
            if (self->step_s == 0) {
                self->animCurFrame--;
                self->step_s |= 1;
            }
        } else {
            self->step_s = 0;
        }
        break;
    }

    for (offsets = D_us_8018081C; *offsets; offsets++) {
        part = self + *offsets;
        PART(part)->unkA8 = 0;
        part->facingLeft = self->facingLeft;
    }
    y = self->posY.i.hi + g_Tilemap.scrollY.i.hi;
    x = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
    if (self->velocityX < 0) {
        if (x < 0x20) {
            self->posX.i.hi = 0x20 - g_Tilemap.scrollX.i.hi;
        }
    } else if (x > 0x1E0) {
        self->posX.i.hi = 0x1E0 - g_Tilemap.scrollX.i.hi;
    }
    if (self->velocityY < 0) {
        if (y < 0x30) {
            self->posY.i.hi = 0x30 - g_Tilemap.scrollY.i.hi;
        }
    } else if (y > 0xB0) {
        self->posY.i.hi = 0xB0 - g_Tilemap.scrollY.i.hi;
    }
}

// Death's second form body parts
void func_us_8019E558(Entity* self) {
    Entity* entity;
    s16 angle;

    switch (self->step) {
    case 0:
        if (self->params == 3 || self->params == 6) {
            InitializeEntity(D_us_801804E8);
        } else {
            InitializeEntity(g_EInitDeath2);
        }
        self->drawFlags |= ENTITY_ROTATE;
        self->animCurFrame = D_us_8018078C[self->params].frame;
        // fallthrough
    case 1:
        self->rotate = PART(self)->rotate;
        if (g_BossMgrState & BOSS_MGR_DEATH_DEFEAT) {
            self->step = 2;
        }
        break;
    case 2:
        self->rotate = PART(self)->rotate;
        if (!(Random() & 0x3F)) {
            PlaySfxPositional(0x654);
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1F, self, entity);
                entity->params = 0;
                entity->zPriority = self->zPriority + 1;
            }
        }
        if (g_BossMgrState & BOSS_MGR_DEATH_REWARD_READY) {
            self->animCurFrame = 0;
            self->step = 3;
        }
        break;
    case 3:
        self->animCurFrame = 0;
        self->hitboxState = 0;
        return;
    }

    switch (self->params) {
    default:
        self->hitboxState = 0;
        return;
    case 3:
        // the arm holding the scythe
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 10;
            if (D_us_80180784) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_1D, self, entity);
                    SCYTHE(entity)->parent = self;
                    entity->facingLeft = self->facingLeft;
                    PART(self)->scytheBack = 0;
                    self->step_s++;
                } else {
                    D_us_80180784 = 0;
                }
            }
            break;
        case 1:
            self->animCurFrame = 9;
            if (PART(self)->scytheBack) {
                D_us_80180784 = 0;
                self->step_s = 0;
            }
            break;
        }
        if ((g_BossMgrState & 0x20) && self->animCurFrame == 10) {
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1D, self, entity);
                entity->facingLeft = self->facingLeft;
                entity->rotate = self->rotate;
                entity->zPriority = self->zPriority;
                self->animCurFrame = 9;
                self->step_s = 8;
            }
        }
        // fallthrough
    case 6:
        angle = self->rotate;
        self->hitboxOffX = -(rsin(angle) * 12) >> 12;
        self->hitboxOffY = (rcos(angle) * 3) >> 10;
        if (D_us_80180788) {
            self->hitboxState = 1;
        } else {
            self->hitboxState = 0;
        }
        if ((g_BossMgrState & 0x20) && self->animCurFrame == 0xE) {
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1D, self, entity);
                entity->facingLeft = self->facingLeft;
                entity->rotate = self->rotate;
                entity->zPriority = self->zPriority;
                self->animCurFrame = 0xD;
            }
        }
        return;
    }
}

// The scythe thrown by the second form
void func_us_8019E920(Entity* self) {
    Entity* entity;
    Entity* parent;
    s32 x, y, dist;
    s16 angle;

    if (g_BossMgrState & BOSS_MGR_DEATH_DEFEAT) {
        if (self->step != 3) {
            self->hitboxState = 0;
            if (self->step) {
                SetStep(3);
            }
        }
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804E8);
        self->animCurFrame = 0xF;
        self->drawFlags = ENTITY_ROTATE;
        if (self->facingLeft) {
            SCYTHE(self)->angle = 0;
        } else {
            SCYTHE(self)->angle = 0x800;
        }
        SCYTHE(self)->speed = FIX(8);
        // fallthrough
    case 1:
        MoveEntity();
        self->rotate -= 0x100;
        if (!(g_Timer & 7)) {
            PlaySfxPositional(0x66E);
        }
        // the angle is limited against its initial value on purpose
        angle = LimitAngleChange(0x10, SCYTHE(self)->angle,
                                 ratan2(PLAYER.posY.i.hi - self->posY.i.hi,
                                        PLAYER.posX.i.hi - self->posX.i.hi));
        self->velocityX = (rcos(angle) * SCYTHE(self)->speed) >> 12;
        self->velocityY = (rsin(angle) * SCYTHE(self)->speed) >> 12;
        if (!(g_Timer & 3)) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1E, self, entity);
                entity->params = self->animCurFrame;
                entity->facingLeft = self->facingLeft;
                entity->rotate = self->rotate;
            }
        }
        SCYTHE(self)->speed -= SCYTHE(self)->speed >> 4;
        if (SCYTHE(self)->speed < 0x4000) {
            SCYTHE(self)->timer = 0x20;
            self->step++;
        }
        break;
    case 2:
        MoveEntity();
        self->rotate -= 0x100;
        if (!(g_Timer & 7)) {
            PlaySfxPositional(0x66E);
        }
        parent = SCYTHE(self)->parent;
        x = parent->posX.i.hi - self->posX.i.hi;
        y = parent->posY.i.hi - self->posY.i.hi;
        angle = ratan2(y, x);
        dist = SquareRoot0(x * x + y * y);
        angle = LimitAngleChange(
            dist > 0x20 ? 0x60 : 0x30, SCYTHE(self)->angle, angle);
        self->velocityX = rcos(angle) << 6;
        self->velocityY = rsin(angle) << 6;
        SCYTHE(self)->angle = angle;
        if (dist < 8) {
            PlaySfxPositional(0x676);
            PART(parent)->scytheBack = 1;
            DestroyEntity(self);
        }
        break;
    case 3:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            self->velocityX = 0;
            self->velocityY = 0;
            self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            self->velocityY += FIX(0.25);
            self->rotate -= 0x80;
            break;
        }
        break;
    }
}

// Afterimage of the thrown scythe
void func_us_8019ECCC(Entity* self) {
    if (g_BossMgrState & BOSS_MGR_DEATH_DEFEAT) {
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        InitializeEntity(D_us_801804DC);
        self->drawFlags = ENTITY_OPACITY;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->opacity = 0x60;
        self->animCurFrame = self->params;
        if (self->rotate) {
            self->drawFlags |= ENTITY_ROTATE;
        }
    }
    self->opacity -= 8;
    if (!self->opacity) {
        DestroyEntity(self);
    }
}

// Based on the body part's angle and length, update its X and Y positions
// using the parent's position with a polar coordinate offset.
static void func_us_8019ED80(Entity* self) {
    s16 angle;
    Entity* src;

    angle = PART(self)->rotate + PART(self)->angleOffset;
    if (g_CurrentEntity->facingLeft) {
        angle = -angle;
    }
    src = PART(self)->parent;
    self->posX.val = src->posX.val;
    self->posY.val = src->posY.val;
    self->posX.val -= PART(self)->length * rsin(angle) * 16;
    self->posY.val += PART(self)->length * rcos(angle) * 16;
    PART(self)->unkA8 |= 1;
}

static void polarPlacePartsWithAngvel(s16* offsets) {
    Entity* entity;

    while (*offsets) {
        if (*offsets != 0xFF) {
            entity = g_CurrentEntity + *offsets;
            PART(entity)->rotate += PART(entity)->rotVel;
            func_us_8019ED80(entity);
        }
        offsets++;
    }
}

static void func_801CDD00(Entity* entity, s16 target, s16 steps) {
    s16 diff = target - PART(entity)->rotate;

    if (diff > 0x800) {
        diff -= 0x1000;
    }
    if (diff < -0x800) {
        diff += 0x1000;
    }
    PART(entity)->rotVel = diff / steps;
    PART(entity)->unkA4 = target;
}

static void func_801CDD80(s16* offsets, unkStr_801CDD80* arg1) {
    s16* angles = arg1->unk4;

    while (*offsets) {
        if (*offsets != 0xFF) {
            func_801CDD00(g_CurrentEntity + *offsets, *angles, arg1->unk0);
        }
        angles++;
        offsets++;
    }
}

static void func_801CDF1C(s16* entOffsets, unkStr_801CDD80* arg1, s32 arg2) {
    ET_RDeathPart* ext = PART(g_CurrentEntity);

    arg1 += ext->unkB0[arg2];
    if (!ext->unkB4[arg2]) {
        func_801CDD80(entOffsets, arg1);
        ext->unkB4[arg2] = arg1->unk0;
    }
    if (!--ext->unkB4[arg2]) {
        arg1++;
        if (!arg1->unk0) {
            ext->unkB0[arg2] = 0;
        } else {
            ext->unkB0[arg2]++;
        }
    }
}

static void func_801CE1E8(s32 step) {
    ET_RDeathPart* ext = PART(g_CurrentEntity);

    g_CurrentEntity->step = step;
    g_CurrentEntity->step_s = 0;
    g_CurrentEntity->pose = 0;
    g_CurrentEntity->poseTimer = 0;
    // BUG: the original loop clears 4 entries of both 2-entry arrays, which
    // also zeroes the following Entity::unkB8 field
    ext->unkB0[0] = ext->unkB0[1] = 0;
    ext->unkB4[0] = ext->unkB4[1] = 0;
    g_CurrentEntity->unkB8 = NULL;
}

static UNUSED void func_801CE228(void) {
    ET_RDeathPart* ext = PART(g_CurrentEntity);

    ext->unkB0[0] = ext->unkB0[1] = 0;
    ext->unkB4[0] = ext->unkB4[1] = 0;
    g_CurrentEntity->unkB8 = NULL;
}

static void polarPlacePartsList(s16* offsets) {
    Entity* entity;

    while (*offsets) {
        entity = g_CurrentEntity + *offsets;
        if (!PART(entity)->unkA8) {
            func_us_8019ED80(entity);
        }
        offsets++;
    }
}

static UNUSED void func_us_8019D260_from_rcen(void) {
    s16 angle;
    s32 i;
    s8 type;
    Entity* entity;

    type = Random() & 3;
    angle = ((Random() & 0xF) << 8) - 0x800;
    for (i = 0; i < 6; i++) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_UNK_1F, g_CurrentEntity, entity);
            entity->ext.et_801A518C.unk89 = 6 - i;
            entity->ext.et_801A518C.unk88 = type;
            entity->params = 2;
            entity->ext.et_801A518C.unk84 = angle;
            entity->zPriority = g_CurrentEntity->zPriority + 1;
        }
    }
}

// Flames burning the second form away once defeated
void func_us_8019F260(Entity* self) {
    s32 index;
    EInitUnk22* init;
    Entity* entity;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        index = self->params & 0xF;
        init = &D_us_80180B44[index];
        self->palette = init->palette + 0x2E0;
        self->blendMode = init->blendMode;
        self->animSet = init->animSet;
        self->unk5A = init->unk5A;
        self->step = index + 1;
        self->ext.et_801A518C.anim = init->anim;
        if (self->params & 0xFF00) {
            self->zPriority = (self->params & 0xFF00) >> 8;
        }
        if (self->params & 0xF0) {
            self->palette = PAL_FLAG(0x19F);
            self->blendMode = BLEND_TRANSP;
            self->facingLeft = 1;
        }
        break;
    case 1:
        if (!self->step_s) {
            self->velocityY = FIX(-0.25);
            self->step_s++;
        }
        MoveEntity();
        if (!AnimateEntity(self->ext.et_801A518C.anim, self)) {
            DestroyEntity(self);
        }
        break;
    case 2:
        if (!self->step_s) {
            self->drawFlags = ENTITY_OPACITY;
            self->opacity = 0x60;
            self->facingLeft = Random() & 1;
            self->velocityX = (Random() << 8) - FIX(0.5);
            self->velocityY = FIX(-2.5);
            self->ext.et_801A518C.acceleration = -(Random() << 4) - 0x1000;
            self->step_s++;
        }
        MoveEntity();
        self->opacity--;
        self->velocityY += self->ext.et_801A518C.acceleration;
        // the original reads pose and poseTimer as a single word
        if (self->pose == 8 && self->poseTimer == 0) {
            entity = AllocEntity(&g_Entities[64], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_1F, self, entity);
                entity->params = 1;
                entity->zPriority = self->zPriority - 1;
            }
        }
        if (!AnimateEntity(self->ext.et_801A518C.anim, self)) {
            DestroyEntity(self);
        }
        break;
    }
}
