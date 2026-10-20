// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"

void func_us_801A19CC(u8 arg0) {
    g_CurrentEntity->poseTimer = 0;
    g_CurrentEntity->pose = 0;
    g_CurrentEntity->ext.et_801A19CC.unk7D = 0;
    g_CurrentEntity->step = arg0;
    g_CurrentEntity->step_s = 0;
}

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_hippogryph", func_us_801A19FC);
#else
// Spawns feather / dust debris around the Hippogryph. The debris are
// Hippogryph entities themselves, running steps 0x30 and 0x32.
void func_us_801A19FC(u8 count, s32 offsetY) {
    Entity* entity;
    s32 i;

    for (i = 0; i < count; i++) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            DestroyEntity(entity);
            MakeEntityFromId(E_UNK_22, g_CurrentEntity, entity);
            entity->flags = 0xC8202000;
            entity->posX.i.hi = entity->posX.i.hi - 0x20 + (Random() & 0x3F);
            entity->velocityY = FIX(-0.25);
            entity->posY.i.hi += (Random() & 0x3F) + (offsetY - 0x20);
            if (Random() & 1) {
                entity->zPriority++;
            }
            entity->step = 0x30;
            entity->params = Random() & 1;
            if (entity->params) {
                entity->step = 0x32;
                if (Random() & 1) {
                    entity->ext.et_801A19CC.accelX = 0xD00;
                    entity->velocityX = -0xD000;
                } else {
                    entity->ext.et_801A19CC.accelX = -0xD00;
                    entity->velocityX = 0xD000;
                }
            }
            if (g_CurrentEntity->step == 0x16) {
                if (g_CurrentEntity->facingLeft) {
                    g_CurrentEntity->velocityX += FIX(0.25);
                } else {
                    g_CurrentEntity->velocityX -= FIX(0.25);
                }
            }
        }
    }
}
#endif

bool CheckHippogryphFloorCollision(s16* sensors) {
    s32 posY;

    if (UnkCollisionFunc3(sensors) & 1) {
        return true;
    } else {
        posY = g_CurrentEntity->posY.i.hi + g_Tilemap.scrollY.i.hi;
        if (posY > 183) {
            g_CurrentEntity->posY.i.hi = 183 - g_Tilemap.scrollY.i.hi;
            g_CurrentEntity->velocityX = 0;
            g_CurrentEntity->velocityY = 0;
            return true;
        }
        return false;
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_hippogryph", func_us_801A1C14);
#else
extern s32 D_us_801806DC;
extern s32 D_us_801806E0;
extern s32 D_us_801806E4[];
extern s32 D_us_801807E4[];
extern AnimateEntityFrame D_us_80180804[];
extern AnimateEntityFrame D_us_80180824[];
extern AnimateEntityFrame D_us_80180830[];
extern AnimateEntityFrame D_us_80180844[];
extern AnimateEntityFrame D_us_80180858[];
extern AnimateEntityFrame D_us_80180884[];
extern AnimateEntityFrame D_us_8018088C[];
extern AnimateEntityFrame D_us_80180894[];
extern AnimateEntityFrame D_us_8018089C[];
extern AnimateEntityFrame D_us_801808AC[];
extern AnimateEntityFrame D_us_801808BC[];
extern AnimateEntityFrame D_us_801808D4[];
extern AnimateEntityFrame D_us_801808FC[];
extern AnimateEntityFrame D_us_8018091C[];
extern AnimateEntityFrame D_us_80180928[];
extern AnimateEntityFrame D_us_80180938[];
extern AnimateEntityFrame D_us_80180944[];
extern AnimateEntityFrame D_us_80180950[];
extern AnimateEntityFrame D_us_8018096C[];
extern AnimateEntityFrame D_us_8018098C[];
extern AnimateEntityFrame D_us_801809D8[];
extern AnimateEntityFrame D_us_80180A2C[];
extern AnimateEntityFrame D_us_80180A48[];
extern s16 D_us_80180AAC[];
extern s16 D_us_80180ACC[];
extern s16 D_us_80180ADC[];
extern s16 D_us_80180AE4[];
extern u8 D_us_80180AEC[];
extern s32 D_us_80180B74;
extern EInit g_EInitHippogryph;
void func_us_801A3B88(Entity* self);

// The death effect stores 32-bit fixed point values split in two 16-bit
// primitive fields (high half first).
#define HILO(hi, lo) ((s32)(((u32)(u16)(hi) << 16) | (u16)(lo)))

// Hippogryph
void func_us_801A1C14(Entity* self) {
    Entity* entity;
    Primitive* prim;
    s32 i;
    s32 count;
    s32 posX;
    s32 posY;
    s32 dx;
    s32 dy;
    s32 radius;
    s32 speed;
    s32 angle;
    s32 velX;
    s32 velY;
    s32 delta;
    s16 primIndex;
    u8 frame;

    if (self->hitParams && (self->step & 1)) {
        if (self->facingLeft) {
            self->velocityX = FIX(-1.5);
        } else {
            self->velocityX = FIX(1.5);
        }
        self->velocityY = FIX(-4);
        func_us_801A19CC(0x20);
    }
    if ((self->flags & FLAG_DEAD) && self->step < 0x22) {
        entity = self->ext.et_801A19CC.unk94;
        self->hitboxState = 0;
        // only the integer parts of the velocity are cleared
        self->velocityX &= 0xFFFF;
        self->velocityY &= 0xFFFF;
        self->ext.et_801A19CC.unk7C = 0;
        self->animCurFrame = 0x6E;
        if (entity != NULL) {
            DestroyEntity(entity);
        }
        if (self->flags & FLAG_HAS_PRIMS) {
            g_api.FreePrimitives(self->primIndex);
            self->flags &= ~FLAG_HAS_PRIMS;
        }
        func_us_801A19CC(0x24);
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitHippogryph);
        self->ext.et_801A19CC.zPriority = self->zPriority;
        self->zPriority = PLAYER.zPriority + 1;
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        self->ext.et_801A19CC.unk7D = 1;
        break;

    case 1:
        self->hitboxState = 0;
        AnimateEntity(D_us_8018091C, self);
        if (self->ext.et_801A19CC.unk7D && GetDistanceToPlayerX() < 0x80) {
            func_us_801A19CC(3);
            if (D_us_801806DC == 0) {
                g_api.TimeAttackController(TIMEATTACK_EVENT_HYPPOGRYPH_DEFEAT,
                                           TIMEATTACK_SET_VISITED);
            }
            self->hitboxState = 3;
            self->nextPart = self + 1;
            D_us_801806DC |= 1;
            CreateEntityFromEntity(E_UNK_23, self, self + 1);
        }
        break;

    case 3:
        if (!AnimateEntity(D_us_80180804, self)) {
            func_us_801A19CC(4);
        }
        break;

    case 4:
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_801808BC, self)) {
                self->animCurFrame = 0x31;
                self->velocityY = FIX(-5);
                self->ext.et_801A19CC.unk7C = 4;
                self->ext.et_801A19CC.unk7D = 0;
                self->step_s++;
            }
            if (self->animCurFrame == 0x29 && !self->ext.et_801A19CC.unk7D) {
                func_us_801A19FC(4, 0);
                self->ext.et_801A19CC.unk7D = 1;
                if (self->facingLeft) {
                    self->velocityX = FIX(0.375);
                } else {
                    self->velocityX = FIX(-0.375);
                }
                self->posY.i.hi -= 4;
            }
            if (self->ext.et_801A19CC.unk7D) {
                MoveEntity();
                if (self->animCurFrame == 0x2A || self->animCurFrame == 0x2B) {
                    self->velocityY -= FIX(0.375);
                }
            }
            if (self->posY.i.hi + g_Tilemap.scrollY.i.hi < 0x72) {
                self->velocityY = FIX(-4.5);
                self->animCurFrame = 0x31;
                self->ext.et_801A19CC.unk7D = 0;
                self->step_s = 2;
            }
            break;
        case 1:
            MoveEntity();
            self->velocityY += 0x1E00;
            if (!--self->ext.et_801A19CC.unk7C) {
                self->pose = 0;
                self->ext.et_801A19CC.unk7C = 0;
                self->step_s++;
            }
            if (self->posY.i.hi + g_Tilemap.scrollY.i.hi < 0x72) {
                self->velocityY = FIX(-4.5);
                self->animCurFrame = 0x31;
                self->ext.et_801A19CC.unk7D = 0;
                self->step_s = 2;
            }
            break;
        case 2:
            MoveEntity();
            self->velocityY += FIX(0.125);
            if (!AnimateEntity(D_us_801808D4, self)) {
                func_us_801A19CC(6);
            }
            break;
        }
        break;

    case 6:
        if (self->step_s == 0) {
            self->ext.et_801A19CC.unk7C = 0x50;
            if (self->facingLeft) {
                self->velocityX = FIX(1.75);
            } else {
                self->velocityX = FIX(-1.75);
            }
            self->velocityY = 0;
            self->step_s++;
        }
        MoveEntity();
        AnimateEntity(D_us_80180830, self);
        if (self->pose == 1 && self->poseTimer == 0) {
            PlaySfxPositional(0x768);
        }
        if (self->animCurFrame == 6 || self->animCurFrame == 7) {
            self->velocityY -= FIX(0.15625);
        } else {
            self->velocityY += 0xE00;
        }
        if (!--self->ext.et_801A19CC.unk7C) {
            posX = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
            if (self->velocityX < 0) {
                posX = 0x200 - posX;
            }
            if (GetDistanceToPlayerX() > 0x50 && posX < 0x90) {
                self->velocityX = 0;
                self->velocityY = 0;
                func_us_801A19CC(0x16);
            } else if (posX < 0x180) {
                func_us_801A19CC(8);
            } else {
                self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
                func_us_801A19CC(6);
            }
        }
        break;

    case 8:
        switch (self->step_s) {
        case 0:
            if (self->facingLeft) {
                self->velocityX = FIX(0.5);
            } else {
                self->velocityX = FIX(-0.5);
            }
            self->velocityY = FIX(3.5);
            self->step_s++;
            // fallthrough
        case 1:
            if (!AnimateEntity(D_us_80180938, self)) {
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s++;
            }
            MoveEntity();
            if (self->posY.i.hi + g_Tilemap.scrollY.i.hi > 0x88) {
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s = 2;
            }
            break;
        case 2:
            AnimateEntity(D_us_80180944, self);
            MoveEntity();
            if (CheckColliderOffsets(D_us_80180ADC, 0)) {
                self->pose = 0;
                self->poseTimer = 0;
                self->ext.et_801A19CC.unk7D = 0;
                self->step_s++;
            }
            break;
        case 3:
            if ((CheckHippogryphFloorCollision(D_us_80180AAC) & 1) &&
                !self->ext.et_801A19CC.unk7D) {
                self->ext.et_801A19CC.unk7D = 1;
            }
            self->velocityY -= 0x7C00;
            if (self->velocityY <= FIX(0.5)) {
                self->velocityY = FIX(0.5);
            }
            if (!AnimateEntity(D_us_80180950, self) &&
                self->ext.et_801A19CC.unk7D) {
                func_us_801A19CC(0xA);
                self->ext.et_801A19CC.unk7D = 0;
            }
            break;
        }
        break;

    case 0xA:
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        i = Random() & 7;
        dx = GetDistanceToPlayerX();
        if (dx > 0x50) {
            i += 4;
        }
        if (dx > 0x80) {
            i += 4;
        }
        func_us_801A19CC(D_us_80180AEC[i & 0xFF]);
        break;

    case 0xC:
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_80180858, self)) {
                self->ext.et_801A19CC.unk7C = 0x40;
                self->step_s++;
                PlaySfxPositional(0x6C3);
            }
            if (self->pose == 4 && self->poseTimer == 0) {
                PlaySfxPositional(0x614);
            }
            break;
        case 1:
            if (self->ext.et_801A19CC.unk7C & 1) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_27, self, entity);
                    entity->pfnUpdate = func_us_801A3B88;
                    entity->facingLeft = self->facingLeft;
                    if (self->facingLeft) {
                        entity->posX.i.hi += 0x1D;
                    } else {
                        entity->posX.i.hi -= 0x1D;
                    }
                    entity->posY.i.hi -= 2;
                    entity->params = self->ext.et_801A19CC.unk7D;
                }
                if (self->ext.et_801A19CC.unk7D) {
                    entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                } else {
                    entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                }
                if (entity != NULL) {
                    DestroyEntity(entity);
                    CreateEntityFromEntity(E_UNK_27, self, entity);
                    entity->pfnUpdate = func_us_801A3B88;
                    entity->facingLeft = self->facingLeft;
                    if (self->facingLeft) {
                        entity->posX.i.hi += 0x1D;
                    } else {
                        entity->posX.i.hi -= 0x1D;
                    }
                    entity->params = 2;
                    entity->posY.i.hi -= 2;
                    entity->params = self->ext.et_801A19CC.unk7D + 2;
                }
                self->ext.et_801A19CC.unk7D ^= 1;
            }
            if (!--self->ext.et_801A19CC.unk7C) {
                self->ext.et_801A19CC.unk7D = 0;
                func_us_801A19CC(0xA);
            }
            break;
        }
        break;

    case 0xD:
        if (!self->ext.et_801A19CC.unk7D) {
            self->ext.et_801A19CC.unk7D = 1;
            self->ext.et_801A19CC.unk7C = 0x80;
            if (self->facingLeft) {
                self->velocityX = FIX(1.25);
            } else {
                self->velocityX = FIX(-1.25);
            }
            self->velocityY = 0;
        }
        MoveEntity();
        AnimateEntity(D_us_80180824, self);
        if (self->animCurFrame == 5) {
            if (self->poseTimer == 0) {
                PlaySfxPositional(0x646);
            }
            if (self->facingLeft) {
                self->velocityX -= 0x1C00;
            } else {
                self->velocityX += 0x1C00;
            }
        }
        if (self->animCurFrame == 1) {
            if (self->facingLeft) {
                self->velocityX += 0x1C00;
            } else {
                self->velocityX -= 0x1C00;
            }
        }
        posX = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
        if (self->facingLeft) {
            if (posX > 0x180) {
                self->facingLeft = 0;
                self->velocityX = FIX(-1.25);
                self->pose = 0;
                self->poseTimer = 0;
            }
        } else if (posX < 0x80) {
            self->facingLeft = 1;
            self->velocityX = FIX(1.25);
            self->pose = 0;
            self->poseTimer = 0;
        }
        if (!CheckColliderOffsets(D_us_80180AE4, self->facingLeft)) {
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            func_us_801A19CC(4);
        }
        if (!--self->ext.et_801A19CC.unk7C) {
            func_us_801A19CC(0xA);
        }
        break;

    case 0x10:
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_us_801808FC, self)) {
                func_us_801A19FC(4, 0);
                self->ext.et_801A19CC.unk7C = 0x40;
                if (self->facingLeft) {
                    self->velocityX = FIX(4.5);
                } else {
                    self->velocityX = FIX(-4.5);
                }
                self->velocityY = 0;
                self->pose = 0;
                self->poseTimer = 0;
                self->animCurFrame = 0x21;
                self->step_s++;
            }
            break;
        case 1:
            if (self->ext.et_801A19CC.unk7C < 0x20) {
                AnimateEntity(D_us_80180884, self);
            }
            MoveEntity();
            self->velocityY -= FIX(1.0 / 16);
            if (self->posY.i.hi + g_Tilemap.scrollY.i.hi < 0x48) {
                self->step_s++;
            }
            break;
        case 2:
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            func_us_801A19CC(6);
            break;
        }
        break;

    case 0x14:
        if (!AnimateEntity(D_us_8018098C, self)) {
            self->ext.et_801A19CC.unk7D = 0;
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            func_us_801A19CC(4);
        }
        if (self->animCurFrame == 0x4D && !self->ext.et_801A19CC.unk7D) {
            self->ext.et_801A19CC.unk7D = 1;
            count = (Random() & 1) | 2;
            velX = FIX(-0.5);
            velY = 0;
            for (i = 0; i < count; i++) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_25, self, entity);
                    entity->ext.et_801A3FD4.timer = (Random() & 0x1F) - 0x40;
                    if (self->facingLeft) {
                        entity->velocityX = velY + FIX(0.5);
                        entity->posX.i.hi -= 0x10;
                    } else {
                        entity->velocityX = velX;
                        entity->posX.i.hi += 0x10;
                    }
                }
                velX += FIX(0.5);
                velY -= FIX(0.5);
            }
        }
        break;

    case 0x12:
        switch (self->step_s) {
        case 0:
            AnimateEntity(D_us_8018088C, self);
            MoveEntity();
            if (GetDistanceToPlayerY() < 0x50) {
                self->step_s++;
            }
            break;
        case 1:
            MoveEntity();
            if (!AnimateEntity(D_us_80180894, self)) {
                self->step_s++;
            }
            self->velocityY -= FIX(1.0 / 32);
            if (!g_Player.grabState) {
                posX = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
                if (self->facingLeft) {
                    posX = 0x200 - posX;
                }
                if (posX >= 0x80) {
                    dy = PLAYER.posY.i.hi - self->posY.i.hi;
                    dx = PLAYER.posX.i.hi - self->posX.i.hi;
                    if (dy < 0) {
                        dy = 0x80;
                    }
                    if (abs(dx) < 0xC && dy < 0x38 &&
                        !(g_Player.status & 0x40C401D2)) {
                        PlaySfxPositional(0x769);
                        g_Player.grabState = 1;
                        g_Player.grabDamage = 0x10;
                        g_Player.grabReactionEffect = 0;
                        self->posX.i.hi = PLAYER.posX.i.hi;
                        self->posY.i.hi = PLAYER.posY.i.hi - 0x20;
                    }
                }
            }
            break;
        case 2:
            MoveEntity();
            self->velocityY -= FIX(0.375);
            if (!AnimateEntity(D_us_8018089C, self)) {
                self->ext.et_801A19CC.unk7C = 0x40;
                self->step_s++;
                if (!g_Player.grabState) {
                    self->velocityY -= FIX(0.375);
                }
            }
            break;
        case 3:
            if (g_Player.grabState) {
                AnimateEntity(D_us_801808AC, self);
                if (!((u8)--self->ext.et_801A19CC.unk7C % 47)) {
                    func_us_801A19FC(8, -8);
                    g_Player.grabState = 3;
                    g_Player.grabDamage = 0x10;
                }
            } else {
                AnimateEntity(D_us_80180830, self);
                if (self->pose == 1 && self->poseTimer == 0) {
                    PlaySfxPositional(0x768);
                }
            }
            MoveEntity();
            if (self->posY.i.hi + g_Tilemap.scrollY.i.hi < 0x48) {
                g_Player.grabState = 0;
                self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
                func_us_801A19CC(6);
            }
            break;
        }
        if (g_Player.grabState) {
            PLAYER.posX.i.hi = self->posX.i.hi;
            PLAYER.posY.i.hi = self->posY.i.hi + 0x20;
        }
        break;

    case 0x16:
        switch (self->step_s) {
        case 0:
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            if (!AnimateEntity(D_us_80180844, self)) {
                self->ext.et_801A19CC.unk7D++;
            }
            if (self->pose == 1 && self->poseTimer == 0) {
                PlaySfxPositional(0x768);
            }
            MoveEntity();
            if (self->animCurFrame == 6 || self->animCurFrame == 7) {
                self->velocityY -= FIX(0.15625);
            } else {
                self->velocityY += FIX(1.0 / 16);
            }
            if (self->ext.et_801A19CC.unk7D >= 2) {
                self->ext.et_801A19CC.unk7D = 0;
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s++;
                if (self->facingLeft) {
                    self->velocityX = FIX(4);
                } else {
                    self->velocityX = FIX(-4);
                }
                self->velocityY = FIX(4.5);
            }
            break;
        case 1:
            MoveEntity();
            AnimateEntity(D_us_80180884, self);
            self->velocityY -= FIX(0.09375);
            if (self->velocityY <= FIX(0.5)) {
                self->velocityY = FIX(0.5);
            }
            if (CheckColliderOffsets(D_us_80180ADC, 0)) {
                self->pose = 0;
                self->poseTimer = 0;
                self->ext.et_801A19CC.unk7D = 0;
                self->step_s++;
            }
            break;
        case 2:
            self->velocityY -= FIX(0.21875);
            if (CheckHippogryphFloorCollision(D_us_80180AAC) & 1) {
                self->ext.et_801A19CC.unk7D = 1;
            }
            if (self->facingLeft) {
                self->velocityX -= FIX(1.0 / 16);
            } else {
                self->velocityX += FIX(1.0 / 16);
            }
            if (self->animCurFrame == 0x5E) {
                func_us_801A19FC(2, -2);
            }
            if (!AnimateEntity(D_us_8018096C, self) &&
                self->ext.et_801A19CC.unk7D) {
                func_us_801A19CC(0xA);
            }
            break;
        }
        break;

    case 0x20:
        AnimateEntity(D_us_80180928, self);
        if (CheckHippogryphFloorCollision(D_us_80180AAC) & 1) {
            self->animCurFrame = 0x50;
            func_us_801A19CC(0xA);
        }
        break;

    case 0x24:
        switch (self->step_s) {
        case 0:
            if (g_Player.grabState) {
                g_Player.grabState = 0;
            }
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_HYPPOGRYPH_DEFEAT, TIMEATTACK_SET_RECORD);
            self->velocityY = 0;
            posX = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
            D_us_801806E0 |= 2;
            if (posX < 0x38) {
                self->velocityX = FIX(0.5);
            }
            if (posX < 0x1D8) {
                self->velocityX = FIX(-0.5);
            }
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            self->velocityY += FIX(0.25);
            if (self->posY.i.hi + g_Tilemap.scrollY.i.hi > 0x80) {
                self->step_s++;
            }
            break;
        case 2:
            if (CheckHippogryphFloorCollision(D_us_80180AAC) & 1) {
                func_us_801A19FC(0x16, -0x20);
                self->step_s = 3;
            } else if (self->posY.i.hi + g_Tilemap.scrollY.i.hi >= 0xB8) {
                func_us_801A19FC(0x16, -0x20);
                self->step_s = 3;
                self->velocityX = 0;
                self->velocityY = 0;
                self->posY.i.hi = 0xB7 - g_Tilemap.scrollY.i.hi;
            }
            break;
        case 3:
            primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 2);
            if (primIndex != -1) {
                prim = &g_PrimBuf[primIndex];
                self->primIndex = primIndex;
                self->ext.et_801A19CC.lines = prim;
                self->flags |= FLAG_HAS_PRIMS;
                while (prim != NULL) {
                    prim->r0 = 0x80;
                    prim->r1 = 0x40;
                    prim->g0 = 0x60;
                    prim->g1 = 0x20;
                    prim->b0 = 0x60;
                    prim->b1 = 0x20;
                    prim->x0 = prim->x1 = self->posX.i.hi;
                    prim->y0 = prim->y1 = self->posY.i.hi;
                    prim->drawMode =
                        DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
                    prim->priority = self->zPriority + 2;
                    prim = prim->next;
                }
                self->step_s++;
            } else {
                self->step_s = 5;
                self->ext.et_801A19CC.lines = NULL;
            }
            self->zPriority = self->ext.et_801A19CC.zPriority - 8;
            break;
        case 4:
            self->ext.et_801A19CC.unk7C += 6;
            delta = 6;
            if (self->ext.et_801A19CC.unk7C > 0x80) {
                delta = -8;
            }
            prim = self->ext.et_801A19CC.lines;
            prim->x1 += delta;
            prim = prim->next;
            prim->x1 -= delta;
            if (self->ext.et_801A19CC.unk7C > 0xF0) {
                g_api.FreePrimitives(self->primIndex);
                self->ext.et_801A19CC.unk7C = 0;
                self->flags &= ~FLAG_HAS_PRIMS;
                self->step_s++;
            }
            break;
        case 5:
            primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 0x6F);
            if (primIndex == -1) {
                self->step_s = 8;
                break;
            }
            prim = &g_PrimBuf[primIndex];
            self->primIndex = primIndex;
            self->ext.et_801A19CC.lines = prim;
            self->flags |= FLAG_HAS_PRIMS;
            // light rays: x2:x3 is the growth speed, y2:y3 the length
            for (i = 0; i < 0x1F; i++) {
                prim->type = PRIM_LINE_G2;
                prim->r0 = 0x60;
                prim->g0 = 0x40;
                prim->b0 = 0x40;
                prim->r1 = 0x10;
                prim->g1 = 8;
                prim->b1 = 8;
                prim->x0 = prim->x1 = self->posX.i.hi;
                prim->y0 = prim->y1 = self->posY.i.hi;
                speed = D_us_801806E4[i];
                prim->x2 = speed >> 16;
                prim->y2 = 0;
                prim->y3 = 0;
                prim->x3 = speed;
                prim->drawMode =
                    DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
                prim->priority = self->zPriority + 2;
                prim = prim->next;
            }
            // sparks: x0:x1 / y0:y1 position, x2:x3 / y2:y3 velocity
            self->ext.et_801A19CC.particles = prim;
            for (i = 0; i < 0x50; i++) {
                prim->type = PRIM_TILE;
                prim->r0 = prim->g0 = prim->b0 = 0x70;
                prim->u0 = 2;
                prim->v0 = 2;
                prim->drawMode =
                    DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
                prim->priority = self->zPriority + 2;
                prim->x0 = self->posX.i.hi;
                prim->x1 = 0;
                prim->y1 = 0;
                prim->y0 = self->posY.i.hi;
                radius = Random() & 7;
                angle = (Random() & 0xF) << 8;
                velX = (rsin(angle) * D_us_801807E4[radius]) >> 0xC;
                prim->x2 = velX >> 16;
                prim->x3 = velX;
                velY = (rcos(angle) * D_us_801807E4[radius]) >> 0xC;
                prim->y2 = velY >> 16;
                prim->y3 = velY;
                prim->r1 = Random() & 0x17;
                prim->r2 = 0;
                prim = prim->next;
            }
            self->step_s++;
            break;
        case 6:
        case 7:
            if (self->step_s == 6) {
                AnimateEntity(D_us_801809D8, self);
            }
            prim = self->ext.et_801A19CC.particles;
            for (i = 0; i < 0x50; i++) {
                if (prim->r1) {
                    prim->r1--;
                } else {
                    posX = HILO(prim->x0, prim->x1) + HILO(prim->x2, prim->x3);
                    posY = HILO(prim->y0, prim->y1) + HILO(prim->y2, prim->y3);
                    prim->x0 = posX >> 16;
                    prim->x1 = posX;
                    prim->y0 = posY >> 16;
                    prim->r2++;
                    prim->y1 = posY;
                    if (prim->r2 > 0x20) {
                        prim->v0 = 1;
                        prim->u0 = 1;
                    }
                    if (prim->r2 > 0x30) {
                        prim->v0 = 0;
                        prim->u0 = 0;
                    }
                }
                prim = prim->next;
            }
            prim = self->ext.et_801A19CC.lines;
            for (i = 0; i < 0x1F; i++) {
                if (self->step_s == 6) {
                    speed = HILO(prim->x2, prim->x3);
                } else {
                    // the rays fade out and shrink back
                    prim->r0 -= 2;
                    prim->g0--;
                    prim->b0--;
                    speed = HILO(-prim->x2, -prim->x3);
                }
                radius = HILO(prim->y2, prim->y3);
                if (self->ext.et_801A19CC.unk7C <
                    (self->step_s == 6 ? 0xC : 0x14)) {
                    radius += speed;
                }
                angle = (i << 7) + self->ext.et_801A19CC.unk7C * 0x10;
                prim->x1 = prim->x0 + (((s16)(radius >> 16) * rcos(angle)) >> 0xC);
                prim->y1 = prim->y0 + (((s16)(radius >> 16) * rsin(angle)) >> 0xC);
                prim->y2 = radius >> 16;
                prim->y3 = radius;
                prim = prim->next;
            }
            if (self->step_s == 6) {
                if (self->ext.et_801A19CC.unk7C++ > 0x30) {
                    self->blendMode = BLEND_TRANSP | BLEND_ADD;
                    self->opacity = 0x80;
                    self->drawFlags |= ENTITY_OPACITY;
                    self->step_s++;
                }
            } else {
                self->opacity -= 4;
                if (self->opacity > 0xE0) {
                    self->opacity = 0;
                }
            }
            if (!(g_Timer & 7)) {
                PlaySfxPositional(0x6AF);
            }
            if (self->step_s == 7 && self->ext.et_801A19CC.unk7C++ > 0x4E) {
                self->ext.et_801A19CC.unk7C = 0;
                g_api.FreePrimitives(self->primIndex);
                self->flags &= ~FLAG_HAS_PRIMS;
                self->step_s++;
            }
            break;
        case 8:
            posX = 0x100 - g_Tilemap.scrollX.i.hi;
            posY = 0x80 - g_Tilemap.scrollY.i.hi;
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromCurrentEntity(E_LIFE_UP_SPAWN, entity);
                entity->posX.i.hi = posX;
                entity->posY.i.hi = posY;
                entity->params = 6;
                D_us_80180B74 = 0;
                CreateEntityFromCurrentEntity(E_CUTSCENE, &g_Entities[200]);
                self->step_s++;
            }
            break;
        }
        break;

    case 0x30:
        AnimateEntity(D_us_80180A2C, self);
        self->velocityY -= FIX(0.21875);
        if (UnkCollisionFunc3(D_us_80180ACC) & 1) {
            DestroyEntity(self);
        }
        break;

    case 0x32:
        AnimateEntity(D_us_80180A48, self);
        self->velocityX += self->ext.et_801A19CC.accelX;
        if ((u32)(self->velocityX + 0xD000) > 0x1A000) {
            self->ext.et_801A19CC.accelX = -self->ext.et_801A19CC.accelX;
        }
        self->velocityY -= FIX(0.21875);
        if (UnkCollisionFunc3(D_us_80180ACC) & 1) {
            DestroyEntity(self);
        }
        break;

    case 0x82:
        FntPrint("charal %x\n", self->animCurFrame);
        if (g_pads[0].pressed & 0x80) {
            if (self->params == 0) {
                self->animCurFrame++;
                self->params |= 1;
            }
        } else {
            self->params = 0;
        }
        break;
    }

    frame = self->animCurFrame;
    if (frame >= 0xE && frame <= 0x18) {
        self->hitboxWidth = 6;
        self->hitboxHeight = 0x14;
        self->hitboxOffX = 8;
        self->hitboxOffY = -8;
    } else if (frame >= 0x3A && frame <= 0x3F) {
        self->hitboxWidth = 6;
        self->hitboxHeight = 0x14;
        self->hitboxOffX = 0;
        self->hitboxOffY = -8;
    } else {
        self->hitboxWidth = 0x14;
        self->hitboxHeight = 8;
        self->hitboxOffX = 0;
        self->hitboxOffY = -2;
    }
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_hippogryph", func_us_801A3B88);
#else
extern AnimateEntityFrame D_us_80180A00[];
extern AnimateEntityFrame D_us_80180A0C[];
extern EInit D_us_801804F0;

// Hippogryph fire breath
void func_us_801A3B88(Entity* self) {
    Entity* entity;
    s16 scale;

    if (D_us_801806E0 & 2) {
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804F0);
        self->animSet = 2;
        self->animCurFrame = 1;
        self->opacity = 0x80;
        self->rotate = ROT(315);
        self->scaleX = self->scaleY = 0x20;
        self->drawFlags |=
            ENTITY_OPACITY | ENTITY_ROTATE | ENTITY_SCALEY | ENTITY_SCALEX;
        self->hitboxState = 0;
        self->blendMode |= BLEND_TRANSP | BLEND_ADD;
        if (self->facingLeft) {
            self->velocityX = FIX(4.40625);
        } else {
            self->velocityX = FIX(-4.40625);
        }
        if (self->params) {
            self->velocityY = FIX(0.1875);
        } else {
            self->velocityY = FIX(-0.375);
        }
        if (self->params >= 2) {
            self->opacity = 0x7C;
            self->velocityY = 0;
            if (self->params == 3) {
                self->hitboxState = 1;
            }
        }
        break;
    case 1:
        AnimateEntity(D_us_80180A00, self);
        MoveEntity();
        self->velocityY -= 0x1400;
        scale = self->scaleX + 0x18;
        self->scaleX = scale;
        self->scaleY = scale;
        self->opacity -= 5;
        if (self->scaleX > 0xF0) {
            self->velocityX /= 4;
            self->pose = 0;
            self->poseTimer = 0;
            self->step++;
        }
        if (self->params == 3 && (Random() & 1)) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                DestroyEntity(entity);
                CreateEntityFromEntity(E_UNK_26, self, entity);
                entity->facingLeft = self->facingLeft;
                entity->params = self->params;
                entity->scaleX = self->scaleX;
                entity->scaleY = self->scaleY;
            }
        }
        if (self->params == 0 && (Random() & 7)) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                DestroyEntity(entity);
                CreateEntityFromEntity(E_UNK_26, self, entity);
                entity->facingLeft = self->facingLeft;
                entity->params = self->params;
                entity->scaleX = self->scaleX;
                entity->scaleY = self->scaleY;
            }
        }
        break;
    case 2:
        MoveEntity();
        if (!AnimateEntity(D_us_80180A0C, self)) {
            DestroyEntity(self);
        }
        break;
    }
}
#endif

extern EInit g_EInitParticle;
extern AnimateEntityFrame D_us_80180A18[];

void func_us_801A3E78(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        self->animSet = 2;
        self->animCurFrame = 1;
        self->drawFlags |= ENTITY_SCALEX | ENTITY_SCALEY | ENTITY_OPACITY;
        self->drawFlags |= ENTITY_ROTATE;
        self->blendMode |= BLEND_TRANSP | BLEND_ADD;
        self->opacity = 0x80;
        if (self->facingLeft) {
            self->velocityX = FIX(1);
        } else {
            self->velocityX = -FIX(1);
        }
        if (self->params) {
            self->velocityY = FIX(0.5);
            return;
        }
        self->velocityY = -FIX(1);
        return;

    case 1:
        MoveEntity();
        if (self->facingLeft) {
            self->velocityX -= FIX(0.125);
        } else {
            self->velocityX += FIX(0.125);
        }
        self->velocityY -= FIX(0.0625);
        if (self->params) {
            self->rotate -= ROT(5.625);
        } else {
            self->rotate += ROT(5.625);
        }
        self->scaleX -= 0xA;
        self->opacity -= 6;
        if (!AnimateEntity(D_us_80180A18, self)) {
            DestroyEntity(self);
        }
        return;
    }
}

extern s32 D_us_801806E0;
extern EInit D_us_801804E4;
extern s16 D_us_80180ABC[];
extern AnimateEntityFrame D_us_801809F0[];
extern AnimateEntityFrame D_us_801809F8[];

void func_us_801A3FD4(Entity* self) {
    Entity* entity;

    if (D_us_801806E0 & 2) {
        self->flags |= FLAG_DEAD;
    }

    if (self->flags & FLAG_DEAD) {
        entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (entity != NULL) {
            DestroyEntity(entity);
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 0;
        }
        DestroyEntity(self);
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804E4);
        self->hitboxState = 2;
        self->drawFlags |= ENTITY_ROTATE;
        self->rotate = ((Random() & 3) << 9) - 0x300;
        self->ext.et_801A3FD4.rotateDir = 1;
        return;

    case 1:
        if (UnkCollisionFunc3(D_us_80180ABC) & 1) {
            self->step++;
        }

    case 2:
        if (!self->ext.et_801A3FD4.timer) {
            if (AnimateEntity(D_us_801809F0, self)) {
                return;
            }
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                DestroyEntity(entity);
                CreateEntityFromEntity(E_EXPLOSION, self, entity);
                entity->params = 0;
            }
            self->facingLeft = GetSideToPlayer() & 1;
            self->drawFlags = ENTITY_DEFAULT;
            if (self->facingLeft) {
                self->velocityX = -FIX(2.5);
            } else {
                self->velocityX = FIX(2.5);
            }
            self->velocityY = -FIX(0.25);
            self->pose = 0;
            self->poseTimer = 0;
            self->hitboxState = 3;
            PlaySfxPositional(SFX_NO1_BIRD_CYCLE);
            self->step++;
            return;
        } else if (self->ext.et_801A3FD4.timer-- < 8) {
            self->rotate += self->ext.et_801A3FD4.rotateDir << 9;
            self->ext.et_801A3FD4.rotateDir = -self->ext.et_801A3FD4.rotateDir;
            return;
        }
        break;

    case 3:
        AnimateEntity(D_us_801809F8, self);
        MoveEntity();
        self->velocityY -= FIX(0.0390625);
        break;
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("boss/bo5/nonmatchings/e_hippogryph", func_us_801A425C);
#else
extern u8 D_us_80180B0C[];
extern EInit g_EInitInteractable;

// Hippogryph head hitbox, follows the Hippogryph in the previous slot
void func_us_801A425C(Entity* self) {
    Entity* parent;
    u8 frame;

    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->hitPoints = 0x7FFE;
        self->hitboxState = 2;
        self->parent = self - 1;
        self->nextPart = self - 1;
        self->attack = 0;
        self->attackElement = 0;
        self->flags |= 0x400000;
    }
    parent = self - 1;
    frame = parent->animCurFrame;
    if (frame >= 0x68) {
        frame = 0;
    }
    switch (D_us_80180B0C[frame]) {
    case 0:
        self->hitboxOffX = 0;
        self->hitboxOffY = 0;
        self->hitboxWidth = 0;
        self->hitboxHeight = 0;
        break;
    case 1:
        self->hitboxOffX = 8;
        self->hitboxOffY = -0x18;
        self->hitboxWidth = 0x14;
        self->hitboxHeight = 0x12;
        break;
    case 2:
        self->hitboxOffX = 0xC;
        self->hitboxOffY = -8;
        self->hitboxWidth = 0x12;
        self->hitboxHeight = 8;
        break;
    case 3:
        self->hitboxOffX = -2;
        self->hitboxOffY = 0x18;
        self->hitboxWidth = 8;
        self->hitboxHeight = 0x12;
        break;
    case 4:
        self->hitboxOffX = 6;
        self->hitboxOffY = 0xA;
        self->hitboxWidth = 0x10;
        self->hitboxHeight = 0xA;
        break;
    case 5:
        self->hitboxOffX = 0x10;
        self->hitboxOffY = -0x20;
        self->hitboxWidth = 0x16;
        self->hitboxHeight = 0x16;
        break;
    case 6:
        self->hitboxOffX = 2;
        self->hitboxOffY = -0x20;
        self->hitboxWidth = 0xC;
        self->hitboxHeight = 0x18;
        break;
    }
    self->facingLeft = parent->facingLeft;
    self->posX.i.hi = parent->posX.i.hi;
    self->posY.i.hi = parent->posY.i.hi;
    if (parent->entityId != E_UNK_22) {
        DestroyEntity(self);
    }
}
#endif

extern EInit D_us_801804F0;

void EntityFireDamageHitbox(Entity* self) {
    if (!self->step) {
        InitializeEntity(D_us_801804F0);
        self->hitboxWidth = 24;
        self->hitboxHeight = 4;
        self->hitboxOffX = -48;
        self->hitboxOffY = -2;
    }
}
