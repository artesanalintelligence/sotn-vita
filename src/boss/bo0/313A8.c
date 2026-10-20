// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B13A8);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1590);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B15BC);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B163C);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B171C);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B17BC);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1864);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1950);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B19FC);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1B30);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1C60);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1CE0);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1DDC);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1E5C);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1EDC);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B1F5C);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B2044);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B20F4);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B2178);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B21F0);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B24CC);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B2690);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B30AC);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B365C);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B5470);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B551C);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B5D6C);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B5E08);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B5E8C);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B619C);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B6520);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B65C0);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B6CA4);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B76E4);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B7BAC);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B7C44);

INCLUDE_ASM("boss/bo0/nonmatchings/313A8", func_us_801B7CC8);
#else
// Olrox's true form (second phase). Non-matching decompilation used by the
// PC port; the PSX build still uses the original assembly above.
#include <scratchpad.h>

// Each leg keeps its state inside a hidden primitive (a common SotN trick).
// The fields below mirror the PSX byte offsets of the Primitive struct. The
// link to the other leg is a pointer and does not fit in the 4 bytes at 0x18
// on 64-bit hosts, so it is kept in the unused Primitive::dummy slot instead.
typedef struct OlroxLeg {
    struct Primitive* next;
    struct OlroxLeg* other; // Primitive::dummy
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 type;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s16 hipAngle;
    /* 0x0E */ s16 hipDelta;
    /* 0x10 */ s16 kneeAngle;
    /* 0x12 */ s16 kneeDelta;
    /* 0x14 */ s32 footDist;
    /* 0x18 */ u32 unk18; // holds `other` on PSX
    /* 0x1C */ f32 footX;
    /* 0x20 */ f32 footY;
    /* 0x24 */ u8 state;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u16 onGround;
    /* 0x28 */ f32 kneeX;
    /* 0x2C */ f32 kneeY;
    /* 0x30 */ u8 u3;
    /* 0x31 */ u8 v3;
    /* 0x32 */ u16 drawMode;
} OlroxLeg;
STATIC_ASSERT(sizeof(OlroxLeg) == sizeof(Primitive), "leg must fit a prim");
STATIC_ASSERT(offsetof(OlroxLeg, drawMode) == offsetof(Primitive, drawMode),
              "leg drawMode must match the prim drawMode");
STATIC_ASSERT(offsetof(OlroxLeg, footX) == offsetof(Primitive, r2),
              "leg layout must follow the prim layout");

// main body (E_UNK_33) and the entities following the legs (E_UNK_34/3B)
typedef struct {
    /* 0x7C */ Primitive* prim;
    /* 0x80 */ OlroxLeg* leg1;
    /* 0x84 */ OlroxLeg* leg2;
    /* 0x88 */ u8 curLeg;
    /* 0x89 */ u8 neckPose;
    /* 0x8A */ u8 stepCount;
    /* 0x8B */ u8 unk8B;
    /* 0x8C */ OlroxLeg* leg;
    /* 0x90 */ s16 angle;
    /* 0x92 */ s16 angleVel;
    /* 0x94 */ s16 timer;
    /* 0x96 */ s16 unk96;
    /* 0x98 */ u8 nextAttack;
    /* 0x99 */ u8 stomping;
    /* 0x9A */ u8 unk9A;
    /* 0x9B */ u8 dying;
    /* 0x9C */ Primitive* lastPrim;
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ struct Entity* unkA4;
    /* 0xA8 */ u8 laserIdx;
    /* 0xA9 */ u8 laserBusy;
    /* 0xAA */ u8 laserMax;
    /* 0xAB */ u8 unkAB;
    /* 0xAC */ u8 fireBusy;
    /* 0xAD */ u8 fireIdx;
} OlroxTF;

// head (E_UNK_35); fields 0xA8-0xAD are shared with the laser entities and
// are accessed through ext.olroxLaser instead
typedef struct {
    /* 0x7C */ Primitive* prim;
    /* 0x80 */ s16 target;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ u8 dir;
    /* 0x85 */ u8 unk85[3];
    /* 0x88 */ s16 timer;
    /* 0x8A */ s16 unk8A[5];
    /* 0x94 */ s16 duration;
} OlroxTFHead;

// tail (E_UNK_36)
typedef struct {
    /* 0x7C */ Primitive* prim;
    /* 0x80 */ u8 pose;
    /* 0x81 */ u8 unk81[3];
    /* 0x84 */ s16 scale;
    /* 0x86 */ s16 unk86;
    /* 0x88 */ s16 joints[9][2];
} OlroxTFTail;

// arms (E_UNK_37)
typedef struct {
    /* 0x7C */ Primitive* prim;
    /* 0x80 */ s16 joints[4][2];
    /* 0x90 */ u8 handFrame[2];
    /* 0x92 */ s8 armLen[2];
    /* 0x94 */ u8 rotateHands;
    /* 0x95 */ u8 unk95[5];
    /* 0x9A */ u8 handsHurt;
} OlroxTFArms;

// hands (E_UNK_38)
typedef struct {
    /* 0x7C */ Primitive* prim;
    /* 0x80 */ s16 unk80[10];
    /* 0x94 */ s16 timer;
} OlroxTFHand;

// death effect (E_UNK_3C)
typedef struct {
    /* 0x7C */ Primitive* prim;
    /* 0x80 */ Primitive* flash;
    /* 0x84 */ s16 count;
    /* 0x86 */ s16 unk86;
    /* 0x88 */ s16 frame;
    /* 0x8A */ s16 unk8A[5];
    /* 0x94 */ s16 timer;
    /* 0x96 */ s16 unk96[3];
    /* 0x9C */ Primitive* pool;
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ struct Entity* child;
} OlroxTFDeath;

STATIC_ASSERT(sizeof(OlroxTF) <= sizeof(Ext), "ext too big");
STATIC_ASSERT(sizeof(OlroxTFHead) <= sizeof(Ext), "ext too big");
STATIC_ASSERT(sizeof(OlroxTFTail) <= sizeof(Ext), "ext too big");
STATIC_ASSERT(sizeof(OlroxTFArms) <= sizeof(Ext), "ext too big");
STATIC_ASSERT(sizeof(OlroxTFDeath) <= sizeof(Ext), "ext too big");

#define TF(e) ((OlroxTF*)&(e)->ext)
#define TF_HEAD(e) ((OlroxTFHead*)&(e)->ext)
#define TF_TAIL(e) ((OlroxTFTail*)&(e)->ext)
#define TF_ARMS(e) ((OlroxTFArms*)&(e)->ext)
#define TF_HAND(e) ((OlroxTFHand*)&(e)->ext)
#define TF_DEATH(e) ((OlroxTFDeath*)&(e)->ext)

// entities relative to the main body
#define TF_FOOT1 1
#define TF_FOOT2 2
#define TF_HEAD_OFS 3
#define TF_TAIL_OFS 4
#define TF_ARMS_OFS 7
#define TF_DEATH_OFS 10

typedef struct {
    s16 target;
    s16 speed;
} OlroxApproach;

typedef struct {
    s16 bodyAngle;
    s16 bodyAngle2;
    s16 headAngle;
    s16 unused;
} OlroxLaserPose;

typedef struct {
    s16 duration;
    s16 range;
    s16 speed;
    s16 interval;
} OlroxFirePose;

Primitive* FindFirstUnkPrim2(Primitive* prim, u8 index);
void MakeExplosions(void);

extern EInit D_us_80180708;
extern EInit D_us_80180714;
extern s16* sprites_bo0_5[]; // D_us_801A5ED4
extern s32 D_us_801CE56C;
extern s32 D_us_801CE5B0;

s32 D_us_80181190 = 0;

// {x offset, u, v, head length, head angle, arms length, arms angle}
static s16 D_us_80181194[][7] = {
    {2, 0x00, 0x50, 0x10, -0x360, 0x0E, -0xE0},
    {2, 0x28, 0x50, 0x10, -0x360, 0x0E, -0xE0},
    {-1, 0x40, 0x20, 0x10, -0x360, 0x13, -0xB0},
    {-2, 0xD7, 0x00, 0x0E, -0x2C0, 0x12, -0x160},
};
static OlroxLaserPose D_us_801811CC[] = {
    {0x040, 0x180, -0x200, 0},
    {0x100, -0xC0, -0x030, 0},
    {0x080, 0x200, -0x180, 0},
    {0x0C0, -0x80, -0x010, 0},
};
static OlroxFirePose D_us_801811EC[] = {
    {0x80, 0x140, 0x04, 0x14},
    {0x100, 0x1C0, 0x08, 0x10},
    {0x180, 0x200, 0x0C, 0x0C},
    {0x200, 0x300, 0x1C, 0x04},
};
static u8 D_us_8018120C[] = {
    0x30, 0x01, 0x0C, 0x01, 0x0C, 0x02, 0x0C, 0x01,
    0x0C, 0x02, 0x0C, 0x01, 0x0C, 0x01, 0x0C, 0x01,
    0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_80181220[] = {
    0x08, 0x01, 0x08, 0x02, 0x08, 0x03, 0x08, 0x02,
    0x08, 0x04, 0x08, 0x05, 0x08, 0x06, 0x08, 0x03,
    0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_80181234[] = {
    0x08, 0x01, 0x08, 0x02, 0x08, 0x03, 0x08, 0x04,
    0x08, 0x03, 0x08, 0x05, 0x08, 0x03, 0x08, 0x02,
    0xFF, 0x00, 0x00, 0x00,
};
static s16 D_us_80181248[] = {
    -0x200, -0xC0, 0xC0, 0x280, 0x180, -0x100, -0x200, -0x300,
};
static s16 D_us_80181258[] = {
    0x000, 0x200, 0x100, 0x000, -0x200, -0x300, 0x000, 0x200,
};
static s16 D_us_80181268[] = {
    -0x340, -0x220, -0x1C0, -0x80, 0x180, 0x240, 0x40, -0x140,
};
static s16 D_us_80181278[] = {
    -0x100, 0x1C0, 0x300, 0x180, 0x000, -0x200, -0x400, -0x480,
};
static s16 D_us_80181288[] = {
    -0x1A0, -0x200, -0x280, -0x2C0, -0x300, -0x340, -0x380, -0x400,
};
static s16 D_us_80181298[] = {
    -0x180, -0x1C0, -0x240, -0x2C0, -0x300, -0x320, -0x300, -0x200,
};
static s16 D_us_801812A8[] = {
    -0x80, 0x000, 0x080, 0x100, 0x200, 0x240, 0x200, 0x180,
};
static s16 D_us_801812B8[] = {
    -0x180, -0x280, -0x380, -0x400, -0x480, -0x500, -0x580, -0x600,
};
UNUSED static s16* D_us_801812C8[] = {D_us_80181248, D_us_80181258};
static s16* D_us_801812D0[] = {D_us_80181268, D_us_80181278};
static s16 D_us_801812D8[] = {0, 3, 7, 8, 9, 4};

// Copies the sprite of the current animation frame into a GT4 primitive
static void func_us_801B13A8(Primitive* prim) {
    s16* spr;
    s16 x, y, w, h;
    s32 uOfs, vOfs;
    u16 flags;

    flags = g_CurrentEntity->unk5A;
    uOfs = (flags & 1) * 0x7F;
    vOfs = ((flags & 2) >> 1) * 0x7F;
    spr = sprites_bo0_5[g_CurrentEntity->animCurFrame];
    spr += 2;
    if (g_CurrentEntity->facingLeft) {
        x = g_CurrentEntity->posX.i.hi - *spr++;
    } else {
        x = g_CurrentEntity->posX.i.hi + *spr++;
    }
    y = g_CurrentEntity->posY.i.hi + *spr++;
    w = *spr++;
    h = *spr++;
    if (g_CurrentEntity->facingLeft) {
        prim->x0 = prim->x2 = x - w + 1;
        prim->x1 = prim->x3 = x + 1;
    } else {
        prim->x0 = prim->x2 = x;
        prim->x1 = prim->x3 = x + w;
    }
    prim->y0 = prim->y1 = y;
    prim->y2 = prim->y3 = y + h;
    prim->clut = *spr++;
    flags = *spr++;
    if (flags & 1) {
        uOfs += 0x7F;
    }
    if (flags & 2) {
        vOfs += 0x7F;
    }
    if (g_CurrentEntity->facingLeft) {
        prim->u1 = prim->u3 = (u8)spr[0] + uOfs;
        prim->v0 = prim->v1 = (u8)spr[1] + vOfs;
        prim->u0 = prim->u2 = (u8)spr[2] + uOfs;
    } else {
        prim->u0 = prim->u2 = (u8)spr[0] + uOfs;
        prim->v0 = prim->v1 = (u8)spr[1] + vOfs;
        prim->u1 = prim->u3 = (u8)spr[2] + uOfs;
    }
    prim->v2 = prim->v3 = (u8)spr[3] + vOfs;
}

static void func_us_801B1590(u8 step) {
    OlroxLeg* leg = TF(g_CurrentEntity)->leg1;

    g_CurrentEntity->step = step;
    g_CurrentEntity->step_s = 0;
    leg->state = 0;
    leg->other->state = 0;
}

#include "../../st/approach_s16.h"
#define func_us_801B15BC func_801CDC80

// Accelerates value[0] towards target, value[1] holding the speed
static s32 func_us_801B163C(s16* value, s16 target, s16 maxSpeed) {
    s16 diff, speed, accel;

    accel = maxSpeed / 16;
    if (!accel) {
        accel = 1;
    }
    diff = target - value[0];
    speed = value[1];
    if (diff > 0) {
        if (speed < maxSpeed) {
            speed += accel;
        } else {
            speed -= accel;
        }
    } else if (-maxSpeed < speed) {
        speed -= accel;
    } else {
        speed += accel;
    }
    if (abs(diff) < abs(speed)) {
        value[0] = target;
        value[1] = diff;
        return 1;
    }
    value[0] += speed;
    value[1] = speed;
    return 0;
}

// Moves both leg joints towards the requested angles
static s32 func_us_801B171C(OlroxLeg* leg, s16 hip, s16 knee, s16 speed) {
    s16* angle;
    s16* delta;
    s16 target, diff;
    s32 notDone = 0;
    s32 i;

    angle = &leg->hipAngle;
    delta = &leg->hipDelta;
    target = hip;
    for (i = 0; i < 2; i++) {
        diff = target - *angle;
        if (abs(diff) < speed) {
            *angle = target;
            *delta = diff;
        } else {
            if (diff > 0) {
                *angle += speed;
                *delta = speed;
            } else {
                *angle -= speed;
                *delta = -speed;
            }
            notDone |= 1;
        }
        angle = &leg->kneeAngle;
        delta = &leg->kneeDelta;
        target = knee;
    }
    return notDone ^ 1;
}

static void func_us_801B17BC(OlroxLeg* leg) {
    switch (leg->state) {
    case 0:
        if (func_us_801B171C(leg, -0x400, 0x200, 0x18)) {
            leg->state++;
        }
        break;
    case 1:
        if (func_us_801B171C(leg, -0x400, -0x40, 0x20)) {
            leg->state++;
        }
        break;
    case 2:
        if (func_us_801B171C(leg, -0x300, -0x40, 0x20)) {
            leg->state++;
        }
        break;
    }
}

static void func_us_801B1864(OlroxLeg* leg) {
    switch (leg->state) {
    case 0:
        if (func_us_801B171C(leg, -0x40, 0x40, 0x1C)) {
            leg->state++;
        }
        if (TF(g_CurrentEntity)->angle > -0x100) {
            TF(g_CurrentEntity)->angle -= 0xC;
        }
        break;
    case 1:
        if (func_us_801B171C(leg, -0x100, 0x300, 0x18)) {
            leg->state++;
        }
        if (TF(g_CurrentEntity)->angle < 0x100) {
            TF(g_CurrentEntity)->angle += 8;
        }
        break;
    }
}

static void func_us_801B1950(OlroxLeg* leg) {
    switch (leg->state) {
    case 0:
        if (func_us_801B171C(leg, -0x200, 0x300, 0x18)) {
            leg->state++;
        }
        break;
    case 1:
        if (func_us_801B171C(leg, 0, 0x280, 0x1C)) {
            leg->state++;
        }
        break;
    case 2:
        if (func_us_801B171C(leg, 0x80, 0x180, 0x18)) {
            leg->state++;
        }
        break;
    }
}

static void func_us_801B19FC(OlroxLeg* leg) {
    switch (leg->state) {
    case 0:
        if (func_us_801B171C(leg, -0x80, 0x80, 0x1C)) {
            leg->state++;
        }
        if (TF(g_CurrentEntity)->angle > -0x100) {
            TF(g_CurrentEntity)->angle -= 0x10;
        }
        break;
    case 1:
        if (func_us_801B171C(leg, -0x100, 0, 0x18)) {
            leg->state++;
        }
        break;
    case 2:
        if (func_us_801B171C(leg, -0x280, -0xC0, 0x14)) {
            leg->state++;
        }
        if (TF(g_CurrentEntity)->angle < 0x100) {
            TF(g_CurrentEntity)->angle += 0x10;
        }
        break;
    }
}

static s32 func_us_801B1B30(OlroxLeg* leg) {
    OlroxLeg* other = leg->other;
    s32 ret;

    switch (leg->state) {
    case 0:
        ret = func_us_801B171C(leg, -0x180, 0, 0x20);
        if (ret + func_us_801B171C(other, 0x80, 0x280, 0x20) == 2) {
            leg->state++;
        }
        break;
    case 1:
        ret = func_us_801B171C(leg, -0x200, -0x100, 0x40);
        func_us_801B171C(other, -0x100, 0x280, 0x40);
        if (ret) {
            leg->state++;
        }
        break;
    case 2:
        ret = func_us_801B171C(leg, -0x280, -0x200, 0x60);
        func_us_801B171C(other, -0x180, 0x300, 0x40);
        if (ret) {
            return 1;
        }
        break;
    }
    return 0;
}

static s32 func_us_801B1C60(OlroxLeg* leg) {
    OlroxLeg* other = leg->other;
    s32 ret;

    func_us_801B163C(&TF(g_CurrentEntity)->angle, -0x100, 0xC);
    ret = func_us_801B171C(leg, -0x1C0, 0x280, 0x10);
    return ret + func_us_801B171C(other, -0x380, 0x140, 0x10) == 2;
}

static s32 func_us_801B1CE0(OlroxLeg* leg) {
    OlroxLeg* other = leg->other;
    u8 state = leg->state;
    s32 ret;

    ret = func_us_801B163C(&TF(g_CurrentEntity)->angle, 0x180, 0x40);
    switch (state) {
    case 0:
        func_us_801B171C(leg, -0x60, 0x340, 0x18);
        if (func_us_801B171C(other, -0x500, 0x80, 0x20)) {
            leg->state++;
        }
        break;
    case 1:
        ret += func_us_801B171C(leg, -0x40, 0x340, 0x18);
        if (ret + func_us_801B171C(other, -0x3C0, 0xE0, 0x20) == 3) {
            return 1;
        }
        break;
    }
    return 0;
}

static s32 func_us_801B1DDC(OlroxLeg* leg) {
    OlroxLeg* other = leg->other;
    s32 ret;

    func_us_801B163C(&TF(g_CurrentEntity)->angle, 0, 0xC);
    ret = func_us_801B171C(leg, -0x2C0, 0x2C0, 0x28);
    return ret + func_us_801B171C(other, -0x100, 0x3C0, 0x38) == 2;
}

static s32 func_us_801B1E5C(OlroxLeg* leg) {
    OlroxLeg* other = leg->other;
    s32 ret;

    func_us_801B163C(&TF(g_CurrentEntity)->angle, 0, 0xC);
    ret = func_us_801B171C(leg, -0x40, 0x40, 0x60);
    return ret + func_us_801B171C(other, -0x200, 0x280, 0x50) == 2;
}

static s32 func_us_801B1EDC(OlroxLeg* leg) {
    OlroxLeg* other = leg->other;
    s32 ret;

    func_us_801B163C(&TF(g_CurrentEntity)->angle, 0, 0x10);
    ret = func_us_801B171C(leg, -0x200, 0x200, 0x60);
    return ret + func_us_801B171C(other, -0x200, 0x180, 0x40) == 2;
}

static s32 func_us_801B1F5C(OlroxLeg* leg) {
    OlroxLeg* other = leg->other;
    s32 ret;

    switch (leg->state) {
    case 0:
        ret = func_us_801B171C(leg, -0x80, 0x80, 0x20);
        if (ret + func_us_801B171C(other, 0, 0x180, 0xC) == 2) {
            leg->state++;
        }
        break;
    case 1:
        ret = func_us_801B171C(leg, -0x300, 0, 0x28);
        if (ret + func_us_801B171C(other, 0x80, 0x100, 0xC) == 2) {
            return 1;
        }
        break;
    }
    return 0;
}

static void func_us_801B2044(OlroxLeg* leg) {
    switch (leg->state) {
    case 0:
        if (func_us_801B171C(leg, -0x400, 0x200, 8)) {
            leg->state++;
        }
        break;
    case 1:
        if (func_us_801B171C(leg, -0x400, -0x40, 8)) {
            leg->state++;
        }
        break;
    case 2:
        if (func_us_801B171C(leg, -0x300, -0x40, 0xC)) {
            leg->state++;
        }
        break;
    }
}

static void func_us_801B20F4(OlroxLeg* leg) {
    switch (leg->state) {
    case 0:
        if (func_us_801B171C(leg, -0x40, 0x40, 8)) {
            leg->state++;
        }
        break;
    case 1:
        if (func_us_801B171C(leg, -0x100, 0x380, 0x14)) {
            leg->state++;
        }
        break;
    }
}

// Returns how much the foot sank below the floor and flags the contact
static s16 func_us_801B2178(OlroxLeg* leg) {
    s16 posY;
    s16 ret = 0;
    s16 onGround;

    posY = g_Tilemap.scrollY.i.hi + leg->footY.i.hi;
    if (posY >= 0x1CC) {
        ret = 0x1CC - posY;
        onGround = 1;
    } else {
        onGround = 0;
    }
    leg->onGround = onGround;
    FntPrint("hit_kind %x\n", onGround);
    return ret;
}

// Places the body from the leg standing on the floor (inverse kinematics)
static void func_us_801B21F0(void) {
    Entity* self = g_CurrentEntity;
    OlroxLeg* leg;
    OlroxLeg* other;
    s16 angle;
    s32 footX, footY, kneeX, kneeY, hipX, hipY, dist;

    leg = TF(self)->leg1;
    leg->footX.val = self[TF_FOOT1].posX.val;
    leg->footY.val = self[TF_FOOT1].posY.val;
    leg = TF(self)->leg2;
    leg->footX.val = self[TF_FOOT2].posX.val;
    leg->footY.val = self[TF_FOOT2].posY.val;

    if (!TF(self)->curLeg) {
        leg = TF(self)->leg1;
    } else {
        leg = TF(self)->leg2;
    }
    footX = leg->footX.val;
    footY = leg->footY.val;
    if (self->facingLeft) {
        angle = 0xC00 - leg->kneeAngle;
    } else {
        angle = leg->kneeAngle + 0xC00;
    }
    kneeX = footX - rcos(angle) * 0x2A0;
    kneeY = footY + rsin(angle) * 0x2A0;
    leg->kneeX.val = kneeX;
    leg->kneeY.val = kneeY;
    if (self->facingLeft) {
        angle = 0xC00 - leg->hipAngle;
    } else {
        angle = leg->hipAngle + 0xC00;
    }
    hipX = kneeX - rcos(angle) * 0x1C0;
    dist = footX - hipX;
    hipY = kneeY + rsin(angle) * 0x1C0;
    leg->footDist = dist;
    if (!self->facingLeft) {
        leg->footDist = -dist;
    }
    self->posX.val = hipX;
    self->posY.val = hipY - FIX(4);

    other = leg->other;
    if (self->facingLeft) {
        angle = 0xC00 - other->hipAngle;
    } else {
        angle = other->hipAngle + 0xC00;
    }
    kneeX = hipX + rcos(angle) * 0x1C0;
    kneeY = hipY - rsin(angle) * 0x1C0;
    other->kneeX.val = kneeX;
    other->kneeY.val = kneeY;
    if (self->facingLeft) {
        angle = 0xC00 - other->kneeAngle;
    } else {
        angle = other->kneeAngle + 0xC00;
    }
    footX = kneeX + rcos(angle) * 0x2A0;
    footY = kneeY - rsin(angle) * 0x2A0;
    dist = hipX - footX;
    other->footX.val = footX;
    other->footY.val = footY;
    other->footDist = dist;
    if (self->facingLeft) {
        other->footDist = -dist;
    }
}

// Places both legs from the body position (forward kinematics, airborne)
static void func_us_801B24CC(void) {
    Entity* self = g_CurrentEntity;
    OlroxLeg* leg;
    s16 angle;
    s32 hipX, hipY, kneeX, kneeY, footX, footY, dist;
    s32 i;

    leg = TF(self)->leg1;
    leg->footX.val = self[TF_FOOT1].posX.val;
    leg->footY.val = self[TF_FOOT1].posY.val;
    leg = TF(self)->leg2;
    leg->footX.val = self[TF_FOOT2].posX.val;
    leg->footY.val = self[TF_FOOT2].posY.val;

    leg = TF(self)->leg1;
    for (i = 0; i < 2; i++) {
        hipX = self->posX.val;
        hipY = self->posY.val;
        if (self->facingLeft) {
            angle = 0xC00 - leg->hipAngle;
        } else {
            angle = leg->hipAngle + 0xC00;
        }
        kneeX = hipX + rcos(angle) * 0x1C0;
        kneeY = hipY - rsin(angle) * 0x1C0;
        leg->kneeX.val = kneeX;
        leg->kneeY.val = kneeY;
        if (self->facingLeft) {
            angle = 0xC00 - leg->kneeAngle;
        } else {
            angle = leg->kneeAngle + 0xC00;
        }
        footX = kneeX + rcos(angle) * 0x2A0;
        footY = kneeY - rsin(angle) * 0x2A0;
        leg->footX.val = footX;
        dist = footX - hipX;
        leg->footY.val = footY;
        leg->footDist = dist;
        if (self->facingLeft) {
            leg->footDist = -dist;
        }
        leg = leg->other;
    }
}

static void SetLegQuad(Primitive* prim, u16 clut, u8 u0, u8 u1, u8 u2, u8 u3,
                       u8 v0, u8 v1, u8 v2, u8 v3) {
    prim->tpage = 0x14;
    prim->clut = clut;
    prim->u0 = u0;
    prim->u1 = u1;
    prim->u2 = u2;
    prim->u3 = u3;
    prim->v0 = v0;
    prim->v1 = v1;
    prim->v2 = v2;
    prim->v3 = v3;
    prim->drawMode = DRAW_UNK02;
}

// Draws a leg with four quads, keeping the toes above the floor
static Primitive* func_us_801B2690(OlroxLeg* leg, Primitive* prim, u8 front) {
    Entity* self = g_CurrentEntity;
    s16 floorY;
    u16 clut;
    u8 prioOfs;
    s16 footX, footY, kneeX, kneeY;
    s16 posX, posY;
    s16 hip, knee, mid, bend;
    s16 x0, x1, x2, x3, y0, y1, y2, y3;
    s16 width, ofs;
    s16 ankle;
    s16 heelX, heelY, toeX, toeY, tipX, tipY, ax, ay, bx, by;
    s16 cx, cy, dx, dy;
    s16 sole;
    s32 angle;

    if (!D_us_801CE5B0) {
        floorY = g_Tilemap.scrollY.i.hi;
    } else {
        floorY = 0xE0;
    }
    clut = front ? 0x20B : 0x218;
    prioOfs = front * 2;

    footX = leg->footX.i.hi;
    footY = leg->footY.i.hi;
    kneeX = leg->kneeX.i.hi;
    kneeY = leg->kneeY.i.hi;
    posX = self->posX.i.hi;
    posY = self->posY.i.hi + 4;
    if (self->facingLeft) {
        hip = -leg->hipAngle;
    } else {
        hip = leg->hipAngle + 0x800;
    }
    if (self->facingLeft) {
        knee = -leg->kneeAngle;
    } else {
        knee = leg->kneeAngle + 0x800;
    }
    mid = hip + (knee - hip) / 2;

    // thigh
    ofs = (rcos(hip) * 12) >> 12;
    width = -(rsin(hip) * 12) >> 12;
    x1 = posX + ofs;
    y1 = posY + width;
    x3 = posX - ofs;
    y3 = posY - width;
    if (!self->facingLeft) {
        bend = hip - (knee - 0x800);
    } else {
        bend = leg->hipAngle - (leg->kneeAngle - 0x800);
    }
    if (bend > 0x400) {
        bend = 0x400;
    }
    width = ((rcos(bend) * 24) >> 12) + 12;
    x0 = kneeX + ((rcos(mid) * 12) >> 12);
    y0 = kneeY + (-(rsin(mid) * 12) >> 12);
    x2 = kneeX - ((width * rcos(mid)) >> 12);
    y2 = kneeY - (-(width * rsin(mid)) >> 12);

    // shin
    ax = footX + ((rcos(knee) * 6) >> 12);
    ay = footY + (-(rsin(knee) * 6) >> 12);
    bx = footX + (-(rcos(knee) * 6) >> 12);
    by = footY + ((rsin(knee) * 6) >> 12);

    SetLegQuad(prim, clut, 0x4C, 0x6C, 0x5C, 0x6C, 0x72, 0x64, 0x80, 0x80);
    prim->x0 = x0;
    prim->x1 = x1;
    prim->x2 = x2;
    prim->x3 = x3;
    prim->y0 = y0;
    prim->y1 = y1;
    prim->y2 = y2;
    prim->y3 = y3;
    prim->priority = prioOfs + (u16)D_us_801CE56C;
    prim = prim->next;

    SetLegQuad(prim, clut, 0x74, 0x68, 0x80, 0x76, 0x4E, 0x30, 0x46, 0x20);
    prim->x0 = ax;
    prim->x1 = x0;
    prim->x2 = bx;
    prim->x3 = x2;
    prim->y0 = ay;
    prim->y1 = y0;
    prim->y2 = by;
    prim->y3 = y2;
    prim->priority = prioOfs + (u16)D_us_801CE56C;
    prim = prim->next;

    // foot
    ankle = leg->kneeAngle;
    if (self->facingLeft) {
        ankle = 0x800 - ankle;
    }
    cx = leg->footX.i.hi;
    cy = leg->footY.i.hi;
    heelX = cx + ((rcos(ankle) * 8) >> 12);
    heelY = cy + (-(rsin(ankle) * 8) >> 12);
    angle = ankle + (self->facingLeft ? 0x400 : -0x400);
    tipX = heelX + ((rcos(angle) * 4) >> 12);
    tipY = heelY + (-(rsin(angle) * 4) >> 12);
    if ((s16)(tipY + floorY) >= 0x1CE) {
        tipY = 0x1D0 - floorY;
        ankle = (self->facingLeft != 0) << 11;
    }
    angle = ankle - 0x800;
    dy = tipY - ((rsin(angle) * 16) >> 12);
    if ((s16)(dy + floorY) >= 0x1D0) {
        sole = tipY + floorY - 0x1D0;
        dy = sole;
        if (!self->facingLeft) {
            dx = tipX - SquareRoot0(0x100 - sole * sole);
        } else {
            dx = tipX + SquareRoot0(0x100 - sole * sole);
        }
        ankle = ratan2(-dy, tipX - dx);
        dy = 0x1D0 - floorY;
        angle = ankle + (self->facingLeft ? -0x400 : 0x400);
        toeX = tipX + ((rcos(angle) * 16) >> 12);
        toeY = tipY - ((rsin(angle) * 16) >> 12);
    } else {
        dx = tipX + ((rcos(angle) * 16) >> 12);
        angle = ankle + (self->facingLeft ? -0x400 : 0x400);
        toeX = tipX + ((rcos(angle) * 16) >> 12);
        toeY = tipY - ((rsin(angle) * 16) >> 12);
    }
    if (!self->facingLeft) {
        angle = ankle / 2 + 0x400;
    } else {
        angle = ankle / 2;
    }
    x1 = dx + ((rcos(angle) * 16) >> 12);
    y1 = dy + (-(rsin(angle) * 16) >> 12);
    angle = ankle + 0x800;
    y2 = dy - ((rsin(angle) * 24) >> 12);
    if ((s16)(y2 + floorY) >= 0x1D0) {
        sole = dy + floorY - 0x1D0;
        y2 = sole;
        if (!self->facingLeft) {
            x2 = dx - SquareRoot0(0x240 - sole * sole);
        } else {
            x2 = dx + SquareRoot0(0x240 - sole * sole);
        }
        ankle = ratan2(-y2, dx - x2);
        y2 = 0x1D0 - floorY;
    } else {
        x2 = dx + ((rcos(angle) * 24) >> 12);
    }
    angle = ankle + (self->facingLeft ? -0x400 : 0x400);
    x0 = x2 + ((rcos(angle) * 16) >> 12);
    y0 = y2 + (-(rsin(angle) * 16) >> 12);

    SetLegQuad(prim, clut, 0xD8, 0xF0, 0xD8, 0xF0, 0x50, 0x50, 0x5E, 0x5E);
    prim->x0 = x0;
    prim->x1 = x1;
    prim->x2 = x2;
    prim->x3 = dx;
    prim->y0 = y0;
    prim->y1 = y1;
    prim->y2 = y2;
    prim->y3 = dy;
    prim->priority = prioOfs + ((u16)D_us_801CE56C + 1);
    prim = prim->next;

    SetLegQuad(prim, clut, 0xF0, 0xFF, 0xF0, 0xFF, 0x50, 0x50, 0x5E, 0x5E);
    prim->x0 = x1;
    prim->x1 = toeX;
    prim->x2 = dx;
    prim->x3 = tipX;
    prim->y0 = y1;
    prim->y1 = toeY;
    prim->y2 = dy;
    prim->y3 = tipY;
    prim->priority = prioOfs + ((u16)D_us_801CE56C + 1);
    return prim->next;
}

// Draws the neck, then places the head and the arms on it
static Primitive* func_us_801B30AC(Primitive* prim) {
    Entity* self = g_CurrentEntity;
    Entity* part;
    s16* pose;
    s16 posX, posY, baseX, backX, backX2, frontX;
    s16 angle;
    s16 ax, ay, bx, by, cx, cy, dx, dy, ex, ey, fx, fy, nx, ny;
    s16 half, rot;
    s16 headX, headY;

    posX = self->posX.i.hi;
    posY = self->posY.i.hi;
    pose = D_us_80181194[TF(self)->neckPose];
    if (!self->facingLeft) {
        baseX = posX + *pose++;
    } else {
        baseX = posX - *pose++;
    }
    if (!self->facingLeft) {
        backX = baseX - 0x20;
        backX2 = baseX + 8;
    } else {
        backX = baseX + 0x20;
        backX2 = baseX - 8;
    }

    half = TF(self)->angle / 2;
    if (!self->facingLeft) {
        rot = half;
        angle = half + 0x400;
    } else {
        rot = 0x800 - half;
        angle = rot - 0x400;
    }
    nx = baseX + ((rcos(angle) * 16) >> 12);
    ny = posY + (-(rsin(angle) * 16) >> 12);
    ax = nx + ((rcos(rot) * 8) >> 12);
    ay = ny + (-(rsin(rot) * 8) >> 12);
    bx = nx + (-(rcos(rot) * 32) >> 12);
    by = ny + ((rsin(rot) * 32) >> 12);

    rot = TF(self)->angle;
    if (!self->facingLeft) {
        angle = rot + 0x400;
    } else {
        rot = 0x800 - rot;
        angle = rot - 0x400;
    }
    nx = nx + ((rcos(angle) * 32) >> 12);
    ny = ny + (-(rsin(angle) * 32) >> 12);
    cx = nx + ((rcos(rot) * 8) >> 12);
    cy = ny + (-(rsin(rot) * 8) >> 12);
    dx = nx + (-(rcos(rot) * 32) >> 12);
    dy = ny + ((rsin(rot) * 32) >> 12);
    headX = dx;
    headY = dy;

    prim->tpage = 0x14;
    prim->clut = 0x20B;
    prim->u1 = prim->u3 = (u8)pose[0];
    prim->u0 = prim->u2 = (u8)pose[0] + 0x28;
    prim->v0 = prim->v1 = (u8)pose[1];
    prim->v2 = prim->v3 = (u8)pose[1] + 0x20;
    prim->x0 = cx;
    prim->x1 = dx;
    prim->x2 = ax;
    prim->x3 = bx;
    prim->y0 = cy;
    prim->y1 = dy;
    prim->y2 = ay;
    prim->y3 = by;
    prim->drawMode = DRAW_UNK02;
    prim->priority = (u16)D_us_801CE56C - 1;
    prim = prim->next;

    prim->tpage = 0x14;
    prim->clut = 0x20B;
    prim->u1 = prim->u3 = (u8)pose[0];
    prim->u0 = prim->u2 = (u8)pose[0] + 0x28;
    prim->v0 = prim->v1 = (u8)pose[1] + 0x20;
    prim->v2 = prim->v3 = (u8)pose[1] + 0x30;
    prim->x0 = ax;
    prim->x1 = bx;
    prim->x2 = backX2;
    prim->x3 = backX;
    prim->y0 = ay;
    prim->y1 = by;
    prim->y2 = posY;
    prim->y3 = posY;
    prim->drawMode = DRAW_UNK02;
    prim->priority = (u16)D_us_801CE56C - 1;
    prim = prim->next;

    pose += 2;
    rot = TF(self)->angle;
    part = self + TF_HEAD_OFS;
    if (!self->facingLeft) {
        part->posX.i.hi = headX + ((pose[0] * rcos(rot + pose[1])) >> 12);
    } else {
        part->posX.i.hi = headX - ((pose[0] * rcos(rot + pose[1])) >> 12);
    }
    part->posY.i.hi = headY + (-(pose[0] * rsin(rot + pose[1])) >> 12);
    if (TF(self)->neckPose == 2) {
        part->zPriority = D_us_801CE56C;
    } else {
        part->zPriority = D_us_801CE56C - 2;
    }

    pose += 2;
    part = self + TF_ARMS_OFS;
    if (!self->facingLeft) {
        part->posX.i.hi = headX + ((pose[0] * rcos(rot + pose[1])) >> 12);
    } else {
        part->posX.i.hi = headX - ((pose[0] * rcos(rot + pose[1])) >> 12);
    }
    part->posY.i.hi = headY + (-(pose[0] * rsin(rot + pose[1])) >> 12);
    return prim;
}

static s16 TrueFormRoomX(Entity* self) {
    s16 x = g_Tilemap.scrollX.i.hi + self->posX.i.hi;
    if (self->facingLeft) {
        x = 0x200 - x;
    }
    return x;
}

static OlroxLeg* CurrentLeg(Entity* self) {
    if (!TF(self)->curLeg) {
        return TF(self)->leg1;
    }
    return TF(self)->leg2;
}

// Olrox true form, main body
void func_us_801B365C(Entity* self) {
    Entity* entity;
    Primitive* prim;
    OlroxLeg* leg;
    OlroxLeg* other;
    s16 sink;
    s16 x, dx, dy;
    s16 primIndex;
    s32 i, n, facing;
    s16 posX, posY;
    s16 target;

    FntPrint("step %x\n", self->step);
    FntPrint("step_s %x\n", self->step_s);
    FntPrint("abs_x %x\n", TrueFormRoomX(self));
    if (!TF(self)->dying && self->flags & FLAG_DEAD && self->step < 16) {
        self->hitboxState = 0;
        func_us_801B1590(16);
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180708);
        self->hitboxWidth = 8;
        self->hitboxHeight = 16;
        self->hitboxOffY = -8;
        D_us_801CE56C = 0xA0;
        self->animCurFrame = 0xB;
        self->zPriority = (u16)D_us_801CE56C;
        CreateEntityFromEntity(E_UNK_34, self, self + TF_FOOT1);
        CreateEntityFromEntity(E_UNK_34, self, self + TF_FOOT2);
        CreateEntityFromEntity(E_UNK_35, self, self + TF_HEAD_OFS);
        CreateEntityFromEntity(E_UNK_36, self, self + TF_TAIL_OFS);
        CreateEntityFromEntity(E_UNK_37, self, self + TF_ARMS_OFS);
        CreateEntityFromEntity(E_UNK_38, self, self + 9);
        self[9].params = 0;
        CreateEntityFromEntity(E_UNK_38, self, self + 8);
        self[8].params = 1;
        primIndex = g_api.func_800EDB58(PRIM_GT4, 0x11);
        if (primIndex == -1) {
            self->step = 0;
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        self->flags |= FLAG_HAS_PRIMS;
        prim->type = PRIM_TILE;
        prim->drawMode = DRAW_HIDE;
        TF(self)->leg1 = (OlroxLeg*)prim;
        TF(self + TF_FOOT1)->leg = (OlroxLeg*)prim;
        prim = prim->next;
        prim->type = PRIM_TILE;
        prim->drawMode = DRAW_HIDE;
        TF(self)->leg2 = (OlroxLeg*)prim;
        TF(self + TF_FOOT2)->leg = (OlroxLeg*)prim;
        prim = prim->next;
        TF(self)->prim = prim;
        for (i = 1; i < 14; i++) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        TF(self)->lastPrim = prim;
        prim->drawMode = DRAW_HIDE;

        CreateEntityFromEntity(E_UNK_3B, self, self + 5);
        TF(self + 5)->leg = TF(self)->leg1;
        CreateEntityFromEntity(E_UNK_3B, self, self + 6);
        TF(self + 6)->leg = TF(self)->leg2;
        entity = self;
        for (i = 1; i < 7; i++) {
            entity->nextPart = entity + 1;
            entity->parent = self + TF_HEAD_OFS;
            entity++;
        }
        entity->nextPart = self;
        entity->parent = self + TF_HEAD_OFS;
        self[TF_HEAD_OFS].parent = NULL;

        leg = TF(self)->leg1;
        leg->other = TF(self)->leg2;
        leg->hipAngle = -0x200;
        leg->kneeAngle = 0x300;
        leg = TF(self)->leg2;
        leg->other = TF(self)->leg1;
        leg->hipAngle = -0x380;
        leg->kneeAngle = 0x100;
        TF(self)->curLeg = 1;
        self->step = 1;
        // fallthrough
    case 1:
        switch (self->step_s) {
        case 0:
            D_us_801CE56C = 0xD0;
            self->zPriority = (u16)D_us_801CE56C;
            func_us_801B21F0();
            self->step_s++;
            break;
        case 1:
            D_us_801CE56C = 0xA0;
            self[TF_FOOT2].posX.i.hi = -g_Tilemap.scrollX.i.hi;
            self->zPriority = (u16)D_us_801CE56C;
            self[TF_FOOT2].posY.i.hi = -0x20 - g_Tilemap.scrollY.i.hi;
            func_us_801B21F0();
            self->step_s++;
            break;
        case 2:
            if (!D_us_801CE5B0) {
                for (i = 0; i < 8; i++) {
                    self[i].flags |= FLAG_POS_CAMERA_LOCKED;
                }
                self->facingLeft = self->params;
                self[TF_FOOT2].posX.i.hi = self->posX.i.hi;
                self[TF_FOOT2].posY.i.hi = self->posY.i.hi;
                func_us_801B21F0();
                TF(self)->timer = 0x20;
                func_us_801B1590(12);
            }
            break;
        }
        break;

    case 2:
        if (!--TF(self)->timer) {
            func_us_801B1590(10);
        }
        func_us_801B21F0();
        break;

    case 10:
        func_us_801B21F0();
        x = TrueFormRoomX(self);
        dx = self->posX.i.hi - PLAYER.posX.i.hi;
        facing = dx < 1;
        dy = self->posY.i.hi - PLAYER.posY.i.hi;
        if (TF(self)->nextAttack) {
            TF(self)->nextAttack = 0;
            if (abs(dx) < 0x80) {
                if (x < 0x180) {
                    func_us_801B1590(4);
                } else {
                    func_us_801B1590(3);
                }
            } else if (self->facingLeft == facing) {
                if (x < 0xD1) {
                    func_us_801B1590(4);
                } else {
                    func_us_801B1590(3);
                }
            } else if (x < 0x100) {
                func_us_801B1590(17);
            } else {
                func_us_801B1590(13);
            }
        } else {
            TF(self)->nextAttack = 1;
            if (self->facingLeft != facing) {
                func_us_801B1590(13);
            } else if (abs(dx) < 0x60) {
                if (dy > 0) {
                    func_us_801B1590(18);
                } else if (Random() & 1) {
                    func_us_801B1590(6);
                } else {
                    func_us_801B1590(8);
                }
            } else if (dy >= 0x41 || Random() & 1) {
                func_us_801B1590(9);
            } else {
                func_us_801B1590(8);
            }
        }
        break;

    case 3: // walk backwards
    case 4: // walk forward
    case 14:
        if (!self->step_s) {
            x = TrueFormRoomX(self);
            if (self->step == 3 ? x < 0xC0 : x > 0x180) {
                func_us_801B1590(10);
                break;
            }
            if (self->step == 14) {
                self[TF_ARMS_OFS].step = 15;
                self[TF_TAIL_OFS].step = 9;
                self[TF_HEAD_OFS].step = 7;
            } else {
                self[TF_ARMS_OFS].step = 2;
                self[TF_TAIL_OFS].step = 3;
                self[TF_HEAD_OFS].step = 1;
            }
            TF(self)->stepCount = 0;
            self->step_s++;
        }
        if (self->step != 14) {
            x = TrueFormRoomX(self);
            if (x < 0x180 && (self[TF_HEAD_OFS].hitFlags & 3) >= 2) {
                func_us_801B1590(14);
            }
        }
        leg = CurrentLeg(self);
        if (self->step == 3) {
            func_us_801B1864(leg);
            leg = leg->other;
            func_us_801B17BC(leg);
        } else {
            func_us_801B19FC(leg);
            leg = leg->other;
            func_us_801B1950(leg);
        }
        func_us_801B21F0();
        sink = func_us_801B2178(leg);
        other = leg->other;
        if (!leg->onGround) {
            break;
        }
        if (self->step == 3 ? other->footDist < leg->footDist
                            : leg->footDist < other->footDist) {
            g_api.PlaySfx(0x647);
            TF(self)->stepCount++;
            leg->state = 0;
            leg->footY.i.hi += sink;
            other->state = 0;
            x = TrueFormRoomX(self);
            if (self->step == 14) {
                if (x < 0x181 && TF(self)->stepCount < 2) {
                    TF(self)->curLeg ^= 1;
                } else {
                    TF(self)->stepCount = 0;
                    func_us_801B1590(10);
                    self[TF_TAIL_OFS].step = 3;
                }
            } else if (self->step == 3 ? x < 0xC0 : x > 0x180) {
                func_us_801B1590(10);
            } else if (TF(self)->stepCount < 2) {
                TF(self)->curLeg ^= 1;
            } else {
                func_us_801B1590(10);
            }
        }
        break;

    case 17: // jump backwards
        leg = CurrentLeg(self);
        other = leg->other;
        switch (self->step_s) {
        case 0:
            leg = TF(self)->leg1;
            other = leg->other;
            TF(self)->curLeg = !(other->footDist < leg->footDist);
            func_us_801B21F0();
            self->step_s++;
            // fallthrough
        case 1:
            func_us_801B21F0();
            if (func_us_801B1B30(leg)) {
                leg->state = 0;
                other->state = 0;
                self->velocityX = FIX(4);
                if (self->facingLeft) {
                    self->velocityX = FIX(-4);
                }
                self->velocityY = FIX(-3);
                self[TF_TAIL_OFS].step = 8;
                self->step_s++;
            }
            break;
        case 2:
            self->posX.val += self->velocityX;
            self->posY.val += self->velocityY;
            self->velocityY += FIX(0.15625);
            func_us_801B171C(other, -0x300, -0x180, 0x28);
            func_us_801B171C(leg, -0x280, 0x180, 0x28);
            func_us_801B24CC();
            sink = func_us_801B2178(leg);
            if (leg->onGround) {
                g_api.PlaySfx(0x63D);
                leg->footY.i.hi += sink;
                func_us_801B21F0();
                self[TF_TAIL_OFS].step = 3;
                self->step_s++;
                func_us_801B1590(3);
            }
            break;
        }
        break;

    case 6: // laser from the mouth
        leg = CurrentLeg(self);
        other = leg->other;
        func_us_801B21F0();
        if (TF_ARMS(self + TF_ARMS_OFS)->handsHurt) {
            func_us_801B1590(10);
            break;
        }
        switch (self->step_s) {
        case 0:
            leg = TF(self)->leg1;
            other = leg->other;
            self[TF_ARMS_OFS].step = 3;
            TF(self)->neckPose = 2;
            self[TF_HEAD_OFS].step = 6;
            self->step_s++;
            TF(self)->curLeg = other->footDist < leg->footDist;
            break;
        case 1:
            if (func_us_801B1C60(leg) && self[TF_ARMS_OFS].step == 1) {
                self[TF_ARMS_OFS].step = 4;
                leg->state = 0;
                other->state = 0;
                self->step_s++;
            }
            break;
        case 2:
            if (func_us_801B1CE0(leg) && self[TF_ARMS_OFS].step == 1) {
                self[TF_ARMS_OFS].step = 4;
                leg->state = 0;
                other->state = 0;
                self->step_s++;
            }
            break;
        case 3:
            self[TF_ARMS_OFS].step = 2;
            self[TF_HEAD_OFS].step = 1;
            TF(self)->neckPose = 0;
            func_us_801B1590(10);
            break;
        }
        break;

    case 18: // jump and stomp
        leg = CurrentLeg(self);
        other = leg->other;
        entity = self + TF_ARMS_OFS;
        switch (self->step_s) {
        case 0:
            leg = TF(self)->leg1;
            other = leg->other;
            TF(self)->curLeg = !(other->footDist < leg->footDist);
            func_us_801B21F0();
            entity->step = 11;
            TF(self)->neckPose = 2;
            self[TF_HEAD_OFS].step = 10;
            self->step_s++;
            break;
        case 1:
            func_us_801B21F0();
            if (TF_ARMS(entity)->handsHurt) {
                func_us_801B1590(10);
                break;
            }
            if (func_us_801B1DDC(leg) && entity->step == 1) {
                entity->step = 12;
                self[TF_TAIL_OFS].step = 6;
                leg->state = 0;
                other->state = 0;
                self->step_s++;
            }
            break;
        case 2:
            func_us_801B21F0();
            if (TF_ARMS(entity)->handsHurt) {
                func_us_801B1590(10);
                break;
            }
            if (func_us_801B1E5C(leg)) {
                TF(self)->neckPose = 3;
                leg->state = 0;
                other->state = 0;
                self->velocityX = FIX(-1);
                if (self->facingLeft) {
                    self->velocityX = FIX(1);
                }
                x = TrueFormRoomX(self);
                if (x < 0x100) {
                    self->velocityX /= 2;
                }
                if (x < 0xC0) {
                    self->velocityX = 0;
                }
                self->velocityY = FIX(-6);
                self->step_s++;
            }
            break;
        case 3:
            self->posX.val += self->velocityX;
            self->posY.val += self->velocityY;
            self->velocityY += FIX(0.15625);
            func_us_801B24CC();
            func_us_801B163C(&TF(self)->angle, -0x100, 0x30);
            func_us_801B171C(leg, -0x80, 0x100, 0x10);
            func_us_801B171C(other, -0x280, 0x280, 0x10);
            if (self->velocityY > FIX(-1)) {
                self[TF_TAIL_OFS].step = 6;
                TF(self)->curLeg ^= 1;
                self->step_s++;
            }
            if (self[TF_HEAD_OFS].rotate < 0x80) {
                self[TF_HEAD_OFS].rotate += 8;
            }
            break;
        case 4:
            self->posX.val += self->velocityX;
            self->posY.val += self->velocityY;
            self->velocityY += FIX(0.15625);
            func_us_801B163C(&TF(self)->angle, 0x100, 0x20);
            func_us_801B171C(leg, -0x180, -0x100, 0x18);
            func_us_801B171C(other, -0x340, 0x2C0, 0x18);
            entity->step = 13;
            func_us_801B24CC();
            sink = func_us_801B2178(leg);
            if (leg->onGround) {
                g_api.ShakeCamera(1);
                g_api.PlaySfx(0x63D);
                entity->step = 14;
                leg->footY.i.hi += sink;
                TF(self)->neckPose = 0;
                self[TF_TAIL_OFS].step = 3;
                self->step_s++;
            }
            if (self[TF_HEAD_OFS].rotate >= -0x1FF) {
                self[TF_HEAD_OFS].rotate -= 0x40;
            }
            break;
        case 5:
            if (func_us_801B1EDC(leg)) {
                leg->state = 0;
                other->state = 0;
                self->step_s++;
            }
            func_us_801B21F0();
            if (self[TF_HEAD_OFS].rotate > 0) {
                self[TF_HEAD_OFS].rotate -= 0x100;
            } else {
                self[TF_HEAD_OFS].rotate = 0;
            }
            break;
        case 6:
            entity->step = 2;
            TF(self)->neckPose = 0;
            self[TF_HEAD_OFS].step = 1;
            func_us_801B1590(10);
            break;
        }
        break;

    case 8: // lasers
        func_us_801B21F0();
        entity = self + TF_HEAD_OFS;
        switch (self->step_s) {
        case 0:
            self[TF_ARMS_OFS].step = 6;
            n = entity->hitPoints / (g_api.enemyDefs[0x38].hitPoints / 4 + 1);
            entity->step = 2;
            TF(self)->laserIdx = 0;
            TF(self)->laserBusy = 0;
            self->step_s++;
            entity->ext.olroxLaser.unkA8 = TF(self)->laserIdx;
            TF(self)->laserMax = 3 - n;
            break;
        case 1:
            func_us_801B163C(&TF(self)->angle,
                             D_us_801811CC[TF(self)->laserIdx].bodyAngle, 0x10);
            if (TF(self)->laserBusy) {
                self->step_s++;
            }
            break;
        case 2:
            func_us_801B163C(&TF(self)->angle,
                             D_us_801811CC[TF(self)->laserIdx].bodyAngle2,
                             0x10);
            TF(self)->neckPose = 0;
            if (TF(self)->laserBusy) {
                break;
            }
            TF(self)->laserIdx++;
            x = TrueFormRoomX(self);
            if (TF(self)->laserIdx & 1 && x < 0xF8) {
                TF(self)->laserIdx = 8;
            }
            if (TF(self)->laserMax < TF(self)->laserIdx) {
                self->step_s++;
            } else {
                entity->step = 2;
                entity->ext.olroxLaser.unkA9 = 0;
                entity->ext.olroxLaser.unkA8 = TF(self)->laserIdx;
                self->step_s--;
            }
            break;
        case 3:
            self[TF_HEAD_OFS].step = 1;
            self[TF_ARMS_OFS].step = 2;
            func_us_801B1590(10);
            break;
        }
        break;

    case 9: // fireballs
        func_us_801B21F0();
        entity = self + TF_HEAD_OFS;
        switch (self->step_s) {
        case 0:
            TF(self)->fireBusy = 0;
            n = entity->hitPoints / (g_api.enemyDefs[0x38].hitPoints / 4 + 1);
            self[TF_ARMS_OFS].step = 6;
            entity->step = 4;
            entity->step_s = 0;
            entity->ext.olroxLaser.unkAC = 0;
            TF(self)->fireIdx = 3 - n;
            self->step_s++;
            entity->ext.olroxLaser.scaleIndex = TF(self)->fireIdx;
            break;
        case 1:
            if (TF(self)->fireBusy) {
                self->step_s++;
            }
            break;
        case 2:
            if (!TF(self)->fireBusy) {
                self->step_s++;
            }
            break;
        case 3:
            self[TF_HEAD_OFS].step = 1;
            self[TF_ARMS_OFS].step = 2;
            func_us_801B1590(10);
            break;
        }
        break;

    case 12: // stomp after landing in the room
        leg = CurrentLeg(self);
        other = leg->other;
        func_us_801B21F0();
        switch (self->step_s) {
        case 0:
            self[TF_ARMS_OFS].step = 7;
            self->step_s++;
            break;
        case 1:
            func_us_801B1F5C(leg);
            func_us_801B21F0();
            sink = func_us_801B2178(other);
            if (other->onGround) {
                g_api.PlaySfx(0x647);
                other->footY.i.hi += sink;
                leg->state = 0;
                other->state = 0;
                self[TF_HEAD_OFS].step = 5;
                TF(self)->stomping = 1;
                self->step_s++;
            }
            break;
        case 2:
            func_us_801B163C(&TF(self)->angle, -0x100, 0x10);
            if (!TF(self)->stomping) {
                self->step_s++;
            }
            break;
        case 3:
            self[TF_HEAD_OFS].step = 1;
            self[TF_ARMS_OFS].step = 2;
            func_us_801B1590(10);
            break;
        }
        break;

    case 13: // turn around
        leg = CurrentLeg(self);
        other = leg->other;
        func_us_801B21F0();
        switch (self->step_s) {
        case 0:
            self[TF_ARMS_OFS].step = 8;
            self[TF_TAIL_OFS].step = 4;
            self->step_s++;
            break;
        case 1:
            n = func_us_801B163C(&TF(self)->angle, -0x100, 0x30);
            n += func_us_801B171C(leg, -0x40, 0x40, 0x20);
            n += func_us_801B171C(other, -0x100, 0x100, 0x20);
            if (n == 3) {
                if (self->facingLeft) {
                    leg->footX.i.hi -= 0x30;
                } else {
                    leg->footX.i.hi += 0x30;
                }
                func_us_801B21F0();
                self->facingLeft ^= 1;
                self->step_s++;
            }
            break;
        case 2:
            self[TF_ARMS_OFS].step = 2;
            self[TF_TAIL_OFS].step = 3;
            func_us_801B1590(3);
            break;
        }
        break;

    case 16: // death
        func_us_801B21F0();
        leg = CurrentLeg(self);
        switch (self->step_s) {
        case 0:
            for (i = 1; i < 10; i++) {
                self[i].flags |= FLAG_DEAD;
            }
            g_api.PlaySfx(0x92);
            g_api.PlaySfx(0x819);
            self[TF_HEAD_OFS].step = 5;
            self[TF_HEAD_OFS].step_s = 0;
            self[TF_TAIL_OFS].step_s = 0;
            self[TF_TAIL_OFS].step = 9;
            currentMusicId = 0x317;
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_OLROX_DEFEAT, TIMEATTACK_SET_RECORD);
            TF(self)->dying = 0;
            TF(self)->timer = 0x60;
            g_api.PlaySfx(0x7C6);
            self->step_s++;
            break;
        case 1:
            func_us_801B1F5C(leg);
            func_us_801B21F0();
            other = leg->other;
            sink = func_us_801B2178(other);
            if (other->onGround) {
                other->footY.i.hi += sink;
                leg->state = 0;
                other->state = 0;
                self->step_s++;
            }
            // fallthrough
        case 2:
            if (!(TF(self)->timer & 1)) {
                if (!(TF(self)->timer & 7)) {
                    g_api.PlaySfx(0x655);
                }
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    prim = TF(self)->prim;
                    n = Random() & 0xF;
                    // The list holds only 15 prims, so the PSX may walk past
                    // its end and read low RAM; stop at the last one instead.
                    for (i = 0; i < n && prim->next != NULL; i++) {
                        prim = prim->next;
                    }
                    posY = prim->y0;
                    posX = prim->x0;
                    if (Random() & 1) {
                        CreateEntityFromCurrentEntity(
                            E_LASER_EXPLOSION, entity);
                        entity->params = 1;
                    } else {
                        CreateEntityFromCurrentEntity(
                            E_INTENSE_EXPLOSION, entity);
                    }
                    entity->posX.i.hi = posX;
                    entity->posY.i.hi = posY;
                    entity->params |= ((u16)D_us_801CE56C + 0x10) << 8;
                }
                target = TF(self)->angle + 0x18;
            } else {
                target = TF(self)->angle - 0x18;
            }
            func_us_801B15BC(&TF(self)->angle, target, 0x18);
            if (!--TF(self)->timer) {
                self->step_s++;
            }
            break;
        case 3:
            entity = self + TF_DEATH_OFS;
            self[TF_HEAD_OFS].step = 8;
            self[TF_TAIL_OFS].step = 5;
            self[TF_ARMS_OFS].step = 10;
            self[TF_HEAD_OFS].step_s = 0;
            self[TF_TAIL_OFS].step_s = 0;
            self[TF_ARMS_OFS].step_s = 0;
            self[9].step_s = 0;
            self[9].step = 3;
            self[8].step_s = 0;
            self[8].step = 3;
            DestroyEntity(entity);
            CreateEntityFromEntity(E_UNK_3C, self, entity);
            prim = TF(self)->lastPrim;
            entity->facingLeft = self->facingLeft;
            func_us_801B13A8(prim);
            g_api.func_800EB758(self->posX.i.hi, self->posY.i.hi, self,
                                self->drawFlags, (POLY_GT4*)prim,
                                self->facingLeft);
            prim->tpage = 0x14;
            prim->clut = 0x20B;
            prim->drawMode = DRAW_UNK02;
            prim->priority = self->zPriority;
            self->step_s++;
            break;
        case 4:
            prim = TF(self)->lastPrim;
            self->palette = 0x219;
            self->unk5A = 0x54;
            prim->drawMode = DRAW_HIDE;
            TF(self)->dying = 1;
            g_api.PlaySfx(0x817);
            self->step_s++;
            break;
        case 5:
            self->palette = 0x219;
            self->unk5A = 0x54;
            if (!(g_Timer & 0x1F)) {
                g_api.PlaySfx(0x817);
            }
            if (TF(self)->dying == 2) {
                x = TrueFormRoomX(self);
                if (x > 0x80) {
                    self[TF_TAIL_OFS].step = 3;
                    TF(self)->stepCount = 0;
                    self->step_s++;
                } else {
                    self->step_s = 7;
                }
            }
            break;
        case 6:
            func_us_801B15BC(&self[TF_HEAD_OFS].rotate, -0x100, 6);
            self[TF_HEAD_OFS].animCurFrame = 1;
            func_us_801B163C(&TF(self)->angle, 0x180, 8);
            func_us_801B20F4(leg);
            leg = leg->other;
            func_us_801B2044(leg);
            func_us_801B21F0();
            sink = func_us_801B2178(leg);
            other = leg->other;
            if (leg->onGround && other->footDist < leg->footDist) {
                g_api.PlaySfx(0x647);
                TF(self)->stepCount++;
                leg->state = 0;
                leg->footY.i.hi += sink;
                other->state = 0;
                if (TF(self)->stepCount >= 2) {
                    self[TF_TAIL_OFS].step_s = 0;
                    self[TF_TAIL_OFS].step = 5;
                    self->step_s++;
                } else {
                    TF(self)->curLeg ^= 1;
                }
            }
            break;
        case 7:
            prim = TF(self)->lastPrim;
            TF(self)->dying = 3;
            self[TF_DEATH_OFS].posX.i.hi = self->posX.i.hi;
            self[TF_DEATH_OFS].posY.i.hi = self->posY.i.hi;
            func_us_801B13A8(prim);
            g_api.func_800EB758(self->posX.i.hi, self->posY.i.hi, self,
                                self->drawFlags, (POLY_GT4*)prim,
                                self->facingLeft);
            prim->tpage = 0x15;
            prim->clut = 0x219;
            prim->drawMode = DRAW_UNK02;
            prim->priority = self->zPriority;
            self->step_s++;
            break;
        case 8:
            entity = self;
            for (i = 0; i < 10; i++) {
                DestroyEntity(entity);
                entity++;
            }
            return;
        }
        break;
    }

    leg = TF(self)->leg1;
    prim = func_us_801B2690(leg, TF(self)->prim, 0);
    prim = func_us_801B2690(leg->other, prim, 1);
    func_us_801B30AC(prim);
    if (D_us_801CE5B0) {
        for (prim = TF(self)->prim; prim != NULL; prim = prim->next) {
            prim->drawMode &= ~DRAW_UNK02;
        }
    }
    if (self->palette & 0x8000) {
        for (prim = TF(self)->prim; prim != NULL; prim = prim->next) {
            prim->clut = self->palette & 0xFFF;
        }
    }
    if (TF(self)->dying) {
        for (prim = TF(self)->prim; prim != NULL; prim = prim->next) {
            prim->tpage = 0x15;
            prim->clut = 0x219;
        }
    }
}

// feet, following the leg data
void func_us_801B5470(Entity* self) {
    OlroxLeg* leg;

    self->facingLeft = self[-1].facingLeft;
    if (self->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180708);
        self->hitboxWidth = 0xC;
        self->hitboxHeight = 4;
        self->hitboxOffX = -0xC;
        self->animCurFrame = 0;
        break;
    case 1:
        leg = TF(self)->leg;
        self->posX.val = leg->footX.val;
        self->posY.val = leg->footY.val;
        break;
    }
}

// head
void func_us_801B551C(Entity* self) {
    Entity* main = self - TF_HEAD_OFS;
    Entity* entity;
    Primitive* prim;
    OlroxFirePose* fire;
    s16 primIndex;
    s16 angle, dx, dy;
    s32 ret;

    self->facingLeft = main->facingLeft;
    self->palette = main->palette;
    if (self->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180714);
        self->hitboxWidth = 0xA;
        self->hitboxHeight = 0xA;
        self->animCurFrame = 1;
        self->drawFlags |= ENTITY_ROTATE;
        self->rotate = -0x100;
        self->zPriority = D_us_801CE56C - 2;
        break;
    case 1:
        AnimateEntity(D_us_8018120C, self);
        func_us_801B15BC(&self->rotate, -0x80, 8);
        self->step_s = 0;
        break;
    case 7:
        AnimateEntity(D_us_80181220, self);
        func_us_801B15BC(&self->rotate, 0xC0, 8);
        if (!self->poseTimer && !(self->pose & 3)) {
            g_api.PlaySfx(0x819);
        }
        self->step_s = 0;
        break;
    case 6:
        self->animCurFrame = 1;
        func_us_801B15BC(&self->rotate, -0x100, 0x20);
        break;
    case 5:
        if (func_us_801B15BC(&self->rotate, 0x180, 0x10)) {
            if (!AnimateEntity(D_us_80181234, self)) {
                TF(main)->stomping = 0;
                self->step = 1;
            }
            if (self->pose == 2 && self->poseTimer == 0) {
                g_api.PlaySfx(0x819);
            }
            if (self->pose >= 3) {
                if (self->poseTimer & 2) {
                    self->posX.i.hi++;
                } else {
                    self->posX.i.hi--;
                }
            }
        } else {
            self->pose = 0;
            self->poseTimer = 0;
        }
        break;
    case 2:
        self->animCurFrame = 2;
        if (func_us_801B15BC(
                &self->rotate,
                D_us_801811CC[self->ext.olroxLaser.unkA8].headAngle, 0x10)) {
            self->ext.olroxLaser.unkA9 = 1;
            self->step = 3;
            self->step_s = 0;
        }
        break;
    case 3:
        switch (self->step_s) {
        case 0:
            entity = AllocEntity(&g_Entities[64], &g_Entities[256]);
            if (entity == NULL) {
                // BUG: falls back to the main body entity
                entity = main;
                TF(main)->laserBusy = 1;
                self->ext.olroxLaser.unkA9 = 0;
                self->step_s++;
            }
            CreateEntityFromEntity(E_LASER_ATTACK, self, entity);
            entity->ext.olroxLaser.next = self;
            entity->params = self->ext.olroxLaser.unkA8 & 1;
            entity->facingLeft = self->facingLeft;
            TF(main)->laserBusy = 1;
            self->animCurFrame = 3;
            self->step_s++;
            break;
        case 1:
            if (!self->ext.olroxLaser.unkA9) {
                TF(main)->laserBusy = 0;
            }
            break;
        }
        break;
    case 4:
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 4;
            dx = self->posX.i.hi - PLAYER.posX.i.hi;
            dy = self->posY.i.hi - PLAYER.posY.i.hi;
            if (self->facingLeft) {
                dx = -dx;
            }
            angle = -ratan2(-dy, dx) + 0x100;
            if (angle > 0x380) {
                angle = 0x380;
            }
            if (angle < -0x180) {
                angle = 0x180;
            }
            TF_HEAD(self)->target = angle;
            self->step_s++;
            // fallthrough
        case 1:
            angle = TF_HEAD(self)->target;
            ret = func_us_801B15BC(&self->rotate, angle, 0x20);
            ret += func_us_801B163C(
                &TF(main)->angle, (s16)(-(angle + 0x180) / 2), 0x20);
            if (ret == 2) {
                TF(main)->fireBusy = 1;
                self->step_s = 0;
                self->step = 9;
            }
            break;
        }
        break;
    case 9:
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 4;
            fire = &D_us_801811EC[self->ext.olroxLaser.scaleIndex];
            TF_HEAD(self)->duration = fire->duration;
            TF_HEAD(self)->dir = 0;
            self->step_s++;
            TF_HEAD(self)->timer = fire->interval;
            // fallthrough
        case 1:
            fire = &D_us_801811EC[self->ext.olroxLaser.scaleIndex];
            if (!--TF_HEAD(self)->timer) {
                TF_HEAD(self)->timer = fire->interval;
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_FIREBALL_ATTACK, self, entity);
                    entity->ext.olroxLaser.next = self;
                    entity->ext.olroxLaser.scaleIndex =
                        self->ext.olroxLaser.scaleIndex;
                    entity->facingLeft = self->facingLeft;
                }
            }
            if (TF_HEAD(self)->dir) {
                self->rotate += fire->speed;
            } else {
                self->rotate -= fire->speed;
            }
            if (self->rotate < TF_HEAD(self)->target - fire->range) {
                TF_HEAD(self)->dir = 1;
            }
            if (TF_HEAD(self)->target + fire->range < self->rotate) {
                TF_HEAD(self)->dir = 0;
            }
            if (self->rotate < -0x180) {
                TF_HEAD(self)->dir = 1;
            }
            if (self->rotate > 0x380) {
                TF_HEAD(self)->dir = 0;
            }
            TF(main)->angle = -(self->rotate + 0x180) / 2;
            if (!--TF_HEAD(self)->duration) {
                self->step_s = 0;
                self->step = 1;
                TF(main)->fireBusy = 0;
            }
            break;
        }
        break;
    case 10:
        self->animCurFrame = 1;
        break;
    case 8:
        switch (self->step_s) {
        case 0:
            primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
            if (primIndex != -1) {
                prim = &g_PrimBuf[primIndex];
                self->primIndex = primIndex;
                TF_HEAD(self)->prim = prim;
                self->flags |= FLAG_HAS_PRIMS;
                func_us_801B13A8(prim);
                g_api.func_800EB758(self->posX.i.hi, self->posY.i.hi, self,
                                    self->drawFlags, (POLY_GT4*)prim,
                                    self->facingLeft);
                prim->tpage = 0x14;
                prim->clut += 0x20B;
                prim->drawMode = DRAW_UNK02;
                prim->priority = self->zPriority;
            }
            self->step_s++;
            break;
        case 1:
            self->palette = 0x219;
            self->unk5A = 0x54;
            if (TF(main)->dying) {
                TF_HEAD(self)->prim->drawMode = DRAW_HIDE;
                self->step_s++;
            }
            break;
        case 2:
            self->palette = 0x8219;
            self->unk5A = 0x54;
            if (TF(main)->dying == 3) {
                prim = TF_HEAD(self)->prim;
                self->animCurFrame = 3;
                func_us_801B13A8(prim);
                g_api.func_800EB758(self->posX.i.hi, self->posY.i.hi, self,
                                    self->drawFlags, (POLY_GT4*)prim,
                                    self->facingLeft);
                prim->tpage = 0x15;
                prim->clut = 0x219;
                prim->drawMode = DRAW_UNK02;
                prim->priority = self->zPriority;
                self->step_s++;
            }
            break;
        }
        break;
    }
    self->hitboxOffX = -((rcos(self->rotate) * 12) >> 12);
    self->hitboxOffY = -((rsin(self->rotate) * 12) >> 12);
    if (!D_us_801CE5B0 && self->step < 8 && !(Random() & 0x3F)) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_OLROX_DROOL, self, entity);
            entity->ext.olroxDrool.entity = self;
            entity->params = (Random() & 7) + 0x10;
        }
    }
}

static s32 func_us_801B5D6C(s16 (*joints)[2], s16* targets, s16 speed) {
    s32 done = 1;
    s16 cur;
    s32 i;

    for (i = 0; i < 8; i++, joints++, targets++) {
        cur = (*joints)[0];
        if (abs(cur - *targets) < speed) {
            (*joints)[0] = *targets;
        } else {
            done = 0;
            if (*targets < cur) {
                (*joints)[0] = cur - speed;
            }
            if ((*joints)[0] < *targets) {
                (*joints)[0] += speed;
            }
        }
    }
    return done;
}

static s32 func_us_801B5E08(s16 (*joints)[2], s16* targets, s16 speed) {
    s32 done = 1;
    s32 i;

    for (i = 0; i < 8; i++, joints++, targets++) {
        if (!func_us_801B163C(*joints, *targets, speed)) {
            done = 0;
        }
    }
    return done;
}

// Draws the tail as a strip of 8 quads
static void func_us_801B5E8C(void) {
    Entity* self = g_CurrentEntity;
    Primitive* prim;
    s16* joint;
    s16 prevAngle, angle, cur;
    s16 posX, posY;
    s16 outX, outY, inX, inY;
    s16 prevOutX, prevOutY, prevInX, prevInY;
    s16 outLen, inLen, len;
    s32 scale;
    s32 i;

    prevAngle = -0x180;
    posX = self->posX.i.hi;
    posY = self->posY.i.hi;
    joint = &TF_TAIL(self)->joints[0][0];
    prim = TF_TAIL(self)->prim;
    outX = outY = inX = inY = 0;
    for (i = 0; i < 9; i++) {
        cur = *joint;
        angle = (prevAngle + cur) / 2 + 0x400;
        outLen = ((rcos((s16)(angle - cur)) * 6) >> 12) + 8;
        inLen = 8 - ((rcos((s16)(angle - cur)) * 6) >> 12);
        prevOutX = outX;
        prevOutY = outY;
        prevInX = inX;
        prevInY = inY;
        if (!i) {
            inLen = 0xC;
            angle = 0x260;
        }
        scale = TF_TAIL(self)->scale / 2 + 0x800;
        outLen = (outLen * scale) >> 12;
        inLen = (inLen * scale) >> 12;
        if (self->facingLeft) {
            angle = 0x800 - angle;
        }
        outX = posX + ((outLen * rcos(angle)) >> 12);
        outY = posY - ((outLen * rsin(angle)) >> 12);
        inX = posX - ((inLen * rcos(angle)) >> 12);
        inY = posY + ((inLen * rsin(angle)) >> 12);
        len = (TF_TAIL(self)->scale * 11) >> 12;
        if (!self->facingLeft) {
            posX += (len * rcos(*joint)) >> 12;
        } else {
            posX -= (len * rcos(*joint)) >> 12;
        }
        posY -= (len * rsin(*joint)) >> 12;
        prevAngle = *joint;
        if (i) {
            prim->tpage = 0x14;
            prim->clut = 0x20B;
            prim->x0 = prevOutX;
            prim->x2 = prevInX;
            prim->y0 = prevOutY;
            prim->y2 = prevInY;
            prim->x3 = inX;
            prim->x1 = outX;
            prim->y3 = inY;
            prim->y1 = outY;
            prim = prim->next;
        }
        joint += 2;
    }
}

// tail
void func_us_801B619C(Entity* self) {
    Entity* main = self - TF_TAIL_OFS;
    Primitive* prim;
    s16 primIndex;
    s32 i;
    u8 u;

    self->facingLeft = main->facingLeft;
    self->palette = main->palette;
    if (!D_us_801CE5B0) {
        self->posX.val = main->posX.val;
        self->posY.val = main->posY.val;
        if (!self->facingLeft) {
            self->posX.i.hi += 4;
        } else {
            self->posX.i.hi -= 4;
        }
    }
    if (self->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180708);
        self->hitboxState = 0;
        self->animCurFrame = 0;
        self->flags |= FLAG_POS_CAMERA_LOCKED;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 8);
        if (primIndex == -1) {
            self->step = 0;
            break;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        TF_TAIL(self)->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        u = 0x80;
        for (i = 0; i < 8; i++) {
            prim->u0 = prim->u2 = u;
            prim->tpage = 0x14;
            prim->clut = 0x20B;
            prim->u1 = prim->u3 = u + 0xB;
            prim->v0 = prim->v1 = 0x50;
            prim->v2 = prim->v3 = 0x60;
            prim->priority = 0xA1;
            prim->drawMode = DRAW_UNK02;
            prim = prim->next;
            u += 0xB;
        }
        for (i = 7; i >= 0; i--) {
            TF_TAIL(self)->joints[7 - i][0] = -0x180;
        }
        TF_TAIL(self)->scale = 0;
        // fallthrough
    case 1:
        if (!D_us_801CE5B0) {
            self->step++;
        }
        break;
    case 2:
        if (TF_TAIL(self)->scale < 0x1000) {
            TF_TAIL(self)->scale += 0x200;
        } else {
            TF_TAIL(self)->scale = 0x1000;
            self->step++;
        }
        break;
    case 3:
        if (func_us_801B5E08(TF_TAIL(self)->joints,
                             D_us_801812D0[TF_TAIL(self)->pose], 0x20) &
            0xFF) {
            TF_TAIL(self)->pose ^= 1;
        }
        break;
    case 4:
        func_us_801B5E08(TF_TAIL(self)->joints, D_us_80181288, 0x20);
        break;
    case 6:
        func_us_801B5E08(TF_TAIL(self)->joints, D_us_80181298, 0x60);
        break;
    case 7:
        func_us_801B5E08(TF_TAIL(self)->joints, D_us_801812A8, 0x80);
        break;
    case 8:
        func_us_801B5E08(TF_TAIL(self)->joints, D_us_801812B8, 0x60);
        break;
    case 9:
        if (func_us_801B5D6C(TF_TAIL(self)->joints,
                             D_us_801812D0[TF_TAIL(self)->pose], 0x40) &
            0xFF) {
            TF_TAIL(self)->pose ^= 1;
        }
        break;
    }
    func_us_801B5E8C();
    if (self->palette & 0x8000) {
        for (prim = TF_TAIL(self)->prim; prim != NULL; prim = prim->next) {
            prim->clut = self->palette & 0xFFF;
        }
    }
    if (TF(main)->dying) {
        for (prim = TF_TAIL(self)->prim; prim != NULL; prim = prim->next) {
            prim->tpage = 0x15;
            prim->clut = 0x219;
        }
    }
}

// Moves the four arm joints, returns true once all of them are in place
static s32 func_us_801B6520(OlroxApproach a0, OlroxApproach a1,
                            OlroxApproach a2, OlroxApproach a3) {
    OlroxTFArms* arms = TF_ARMS(g_CurrentEntity);
    s32 done;

    done = func_us_801B163C(arms->joints[2], a2.target, a2.speed);
    done += func_us_801B163C(arms->joints[3], a3.target, a3.speed);
    done += func_us_801B163C(arms->joints[0], a0.target, a0.speed);
    done += func_us_801B163C(arms->joints[1], a1.target, a1.speed);
    return done == 4;
}

#define APPROACH(target, speed) ((OlroxApproach){(target), (speed)})

// Draws one arm and places the matching hand
static Primitive* func_us_801B65C0(
    s16 originX, s16 originY, s16 (*joints)[2], Primitive* prim, u8 side) {
    Entity* self = g_CurrentEntity;
    Entity* hand;
    u16 clut;
    u8 prioOfs;
    s8 armLen;
    s16 x, y, angle, len;
    s16 ax, ay, bx, by, cx, cy, dx, dy, ex, ey, fx, fy;

    if (side) {
        clut = 0x20B;
        armLen = TF_ARMS(self)->armLen[1];
    } else {
        clut = 0x218;
        armLen = TF_ARMS(self)->armLen[0];
    }
    prioOfs = side * 6;
    x = originX;
    y = originY;

    angle = joints[0][0];
    if (self->facingLeft) {
        angle = 0x800 - angle;
    }
    ax = x + (-(rcos(angle) * 8) >> 12);
    bx = x + ((rcos(angle) * 8) >> 12);
    by = y + ((rsin(angle) * 8) >> 12);
    ay = y + (-(rsin(angle) * 8) >> 12);

    len = ((u8)armLen + 0x100) / 8;
    if (!self->facingLeft) {
        x += (len * rcos(joints[0][0] - 0x400)) >> 12;
    } else {
        x -= (len * rcos(joints[0][0] - 0x400)) >> 12;
    }
    y += (-len * rsin(joints[0][0] - 0x400)) >> 12;
    angle = joints[0][0];
    if (self->facingLeft) {
        angle = 0x800 - angle;
    }
    cx = x + (-(rcos(angle) * 6) >> 12);
    cy = y + ((rsin(angle) * 6) >> 12);
    dx = x + ((rcos(angle) * 6) >> 12);
    dy = y + (-(rsin(angle) * 6) >> 12);

    prim->tpage = 0x14;
    prim->u0 = prim->u1 = 0xC8;
    prim->u2 = 0xD4;
    prim->u3 = 0xDA;
    prim->v0 = 0x1E;
    prim->v1 = 4;
    prim->v2 = 0x20;
    prim->v3 = 4;
    prim->clut = clut;
    prim->x0 = cx;
    prim->x1 = ax;
    prim->x2 = dx;
    prim->x3 = bx;
    prim->y0 = cy;
    prim->y1 = by;
    prim->y2 = dy;
    prim->y3 = ay;
    prim->drawMode = DRAW_UNK02;
    prim->priority = prioOfs + ((u16)D_us_801CE56C - 3);
    prim = prim->next;

    if (!self->facingLeft) {
        x += (rcos(joints[1][0] + 0x400) * 6) >> 12;
    } else {
        x -= (rcos(joints[1][0] + 0x400) * 6) >> 12;
    }
    y += -(rsin(joints[1][0] + 0x400) * 6) >> 12;
    angle = joints[1][0];
    if (self->facingLeft) {
        angle = 0x800 - angle;
    }
    cx = x + (-(rcos(angle) * 6) >> 12);
    cy = y + ((rsin(angle) * 6) >> 12);
    dx = x + ((rcos(angle) * 6) >> 12);
    dy = y + (-(rsin(angle) * 6) >> 12);

    len = (((u8)armLen + 0x100) * 24) / 256;
    if (!self->facingLeft) {
        x += (len * rcos(joints[1][0] - 0x400)) >> 12;
    } else {
        x -= (len * rcos(joints[1][0] - 0x400)) >> 12;
    }
    y += (-len * rsin(joints[1][0] - 0x400)) >> 12;
    angle = joints[1][0];
    if (self->facingLeft) {
        angle = 0x800 - angle;
    }
    ex = x + (-(rcos(angle) * 6) >> 12);
    ey = y + ((rsin(angle) * 6) >> 12);
    fx = x + ((rcos(angle) * 6) >> 12);
    fy = y + (-(rsin(angle) * 6) >> 12);

    hand = side ? self + 2 : self + 1;
    hand->posX.i.hi = x;
    hand->posY.i.hi = y;
    hand->animCurFrame = TF_ARMS(self)->handFrame[side ? 1 : 0];
    if (TF_ARMS(self)->rotateHands) {
        hand->rotate = -(joints[1][0] + 0x300);
    } else {
        hand->rotate = 0;
    }

    prim->tpage = 0x14;
    prim->clut = clut;
    prim->u0 = 0x88;
    prim->u1 = 0x9C;
    prim->u2 = 0x8C;
    prim->u3 = 0xA2;
    prim->v0 = 0x74;
    prim->v1 = 0x6D;
    prim->v2 = 0x80;
    prim->v3 = 0x77;
    prim->x0 = ex;
    prim->x1 = cx;
    prim->x2 = fx;
    prim->x3 = dx;
    prim->y0 = ey;
    prim->y1 = cy;
    prim->y2 = fy;
    prim->y3 = dy;
    prim->drawMode = DRAW_UNK02;
    prim->priority = prioOfs + ((u16)D_us_801CE56C - 3);
    return prim->next;
}

// arms
void func_us_801B6CA4(Entity* self) {
    Entity* main = self - TF_ARMS_OFS;
    OlroxTFArms* arms = TF_ARMS(self);
    Primitive* prim;
    s16 primIndex;
    s16 speed1, speed2, target1, target2;
    s32 ret;

    self->facingLeft = main->facingLeft;
    self->palette = main->palette;
    if (!TF(main)->dying) {
        arms->handsHurt = 0;
        if (self[1].flags & FLAG_DEAD) {
            arms->handsHurt = 2;
        }
        if (self[2].flags & FLAG_DEAD) {
            arms->handsHurt |= 1;
        }
        if (arms->handsHurt && self->step != 10) {
            self->step = 9;
        }
    }
    if (self->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180708);
        self->hitboxWidth = 0xC;
        self->hitboxHeight = 8;
        self->hitboxOffX = 0;
        self->hitboxOffY = 8;
        self->animCurFrame = 0;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 6);
        if (primIndex == -1) {
            self->step = 0;
            break;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        arms->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        arms->joints[2][0] = 0x180;
        arms->joints[3][0] = -0x180;
        arms->joints[0][0] = 0x100;
        arms->joints[1][0] = -0x80;
        arms->handFrame[1] = 0x1D;
        arms->handFrame[0] = 0x21;
        arms->rotateHands = 0;
        // fallthrough
    case 1:
        self->step_s = 0;
        self[2].attack = g_api.enemyDefs[0x37].attack;
        break;
    case 2:
        arms->rotateHands = 0;
        switch (self->step_s) {
        case 0:
            arms->handFrame[1] = 0x1D;
            arms->handFrame[0] = 0x21;
            if (func_us_801B6520(APPROACH(0x100, 0x10), APPROACH(-0x300, 0x10),
                                 APPROACH(-0x40, 0x10),
                                 APPROACH(-0x200, 0x10))) {
                self->step_s = 1;
            }
            break;
        case 1:
            if (func_us_801B6520(APPROACH(0, 0x10), APPROACH(-0x280, 0x10),
                                 APPROACH(0x80, 0x10),
                                 APPROACH(-0x300, 0x10))) {
                self->step_s = 0;
            }
            break;
        }
        break;
    case 3:
        arms->handFrame[1] = 0x1F;
        arms->handFrame[0] = 0x20;
        arms->rotateHands = 0;
        if (func_us_801B6520(APPROACH(0x100, 0x10), APPROACH(-0x280, 0x10),
                             APPROACH(0x100, 0x10), APPROACH(-0x300, 0x10))) {
            self->step = 1;
        }
        break;
    case 4:
        arms->handFrame[1] = 0x1F;
        arms->handFrame[0] = 0x20;
        arms->rotateHands = 0;
        ret = func_us_801B6520(APPROACH(0x200, 0x60), APPROACH(-0x300, 0x60),
                               APPROACH(-0x240, 0x60), APPROACH(-0x240, 0x60));
        if (arms->joints[2][0] < 0x80) {
            TF(main)->neckPose = 0;
        }
        if (arms->joints[2][0] < -0x80) {
            TF(main)->neckPose = 3;
        }
        self[2].attack = g_api.enemyDefs[0x39].attack;
        if (ret) {
            if (arms->armLen[1] < 0x60) {
                arms->armLen[1] += 0x10;
            } else {
                self[2].attack = g_api.enemyDefs[0x37].attack;
                self->step = 5;
                PlaySfxPositional(0x7C8);
            }
        }
        break;
    case 5:
        arms->rotateHands = 0;
        if (arms->armLen[1] > 0) {
            arms->armLen[1] -= 0xC;
        } else {
            arms->armLen[1] = 0;
            self->step = 1;
        }
        break;
    case 11:
        arms->rotateHands = 1;
        arms->handFrame[1] = 0x1F;
        arms->handFrame[0] = 0x20;
        if (func_us_801B6520(APPROACH(-0x200, 0x20), APPROACH(-0x400, 0x20),
                             APPROACH(0x280, 0x18), APPROACH(-0x200, 0x18))) {
            self->step = 1;
        }
        break;
    case 12:
        arms->rotateHands = 1;
        arms->handFrame[1] = 0x1D;
        arms->handFrame[0] = 0x21;
        if (arms->armLen[1] < 0x40) {
            arms->armLen[1] += 8;
        }
        ret = func_us_801B6520(APPROACH(0x180, 0x40), APPROACH(-0x200, 0x40),
                               APPROACH(-0x500, 0x60), APPROACH(-0x780, 0x78));
        self[2].attack = g_api.enemyDefs[0x3A].attack;
        if (ret) {
            self->step = 1;
        }
        break;
    case 13:
        arms->rotateHands = 1;
        arms->armLen[1] -= 0xC;
        if (arms->armLen[1] < 0) {
            arms->armLen[1] = 0;
        }
        arms->handFrame[1] = 0x1F;
        arms->handFrame[0] = 0x21;
        if (func_us_801B6520(APPROACH(0, 0x40), APPROACH(-0x80, 0x40),
                             APPROACH(0x100, 0x40), APPROACH(-0x200, 0x50))) {
            self->step = 1;
        }
        break;
    case 14:
        arms->rotateHands = 1;
        arms->handFrame[1] = 0x1F;
        arms->handFrame[0] = 0x20;
        arms->armLen[1] -= 0xC;
        if (arms->armLen[1] < 0) {
            arms->armLen[1] = 0;
        }
        if (func_us_801B6520(APPROACH(0x100, 0x20), APPROACH(-0x300, 0x20),
                             APPROACH(-0x80, 0x20), APPROACH(-0x200, 0x20))) {
            self->step = 1;
        }
        break;
    case 6:
        arms->handFrame[1] = 0x1F;
        arms->handFrame[0] = 0x20;
        arms->rotateHands = 0;
        if (func_us_801B6520(APPROACH(0, 0x20), APPROACH(-0x380, 0x20),
                             APPROACH(0x80, 0x20), APPROACH(-0x280, 0x20))) {
            self->step = 1;
        }
        break;
    case 7:
        arms->rotateHands = 1;
        arms->handFrame[1] = 0x1D;
        arms->handFrame[0] = 0x20;
        if (func_us_801B6520(APPROACH(0, 0x10), APPROACH(-0x200, 0x10),
                             APPROACH(0x100, 0x10), APPROACH(-0x100, 0x10))) {
            self->step = 1;
        }
        break;
    case 8:
        arms->rotateHands = 1;
        arms->handFrame[1] = 0x1F;
        arms->handFrame[0] = 0x21;
        if (func_us_801B6520(APPROACH(0, 0x20), APPROACH(-0x140, 0x20),
                             APPROACH(0x180, 0x20), APPROACH(-0x100, 0x20))) {
            self->step = 1;
        }
        break;
    case 15:
        arms->rotateHands = 1;
        arms->handFrame[1] = 0x1D;
        arms->handFrame[0] = 0x20;
        if (func_us_801B6520(APPROACH(0, 0x20), APPROACH(-0x200, 0x20),
                             APPROACH(0x80, 0x20), APPROACH(-0x100, 0x20))) {
            self->step = 1;
        }
        break;
    case 9: // a hand has been destroyed
        arms->rotateHands = 1;
        arms->handFrame[0] = 0x20;
        if (arms->handsHurt == 2) {
            speed1 = 0x18;
            target1 = 0;
        } else {
            speed1 = 0x10;
            target1 = 0x200;
        }
        arms->handFrame[1] = 0x1D;
        if (arms->handsHurt == 1) {
            speed2 = 0x18;
            target2 = 0;
        } else {
            speed2 = 0x10;
            target2 = 0x200;
        }
        func_us_801B6520(APPROACH(target1, speed1), APPROACH(-0x100, speed1),
                         APPROACH(target2, speed2), APPROACH(-0x100, speed2));
        if (!arms->handsHurt) {
            self->step = 1;
        }
        break;
    case 10:
        switch (self->step_s) {
        case 0:
            if (TF(main)->dying == 2) {
                self->step_s++;
            }
            break;
        case 1:
            arms->rotateHands = 1;
            arms->handFrame[1] = 0x1D;
            arms->handFrame[0] = 0x21;
            if (func_us_801B6520(APPROACH(-0x100, 0x10),
                                 APPROACH(-0x400, 0x10),
                                 APPROACH(-0x200, 0x10),
                                 APPROACH(-0x300, 0x10))) {
                self->step_s++;
            }
            if (TF(main)->dying == 3) {
                self->step_s = 3;
            }
            break;
        case 2:
            arms->rotateHands = 1;
            arms->handFrame[1] = 0x1D;
            arms->handFrame[0] = 0x21;
            func_us_801B6520(APPROACH(0x100, 0xA), APPROACH(-0x200, 0xA),
                             APPROACH(0, 6), APPROACH(-0x100, 6));
            if (TF(main)->dying == 3) {
                self->step_s = 3;
            }
            break;
        }
        break;
    }
    prim = func_us_801B65C0(self->posX.i.hi, self->posY.i.hi, &arms->joints[2],
                            arms->prim, 1);
    func_us_801B65C0(
        self->posX.i.hi, self->posY.i.hi, &arms->joints[0], prim, 0);
    if (self->palette & 0x8000) {
        for (prim = arms->prim; prim != NULL; prim = prim->next) {
            prim->clut = self->palette & 0xFFF;
        }
    }
    if (TF(main)->dying) {
        for (prim = arms->prim; prim != NULL; prim = prim->next) {
            prim->tpage = 0x15;
            prim->clut = 0x219;
        }
    }
}

// hands
void func_us_801B76E4(Entity* self) {
    Entity* main;
    Primitive* prim;
    s16 primIndex;
    u16 palette;

    main = self->params ? self - 8 : self - 9;
    self->facingLeft = main->facingLeft;
    palette = main->palette;
    if (palette & 0x8000) {
        self->palette = palette;
    } else if (!(self->flags & 0xF)) {
        self->palette = 0x20B;
    }
    if (self->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180708);
        self->hitPoints = 0x80;
        self->hitboxWidth = 0xC;
        self->hitboxHeight = 6;
        self->parent = NULL;
        self->drawFlags |= ENTITY_ROTATE;
        if (self->params) {
            self->animCurFrame = 0x20;
            self->zPriority = D_us_801CE56C - 4;
        } else {
            self->animCurFrame = 0x1F;
            self->zPriority = D_us_801CE56C + 2;
        }
        break;
    case 1:
        if (self->flags & FLAG_DEAD) {
            self->hitboxState = 0;
            self->step = 2;
            self->step_s = 0;
        }
        break;
    case 2:
        switch (self->step_s) {
        case 0:
            MakeExplosions();
            TF_HAND(self)->timer = 0x100;
            self->animCurFrame = 0;
            self->scaleY = 0;
            self->scaleX = 0;
            self->drawFlags |= ENTITY_SCALEX | ENTITY_SCALEY;
            self->flags &= ~FLAG_DEAD;
            self->step_s++;
            break;
        case 1:
            if (!--TF_HAND(self)->timer) {
                self->step_s++;
            }
            break;
        case 2:
            self->scaleX += 2;
            self->scaleY = self->scaleX;
            if (self->scaleX > 0x100) {
                self->hitPoints = 0x80;
                self->drawFlags = ENTITY_ROTATE;
                self->hitboxState = 3;
                self->palette = 0x20B;
                self->step = 1;
                self->hitEffect = 0;
                self->step_s = 0;
                self->flags &= ~FLAG_DEAD;
            }
            break;
        }
        break;
    case 3:
        self->drawFlags = ENTITY_ROTATE;
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
            if (primIndex == -1) {
                self->step_s = 2;
                break;
            }
            prim = &g_PrimBuf[primIndex];
            self->primIndex = primIndex;
            TF_HAND(self)->prim = prim;
            self->flags |= FLAG_HAS_PRIMS;
            func_us_801B13A8(prim);
            g_api.func_800EB758(self->posX.i.hi, self->posY.i.hi, self,
                                self->drawFlags, (POLY_GT4*)prim,
                                self->facingLeft);
            prim->tpage = 0x14;
            prim->clut += 0x20B;
            prim->drawMode = DRAW_UNK02;
            prim->priority = self->zPriority;
            self->step_s++;
            break;
        case 1:
            self->palette = 0x219;
            self->unk5A = 0x54;
            if (TF(main)->dying) {
                TF_HAND(self)->prim->drawMode = DRAW_HIDE;
                self->step_s++;
            }
            break;
        case 2:
            self->palette = 0x8219;
            self->unk5A = 0x54;
            if (TF(main)->dying == 3) {
                prim = TF_HAND(self)->prim;
                func_us_801B13A8(prim);
                g_api.func_800EB758(self->posX.i.hi, self->posY.i.hi, self,
                                    self->drawFlags, (POLY_GT4*)prim,
                                    self->facingLeft);
                prim->tpage = 0x15;
                prim->clut = 0x219;
                prim->drawMode = DRAW_UNK02;
                prim->priority = self->zPriority;
                self->step_s++;
            }
            break;
        }
        break;
    }
    if (self->params) {
        self->zPriority = D_us_801CE56C - 4;
    } else {
        self->zPriority = D_us_801CE56C + 2;
    }
    if (main->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    self->hitboxOffX = -((rcos(self->rotate - 0x100) * 10) >> 12);
    self->hitboxOffY = -((rsin(self->rotate - 0x100) * 10) >> 12);
}

// knees, following the leg data
void func_us_801B7BAC(Entity* self) {
    OlroxLeg* leg;

    if (self->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    if (!self->step) {
        InitializeEntity(D_us_80180708);
        self->hitboxOffY = 4;
        self->hitboxWidth = 4;
        self->animCurFrame = 0;
        self->hitboxHeight = 0x10;
    }
    leg = TF(self)->leg;
    self->posX.i.hi = leg->kneeX.i.hi;
    self->posY.i.hi = leg->kneeY.i.hi;
}

// Finds `count` consecutive hidden primitives
static Primitive* func_us_801B7C44(Primitive* prim, u8 count) {
    Primitive* first;
    s32 i;

    while (prim != NULL) {
        if (prim->drawMode & DRAW_HIDE) {
            first = prim;
            for (i = 1; i < count; i++) {
                prim = prim->next;
                if (prim == NULL) {
                    return NULL;
                }
                if (!(prim->drawMode & DRAW_HIDE)) {
                    break;
                }
            }
            if (i == count) {
                return first;
            }
        }
        prim = prim->next;
    }
    return NULL;
}

// Copies the primitives of an entity into the dissolve pool
static Primitive* TrueFormCopyPrims(Primitive* dst, Primitive* src, u16 clut,
                                    bool tint) {
    Primitive* next;

    while (src != NULL) {
        next = dst->next;
        *dst = *src;
        dst->next = next;
        dst->clut = clut;
        if (tint) {
            dst->r0 = dst->r1 = dst->r2 = dst->r3 = 0x80;
            dst->priority += 8;
        }
        src = src->next;
        dst = dst->next;
    }
    return dst;
}

// death effect: the body breaks apart and dissolves
void func_us_801B7CC8(Entity* self) {
    Entity* main = self - TF_DEATH_OFS;
    OlroxTFDeath* death = TF_DEATH(self);
    Entity* entity;
    Primitive* prim;
    Primitive* dst;
    s16 primIndex;
    s32 i, count;
    u8 color;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180708);
        self->zPriority = D_us_801CE56C + 8;
        self->animCurFrame = 0;
        self->flags |= FLAG_POS_CAMERA_LOCKED;
        primIndex = g_api.func_800EDB58(PRIM_GT4, 0x108);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        dst = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        death->prim = dst;
        self->flags |= FLAG_HAS_PRIMS;
        for (i = 0; i < 10; i++) {
            dst = TrueFormCopyPrims(dst, TF(main + i)->prim, 0x20B, true);
        }
        death->pool = dst;
        while (dst != NULL) {
            dst->drawMode = DRAW_HIDE;
            dst = dst->next;
        }
        break;
    case 1:
        count = 0;
        for (prim = death->prim; prim != death->pool; prim = prim->next) {
            if (prim->drawMode & DRAW_HIDE) {
                continue;
            }
            count++;
            if (Random() & 0xF) {
                continue;
            }
            dst = func_us_801B7C44(death->pool, 4);
            if (dst != NULL) {
                prim->r0 = 0xB0;
                prim->r1 = 0x98;
                prim->r2 = 0xA0;
                prim->r3 = 0x88;
                UnkRecursivePrimFunc2(prim, 1, dst, (u8*)SP(0));
                prim->drawMode = DRAW_HIDE;
            }
        }
        for (prim = death->pool; prim != NULL; prim = prim->next) {
            if (prim->drawMode & DRAW_HIDE) {
                continue;
            }
            count++;
            if (prim->r0 > 0x80) {
                prim->r0--;
                if (prim->r0 != 0x80) {
                    continue;
                }
                dst = func_us_801B7C44(death->pool, 4);
                if (dst != NULL) {
                    prim->r0 = 0x20;
                    prim->r1 = 0x14;
                    prim->r2 = 0x18;
                    prim->r3 = 0x10;
                    UnkRecursivePrimFunc2(prim, 1, dst, (u8*)SP(0));
                    prim->drawMode = DRAW_HIDE;
                    prim->p3 = 0;
                } else {
                    prim->r0 += (Random() & 0x1F) + 2;
                }
            } else {
                prim->r0--;
                prim->p3 = 0;
                if (!prim->r0) {
                    prim->drawMode = DRAW_HIDE;
                } else if (prim->r0 < 0x10) {
                    prim->y0 -= (prim->r0 >> 1) - 8;
                    prim->y1 -= (prim->r0 >> 1) - 8;
                    prim->y2 -= (prim->r0 >> 1) - 8;
                    prim->y3 -= (prim->r0 >> 1) - 8;
                }
            }
        }
        if (!count) {
            TF(main)->dying = 2;
            self->step++;
        }
        break;
    case 2:
        if (TF(main)->dying != 3) {
            break;
        }
        prim = death->prim;
        death->flash = prim;
        prim->type = PRIM_G4;
        prim->r0 = prim->g0 = prim->b0 = 0;
        prim->r1 = prim->g1 = prim->b1 = 0;
        prim->p1 = prim->type;
        prim->r2 = prim->g2 = prim->b2 = 0xF0;
        prim->r3 = prim->r2;
        prim->g3 = prim->g2;
        prim->b3 = prim->b2;
        prim->p3 = prim->p2;
        prim->y2 = prim->y3 = 0xF0;
        prim->y0 = prim->y1 = 0;
        prim->drawMode = 0x77;
        prim->x0 = prim->x1 = prim->x2 = prim->x3 = self->posX.i.hi;
        prim->priority = D_us_801CE56C + 0x10;
        dst = prim->next;
        death->prim = dst;
        for (i = 0; i < 6; i++) {
            dst = TrueFormCopyPrims(
                dst, TF(main + D_us_801812D8[i])->prim, 0x219, false);
        }
        death->pool = dst;
        while (dst != NULL) {
            dst->drawMode = DRAW_HIDE;
            dst = dst->next;
        }
        death->count = 0x28;
        self->step++;
        break;
    case 3:
        prim = death->flash;
        prim->y2 = prim->y3 = 0xF0;
        prim->y0 = prim->y1 = 0;
        prim->x0 -= 6;
        prim->x2 = prim->x0;
        prim->x1 += 6;
        prim->x3 = prim->x1;
        if (!(prim->x0 & 0xF000) || !(prim->x3 & 0xF00)) {
            break;
        }
        prim->x0 = prim->x2 = 0;
        prim->x1 = prim->x3 = 0x100;
        prim->drawMode &= ~DRAW_UNK02;
        death->timer = 1;
        self->step++;
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_UNK_3E, self, entity);
            entity->facingLeft = self->facingLeft;
            death->child = entity;
        }
        break;
    case 4:
        if (!--death->timer) {
            g_api.PlaySfx(0x60D);
            prim = death->prim;
            death->timer = 4;
            for (i = 0; i < death->count; i++) {
                prim = prim->next;
            }
            dst = FindFirstUnkPrim2(death->pool, 4);
            if (dst != NULL) {
                prim->p3 = 1;
                prim->r0 = 0x24;
                prim->r1 = 0x28;
                prim->r2 = 0x2C;
                prim->r3 = 0x21;
                UnkRecursivePrimFunc2(prim, 1, dst, (u8*)SP(0));
                prim->drawMode |= DRAW_HIDE;
                if (--death->count < 0) {
                    g_api.PlaySfx(0x636);
                    self->step++;
                }
            }
        }
        // fallthrough
    case 5:
        death->flash->drawMode = (g_Timer & 1) ? 0x71 : DRAW_HIDE;
        count = death->count;
        death->frame++;
        for (prim = death->pool; prim != NULL; prim = prim->next) {
            if (prim->drawMode & DRAW_HIDE) {
                continue;
            }
            prim->r0--;
            count++;
            switch ((prim->r0 & 0xF0) >> 4) {
            case 1:
                if (prim->r0 == 0x10) {
                    dst = FindFirstUnkPrim2(death->pool, 4);
                    if (dst != NULL) {
                        prim->p3 = 3;
                        prim->r0 = 0xD;
                        prim->r1 = 6;
                        prim->r2 = 0xE;
                        prim->r3 = 0x14;
                        UnkRecursivePrimFunc2(prim, 1, dst, (u8*)SP(0));
                        prim->drawMode = DRAW_HIDE;
                        prim->p3 = 0;
                    } else {
                        prim->r0 += Random() & 0xF;
                    }
                }
                prim->y0--;
                prim->y1--;
                prim->y2--;
                prim->y3--;
                break;
            case 0:
                color = prim->r0;
                prim->y0 += (color >> 1) - 8;
                prim->y1 += (color >> 1) - 8;
                prim->y2 += (color >> 1) - 8;
                prim->y3 += (color >> 1) - 8;
                if (!prim->r0) {
                    prim->drawMode = DRAW_HIDE;
                    prim->p3 = 0;
                }
                break;
            case 2:
            case 3:
                if (death->frame & 1) {
                    prim->x0++;
                    prim->x1++;
                    prim->x2++;
                    prim->x3++;
                } else {
                    prim->x0--;
                    prim->x1--;
                    prim->x2--;
                    prim->x3--;
                }
                break;
            }
        }
        if (count < 0) {
            TF(main)->dying = 2;
            if (death->child != NULL) {
                death->child->step++;
            }
            self->step++;
        }
        break;
    case 6:
        prim = death->flash;
        prim->drawMode = 0x71;
        prim->x0 += 2;
        prim->x1 -= 2;
        prim->y0 = prim->y1 = 0;
        prim->y2 = prim->y3 = 0xF0;
        prim->x2 = prim->x0;
        prim->x3 = prim->x1;
        if (prim->x1 < prim->x0) {
            prim->drawMode = DRAW_HIDE;
            death->timer = 1;
            self->step++;
        }
        break;
    case 7:
        entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_LIFE_UP_SPAWN, self, entity);
            entity->params = 0;
            D_us_80181190 = 1;
            stopMusicFlag = 1;
            entity->posY.i.hi -= 0x28;
            currentMusicId = 0x317;
            self->step++;
        }
        break;
    case 8:
        if (!g_api.func_80131F68()) {
            stopMusicFlag = 0;
            g_api.PlaySfx(currentMusicId);
            DestroyEntity(self);
        }
        break;
    }
}
#endif
