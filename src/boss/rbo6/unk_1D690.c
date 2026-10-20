// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo6.h"

// The PC build provides EntityBreakable from e_breakable.c
INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", EntityBreakable);

#ifdef VERSION_PC
// Dracula Lv98 and all his body parts are positioned relative to this slot
#define DRACULA g_Entities[E_BOSS_WEAPON]

extern s32 D_us_801806E4;
extern s32 D_us_801806E8;
extern s32 D_us_801806EC;
extern s32 D_us_801806F0;
extern s32 D_us_801806F4;
extern s32 D_us_801806F8;
extern s32 D_us_801806FC;
extern s32 D_us_80180700;
// declared like the PSX functions of this file do; only the low half is used
extern u16 D_us_80180704;
extern EInit g_EInitDraculaLvl98;
extern u16 D_us_80180540;
extern EInit D_us_8018054C;
extern EInit D_us_80180558;
extern EInit D_us_80180564;
extern EInit D_us_80180570;
extern EInit g_EInitInteractable;
extern u32 g_CutsceneFlags;
extern u8 D_us_801807E0[];
#endif

#ifndef VERSION_PC
// decompiled in src/boss/bo1/e_explosion_flame.c
INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019D260_from_rcen);
#else
// Same as BO1's func_us_8019D260_from_rcen
void func_us_8019D260_from_rcen(void) {
    s16 temp_s3;
    s32 i;
    s8 temp_s4;
    Entity* entity;

    temp_s4 = Random() & 3;
    temp_s3 = ((Random() & 0xF) << 8) - 0x800;
    for (i = 0; i < 6; i++) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_UNK_16, g_CurrentEntity, entity);
            entity->ext.et_801A518C.unk89 = 6 - i;
            entity->ext.et_801A518C.unk88 = temp_s4;
            entity->params = 2;
            entity->ext.et_801A518C.unk84 = temp_s3;
            entity->zPriority = g_CurrentEntity->zPriority + 1;
        }
    }
}
#endif

void func_us_8019D330_from_rcen(Entity* self) {
    typedef struct {
        u16 animSet;
        u16 unk2;
        u16 palette;
        u8 blendMode;
        u8 unused;
#ifdef VERSION_PC
        u8* animation;
#else
        u32 animation;
#endif
    } Borbo6ParticleData;
    extern Borbo6ParticleData D_us_801806B4[];
    extern u16 g_EInitParticle;

    s16 temp_v1_3;
    s16 var_s0;
    s32 temp_a0;
    u16 temp_v1;
    u16 var_s0_2;
    Borbo6ParticleData* temp_v1_2;

    switch (self->step) {
    case 0:
        InitializeEntity(&g_EInitParticle);
        temp_a0 = self->params & 0xF;
        temp_v1_2 = &D_us_801806B4[temp_a0];
        self->palette = temp_v1_2->palette + 0x2E0;
        self->blendMode = temp_v1_2->blendMode;
        self->animSet = temp_v1_2->animSet;
        self->unk5A = temp_v1_2->unk2;
        self->ext.et_801A518C.anim = (u8*)temp_v1_2->animation;
        self->step = temp_a0 + 1;
        temp_v1 = self->params;
        {
            u32 temp_v0;
            temp_v0 = temp_v1 & 0xFF00;
            if (temp_v0 != 0) {
                self->zPriority = temp_v0 >> 8;
            }
        }
        if (self->params & 0xF0) {
            self->palette = PAL_FLAG(PAL_UNK_19F);
            self->blendMode = BLEND_TRANSP;
            self->facingLeft = 1;
        }
        return;

    case 1:
        if (AnimateEntity(self->ext.et_801A518C.anim, self) == 0) {
            DestroyEntity(self);
        }
        break;

    case 2:
        MoveEntity();
        self->velocityY = FIX(-1.0);
        if (AnimateEntity(self->ext.et_801A518C.anim, self) == 0) {
            DestroyEntity(self);
        }
        break;

    case 3:
        if (self->step_s == 0) {
            self->drawFlags = ENTITY_OPACITY;
            self->drawFlags = ENTITY_OPACITY | ENTITY_ROTATE;
            self->opacity = 0x80;
            self->facingLeft = Random() & 1;
            self->rotate = rand() & 0xFFF;
            var_s0 = self->rotate;
            if (self->facingLeft) {
                var_s0 = -self->rotate;
            }
            self->velocityX = rsin((s32)var_s0) * 0x28;
            self->velocityY = -(rcos((s32)var_s0) * 0x28);
            self->ext.et_801A518C.acceleration = (Random() << 4) + 0x1000;
            self->step_s++;
        }
        MoveEntity();
        var_s0_2 = self->rotate;
        self->opacity += 0xFF;
        if (self->facingLeft) {
            var_s0_2 = -var_s0_2;
        }
        self->velocityX += (rsin((s32)(s16)var_s0_2) *
                            (s32)self->ext.et_801A518C.acceleration) >>
                           12;
        self->velocityY += (-(s32)self->ext.et_801A518C.acceleration *
                            rcos((s32)(s16)var_s0_2)) >>
                           12;
        if (AnimateEntity(self->ext.et_801A518C.anim, self) == 0) {
            DestroyEntity(self);
        }
        break;

    case 4:
        if (self->step_s == 0) {
            self->drawFlags = ENTITY_OPACITY;
            self->opacity = 0x80;
            self->facingLeft = Random() & 1;
            self->velocityX = (Random() << 9) + 0xFFFF0000;
            self->velocityY = FIX(-2.5);
            self->ext.et_801A518C.accelerationY = -(Random() << 4) - 0x1000;
            self->step_s++;
        }
        MoveEntity();
        self->velocityY += (s32)self->ext.et_801A518C.accelerationY;
        self->opacity += 0xFF;
        if (AnimateEntity(self->ext.et_801A518C.anim, self) == 0) {
            DestroyEntity(self);
        }
        break;

    default:
        return;
    }
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019DB9C);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019EADC);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019EE30);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019F1CC);
#else
extern u8 D_us_80180708[];
extern u8 D_us_80180710[];
extern u8 D_us_801807A0[];
extern u16 D_us_80180830[];

// Dracula Lv98, the body that spawns and drives all the other parts
void func_us_8019DB9C(Entity* self) {
    ET_Rbo6Dracula* ext = RBO6_EXT(self, ET_Rbo6Dracula);
    Entity* entity;
    s32 i;
    s32 dx;
    s32 dy;
    s16 angle;
    s32 sfx;

    if ((self->hitFlags & 3) && !(Random() & 7)) {
        sfx = 0x85B;
        if (!(Random() & 1)) {
            sfx = 0x85C;
        }
        g_api.PlaySfx(sfx);
    }
    if (self->flags & FLAG_DEAD) {
        if (self->step != 0xC) {
            SetStep(0xC);
        }
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitDraculaLvl98);
        self->zPriority = 0x80;
        self->hitboxOffY = -4;
        for (i = 0; i < 5; i++) {
            entity = &g_Entities[81 + i];
            CreateEntityFromCurrentEntity(E_UNK_19, entity);
            entity->params = i;
            entity->zPriority = self->zPriority + 1;
        }
        CreateEntityFromCurrentEntity(E_UNK_1A, &g_Entities[86]);
        g_Entities[86].params = 0;
        g_Entities[86].zPriority = self->zPriority + 3;
        CreateEntityFromCurrentEntity(E_UNK_1A, &g_Entities[88]);
        g_Entities[88].params = 1;
        g_Entities[88].zPriority = self->zPriority + 3;
        self->nextPart = &g_Entities[87];
        self->parent = NULL;
        CreateEntityFromCurrentEntity(E_UNK_18, &g_Entities[90]);
        g_Entities[90].zPriority = self->zPriority + 1;
        for (i = 0; i < 3; i++) {
            entity = &g_Entities[91 + i * 6];
            CreateEntityFromCurrentEntity(E_UNK_1C, entity);
            entity->params = i;
            entity->zPriority = self->zPriority + 6;
        }
        CreateEntityFromCurrentEntity(E_UNK_1E, &g_Entities[109]);
        g_Entities[109].zPriority = self->zPriority;
        SetStep(1);
        // fallthrough
    case 1:
        self->hitboxState = 0;
        D_us_801806E8 = 1;
        switch (self->step_s) {
        case 0:
            self->drawFlags = ENTITY_OPACITY;
            self->opacity = 0;
            self->animCurFrame = 1;
            if (g_CutsceneFlags & 0x20) {
                ext->timer = 0x80;
                self->step_s++;
            }
            break;
        case 1:
            AnimateEntity(D_us_80180708, self);
            if (!--ext->timer) {
                self->step_s++;
            }
            break;
        case 2:
            AnimateEntity(D_us_80180708, self);
            self->opacity++;
            if (self->opacity >= 0x80) {
                self->drawFlags = ENTITY_DEFAULT;
                self->step_s++;
            }
            break;
        case 3:
            AnimateEntity(D_us_80180708, self);
            if (g_CutsceneFlags & 2) {
                g_api.TimeAttackController(
                    TIMEATTACK_EVENT_UNUSED_28, TIMEATTACK_SET_VISITED);
                g_api.PlaySfx(0x86B);
                SetStep(2);
            }
            break;
        }
        break;
    case 0xC:
        switch (self->step_s) {
        case 0:
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_UNUSED_28, TIMEATTACK_SET_RECORD);
            self->palette = 0x200;
            self->hitboxState = 0;
            D_us_801806E4 |= 1;
            self->step_s++;
            self->flags &= ~0xF;
            break;
        case 1:
            CreateEntityFromCurrentEntity(E_UNK_27, &g_Entities[200]);
            g_Entities[200].params = 1;
            g_unkGraphicsStruct.unk28 = 0xFF;
            g_Entities[200].flags |= FLAG_UNK_10000;
            self->step_s++;
            break;
        case 2:
            dx = 0x80 - self->posX.i.hi;
            dy = 0x8A - self->posY.i.hi;
            angle = ratan2(dy, dx);
            self->velocityX = (rcos(angle) << 0xF) >> 0xC;
            self->velocityY = (rsin(angle) << 0xF) >> 0xC;
            MoveEntity();
            if (SquareRoot0(dx * dx + dy * dy) < 3) {
                self->step_s++;
            }
            break;
        case 3:
            self->velocityX = 0;
            self->velocityY = 0;
            D_us_801806E4 |= 2;
            self->step_s++;
            break;
        case 4:
            break;
        }
        break;
    case 2:
        AnimateEntity(D_us_80180708, self);
        D_us_801806E8 = 1;
        self->hitboxState = 0;
        switch (self->step_s) {
        case 0:
            self->velocityY = FIX(6.0);
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            self->velocityY -= FIX(0.25);
            if (self->posY.i.hi < -0x60) {
                SetStep(3);
            }
            break;
        }
        break;
    case 3:
        AnimateEntity(D_us_801807A0, self);
        D_us_801806E8 = 0;
        self->hitboxState = 2;
        switch (self->step_s) {
        case 0:
            self->velocityY = FIX(1.25);
            self->posY.i.hi = -0x50;
            self->velocityX = 0;
            self->posX.i.hi = 0x80;
            self->zPriority = 0x80;
            ext->timer = 0;
            self->step_s++;
            // fallthrough
        case 1:
            if (!(ext->timer & 0x3F)) {
                g_api.PlaySfx(0x7D3);
            }
            ext->timer++;
            MoveEntity();
            if (self->posY.i.hi > 0x90) {
                SetStep(4);
            }
            break;
        }
        break;
    case 4:
        if (!self->step_s) {
            ext->timer = 0x40;
            self->step_s++;
        }
        AnimateEntity(D_us_801807A0, self);
        D_us_801806E8 = 0;
        self->hitboxState = 2;
        MoveEntity();
        ext->angle += 0x20;
        self->velocityY = (rcos(ext->angle) << 0xF) >> 0xC;
        if (!--ext->timer) {
            SetStep(D_us_80180830[ext->attackIndex]);
            ext->attackIndex = (ext->attackIndex + 1) & 3;
        }
        break;
    case 5:
        AnimateEntity(D_us_801807A0, self);
        D_us_801806E8 = 0;
        self->hitboxState = 2;
        MoveEntity();
        ext->angle += 0x20;
        self->velocityY = (rcos(ext->angle) << 0xF) >> 0xC;
        switch (self->step_s) {
        case 0:
            D_us_801806EC = 1;
            CreateEntityFromCurrentEntity(E_UNK_20, &g_Entities[129]);
            g_Entities[129].zPriority = self->zPriority + 3;
            self->step_s++;
            // fallthrough
        case 1:
            if (D_us_801806EC == 2) {
                self->step_s++;
            }
            break;
        case 2:
            D_us_801806EC = 3;
            g_api.PlaySfx(0x854);
            self->step_s++;
            // fallthrough
        case 3:
            if (D_us_801806EC == 5) {
                ext->timer = 0xA0;
                self->step_s++;
            }
            break;
        case 4:
            if (!--ext->timer) {
                D_us_801806F0++;
                if (D_us_801806F0 >= 8) {
                    D_us_801806F0 = 0;
                }
                D_us_801806EC = 0;
                SetStep(6);
            }
            break;
        }
        break;
    case 6:
        if (!self->step_s) {
            self->velocityX = 0;
            self->velocityY = FIX(1.0);
            g_api.PlaySfx(0x84F);
            self->step_s++;
        }
        AnimateEntity(D_us_801807A0, self);
        D_us_801806E8 = 0;
        self->hitboxState = 2;
        MoveEntity();
        self->velocityY -= FIX(1.0 / 16);
        if (self->posY.i.hi < -0x40) {
            SetStep(7);
        }
        break;
    case 7:
        if (!self->step_s) {
            self->posY.i.hi = -0x40;
            self->posX.i.hi = 0x80;
            self->velocityX = 0;
            self->velocityY = FIX(3.0);
            self->zPriority = 0x80;
            self->step_s++;
        }
        AnimateEntity(D_us_80180708, self);
        D_us_801806E8 = 1;
        self->hitboxState = 0;
        MoveEntity();
        self->velocityY -= self->velocityY >> 6;
        if (self->posY.i.hi > 0x70) {
            SetStep(8);
        }
        break;
    case 8:
        if (!self->step_s) {
            D_us_801806F4 = 0;
            self->step_s++;
        }
        D_us_801806E8 = 1;
        self->hitboxState = 0;
        if (!AnimateEntity(D_us_80180710, self)) {
            SetStep(2);
        }
        if (self->pose == 2) {
            CreateEntityFromEntity(E_UNK_23, self, &g_Entities[132]);
            g_Entities[132].zPriority = self->zPriority + 1;
        }
        if (self->pose == 3) {
            g_api.PlaySfx(0x859);
            D_us_801806F4 = 1;
        }
        break;
    case 9:
        AnimateEntity(D_us_801807A0, self);
        D_us_801806E8 = 0;
        self->hitboxState = 2;
        MoveEntity();
        ext->angle += 0x20;
        self->velocityY = (rcos(ext->angle) << 0xF) >> 0xC;
        switch (self->step_s) {
        case 0:
            ext->count = 4;
            self->step_s++;
            // fallthrough
        case 1:
            if (PLAYER.posX.i.hi < 0x80) {
                D_us_801806F8 = 1;
                self->velocityX = FIX(-0.5);
            } else {
                D_us_801806F8 = 2;
                self->velocityX = FIX(0.5);
            }
            ext->timer = 0x28;
            self->step_s++;
            // fallthrough
        case 2:
            self->velocityX -= self->velocityX >> 4;
            if (!--ext->timer) {
                if (D_us_801806F8 == 1) {
                    self->velocityX = FIX(2.0);
                } else {
                    self->velocityX = FIX(-2.0);
                }
                self->step_s++;
            }
            break;
        case 3:
            self->velocityX -= self->velocityX >> 4;
            if (D_us_801806F8 == 3) {
                self->step_s++;
            }
            break;
        case 4:
            dx = 0x80 - self->posX.i.hi;
            if (dx > 0) {
                self->velocityX = FIX(1.125);
            }
            if (dx < 0) {
                self->velocityX = FIX(-1.125);
            }
            if (abs(dx) < 3) {
                self->velocityX = 0;
                if (!--ext->count) {
                    D_us_801806F8 = 0;
                    SetStep(4);
                } else {
                    self->step_s = 1;
                }
            }
            break;
        }
        break;
    case 0xA:
        AnimateEntity(D_us_801807A0, self);
        D_us_801806E8 = 0;
        self->hitboxState = 2;
        MoveEntity();
        ext->angle += 0x20;
        self->velocityY = (rcos(ext->angle) << 0xF) >> 0xC;
        switch (self->step_s) {
        case 0:
            D_us_801806FC = 1;
            self->step_s++;
            // fallthrough
        case 1:
            if (D_us_801806FC == 4) {
                D_us_801806FC = 0;
                SetStep(4);
            }
            break;
        }
        break;
    case 0xB:
        AnimateEntity(D_us_801807A0, self);
        D_us_801806E8 = 0;
        self->hitboxState = 2;
        switch (self->step_s) {
        case 0:
            D_us_80180700 = 1;
            self->step_s++;
            // fallthrough
        case 1:
            if (D_us_80180700 == 4) {
                ext->timer = 0x40;
                self->step_s++;
            }
            break;
        case 2:
            if (!(ext->timer & 0xF)) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_24, entity);
                    entity->zPriority = self->zPriority + 8;
                }
            }
            if (!--ext->timer) {
                ext->timer = 0x40;
                self->step_s++;
            }
            break;
        case 3:
            if (!--ext->timer) {
                D_us_80180700 = 5;
                self->step_s++;
            }
            break;
        case 4:
            D_us_80180700 = 0;
            SetStep(4);
            break;
        }
        break;
    case 0xFF:
        FntPrint("charal %x\n", self->animCurFrame);
        if (g_pads[1].pressed & PAD_SQUARE) {
            if (self->params != 0) {
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
    D_us_80180704 = self->palette;
}

extern u8 D_us_8018074C[];
extern u8 D_us_80180758[];
extern u8 D_us_80180764[];
extern u8 D_us_80180770[];

// Dracula's arm, holding the claw in the next entity slot
void func_us_8019EADC(Entity* self) {
    s16 posX;
    s16 posY;

    self->posX.i.hi = DRACULA.posX.i.hi;
    self->posY.i.hi = DRACULA.posY.i.hi;
    if (!self->params) {
        posX = self->posX.i.hi - 0x34;
        posY = self->posY.i.hi + 0x10;
    } else {
        posX = self->posX.i.hi + 0x3B;
        posY = self->posY.i.hi + 0x18;
    }
    self->posX.i.hi = posX;
    self->posY.i.hi = posY;
    self->palette = D_us_80180704;
    if ((D_us_801806E4 & 1) && self->step != 0x10) {
        SetStep(0x10);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        self->facingLeft = self->params;
        CreateEntityFromCurrentEntity(E_UNK_1B, self + 1);
        (self + 1)->params = self->params;
        (self + 1)->zPriority = self->zPriority + 1;
        // fallthrough
    case 1:
        self->animCurFrame = 0xC;
        if (D_us_801806EC == 1) {
            SetStep(2);
        }
        if (D_us_801806F8 == self->params + 1) {
            SetStep(3);
        }
        break;
    case 2:
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 0xC;
            if (D_us_801806EC == 3) {
                self->step_s++;
            }
            break;
        case 1:
            if (self->pose == 3) {
                D_us_801806EC = 4;
            }
            if (!AnimateEntity(D_us_8018074C, self)) {
                D_us_801806EC = 5;
                SetSubStep(2);
            }
            break;
        case 2:
            if (!AnimateEntity(D_us_80180770, self)) {
                SetStep(1);
            }
            break;
        }
        break;
    case 3:
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_80180758, self)) {
                SetSubStep(1);
            }
            if (self->pose == 2) {
                PlaySfxPositional(0x662);
            }
            break;
        case 1:
            if (!AnimateEntity(D_us_80180770, self)) {
                D_us_801806F8 = 3;
                SetStep(1);
            }
            break;
        }
        break;
    case 8:
        if (!AnimateEntity(D_us_80180764, self)) {
            self->pose = 0;
            self->poseTimer = 0;
        }
        break;
    case 0x10:
        switch (self->step_s) {
        case 0:
            self->pose = 0x10 - self->animCurFrame;
            self->step_s++;
            // fallthrough
        case 1:
            if (!AnimateEntity(D_us_80180770, self)) {
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s++;
            }
            break;
        }
        break;
    }
    if (D_us_801806E8) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}

extern u8 D_us_8018072C[];
extern u8 D_us_80180734[];
extern u8 D_us_80180744[];
extern s16 D_us_80180808[][2];
extern s8 D_us_80180838[][4];
extern u8 D_us_80180854[];

// Dracula's claw, follows the arm in the previous entity slot
void func_us_8019EE30(Entity* self) {
    Entity* entity;
    s32 i;
    s32 frame;
    s8* hitbox;

    self->palette = D_us_80180704;
    if ((D_us_801806E4 & 1) && self->step != 0x10) {
        SetStep(0x10);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180558);
        self->parent = &DRACULA;
        self->facingLeft = self->params;
        if (!self->params) {
            self->nextPart = self + 2;
        } else {
            self->nextPart = &DRACULA;
        }
        // fallthrough
    case 1:
        AnimateEntity(D_us_8018072C, self);
        if (D_us_801806EC == 1) {
            SetStep(2);
        }
        if (D_us_801806F8 == self->params + 1) {
            self->animCurFrame = 0xA;
        }
        break;
    case 2:
        switch (self->step_s) {
        case 0:
            for (i = 0; i < 2; i++) {
                entity = AllocEntity(&g_Entities[144], &g_Entities[152]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_1F, entity);
                    entity->params = self->params;
                    entity->zPriority = self->zPriority + 4;
                }
            }
            self->animCurFrame = 9;
            self->step_s++;
            // fallthrough
        case 1:
            if (D_us_801806EC == 3) {
                SetSubStep(2);
            }
            break;
        case 2:
            AnimateEntity(D_us_80180734, self);
            if (D_us_801806EC == 5) {
                SetSubStep(3);
            }
            break;
        case 3:
            if (!AnimateEntity(D_us_80180744, self)) {
                SetStep(1);
            }
            break;
        }
        break;
    case 0x10:
        if (!self->step_s) {
            self->hitboxState = 0;
            self->animCurFrame = 9;
            self->step_s++;
        }
        break;
    }
    frame = (self - 1)->animCurFrame - 0xC;
    self->posX.i.hi = (self - 1)->posX.i.hi;
    self->posY.i.hi = (self - 1)->posY.i.hi;
    if (frame < 0) {
        frame = 0;
    }
    if (frame > 4) {
        frame = 4;
    }
    if (!self->params) {
        self->posX.i.hi += D_us_80180808[frame][0];
    } else {
        self->posX.i.hi -= D_us_80180808[frame][0];
    }
    self->posY.i.hi += D_us_80180808[frame][1];
    hitbox = D_us_80180838[D_us_80180854[self->animCurFrame]];
    self->hitboxOffX = hitbox[0];
    self->hitboxOffY = hitbox[1];
    self->hitboxWidth = hitbox[2];
    self->hitboxHeight = hitbox[3];
    if (D_us_801806E8) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
        return;
    }
    self->hitboxState = 3;
}

extern u8 D_us_80180780[];
extern u8 D_us_80180788[];
extern u8 D_us_80180794[];
extern s16 D_us_8018081C[][2];
extern s16 D_us_80180828[];

// One of the three hands, dragging the chain of entities that follow it
void func_us_8019F1CC(Entity* self) {
    Entity* entity;
    s32 i;
    s32 dx;
    s32 dy;
    s16 angle;
    s32 x;
    s32 y;
    s32 midX;
    s32 midY;
    s16 posX;
    s16 posY;

    self->palette = D_us_80180704;
    if ((D_us_801806E4 & 1) && self->step != 0x10) {
        SetStep(0x10);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_8018054C);
        entity = self + 1;
        for (i = 0; i < 5; i++, entity++) {
            CreateEntityFromCurrentEntity(E_UNK_1D, entity);
            entity->zPriority = self->zPriority - i;
        }
        // fallthrough
    case 1:
        AnimateEntity(D_us_80180780, self);
        self->posX.i.hi = DRACULA.posX.i.hi + D_us_8018081C[self->params][0];
        self->posY.i.hi = DRACULA.posY.i.hi + D_us_8018081C[self->params][1];
        self->zPriority = DRACULA.zPriority + 2;
        self->hitboxState = 0;
        if (D_us_801806FC == self->params + 1) {
            self->zPriority = DRACULA.zPriority + 6;
            SetStep(2);
        }
        if (D_us_80180700 == 1) {
            self->zPriority = DRACULA.zPriority + 6;
            SetStep(3);
        }
        break;
    case 2:
        self->hitboxState = 3;
        switch (self->step_s) {
        case 0:
            angle = ratan2(PLAYER.posY.i.hi - self->posY.i.hi,
                           PLAYER.posX.i.hi - self->posX.i.hi);
            self->velocityX = (rcos(angle) << 0x12) >> 0xC;
            self->velocityY = (rsin(angle) << 0x12) >> 0xC;
            PlaySfxPositional(0x866);
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            if (!AnimateEntity(D_us_80180788, self)) {
                SetSubStep(2);
            }
            break;
        case 2:
            AnimateEntity(D_us_80180794, self);
            MoveEntity();
            dx = (DRACULA.posX.i.hi + D_us_8018081C[self->params][0]) -
                 self->posX.i.hi;
            dy = (DRACULA.posY.i.hi + D_us_8018081C[self->params][1]) -
                 self->posY.i.hi;
            angle = ratan2(dy, dx);
            self->velocityX = rcos(angle) << 5;
            self->velocityY = rsin(angle) << 5;
            if (abs(dx) < 3 && abs(dy) < 3) {
                D_us_801806FC++;
                SetStep(1);
            }
            break;
        }
        break;
    case 3:
        switch (self->step_s) {
        case 0:
            angle = D_us_80180828[self->params];
            self->velocityX = (rcos(angle) << 0x12) >> 0xC;
            self->velocityY = (rsin(angle) << 0x12) >> 0xC;
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            if (!AnimateEntity(D_us_80180788, self)) {
                g_api.PlaySfx(0x62F);
                entity = AllocEntity(&g_Entities[192], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_UNK_25, entity);
                    entity->ext.succubus.real = self;
                    entity->zPriority = self->zPriority + 1;
                }
                D_us_80180700++;
                self->step_s++;
            }
            break;
        case 2:
            if (D_us_80180700 == 5) {
                self->step_s++;
            }
            break;
        case 3:
            AnimateEntity(D_us_80180794, self);
            MoveEntity();
            dx = (DRACULA.posX.i.hi + D_us_8018081C[self->params][0]) -
                 self->posX.i.hi;
            dy = (DRACULA.posY.i.hi + D_us_8018081C[self->params][1]) -
                 self->posY.i.hi;
            angle = ratan2(dy, dx);
            self->velocityX = rcos(angle) * 0x28;
            self->velocityY = rsin(angle) * 0x28;
            if (abs(dx) < 3 && abs(dy) < 3) {
                SetStep(1);
            }
            break;
        }
        break;
    case 0x10:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            self->animCurFrame = 0x12;
            MoveEntity();
            dx = (DRACULA.posX.i.hi + D_us_8018081C[self->params][0]) -
                 self->posX.i.hi;
            dy = (DRACULA.posY.i.hi + D_us_8018081C[self->params][1]) -
                 self->posY.i.hi;
            angle = ratan2(dy, dx);
            self->velocityX = rcos(angle) * 0x28;
            self->velocityY = rsin(angle) * 0x28;
            if (abs(dx) < 3 && abs(dy) < 3) {
                self->zPriority = DRACULA.zPriority + 2;
                self->step_s++;
            }
            break;
        case 1:
            self->posX.i.hi =
                DRACULA.posX.i.hi + D_us_8018081C[self->params][0];
            self->posY.i.hi =
                DRACULA.posY.i.hi + D_us_8018081C[self->params][1];
            break;
        }
        break;
    }

    // smoothly drag the chain, from the last link towards the hand
    entity = self + 5;
    x = (D_us_8018081C[self->params][0] << 0x10) + DRACULA.posX.val;
    y = (D_us_8018081C[self->params][1] << 0x10) + DRACULA.posY.val;
    for (i = 0; i < 5; i++, entity--) {
        midX = (x + (entity - 1)->posX.val) >> 1;
        midY = (y + (entity - 1)->posY.val) >> 1;
        x = midX + ((midX - entity->posX.val) >> 2);
        y = midY + ((midY - entity->posY.val) >> 2);
        entity->posX.val = x;
        entity->posY.val = y;
        entity->zPriority = self->zPriority + i - 5;
    }

    // then keep every link at most 12 pixels away from the previous one
    entity = self + 5;
    posX = DRACULA.posX.i.hi + D_us_8018081C[self->params][0];
    posY = DRACULA.posY.i.hi + D_us_8018081C[self->params][1];
    for (i = 0; i < 6; i++, entity--) {
        dx = entity->posX.i.hi - posX;
        dy = entity->posY.i.hi - posY;
        angle = ratan2(dy, dx);
        if (dx * dx + dy * dy > 0x90) {
            entity->posX.i.hi = posX + ((rcos(angle) * 0xC) >> 0xC);
            entity->posY.i.hi = posY + ((rsin(angle) * 0xC) >> 0xC);
        }
        posX = entity->posX.i.hi;
        posY = entity->posY.i.hi;
    }
    if (D_us_801806E8) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}
#endif

void func_us_8019FA78(Entity* self) {
    extern u16 D_us_80180540;
    extern s32 D_us_801806E8;
    extern u16 D_us_80180704;

    self->palette = D_us_80180704;

    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        self->animCurFrame = 0x11;
        /* fall through */
    case 1:
        self->animCurFrame = 0x11;
        break;
    }

    if (D_us_801806E8 != 0) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}

void func_us_8019FB04(Entity* self) {
    extern u16 D_us_80180704;
    extern s32 D_us_801806E8;
    extern u8* D_us_801807F4[];
    extern u16 D_us_80180540;
    extern void InitializeEntity(u16*);

    u16 value;

    self->palette = D_us_80180704;
    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        /* fall through */
    case 1:
        AnimateEntity(D_us_801807F4[self->params], self);
        value = g_Entities[E_BOSS_WEAPON].posX.i.hi;
        self->posX.i.hi = value;
        value = g_Entities[E_BOSS_WEAPON].posY.i.hi;
        self->posY.i.hi = value;
        break;
    }

    if (D_us_801806E8 != 0) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019FBC0);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019FCB4);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A01A4);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A0710);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A0860);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A0AB4);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A0DC0);
#else
extern u8 D_us_801807A8[];

// Dracula's head
void func_us_8019FBC0(Entity* self) {
    self->posX.i.hi = DRACULA.posX.i.hi;
    self->palette = D_us_80180704;
    self->posY.i.hi = DRACULA.posY.i.hi;
    if (D_us_801806E4 & 1) {
        if (self->step != 0x10) {
            SetStep(0x10);
        }
    }
    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        // fallthrough
    case 1:
        AnimateEntity(D_us_801807A8, self);
        break;
    case 0x10:
        self->animCurFrame = 0x1B;
        break;
    }
    if (D_us_801806E8) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}

// Dracula's wings, drawn with primitives
void func_us_8019FCB4(Entity* self) {
    ET_Rbo6Wings* ext = RBO6_EXT(self, ET_Rbo6Wings);
    Primitive* prim;
    s16 primIndex;
    s32 i;
    s16 rot;
    s16 x;
    s16 y;
    u16 clut;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 4);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        ext->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        while (prim != NULL) {
            prim->tpage = 0x14;
            prim->clut = 0x201;
            prim->drawMode = DRAW_HIDE;
            prim->priority = self->zPriority - 1;
            prim = prim->next;
        }
        prim = ext->prim;
        for (i = 0; i < 2; i++) {
            prim->u0 = prim->u2 = 0xC1;
            prim->u1 = prim->u3 = 0xFF;
            prim->v0 = prim->v1 = 1;
            prim->v2 = prim->v3 = 0x47;
            prim->priority = self->zPriority - 2;
            prim = prim->next;
            prim->u0 = prim->u2 = 1;
            prim->u1 = prim->u3 = 0xBF;
            prim->v0 = prim->v1 = 1;
            prim->v2 = prim->v3 = 0xB4;
            prim = prim->next;
        }
        // fallthrough
    case 1:
        ext->angle += 0x10;
        self->posX.i.hi = DRACULA.posX.i.hi;
        self->posY.i.hi = DRACULA.posY.i.hi;
        rot = (rsin(ext->angle) << 7) >> 0xC;
        prim = ext->prim;
        self->rotate = rot;
        for (i = 0; i < 2; i++) {
            // the inner part of the wing
            if (!i) {
                x = self->posX.i.hi - 8;
            } else {
                x = self->posX.i.hi + 8;
            }
            prim->x1 = prim->x3 = x;
            y = self->posY.i.hi - 0x34;
            prim->y1 = y;
            prim->y3 = self->posY.i.hi + 0x12;
            if (!i) {
                x -= (rcos(rot) * 0x3E) >> 0xC;
            } else {
                x += (rcos(rot) * 0x3E) >> 0xC;
            }
            y += (rsin(rot) * 0x3E) >> 0xC;
            prim->x0 = x;
            prim->y0 = y;
            if (!i) {
                prim->x2 = x + ((rsin(rot) * 0x46) >> 0xC);
            } else {
                prim->x2 = x - ((rsin(rot) * 0x46) >> 0xC);
            }
            prim->y2 = y + ((rcos(rot) * 0x46) >> 0xC);
            prim->drawMode = DRAW_UNK02;
            prim = prim->next;

            // the outer part of the wing
            if (!i) {
                x += 0x18;
            } else {
                x -= 0x18;
            }
            y -= 0x78;
            prim->x1 = x;
            prim->y1 = y;
            prim->x3 = x;
            prim->y3 = y + 0xB3;
            if (!i) {
                x -= (rcos(rot) * 0xBE) >> 0xC;
            } else {
                x += (rcos(rot) * 0xBE) >> 0xC;
            }
            y += (rsin(rot) * 0xBE) >> 0xC;
            prim->x0 = x;
            prim->y0 = y;
            if (!i) {
                prim->x2 = x + ((rsin(rot) * 0xB3) >> 0xC);
            } else {
                prim->x2 = x - ((rsin(rot) * 0xB3) >> 0xC);
            }
            prim->drawMode = DRAW_UNK02;
            prim->y2 = y + ((rcos(rot) * 0xB3) >> 0xC);
            prim = prim->next;
        }
        break;
    }
    prim = ext->prim;
    clut = D_us_80180704 & 0xFFF;
    if (clut == 0x200) {
        clut = 0x201;
    }
    while (prim != NULL) {
        prim->clut = clut;
        if (D_us_801806E8) {
            prim->drawMode |= DRAW_HIDE;
        }
        prim = prim->next;
    }
    FntPrint("color %x\n", D_us_80180704);
}

// Lightning running from one claw to the other one
void func_us_801A01A4(Entity* self) {
    ET_Rbo6Lightning* ext = RBO6_EXT(self, ET_Rbo6Lightning);
    Primitive* prim;
    Primitive* nextPrim;
    Entity* to;
    s16 primIndex;
    s32 i;
    s32 dx;
    s32 dy;
    s32 isNear;
    s32 delta;
    u16 angle;

    if (D_us_801806E4 & 1) {
        DestroyEntity(self);
        return;
    }
    if (D_us_801806EC == 2) {
        DestroyEntity(self);
        return;
    }
    if (!self->params) {
        ext->from = &g_Entities[86];
        ext->to = &g_Entities[88];
    } else {
        ext->to = &g_Entities[86];
        ext->from = &g_Entities[88];
    }
    self->posX.i.hi = ext->from->posX.i.hi;
    self->posY.i.hi = ext->from->posY.i.hi;
    if (!(g_Timer & 0xF)) {
        g_api.PlaySfx(0x869);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x14);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[primIndex];
        i = 0;
        self->primIndex = primIndex;
        ext->prim = prim;
        ext->curPrim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        while (prim != NULL) {
            prim->r0 = prim->g0 = prim->b0 = 0x50;
            prim->tpage = 0x1A;
            prim->clut = 0x194;
            prim->u0 = prim->u1 = i * 0x10 - 0x70;
            prim->u2 = prim->u3 = i * 0x10 - 0x60;
            prim->v0 = prim->v2 = 0xD0;
            prim->v1 = prim->v3 = 0xC0;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->drawMode = DRAW_HIDE;
            prim->priority = self->zPriority + 1;
            prim = prim->next;
            i++;
            if (i >= 6) {
                i = 0;
            }
        }
        // fallthrough
    case 1:
        prim = ext->prim;
        prim->x0 = self->posX.i.hi;
        prim->y0 = self->posY.i.hi;
        prim->x1 = prim->x0;
        prim->y1 -= 0x10; // PSX bug: y0 was probably meant here
        prim->x2 = self->posX.i.hi;
        prim->y2 = self->posY.i.hi;
        prim->x3 = prim->x2;
        prim->y3 = prim->y2 - 0x10;
        ext->curPrim = prim;
        ext->angle = Random() << 4;
        while (prim != NULL) {
            prim->r0 = prim->g0 = prim->b0 = 0x50;
            prim->drawMode = DRAW_HIDE;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim = prim->next;
        }
        ext->segments = 0;
        self->step++;
        // fallthrough
    case 2:
        while (true) {
            prim = ext->curPrim;
            to = ext->to;
            angle = ext->angle;
            dx = to->posX.i.hi - prim->x2;
            dy = to->posY.i.hi - prim->y2;
            isNear = false;
            if (abs(dx) < 0x10 && abs(dy) < 0x10) {
                break;
            }
            if (abs(dx) < 0x20) {
                isNear = abs(dy) < 0x20;
            }
            if (!ext->segments) {
                if (isNear) {
                    ext->segments = 2;
                } else {
                    ext->segments = 4;
                }
                delta = ratan2(-dy, dx) - angle;
                if ((s16)delta > 0x800) {
                    delta -= 0x1000;
                }
                if ((s16)delta < -0x800) {
                    delta += 0x1000;
                }
                if (!isNear) {
                    ext->angleStep = (s16)delta / 4;
                } else {
                    ext->angleStep = (s16)delta / 2;
                }
            }
            angle += ext->angleStep;
            if (!isNear) {
                angle = angle + 0x60 - ((Random() & 3) << 6);
            }
            angle &= 0xFFF;
            nextPrim = prim->next;
            if (nextPrim == NULL) {
                break;
            }
            nextPrim->x0 = prim->x2;
            nextPrim->y0 = prim->y2;
            nextPrim->x1 = prim->x3;
            nextPrim->y1 = prim->y3;
            ext->angle = angle;
            ext->curPrim = nextPrim;
            nextPrim->x2 = nextPrim->x0 + ((rcos(angle) * 0xC) >> 0xC);
            nextPrim->y2 = nextPrim->y0 - ((rsin(angle) * 0xC) >> 0xC);
            nextPrim->x3 =
                nextPrim->x2 + ((rcos((s16)(angle - 0x400)) * 0x10) >> 0xC);
            nextPrim->y3 =
                nextPrim->y2 - ((rsin((s16)(angle - 0x400)) * 0x10) >> 0xC);
            nextPrim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                                 DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
            ext->segments--;
        }
        self->step = 1;
        break;
    case 3:
        prim = ext->prim;
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        self->step = 1;
        break;
    }
}

// Spawns the fireball that grows in front of Dracula
void func_us_801A0710(Entity* self) {
    s16 scale;

    if (D_us_801806E4 & 1) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = DRACULA.posX.i.hi;
    self->posY.i.hi = DRACULA.posY.i.hi + 0x10;
    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        self->animCurFrame = 0x2A;
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        self->scaleX = self->scaleY = 0;
        // fallthrough
    case 1:
        scale = self->scaleY + 0x10;
        self->scaleX = self->scaleY = scale;
        if (scale > 0x180) {
            self->step++;
        }
        break;
    case 2:
        CreateEntityFromCurrentEntity(E_UNK_21, &g_Entities[130]);
        g_Entities[130].zPriority = self->zPriority + 1;
        self->step++;
        break;
    case 3:
        if (D_us_801806EC == 2) {
            self->step++;
        }
        break;
    case 4:
        scale = self->scaleX - 0x20;
        self->scaleX = self->scaleY = scale;
        if (scale < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

// The fireball, bursting into sparks
void func_us_801A0860(Entity* self) {
    ET_Rbo6Spark* ext = RBO6_EXT(self, ET_Rbo6Spark);
    Entity* entity;
    s16 scale;
    s32 i;

    if (D_us_801806E4 & 1) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = DRACULA.posX.i.hi;
    self->posY.i.hi = DRACULA.posY.i.hi + 0x10;
    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        self->scaleX = self->scaleY = 0;
        self->opacity = 0;
        self->animCurFrame = D_us_801806F0 + 0x2D;
        self->drawFlags |= ENTITY_OPACITY;
        // fallthrough
    case 1:
        scale = self->scaleY + 8;
        self->scaleX = self->scaleY = scale;
        if (scale >= 0x100) {
            self->scaleX = self->scaleY = 0x100;
            self->step++;
        }
        // fallthrough
    case 2:
        self->opacity += 2;
        if (self->opacity >= 0x80) {
            D_us_801806EC = 2;
            self->step = 3;
        }
        break;
    case 3:
        if (D_us_801806EC == 4) {
            g_api.PlaySfx(0x868);
            self->step++;
        }
        break;
    case 4:
        scale = self->scaleY - 0x18;
        self->scaleX += 8;
        if (scale < 0) {
            scale = 0;
        }
        self->scaleY = scale;
        for (i = 0; i < 0x10; i++) {
            entity = AllocEntity(&g_Entities[192], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_22, self, entity);
                entity->zPriority = self->zPriority + 1;
            }
        }
        if (scale == 0) {
            self->animCurFrame = 0;
            ext->timer = 0x50;
            self->step++;
        }
        break;
    case 5:
        if (!--ext->timer) {
            g_api.PlaySfx(0x672);
            DestroyEntity(self);
        }
        break;
    }
}

// Sparks of the fireball, flying outwards and then towards Dracula
void func_us_801A0AB4(Entity* self) {
    ET_Rbo6Spark* ext = RBO6_EXT(self, ET_Rbo6Spark);
    s16 scale;
    s32 angle;
    s32 speed;
    s32 dx;
    s32 dy;

    if (D_us_801806E4 & 1) {
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        scale = (Random() & 0x7F) | 0x80;
        self->scaleX = self->scaleY = scale;
        self->blendMode = BLEND_TRANSP;
        angle = rand() & 0xFFF;
        speed = (Random() & 0x1F) + 0x10;
        self->velocityX = speed * rcos(angle);
        self->velocityY = speed * rsin(angle);
        self->posX.val += self->velocityX * 4;
        self->posY.val += self->velocityY * 4;
        ext->timer = (Random() & 0x1F) + 0x10;
        // fallthrough
    case 1:
        MoveEntity();
        AnimateEntity(D_us_801807E0, self);
        self->velocityX -= self->velocityX >> 5;
        self->velocityY -= self->velocityY >> 5;
        if (D_us_801806EC == 5) {
            self->velocityX = 0;
            self->velocityY = 0;
            self->step++;
        }
        break;
    case 2:
        AnimateEntity(D_us_801807E0, self);
        if (!--ext->timer) {
            self->step++;
        }
        if (!(g_Timer & 7)) {
            g_api.PlaySfx(0x673);
        }
        break;
    case 3:
        MoveEntity();
        if (self->scaleX > 0x20) {
            scale = self->scaleY - 2;
            self->scaleX = self->scaleY = scale;
        }
        dy = DRACULA.posY.i.hi - (self->posY.i.hi + 0x18);
        dx = DRACULA.posX.i.hi - self->posX.i.hi;
        angle = ratan2(dy, dx);
        if (ext->speed < 0x30000) {
            ext->speed += 0x1000;
        }
        self->velocityX = (ext->speed * rcos((s16)angle)) >> 0xC;
        self->velocityY = (ext->speed * rsin((s16)angle)) >> 0xC;
        if (dx * dx + dy * dy < 0x40) {
            DestroyEntity(self);
        }
        break;
    }
}

extern u8 D_us_8018071C[];

// Energy ball that Dracula shoots at the player
void func_us_801A0DC0(Entity* self) {
    ET_Rbo6Spark* ext = RBO6_EXT(self, ET_Rbo6Spark);
    Entity* entity;
    s16 scale;
    s16 dx;
    s16 dy;
    s32 radius;

    if (D_us_801806E4 & 1) {
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180564);
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        self->opacity = 0x60;
        self->scaleX = self->scaleY = 0;
        self->hitboxState = 0;
        self->blendMode |= BLEND_TRANSP | BLEND_ADD;
        self->drawFlags |= ENTITY_OPACITY;
        // fallthrough
    case 1:
        AnimateEntity(D_us_8018071C, self);
        scale = self->scaleX + 0x10;
        if (scale > 0xCC) {
            scale = 0xCC;
        }
        self->scaleX = self->scaleY = scale;
        if (D_us_801806F4) {
            ext->timer = 0x60;
            self->step++;
            self->velocityX = (PLAYER.posX.val - self->posX.val) / 96;
            self->velocityY = (PLAYER.posY.val - self->posY.val) / 96;
        }
        if (!(g_Timer & 0x1F)) {
            g_api.PlaySfxVolPan(0x6DE, 0x50, 0);
        }
        break;
    case 2:
        AnimateEntity(D_us_8018071C, self);
        MoveEntity();
        scale = 0x6000 / (ext->timer + 0x18);
        self->scaleX = self->scaleY = scale;
        if (!(ext->timer & 3)) {
            entity = AllocEntity(&g_Entities[192], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_UNK_26, self, entity);
                entity->scaleX = entity->scaleY = self->scaleX;
                entity->zPriority = self->zPriority;
                entity->opacity = self->opacity;
            }
        }
        if (!(ext->timer & 0xF)) {
            g_api.PlaySfxVolPan(0x6E2, 0x80 - (ext->timer / 2), 0);
        }
        if (!--ext->timer) {
            g_api.PlaySfx(0x661);
            radius = (self->scaleX * 0x14) / 256;
            self->zPriority = g_unkGraphicsStruct.g_zEntityCenter + 8;
            dx = PLAYER.posX.i.hi - self->posX.i.hi;
            dy = PLAYER.posY.i.hi - self->posY.i.hi;
            if (dx * dx + dy * dy < radius * radius) {
                self->hitboxOffX = dx;
                self->hitboxOffY = dy;
                self->hitboxState = 1;
            }
            self->step++;
        }
        break;
    case 3:
        DestroyEntity(self);
        break;
    }
}
#endif

void func_us_801A1150(Entity* self) {
    extern u16 D_us_80180540;
    extern u8 D_us_80180724;
    s32 var_v0;

    if (!self->step) {
        InitializeEntity(&D_us_80180540);
        self->drawFlags = ENTITY_OPACITY | ENTITY_SCALEY | ENTITY_SCALEX;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
    }
    AnimateEntity(&D_us_80180724, self);
    var_v0 = self->opacity;
    var_v0 -= 2;
    if (var_v0 < 0) {
        var_v0 = 0;
    }
    self->opacity = (u8)var_v0;
    if (var_v0 == 0) {
        DestroyEntity(self);
    }
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A11DC);
#else
// Energy triangle formed by the three hands, then thrown at the player
void func_us_801A11DC(Entity* self) {
    ET_Rbo6Triangle* ext = RBO6_EXT(self, ET_Rbo6Triangle);
    struct {
        s16 x;
        s16 y;
    } points[4];
    Primitive* prim;
    Entity* hand;
    s16 primIndex;
    s32 i;
    s32 sumX;
    s32 sumY;
    s16 scale;
    s32 sine;
    s32 cosine;
    u16 phaseR;
    u16 phaseG;
    u16 phaseB;
    u8 opacity;

    if (D_us_801806E4 & 1) {
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180570);
        self->hitboxState = 0;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 10);
        if (primIndex == -1) {
            self->step = 0;
            return;
        }
        prim = &g_PrimBuf[primIndex];
        self->primIndex = primIndex;
        ext->prim = prim;
        self->flags |= FLAG_HAS_PRIMS;
        for (i = 0; i < 3; i++) {
            prim->type = PRIM_LINE_G2;
            prim->r0 = prim->g0 = prim->b0 = 0xC0;
            prim->r1 = prim->g1 = prim->b1 = 0xC0;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
            prim->priority = self->zPriority;
            prim = prim->next;
        }
        prim->type = PRIM_GT3;
        prim->r0 = prim->g0 = prim->b0 = 0x10;
        prim->tpage = 0x1A;
        prim->clut = 0x15F;
        prim->u0 = prim->v0 = 0x10;
        prim->u1 = 0x14;
        prim->v1 = 0x10;
        prim->u2 = 0x14;
        prim->v2 = 0x14;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        prim->priority = self->zPriority;
        self->opacity = 0;
        // fallthrough
    case 1:
        hand = &g_Entities[91];
        for (i = 0; i < 3; i++, hand += 6) {
            points[i].x = hand->posX.i.hi;
            points[i].y = hand->posY.i.hi + 0xC;
        }
        points[3].x = g_Entities[91].posX.i.hi;
        points[3].y = g_Entities[91].posY.i.hi + 0xC;
        prim = ext->prim;
        for (i = 0; i < 3; i++) {
            prim->x0 = points[i].x;
            prim->y0 = points[i].y;
            prim = prim->next;
        }
        prim = ext->prim;
        for (i = 0; i < 3; i++) {
            prim->x1 = points[i + 1].x;
            prim->y1 = points[i + 1].y;
            prim = prim->next;
        }
        prim->x0 = points[0].x;
        prim->y0 = points[0].y;
        prim->x1 = points[1].x;
        prim->y1 = points[1].y;
        prim->x2 = points[2].x;
        prim->y2 = points[2].y;
        ext->phaseR += 0x100;
        ext->phaseG += 0x40;
        ext->phaseB += 0x80;
        self->opacity += 3;
        opacity = self->opacity;
        phaseR = ext->phaseR;
        phaseG = ext->phaseG;
        phaseB = ext->phaseB;
        for (i = 0; i < 3; i++) {
            (&prim->r0)[i * 12] =
                (opacity + ((opacity * rcos((s16)phaseR)) >> 0xC)) / 2;
            (&prim->g0)[i * 12] =
                (opacity + ((opacity * rsin((s16)phaseG)) >> 0xC)) / 2;
            (&prim->b0)[i * 12] =
                (opacity + ((opacity * rcos((s16)phaseB)) >> 0xC)) / 2;
            phaseR += 0x40;
            phaseG += 0x280;
            phaseB += 0x100;
        }
        if (self->opacity <= 0x40) {
            break;
        }
        sumX = 0;
        sumY = 0;
        for (i = 0; i < 3; i++) {
            sumX += points[i].x;
            sumY += points[i].y;
        }
        self->posX.i.hi = sumX / 3;
        self->posY.i.hi = sumY / 3;
        for (i = 0; i < 3; i++) {
            ext->offsets[i][0] = points[i].x - self->posX.i.hi;
            ext->offsets[i][1] = points[i].y - self->posY.i.hi;
        }
        self->scaleX = 0x100;
        self->step++;
        break;
    case 2:
        prim = ext->prim;
        for (i = 0; i < 3; i++) {
            if (!PrimDecreaseBrightness(prim, 8)) {
                prim->drawMode = DRAW_HIDE;
            }
            prim = prim->next;
        }
        self->scaleX -= 8;
        scale = self->scaleX;
        for (i = 0; i < 3; i++) {
            (&prim->x0)[i * 6] =
                self->posX.i.hi + ((ext->offsets[i][0] * scale) >> 8);
            (&prim->y0)[i * 6] =
                self->posY.i.hi + ((ext->offsets[i][1] * scale) >> 8);
        }
        if (self->opacity < 0xC0) {
            self->opacity += 4;
        }
        opacity = self->opacity;
        ext->phaseR += 0x100;
        ext->phaseG += 0x40;
        ext->phaseB += 0x80;
        phaseR = ext->phaseR;
        phaseG = ext->phaseG;
        phaseB = ext->phaseB;
        for (i = 0; i < 3; i++) {
            (&prim->r0)[i * 12] =
                (opacity + ((opacity * rcos((s16)phaseR)) >> 0xC)) / 2;
            (&prim->g0)[i * 12] =
                (opacity + ((opacity * rsin((s16)phaseG)) >> 0xC)) / 2;
            (&prim->b0)[i * 12] =
                (opacity + ((opacity * rcos((s16)phaseB)) >> 0xC)) / 2;
            phaseR += 0x40;
            phaseG += 0x280;
            phaseB += 0x100;
        }
        if (self->scaleX < 0x60) {
            prim = ext->prim;
            for (i = 1; i < 3; i++) {
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
            }
            scale = self->scaleX;
            for (i = 0; i < 3; i++) {
                ext->offsets[i][0] = (ext->offsets[i][0] * scale) >> 8;
                ext->offsets[i][1] = (ext->offsets[i][1] * scale) >> 8;
            }
            self->step++;
        }
        break;
    case 3:
        sine = ratan2(PLAYER.posY.i.hi - self->posY.i.hi,
                      PLAYER.posX.i.hi - self->posX.i.hi);
        self->velocityX = (rcos((s16)sine) << 0x12) >> 0xC;
        self->velocityY = (rsin((s16)sine) << 0x12) >> 0xC;
        self->hitboxState = 1;
        g_api.PlaySfx(0x8BF);
        self->step++;
        // fallthrough
    case 4:
        MoveEntity();
        self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
        if (self->velocityX > 0) {
            self->rotate += 0x40;
        } else {
            self->rotate -= 0x40;
        }
        prim = ext->prim;
        for (i = 0; i < 3; i++) {
            prim = prim->next;
        }
        opacity = self->opacity;
        ext->phaseR += 0x100;
        ext->phaseG += 0x40;
        ext->phaseB += 0x80;
        phaseR = ext->phaseR;
        phaseG = ext->phaseG;
        phaseB = ext->phaseB;
        for (i = 0; i < 3; i++) {
            (&prim->r0)[i * 12] =
                (opacity + ((opacity * rcos((s16)phaseR)) >> 0xC)) / 2;
            (&prim->g0)[i * 12] =
                (opacity + ((opacity * rsin((s16)phaseG)) >> 0xC)) / 2;
            (&prim->b0)[i * 12] =
                (opacity + ((opacity * rcos((s16)phaseB)) >> 0xC)) / 2;
            phaseR += 0x40;
            phaseG += 0x280;
            phaseB += 0x100;
        }
        sine = rsin(self->rotate);
        cosine = rcos(self->rotate);
        for (i = 0; i < 3; i++) {
            (&prim->x0)[i * 6] =
                self->posX.i.hi + ((ext->offsets[i][0] * cosine -
                                    ext->offsets[i][1] * sine) >>
                                   0xC);
            (&prim->y0)[i * 6] =
                self->posY.i.hi + ((ext->offsets[i][0] * sine +
                                    ext->offsets[i][1] * cosine) >>
                                   0xC);
        }
        break;
    }
}
#endif

void func_us_801A1B38(Entity* self) {
    Entity* real;

#ifdef VERSION_PC
    extern u8 D_us_801807EC[];

    if (D_us_801806E4 & 1) {
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        InitializeEntity(&D_us_80180540);
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
    }
    AnimateEntity(D_us_801807EC, self);
    real = self->ext.succubus.real;
    self->posX.i.hi = real->posX.i.hi;
    self->posY.i.hi = real->posY.i.hi + 0xC;
    if (D_us_80180700 == 5) {
        DestroyEntity(self);
    }
#else
    if (*(s32*)0x801806E4 & 1) {
        DestroyEntity(self);
        return;
    }

    if (!self->step) {
        InitializeEntity((u16*)((u32)func_us_801A1B38 - 0x215F8));
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
    }

    AnimateEntity((u8*)((u32)func_us_801A1B38 - 0x2134C), self);

    real = self->ext.succubus.real;
    self->posX.i.hi = real->posX.i.hi;
    self->posY.i.hi = real->posY.i.hi + 0xC;

    if (*(s32*)0x80180700 == 5) {
        DestroyEntity(self);
    }
#endif
}
