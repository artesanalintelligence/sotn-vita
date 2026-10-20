// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo6.h"

extern s32 D_us_80181278;
extern AnimationFrame D_us_80181F1C[];
extern AnimationFrame D_us_801823C8[];

// ending 2 function
void func_us_801B9144(void) {
    Entity* entity;
    switch (RIC.step_s) {
    case 0:
        RicSetAnimation(D_us_80181F1C);
        g_api.PlaySfx(SFX_BOSS_RIC_LAUGH);
        if (RIC.posX.i.hi < 0x80) {
            RIC.facingLeft = 0;
        } else {
            RIC.facingLeft = 1;
        }
        RIC.step_s++;
        // fallthrough

    case 1:
        D_us_80181278 = 0x14;
        entity = &g_Entities[200];
        CreateEntityFromCurrentEntity(E_ID_17, entity);
        entity->params = 1;
        RIC.step_s++;
        // fallthrough

    case 2:
        if (D_us_80181278 == 0x1E) {
            RicSetAnimation(D_us_801823C8);
            RicCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(E_ID_24, 0x1), 0);
            RIC.step_s++;
        }
        break;
    case 3:
        if (RIC.animCurFrame == 0xB5) {
            if (RIC.poseTimer == 1) {
                RicCreateEntFactoryFromEntity(
                    g_CurrentEntity, FACTORY(E_ID_23, 0), 0);
                g_api.PlaySfx(SFX_WEAPON_APPEAR);
            }
        }
        if (RIC.poseTimer < 0) {
            D_us_80181278 = 0x28;
            RicSetStand(0);
            RicCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(E_ID_21, 0x45), 0);
            g_Ric.timers[ALU_T_POISON] = 0x800;
        }
        break;
    }
}

void func_us_801B9338(void) {}

extern s16 D_us_8018221C[];

void func_us_801B9340(void) {
    switch (RIC.step_s) {
    case 0:
        RicResetPose();
        RIC.velocityY = FIX(-5);
        func_us_801B9ACC(0xFFFF1000);
        RIC.anim = D_us_8018221C;
        g_api.PlaySfx(SFX_BOSS_RIC_DEATH);
        g_Ric.damagePalette = PAL_FLAG(PAL_CC_RED_EFFECT_A);
        g_Ric.timers[2] = 8;
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(E_ID_21, 0x58), 0);
        RIC.step_s += 1;
        return;
    case 1:
        if ((g_Ric.vram_flag & TOUCHING_CEILING) && (FIX(-1) > RIC.velocityY)) {
            RIC.velocityY = FIX(-1);
        }
        if (RicCheckInput(0x20280) != 0) {
            RIC.step = 0x70;
            RIC.step_s = 2;
            return;
        }
        return;
    case 2:
        DecelerateX(FIX(0.125));
        if ((PLAYER.posX.i.hi - RIC.posX.i.hi) > 0) {
            RIC.facingLeft = 0;
            return;
        }
        RIC.facingLeft = 1;
        break;
    }
}

// split pl_utils

// maybe func_8015C4AC
extern u8 D_us_80181284[];

// Same as InitRicAfterImage in RIC
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801B94CC);
#else
void func_us_801B94CC(void) {
    Primitive* prim;
    s32 i;

    if (g_Entities[65].ext.afterImage.disableFlag) {
        return;
    }
    if ((g_Ric.padTapped & ~(PAD_START | PAD_SELECT)) ||
        ((g_Ric.padHeld ^ g_Ric.padPressed) & g_Ric.padHeld &
         ~(PAD_START | PAD_SELECT)) ||
        (RIC.velocityY > FIX(0.5))) {
        g_Entities[65].ext.afterImage.index = 0;
        g_Entities[65].ext.afterImage.timer = 0;
    } else {
        if (g_Entities[65].ext.afterImage.index >= 10) {
            return;
        }
        if (g_Entities[65].ext.afterImage.timer == 0) {
            g_Entities[65].ext.afterImage.timer =
                D_us_80181284[g_Entities[65].ext.afterImage.index];
        }
        if (--g_Entities[65].ext.afterImage.timer == 0) {
            g_Entities[65].ext.afterImage.index++;
            g_Entities[65].ext.afterImage.timer =
                D_us_80181284[g_Entities[65].ext.afterImage.index];
        }
    }
    if (g_Entities[65].pose) {
        g_Entities[65].pose--;
        return;
    }
    for (prim = &g_PrimBuf[g_Entities[65].primIndex], i = 0; prim != NULL;
         i++, prim = prim->next) {
        if (i == g_Entities[65].entityId) {
            prim->r0 = prim->g0 = prim->b0 = 0x80;
            prim->x0 = RIC.posX.i.hi;
            prim->y0 = RIC.posY.i.hi;
            prim->x1 = RIC.animCurFrame;
            prim->y1 = 0;
            prim->x2 = RIC.facingLeft;
            prim->y2 = RIC.palette;
        }
    }
    g_Entities[65].pose = 2;
    g_Entities[65].entityId++;
    if (g_Entities[65].entityId >= 6) {
        g_Entities[65].entityId = 0;
    }
}
#endif

// maybe func_8015C6D4
extern u8 D_us_80181298[];
extern u8 D_us_801812A8[];

void func_us_801B96F4(void) {
    byte pad[0x28];
    Primitive* prim;
    PlayerDraw* draw;
    s32 i;
    u8 var_s3;
    u8 var_s5;
    u8 resetAnim;

    resetAnim = g_Entities[65].ext.afterImage.resetFlag;
    prim = &g_PrimBuf[g_Entities[65].primIndex];
    i = 0;
    draw = &g_PlayerDraw[9];
    var_s5 = D_us_80181298[g_Entities[65].ext.afterImage.index];
    var_s3 = D_us_801812A8[g_Entities[65].ext.afterImage.index];
    while (prim != NULL) {
        if (prim->r0 > var_s3) {
            prim->r0 -= var_s5;
        }
        if (prim->r0 < 112 && prim->b0 < 240) {
            prim->b0 += 6;
        }
        if (prim->r0 < 88) {
            prim->y1 = 16;
        } else {
            prim->y1 = 0;
        }
        if (prim->r0 <= var_s3) {
            prim->x1 = 0;
        }
        if ((i ^ g_Timer) & 1) {
            g_Entities[i / 2 + 65].posX.i.hi = prim->x0;
            g_Entities[i / 2 + 65].posY.i.hi = prim->y0;
            g_Entities[i / 2 + 65].animCurFrame = prim->x1;
            g_Entities[i / 2 + 65].blendMode = prim->y1;
            g_Entities[i / 2 + 65].facingLeft = prim->x2;
            g_Entities[i / 2 + 65].palette = prim->y2;
            g_Entities[i / 2 + 65].zPriority = RIC.zPriority - 2;
            if (resetAnim) {
                g_Entities[i / 2 + 65].animCurFrame = 0;
                prim->x1 = 0;
            }

            draw->r0 = draw->r1 = draw->r2 = draw->r3 = draw->g0 = draw->g1 =
                draw->g2 = draw->g3 = prim->r0;
            draw->b0 = draw->b1 = draw->b2 = draw->b3 = prim->b0;
            draw->enableColorBlend = true;
            draw++;
        }
        i++;
        prim = prim->next;
    }
}

// RicSetStep
void RicSetStep(s16 step) {
    RIC.step = step;
    RIC.step_s = 0;
}

void RicSetAnimation(AnimationFrame* anim) {
    g_CurrentEntity->anim = anim;
    g_CurrentEntity->poseTimer = 0;
    g_CurrentEntity->pose = 0;
}

#include "../../decelerate.h"

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicCheckFacing);
#else
s32 RicCheckFacing(void) {
    if (g_Ric.unk44 & 2) {
        return 0;
    }
    if (RIC.facingLeft == 1) {
        if (g_Ric.padPressed & PAD_RIGHT) {
            RIC.facingLeft = 0;
            g_Ric.unk4C = 1;
            return -1;
        }
        if (g_Ric.padPressed & PAD_LEFT) {
            return 1;
        }
        return 0;
    }
    if (g_Ric.padPressed & PAD_RIGHT) {
        return 1;
    }
    if (g_Ric.padPressed & PAD_LEFT) {
        RIC.facingLeft = 1;
        g_Ric.unk4C = 1;
        return -1;
    }
    return 0;
}
#endif

void RicSetSpeedX(s32 speed) {
    if (g_CurrentEntity->facingLeft == 1)
        speed = -speed;
    g_CurrentEntity->velocityX = speed;
}

// set velocity
void func_us_801B9ACC(s32 speed) {
    if (RIC.entityRoomIndex == 1)
        speed = -speed;
    RIC.velocityX = speed;
}

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicSetInvincibilityFrames);
#else
void RicSetInvincibilityFrames(s32 kind, s16 invincibilityFrames) {
    if (!kind) {
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_CRASH_DAGGER, 0x15), 0);
        if (g_Ric.timers[PL_T_INVINCIBLE_SCENE] <= invincibilityFrames) {
            g_Ric.timers[PL_T_INVINCIBLE_SCENE] = invincibilityFrames;
        }
    } else if (g_Ric.timers[PL_T_INVINCIBLE] <= invincibilityFrames) {
        g_Ric.timers[PL_T_INVINCIBLE] = invincibilityFrames;
    }
}
#endif

// similar to func_8010DFF0 in DRA or func_us_801C5354 in BO4
void DisableAfterImage(s32 resetAnims, s32 time) {
    Primitive* prim;

    if (resetAnims) {
        g_Entities[E_ID_41].ext.disableAfterImage.resetFlag = 1;
        g_Entities[E_ID_41].animCurFrame = g_Entities[E_ID_42].animCurFrame =
            g_Entities[E_ID_43].animCurFrame = 0;
        prim = &g_PrimBuf[g_Entities[E_ID_41].primIndex];
        while (prim != NULL) {
            prim->x1 = 0;
            prim = prim->next;
        }
    }
    g_Entities[E_ID_41].ext.disableAfterImage.disableFlag = 1;
    g_Entities[E_ID_41].ext.disableAfterImage.index = 0xA;
    if (time) {
        g_Ric.timers[ALU_T_15] = 4;
    }
}

void func_us_801B9C14() {
    g_Entities[65].ext.afterImage.timer = 0;
    g_Entities[65].ext.afterImage.index = 0;
    g_Entities[65].ext.afterImage.resetFlag = 0;
    g_Entities[65].ext.afterImage.disableFlag = 0;
}

void RicSetDebug() { RicSetStep(PL_S_DEBUG); }

extern AnimationFrame D_us_80182038[];
extern AnimationFrame D_us_80182048[];
extern AnimationFrame D_us_80182050[];
extern AnimationFrame D_us_80182058[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicSetCrouch);
#else
void RicSetCrouch(s32 kind, s32 velocityX) {
    RicSetStep(PL_S_CROUCH);
    RicSetAnimation(D_us_80182048);
    RIC.velocityX = velocityX;
    RIC.velocityY = 0;
    if (kind == 1) {
        RIC.anim = D_us_80182038;
        RIC.step_s = 4;
    }
    if (kind == 2) {
        RIC.anim = D_us_80182058;
        RIC.step_s = 1;
    }
    if (kind == 3) {
        RIC.anim = D_us_80182050;
        RIC.step_s = 4;
    }
}
#endif

extern AnimationFrame ric_anim_stand[];

void RicSetStand(s32 velocityX) {
    RIC.velocityX = velocityX;
    RIC.velocityY = 0;
    g_Ric.unk44 = 0;
    RicSetStep(PL_S_STAND);
    RicSetAnimation(ric_anim_stand);
}

extern s16 D_us_801821F8[];

void func_us_801B9D74(void) {
    g_Ric.unk44 = 0;
    RicSetStep(0x1A);
    RicSetAnimation(D_us_801821F8);
    RicSetSpeedX(FIX(2.25));
    g_Ric.timers[11] = 0x28;
    RIC.velocityY = 0;
    RicCreateEntFactoryFromEntity(g_CurrentEntity, 0x50001, 0);
}

extern s16 D_us_80182010[];

void func_us_801B9DE4(void) {
    if (g_Ric.timers[8] != 0) {
        func_us_801B9D74();
    } else {
        g_Ric.timers[1] = 8;
        g_Ric.timers[1] = g_Ric.timers[8] = 12;
        g_Ric.unk44 = 0;
        RicSetStep(2);
        RicSetAnimation(D_us_80182010);
        RicSetSpeedX(FIX(1.25));
        RIC.velocityY = 0;
    }
}

extern s16 D_us_80182078[];
extern s16 D_us_80182094[];

void func_us_801B9E70(void) {
    if ((RicCheckFacing() != 0) || (RIC.step == 0x18)) {
        RicSetAnimation(D_us_80182094);
        if (RIC.step == 0x1A) {
            RicSetSpeedX(FIX(2.25));
            g_Ric.unk44 = 0x10;
        } else {
            RicSetSpeedX(0x14000);
            g_Ric.unk44 = 0;
        }
    } else {
        RicSetAnimation(D_us_80182078);
        RIC.velocityX = 0;
        g_Ric.unk44 = 4;
    }
    RicSetStep(5);
    RIC.velocityY = FIX(-4.6875);
}

extern AnimationFrame D_us_801820BC[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicSetFall);
#else
void RicSetFall(void) {
    if (g_Ric.prev_step != PL_S_RUN && g_Ric.prev_step != PL_S_SLIDE) {
        RIC.velocityX = 0;
    }
    if (g_Ric.prev_step != PL_S_WALK && g_Ric.prev_step != PL_S_RUN) {
        RicSetAnimation(D_us_801820BC);
    }
    if (g_Ric.prev_step == PL_S_RUN) {
        g_Ric.unk44 = 0x10;
    }
    RicSetStep(PL_S_FALL);
    RIC.velocityY = FIX(2);
    g_Ric.timers[PL_T_5] = 8;
    g_Ric.timers[PL_T_6] = 8;
    g_Ric.timers[PL_T_CURSE] = 0;
    g_Ric.timers[PL_T_8] = 0;
    if (g_Ric.prev_step == PL_S_SLIDE) {
        g_Ric.timers[PL_T_5] = g_Ric.timers[PL_T_6] = 0;
        RIC.pose = 2;
        RIC.poseTimer = 0x10;
        RIC.velocityX /= 2;
    }
}
#endif

extern s16 D_us_80182324[];

void func_us_801BA050(void) {
    RicSetStep(9);
    RIC.velocityX = 0;
    RicSetSpeedX(FIX(1.25));
    RIC.velocityY = FIX(-7.5);
    g_Ric.high_jump_timer = 0;
    RicSetAnimation(D_us_80182324);
    func_us_801B9C14();
    RicCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(0x2D, 0), 0);
    g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_B);
    g_Ric.timers[12] = 4;
}

static s32 RicCheckSubwpnChainLimit(s16 subwpnId, s16 limit) {
    Entity* entity;
    s32 i;
    u32 nEmpty;
    s32 nFound;

    entity = &g_Entities[0x60];
    for (i = 0, nFound = 0, nEmpty = 0; i < 32; i++, entity++) {
        if (!entity->entityId) {
            nEmpty++;
        }

        if (entity->ext.subweapon.subweaponId != 0 &&
            entity->ext.subweapon.subweaponId == subwpnId) {
            nFound++;
        }

        if (nFound >= limit) {
            return -1;
        }
    }

    if (nEmpty < 1) {
        return -1;
    } else {
        return 0;
    }
}

extern u16 D_us_80182170[][2];
extern u16 D_us_801821C0[][2];

s32 RicDoSubweapon(void) {
    SubweaponDef subweapon;
    s16 subweaponId;
    s16 chainLimit;

    if (!(g_Ric.padPressed & PAD_UP)) {
        return 1;
    }

    subweaponId = RicCheckSubweapon(&subweapon, 0, 0);
    chainLimit = subweapon.chainLimit;
    if (RicCheckSubwpnChainLimit(subweaponId, chainLimit) < 0) {
        return 2;
    }

    RicCreateEntFactoryFromEntity(g_CurrentEntity, subweapon.blueprintNum, 0);
    g_Ric.timers[PL_T_10] = 4;
    switch (RIC.step) {
    case PL_S_RUN:
        RIC.step = PL_S_STAND;
        RicCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
        RicSetAnimation(D_us_80182170);
        break;
    case PL_S_STAND:
    case PL_S_WALK:
    case PL_S_CROUCH:
        RIC.step = PL_S_STAND;
        RicSetAnimation(D_us_80182170);
        break;
    case PL_S_FALL:
    case PL_S_JUMP:
        RIC.step = PL_S_JUMP;
        RicSetAnimation(D_us_801821C0);
        break;
    }
    g_Ric.unk46 = 3;
    RIC.step_s = 0x42;
    // n.b.! this was just set before the switch
    g_Ric.timers[PL_T_10] = 4;
    return 0;
}

extern AnimationFrame D_us_80182110[];
extern AnimationFrame D_us_80182130[];
extern AnimationFrame D_us_80182150[];

#ifdef VERSION_PC
static void RicPlayAttackGrunt(s16 sfxGrunt) {
    if (sfxGrunt == 0) {
        g_api.PlaySfx(0x82C);
    }
    if (sfxGrunt == 1) {
        g_api.PlaySfx(0x82D);
    }
    if (sfxGrunt == 2) {
        g_api.PlaySfx(0x82E);
    }
    if (sfxGrunt == 3) {
        g_api.PlaySfx(0x82F);
    }
}
#endif

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicDoAttack);
#else
s32 RicDoAttack(void) {
    s32 i;
    s32 poisoned;
    s16 sfxGrunt;

    sfxGrunt = rand() % 6;
    if (RicDoSubweapon() == 0) {
        RicPlayAttackGrunt(sfxGrunt);
        return 1;
    }
    poisoned = g_Ric.timers[PL_T_POISON] != 0;
    for (i = 80; i < 95; i++) {
        DestroyEntity(&g_Entities[i]);
    }
    if (RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_WHIP, poisoned), 0) == NULL) {
        return 0;
    }
    if (poisoned) {
        g_api.PlaySfx(0x6B5);
    } else {
        g_api.PlaySfx(0x825);
    }
    RicPlayAttackGrunt(sfxGrunt);
    switch (RIC.step) {
    case PL_S_STAND:
    case PL_S_WALK:
        RIC.step = PL_S_STAND;
        RicSetAnimation(D_us_80182110);
        g_CurrentEntity->velocityX = 0;
        break;
    case PL_S_RUN:
        RIC.step = PL_S_STAND;
        RicSetAnimation(D_us_80182110);
        RicCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
        break;
    case PL_S_CROUCH:
        RicSetAnimation(D_us_80182130);
        g_CurrentEntity->velocityX = 0;
        break;
    case PL_S_FALL:
    case PL_S_JUMP:
        RIC.step = PL_S_JUMP;
        RicSetAnimation(D_us_80182150);
        break;
    default:
        return 0;
    }
    g_Ric.unk46 = 1;
    RIC.step_s = 0x40;
    g_Ric.timers[PL_T_ATTACK] = 4;
    return 1;
}
#endif

extern AnimationFrame D_us_801823C8[];
extern AnimationFrame D_us_80182190[];
extern AnimationFrame D_us_801821E0[];
extern AnimationFrame D_us_80182334[];
extern AnimationFrame D_us_8018246C[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicDoCrash);
#else
s32 RicDoCrash(void) {
    SubweaponDef subWpn;
    Entity* subWpnEnt;
    s16 subWpnID;

    subWpnEnt = NULL;
    subWpnID = RicCheckSubweapon(&subWpn, 1, 0);
    if (subWpnID == PL_W_HOLYWATER && g_Ric.timers[PL_T_3]) {
        return 0;
    }
    if (subWpn.blueprintNum) {
        if (subWpnID == 1) {
            subWpnEnt = RicCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(subWpn.blueprintNum, 1), 0);
        } else {
            subWpnEnt = RicCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(subWpn.blueprintNum, 0), 0);
        }
    }
    if (subWpnEnt == NULL) {
        return 0;
    }
    g_Ric.unk46 = 4;
    g_Ric.unk4E = 0;
    RIC.velocityY = 0;
    RIC.velocityX = 0;
    switch (subWpnID) {
    case PL_W_NONE:
        RicSetStep(PL_S_FLAME_WHIP);
        RicSetAnimation(D_us_801823C8);
        RicCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(BP_36, 1), 0);
        break;
    case PL_W_DAGGER:
        RicSetStep(PL_S_THROW_DAGGERS);
        RicSetAnimation(D_us_80182190);
        g_api.PlaySfx(0x827);
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 2), 0);
        break;
    case PL_W_AXE:
        RicSetStep(PL_S_STAND_IN_AIR);
        RicSetAnimation(D_us_801821E0);
        RIC.velocityY = FIX(-4.6875);
        func_us_801B9C14();
        g_api.PlaySfx(0x827);
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 2), 0);
        break;
    case PL_W_HOLYWATER:
        RicSetStep(PL_S_HYDROSTORM);
        RicSetAnimation(D_us_80182334);
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x40), 0);
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x47), 0);
        g_api.PlaySfx(0x828);
        break;
    case PL_W_REBNDSTONE:
    case PL_W_VIBHUTI:
    case PL_W_AGUNEA:
        RicSetStep(PL_S_SUBWPN_CRASH);
        RicSetAnimation(D_us_80182334);
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x40), 0);
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x47), 0);
        g_api.PlaySfx(0x827);
        break;
    case PL_W_BIBLE:
    case PL_W_STOPWATCH:
        RicSetStep(PL_S_SUBWPN_CRASH);
        RicSetAnimation(D_us_8018246C);
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x40), 0);
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x47), 0);
        g_api.PlaySfx(0x827);
        break;
    case PL_W_CROSS:
        RicSetStep(PL_S_STAND_IN_AIR);
        RicSetAnimation(D_us_801821E0);
        RIC.velocityY = FIX(-4.6875);
        func_us_801B9C14();
        g_api.PlaySfx(0x829);
        RicCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 2), 0);
        break;
    }
    g_Ric.timers[PL_T_12] = 4;
    return 1;
}
#endif

void RicSetDeadPrologue() { RicSetStep(PL_S_DEAD_PROLOGUE); }

extern AnimationFrame D_us_801822D8[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicSetSlide);
#else
void RicSetSlide(void) {
    RicCheckFacing();
    RicSetStep(PL_S_SLIDE);
    RicSetAnimation(D_us_801822D8);
    g_CurrentEntity->velocityY = 0;
    RicSetSpeedX(FIX(5.5));
    func_us_801B9C14();
    RicCreateEntFactoryFromEntity(g_CurrentEntity, BP_25, 0);
    g_api.PlaySfx(0x826);
    g_Ric.timers[PL_T_12] = 4;
}
#endif

extern AnimationFrame D_us_80182304[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicSetSlideKick);
#else
void RicSetSlideKick(void) {
    g_Ric.unk44 = 0;
    RicSetStep(PL_S_SLIDE_KICK);
    RicSetAnimation(D_us_80182304);
    g_CurrentEntity->velocityY = FIX(-2);
    RicSetSpeedX(FIX(5.5));
    func_us_801B9C14();
    RicCreateEntFactoryFromEntity(g_CurrentEntity, BP_25, 0);
    g_api.PlaySfx(0x82C);
    g_Ric.timers[PL_T_12] = 4;
    RicCreateEntFactoryFromEntity(g_CurrentEntity, BP_31, 0);
}
#endif

extern AnimationFrame D_us_80182360[];

// Equivalent of RicSetBladeDash
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BA9D0);
#else
void func_us_801BA9D0(void) {
    RicSetStep(PL_S_BLADEDASH);
    RicSetAnimation(D_us_80182360);
    g_CurrentEntity->velocityY = 0;
    RicSetSpeedX(FIX(5.5));
    g_Ric.unk46 = 5;
    g_Ric.timers[PL_T_12] = 4;
    RicCreateEntFactoryFromEntity(g_CurrentEntity, BP_BLADE_DASH, 0);
    func_us_801B9C14();
    g_api.PlaySfx(0x834);
    g_api.PlaySfx(0x707);
}
#endif

extern AnimationFrame D_us_80182110[];
extern AnimationFrame D_us_80182130[];
extern AnimationFrame D_us_80182150[];
extern AnimationFrame D_us_801822B8[];
extern AnimationFrame D_us_801822C0[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicCheckInput);
#else
s32 RicCheckInput(s32 checks) {
    s32 velYChange;

    if (checks & CHECK_FACING) {
        if (g_Ric.unk46 == 0) {
            RicCheckFacing();
        }
    }
    velYChange = 0;
    if (checks & CHECK_GRAVITY_FALL) {
        velYChange = FIX(28.0 / 128);
    }
    if (checks & CHECK_GRAVITY_JUMP) {
        if (RIC.velocityY < FIX(3.0 / 8) && RIC.velocityY > FIX(-1.0 / 8) &&
            !(g_Ric.unk44 & 0x20) && (g_Ric.padPressed & PAD_CROSS)) {
            velYChange = FIX(5.6 / 128);
        } else {
            velYChange = FIX(28.0 / 128);
        }
    }
    if (checks & CHECK_GRAVITY_HIT) {
        if (RIC.velocityY < FIX(3.0 / 8) && RIC.velocityY > FIX(-1.0 / 8)) {
            velYChange = FIX(14.0 / 128);
        } else {
            velYChange = FIX(28.0 / 128);
        }
    }
    RIC.velocityY += velYChange;
    if (RIC.velocityY > FIX(7)) {
        RIC.velocityY = FIX(7);
    }
    if ((checks & CHECK_80) && (g_Ric.vram_flag & TOUCHING_CEILING) &&
        (RIC.velocityY < FIX(-1))) {
        RIC.velocityY = FIX(-1);
    }
    if (RIC.velocityY >= 0) {
        if ((checks & CHECK_GROUND) && (g_Ric.vram_flag & TOUCHING_GROUND)) {
            switch (g_Ric.unk46) {
            case 0:
            default:
                if (g_Ric.unk44 & 8) {
                    RicSetCrouch(3, RIC.velocityX);
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    RicCreateEntFactoryFromEntity(
                        g_CurrentEntity, BP_SKID_SMOKE, 0);
                    return 1;
                }
                if (RIC.velocityY > FIX(6.875)) {
                    RicSetCrouch(1, 0);
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    RicCreateEntFactoryFromEntity(
                        g_CurrentEntity, BP_SKID_SMOKE, 0);
                    return 1;
                }
                if (g_Ric.padPressed & (PAD_LEFT | PAD_RIGHT)) {
                    if (g_Ric.unk44 & 0x10) {
                        func_us_801B9D74();
                    } else {
                        func_us_801B9DE4();
                    }
                    return 1;
                }
                RicSetStand(0);
                return 1;
            case 1:
                if (RIC.velocityY > FIX(6.875)) {
                    RIC.step = PL_S_CROUCH;
                    RIC.anim = D_us_80182130;
                    g_api.PlaySfx(SFX_STOMP_HARD_B);
                    RicCreateEntFactoryFromEntity(
                        g_CurrentEntity, BP_SKID_SMOKE, 0);
                } else {
                    RIC.step = PL_S_STAND;
                    RIC.anim = D_us_80182110;
                    if (!(g_Ric.unk44 & 8)) {
                        RIC.velocityX = 0;
                    } else {
                        RicCreateEntFactoryFromEntity(
                            g_CurrentEntity, BP_SKID_SMOKE, 0);
                        g_api.PlaySfx(SFX_STOMP_HARD_B);
                    }
                }
                RIC.velocityY = 0;
                g_Ric.unk44 = 0;
                return 1;
            case 2:
                if (RIC.velocityY > FIX(6.875)) {
                    RIC.step = PL_S_CROUCH;
                    RIC.anim = D_us_801822C0;
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    RicCreateEntFactoryFromEntity(
                        g_CurrentEntity, BP_SKID_SMOKE, 0);
                } else {
                    RIC.step = PL_S_STAND;
                    RIC.anim = D_us_801822B8;
                    if (!(g_Ric.unk44 & 8)) {
                        RIC.velocityX = 0;
                    } else {
                        RicCreateEntFactoryFromEntity(
                            g_CurrentEntity, BP_SKID_SMOKE, 0);
                        g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    }
                }
                RIC.velocityY = 0;
                g_Ric.unk44 = 0;
                return 1;
            case 3:
                if ((RIC.velocityY > FIX(6.875)) || (g_Ric.unk44 & 8)) {
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    RicCreateEntFactoryFromEntity(
                        g_CurrentEntity, BP_SKID_SMOKE, 0);
                }
                RIC.velocityY = 0;
                if (!(g_Ric.unk44 & 8)) {
                    RIC.velocityX = 0;
                }
                RIC.step = PL_S_STAND;
                RIC.anim = D_us_80182170;
                g_Ric.unk44 = 0;
                return 1;
            }
        } else if ((checks & CHECK_GROUND_AFTER_HIT) &&
                   (g_Ric.vram_flag & TOUCHING_GROUND)) {
            RicSetCrouch(1, RIC.velocityX);
            g_api.PlaySfx(SFX_STOMP_SOFT_A);
            if (g_Ric.unk5C && (g_Status.hp < 2)) {
                RicSetDeadPrologue();
                return 1;
            }
            RicCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
            return 1;
        }
    }
    if ((checks & CHECK_FALL) && !(g_Ric.vram_flag & TOUCHING_GROUND)) {
        switch (g_Ric.unk46) {
        case 0:
            RicSetFall();
            return 1;
        case 1:
            RIC.step_s = 0x40;
            RIC.step = PL_S_JUMP;
            RIC.anim = D_us_80182150;
            return 1;
        case 2:
            RIC.step_s = 0x41;
            RIC.step = PL_S_JUMP;
            RIC.anim = D_us_80182150;
            return 1;
        case 3:
            RIC.step_s = 0x42;
            RIC.step = PL_S_JUMP;
            RIC.anim = D_us_801821C0;
            return 1;
        }
        return 0;
    }
    if (g_Ric.unk46) {
        return 0;
    }
    if ((checks & CHECK_CRASH) && (g_Ric.padTapped & PAD_TRIANGLE) &&
        RicDoCrash()) {
        return 1;
    }
    // n.b.! the floor sensors are Alucard's, not Richter's
    if ((checks & CHECK_SLIDE) &&
        !(RIC.facingLeft == 0 &&
          g_Player.colFloor[2].effects & EFFECT_UNK_8000) &&
        !(RIC.facingLeft && g_Player.colFloor[3].effects & EFFECT_UNK_8000) &&
        (RIC.posX.i.hi < (u8)-4 || RIC.facingLeft != 0) &&
        (RIC.posX.i.hi >= 5 || !RIC.facingLeft) &&
        (g_Ric.padPressed & PAD_DOWN) && (g_Ric.padTapped & PAD_CROSS)) {
        RicSetSlide();
        return 1;
    }
    if ((checks & CHECK_JUMP) && (g_Ric.padTapped & PAD_CROSS)) {
        func_us_801B9E70();
        return 1;
    }
    if ((checks & CHECK_ATTACK) && (g_Ric.padTapped & PAD_SQUARE) &&
        RicDoAttack()) {
        return 1;
    }
    if ((checks & CHECK_CROUCH) && (g_Ric.padPressed & PAD_DOWN)) {
        RicSetCrouch(2, 0);
        return 1;
    }
    return 0;
}
#endif

Entity* RicGetFreeEntity(s16 start, s16 end) {
    Entity* entity = &g_Entities[start];
    s16 i;

    for (i = start; i < end; i++, entity++) {
        if (entity->entityId == E_NONE) {
            return entity;
        }
    }
    return NULL;
}

// pl_blueprints?

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicGetFreeEntityReverse);
#else
Entity* RicGetFreeEntityReverse(s16 start, s16 end) {
    Entity* entity = &g_Entities[end - 1];
    s16 i;

    for (i = end - 1; i >= start; i--, entity--) {
        if (entity->entityId == E_NONE) {
            return entity;
        }
    }
    return NULL;
}
#endif

extern AnimationFrame* D_us_801812B8[];
extern u8 D_us_801D07FC;
extern u8 D_us_801D0800;
extern u8 D_us_801D0804;
extern u8 D_us_801D0808;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BB314);
#else
void func_us_801BB314(s32 arg0) {
    D_us_801D07FC = ((u8*)D_us_801812B8)[arg0 * 4 + 0];
    D_us_801D0800 = ((u8*)D_us_801812B8)[arg0 * 4 + 1];
    D_us_801D0804 = ((u8*)D_us_801812B8)[arg0 * 4 + 2];
    D_us_801D0808 = ((u8*)D_us_801812B8)[arg0 * 4 + 3];
}
#endif

// Equivalent of RicSetSubweaponParams
extern SubweaponDef subweapons_def[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BB370);
#else
void func_us_801BB370(Entity* entity) {
    SubweaponDef* subwpn = &subweapons_def[entity->ext.subweapon.subweaponId];

    if (g_Ric.timers[PL_T_INVINCIBLE_SCENE]) {
        entity->attack = subwpn->attack * 2;
    } else {
        entity->attack = subwpn->attack;
    }
    entity->attackElement = subwpn->attackElement;
    entity->hitboxState = subwpn->hitboxState;
    entity->nFramesInvincibility = subwpn->nFramesInvincibility;
    entity->stunFrames = subwpn->stunFrames;
    entity->hitEffect = subwpn->hitEffect;
    entity->entityRoomIndex = subwpn->entityRoomIndex;
}
#endif

extern SubweaponDef subweapons_def[];

s32 RicCheckSubweapon(
    SubweaponDef* actualSubwpn, s32 isItemCrash, s32 useHearts) {
    SubweaponDef* subwpn;
    SubweaponDef* wpn;
    s32 i;
    s32 subweaponId;
    s32 distanceX;

    distanceX = abs(RIC.posX.i.hi - PLAYER.posX.i.hi);

    if (isItemCrash == 0) {
        if (distanceX < 0x50) {
            subweaponId = 3;
        } else {
            subweaponId = 4;
        }
        if ((RIC.posY.i.hi - PLAYER.posY.i.hi) > 0x18) {
            subweaponId = 2;
        }
        if ((RIC.posY.i.hi - PLAYER.posY.i.hi) < -0x18) {
            subweaponId = 3;
        }
        if (g_Player.status & PLAYER_STATUS_BAT_FORM) {
            subweaponId = 2;
        }

        *actualSubwpn = subweapons_def[subweaponId];
    } else {
        subweaponId = 4;
        if (g_Ric.padPressed & 4) {
            subweaponId = 2;
        } else {
            if (g_Ric.padPressed & 1) {
                subweaponId = 3;
            }
            if (g_Ric.padPressed & 0x800) {
                subweaponId = 9;
            }
            if (g_Ric.padPressed & 0x100) {
                subweaponId = 5;
            }
        }

        subwpn = &subweapons_def[subweaponId];
        *actualSubwpn = subweapons_def[subwpn->crashId];
    }
    return subweaponId;
}

extern u8 D_us_80181524[][8];

// Equivalent of func_8015FDB0 in RIC
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BB5BC);
#else
s32 func_us_801BB5BC(Primitive* prim, s16 posX, s16 posY) {
    s16 offset;
    s32 ret;
    u8* uvAnim;

    ret = 0;
    if (prim->b0 >= 6) {
        prim->b0 = 0;
        ret = -1;
    }
    uvAnim = D_us_80181524[prim->b0];
    if (prim->b0 >= 3) {
        offset = 4;
    } else {
        offset = 6;
    }
    prim->x0 = posX - offset;
    prim->y0 = posY - offset;
    prim->x1 = posX + offset;
    prim->y1 = posY - offset;
    prim->x2 = posX - offset;
    prim->y2 = posY + offset;
    prim->x3 = posX + offset;
    prim->y3 = posY + offset;
    prim->u0 = *uvAnim++;
    prim->v0 = *uvAnim++;
    prim->u1 = *uvAnim++;
    prim->v1 = *uvAnim++;
    prim->u2 = *uvAnim++;
    prim->v2 = *uvAnim++;
    prim->u3 = *uvAnim++;
    prim->v3 = *uvAnim;
    prim->b1++;
    if (!(prim->b1 & 1)) {
        prim->b0++;
    }
    return ret;
}
#endif

extern Point16 D_us_801D080C[16];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicEntityHitByHoly);
#else
void RicEntityHitByHoly(Entity* entity) {
    Primitive* prim;
    s32 i;
    s32 temp;
    s16 hitboxX;
    s16 hitboxY;

    switch (entity->step) {
    case 0:
        entity->primIndex = (s16)g_api.AllocPrimitives(PRIM_GT4, 16);
        if (entity->primIndex == -1) {
            DestroyEntity(entity);
            return;
        }
        entity->flags = FLAG_HAS_PRIMS | FLAG_POS_CAMERA_LOCKED;
        hitboxX = RIC.posX.i.hi + RIC.hitboxOffX;
        hitboxY = RIC.posY.i.hi + RIC.hitboxOffY;
        prim = &g_PrimBuf[entity->primIndex];
        for (i = 0; i < 16; i++) {
            D_us_801D080C[i].x = hitboxX + rand() % 24 - 12;
            D_us_801D080C[i].y = hitboxY + rand() % 48 - 24;
            prim->clut = PAL_UNK_1B2;
            prim->tpage = 0x1A;
            prim->b0 = 0;
            prim->b1 = 0;
            prim->g0 = 0;
            prim->g1 = (rand() & 7) + 1;
            prim->g2 = 0;
            prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE | DRAW_HIDE |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim->priority = RIC.zPriority + 4;
            if (rand() & 1) {
                prim->drawMode =
                    DRAW_UNK_100 | DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                    DRAW_HIDE | DRAW_UNK02 | DRAW_TRANSP;
            }
            prim = prim->next;
        }
        entity->step++;
        break;
    case 1:
        if (!(g_Ric.status & PLAYER_STATUS_UNK10000)) {
            DestroyEntity(entity);
            return;
        }
        break;
    }

    prim = &g_PrimBuf[entity->primIndex];
    for (i = 0; i < 16; i++) {
        switch (prim->g0) {
        case 0:
            if (--prim->g1 == 0) {
                prim->g0++;
            }
            break;
        case 1:
            temp =
                func_us_801BB5BC(prim, D_us_801D080C[i].x, D_us_801D080C[i].y);
            D_us_801D080C[i].y--;
            if (temp < 0) {
                prim->drawMode |= DRAW_HIDE;
                prim->g0++;
            } else {
                prim->drawMode &= ~DRAW_HIDE;
            }
            break;
        }
        prim = prim->next;
    }
}
#endif

extern AnimationFrame D_us_80181554[];
extern s32 D_us_801D084C;

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicEntityHitByDark);
#else
void RicEntityHitByDark(Entity* entity) {
    switch (entity->step) {
    case 0:
        entity->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        entity->unk5A = 0x79;
        entity->animSet = ANIMSET_DRA(14);
        entity->palette = PAL_FLAG(PAL_UNK_19F);
        entity->zPriority = RIC.zPriority + 2;
        if (D_us_801D084C & 1) {
            entity->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        } else {
            entity->blendMode = BLEND_TRANSP;
        }
        entity->scaleX = entity->scaleY = 0x40;
        entity->anim = D_us_80181554;
        D_us_801D084C++;
        entity->opacity = 0xFF;
        entity->drawFlags =
            ENTITY_SCALEX | ENTITY_SCALEY | ENTITY_MASK_R | ENTITY_MASK_G;
        entity->posY.i.hi += (rand() % 35) - 15;
        entity->posX.i.hi += (rand() % 20) - 10;
        entity->velocityY = -0x6000 - (rand() & 0x3FFF);
        entity->step++;
        break;
    case 1:
        if (entity->opacity > 16) {
            entity->opacity -= 8;
        }
        entity->posY.val += entity->velocityY;
        entity->scaleX += 8;
        entity->scaleY += 8;
        if (entity->poseTimer < 0) {
            DestroyEntity(entity);
        }
        break;
    }
}
#endif

void func_us_801BBBC0(void) {}

void func_us_801BBBC8(void) {}

extern AnimationFrame* D_us_801812B8[];
extern PfnEntityUpdate D_us_8018158C[];
extern u8 D_us_801D0800;
extern u8 D_us_801D0804;
extern u8 D_us_801D0808;
extern u8 D_us_801D07FC;

void func_us_801BBBD0(void) {
    Entity* entity;
    PfnEntityUpdate entityUpdate;
    s32 i;

    entity = g_CurrentEntity = &g_Entities[E_ID_44];

    for (i = E_ID_44; i < E_ID_90; i++, g_CurrentEntity++, entity++) {
        if (entity->entityId) {
            entityUpdate = D_us_8018158C[entity->entityId];
            entityUpdate(entity);
            entity = g_CurrentEntity;

            if (entity->entityId) {
                if (!(entity->flags & FLAG_UNK_10000000) &&
                    (entity->posX.i.hi > 288 || entity->posX.i.hi < -32 ||
                     entity->posY.i.hi > 256 || entity->posY.i.hi < -16)) {
                    DestroyEntity(entity);
                } else {
                    if (entity->flags & FLAG_UNK_20000000) {
                        g_api.UpdateAnim(NULL, D_us_801812B8);
                    }
                    entity->flags |= FLAG_NOT_AN_ENEMY;
                }
            }
        }
    }

    if (D_us_801D07FC) {
        D_us_801D07FC--;
        if (D_us_801D07FC & 1) {
            g_api.func_800EA5AC(1, D_us_801D0800, D_us_801D0804, D_us_801D0808);
        }
    }
    if ((RIC.step == 0x11) || (RIC.step == 0x60) || (RIC.step == 0x70)) {
        FntPrint("dead boss\n");
        entity = &g_Entities[E_ID_44];
        for (i = E_ID_44; i < E_ID_90; i++, entity++) {
            entity->hitboxState = 0;
        };
    }
}

Entity* RicCreateEntFactoryFromEntity(
    Entity* source, u32 factoryParams, s32 arg2) {
    Entity* entity = RicGetFreeEntity(68, 80);
    if (!entity) {
        return NULL;
    }
    DestroyEntity(entity);
    entity->entityId = E_FACTORY;
    // the parent pointer must align for anything the factory creates
    entity->ext.factory.parent = source;
    entity->posX.val = source->posX.val;
    entity->posY.val = source->posY.val;
    entity->facingLeft = source->facingLeft;
    entity->zPriority = source->zPriority;
    entity->params = factoryParams & 0xFFF;
    entity->ext.factory.paramsBase = (factoryParams & 0xFF0000) >> 8;
    return entity;
}

extern FactoryBlueprint D_us_801816A0[];
extern u8 D_us_80181868[][2];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicEntityFactory);
#else
void RicEntityFactory(Entity* self) {
    Entity* newEntity;
    s16 nPerCycle;
    s16 i;
    s16 startIndex;
    s16 endIndex;
    u8* data;

    if (self->step == 0) {
        data = (u8*)&D_us_801816A0[self->params];
        self->ext.factory.newEntityId = *data++;
        self->ext.factory.amount = *data++;
        self->ext.factory.nPerCycle = *data & 0x3F;
        self->ext.factory.isNonCritical = *data >> 7;
        self->ext.factory.incParamsKind = (*data++ >> 6) & 1;
        self->ext.factory.tCycle = *data++;
        self->ext.factory.kind = *data & 0x7;
        self->ext.factory.origin = *data++ >> 3;
        self->ext.factory.delay = *data;
        self->step++;
        self->flags |= FLAG_UNK_10000000;
        switch (self->ext.factory.origin) {
        case 4:
        case 2:
        case 9:
            self->flags |= FLAG_POS_CAMERA_LOCKED;
        case 3:
        case 7:
            self->posX.val = RIC.posX.val;
            self->posY.val = RIC.posY.val;
            break;
        case 8:
            if (self->ext.factory.parent != NULL) {
                self->posX.val = self->ext.factory.parent->posX.val;
                self->posY.val = self->ext.factory.parent->posY.val;
            }
        case 0:
            self->flags |= FLAG_POS_CAMERA_LOCKED;
            break;
        }
    } else {
        switch (self->ext.factory.origin) {
        case 9:
            if (g_Ric.unk4E) {
                DestroyEntity(self);
                return;
            }
        case 2:
            self->posX.val = RIC.posX.val;
            self->posY.val = RIC.posY.val;
            break;
        case 4:
            self->posX.val = RIC.posX.val;
            self->posY.val = RIC.posY.val;
            if (RIC.step != PL_S_RUN) {
                self->entityId = 0;
                return;
            }
            break;
        case 3:
            self->posX.val = RIC.posX.val;
            self->posY.val = RIC.posY.val;
            if (RIC.step == PL_S_HIT) {
                self->entityId = 0;
                return;
            }
            break;
        case 7:
            self->posX.val = RIC.posX.val;
            self->posY.val = RIC.posY.val;
            if (RIC.step != PL_S_HIT) {
                self->entityId = 0;
                return;
            }
            break;
        case 8:
            if (self->ext.factory.parent != NULL) {
                self->posX.val = self->ext.factory.parent->posX.val;
                self->posY.val = self->ext.factory.parent->posY.val;
            }
            break;
        }
    }
    if (self->ext.factory.delay) {
        if (--self->ext.factory.delay) {
            return;
        }
        self->ext.factory.delay = self->ext.factory.tCycle;
    }
    nPerCycle = self->ext.factory.nPerCycle;
    for (i = 0; i < nPerCycle; i++) {
        startIndex = D_us_80181868[self->ext.factory.kind][0];
        endIndex = D_us_80181868[self->ext.factory.kind][1];
        switch (self->ext.factory.kind) {
        case 0:
            newEntity = RicGetFreeEntityReverse(startIndex, endIndex + 1);
            break;
        case 4:
            newEntity = &g_Entities[95];
            break;
        case 5:
            newEntity = &g_Entities[112];
            break;
        default:
            newEntity = RicGetFreeEntity(startIndex, endIndex + 1);
            break;
        }
        if (newEntity == NULL) {
            if (self->ext.factory.isNonCritical == 1) {
                self->entityId = 0;
            } else {
                self->ext.factory.delay = self->ext.factory.tCycle;
            }
            return;
        }
        DestroyEntity(newEntity);
        newEntity->entityId =
            self->ext.factory.newEntityId + self->ext.factory.entityIdMod;
        newEntity->params = self->ext.factory.paramsBase;
        newEntity->ext.factory.parent = self->ext.factory.parent;
        newEntity->posX.val = self->posX.val;
        newEntity->posY.val = self->posY.val;
        newEntity->facingLeft = self->facingLeft;
        newEntity->zPriority = self->zPriority;
        if (self->ext.factory.incParamsKind) {
            newEntity->params += self->ext.factory.spawnIndex;
        } else {
            newEntity->params += i;
        }
        self->ext.factory.spawnIndex++;
        if (self->ext.factory.spawnIndex == self->ext.factory.amount) {
            self->entityId = 0;
            return;
        }
    }
    self->ext.factory.delay = self->ext.factory.tCycle;
}
#endif

extern EInit D_us_8018043C;

// Equivalent of RicEntitySlideKick
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BC2F0);
#else
void func_us_801BC2F0(Entity* self) {
    if (RIC.step != PL_S_SLIDE) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = RIC.posX.i.hi;
    self->posY.i.hi = RIC.posY.i.hi;
    self->facingLeft = RIC.facingLeft;
    if (self->step == 0) {
        InitializeEntity(D_us_8018043C);
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->hitboxOffX = 0x14;
        self->hitboxOffY = 0xC;
        self->hitboxWidth = self->hitboxHeight = 9;
        self->step = 1;
        self->ext.subweapon.timer = self->hitboxState;
    }
    self->hitboxState = self->ext.subweapon.timer;
    if (RIC.pose < 2) {
        self->hitboxState = 0;
    }
    if (RIC.pose > 7) {
        DestroyEntity(self);
    }
}
#endif

extern EInit D_us_80180448;

void func_us_801BC3E0(Entity* self) {
    if (RIC.step != PL_S_SLIDE_KICK) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = RIC.posX.i.hi;
    self->posY.i.hi = RIC.posY.i.hi;
    self->facingLeft = RIC.facingLeft;
    if (self->step == 0) {
        InitializeEntity(D_us_80180448);
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->hitboxOffX = 0x14;
        self->hitboxWidth = self->hitboxHeight = 9;
        self->step = 1;
    }

    if (RIC.animCurFrame == 140) {
        self->hitboxOffY = 0;
    }

    if (RIC.animCurFrame == 141) {
        self->hitboxOffY = 12;
    }

    if (self->hitFlags) {
        g_Ric.unk44 |= 0x80;
    } else {
        g_Ric.unk44 &= ~0x80;
    }
    self->hitFlags = 0;
}

extern EInit D_us_80180424;

// Equivalent of RicEntityBladeDash
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BC4F8);
#else
void func_us_801BC4F8(Entity* self) {
    if (RIC.step != PL_S_BLADEDASH) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = RIC.posX.i.hi;
    self->posY.i.hi = RIC.posY.i.hi;
    self->facingLeft = RIC.facingLeft;
    if (self->step == 0) {
        InitializeEntity(D_us_80180424);
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->hitboxWidth = self->hitboxHeight = 20;
        self->hitboxWidth = self->hitboxHeight = 12;
        self->hitboxOffX = self->hitboxOffY = 0;
        self->ext.subweapon.subweaponId = PL_W_BIBLE_BEAM;
        self->step++;
    }
    if (RIC.pose > 18) {
        DestroyEntity(self);
    }
}
#endif

extern EInit D_us_80180430;

// Equivalent of func_80160F0C (high jump hitbox)
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BC5C8);
#else
void func_us_801BC5C8(Entity* self) {
    if (RIC.step != PL_S_HIGHJUMP) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = RIC.posX.i.hi;
    self->posY.i.hi = RIC.posY.i.hi;
    self->facingLeft = RIC.facingLeft;
    if (self->step == 0) {
        InitializeEntity(D_us_80180430);
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->hitboxOffX = 0xC;
        self->hitboxOffY = -0x1A;
        self->hitboxWidth = 0xC;
        self->hitboxHeight = 0xC;
        self->step = 1;
    }
}
#endif

extern AnimationFrame D_us_801818A8[];
extern s16 D_us_80181878[];
extern s32 D_us_80181884[];
extern s16 D_us_8018189C[];

// Equivalent of RicEntitySmokePuff
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BC678);
#else
void func_us_801BC678(Entity* self) {
    s16 posX;
    s16 paramsHi = self->params >> 8;
    s16 paramsLo = self->params & 0xFF;

    switch (self->step) {
    case 0:
        self->animSet = ANIMSET_DRA(5);
        self->anim = D_us_801818A8;
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY | ENTITY_OPACITY;
        self->opacity = 0x60;
        self->zPriority = RIC.zPriority + 2;
        posX = D_us_80181878[paramsLo];
        if (paramsHi == 0) {
            posX += 6;
        }
        if (paramsHi == 1) {
            posX -= 8;
        }
        if (paramsHi == 2) {
            posX -= 6;
        }
        if (paramsHi == 5) {
            posX = -6;
        }
        if (paramsHi == 3) {
            self->posY.i.hi -= 8;
        }
        if (paramsHi == 4) {
            DestroyEntity(self);
            return;
        }
        if (paramsHi == 1) {
            if (g_Ric.vram_flag & TOUCHING_ANY_SLOPE) {
                posX /= 2;
            }
        }
        if (self->facingLeft) {
            posX = -posX;
        }
        self->posY.i.hi += 0x18;
        self->posX.i.hi += posX;
        self->scaleX = D_us_8018189C[paramsLo] + 0x40;
        self->velocityY = D_us_80181884[paramsLo];
        if (paramsHi == 1) {
            self->velocityY = FIX(-0.25);
            RicSetSpeedX(-0x3000);
            self->scaleX = D_us_8018189C[1] + 0x40;
        }
        if (paramsHi == 5) {
            self->velocityY = D_us_80181884[4 - paramsLo * 2];
        }
        if (paramsHi == 2) {
            self->velocityY = FIX(-0.5);
            RicSetSpeedX(-0x3000);
            self->scaleX = D_us_8018189C[1] + 0x40;
        }
        self->scaleY = self->scaleX;
        self->step++;
        break;
    case 1:
        self->opacity -= 2;
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}
#endif

extern unkStr_8011E4BC* D_us_801819AC[];
extern s32 D_us_801D11C8[];
extern s32 D_us_801D1248[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", RicEntityHitByCutBlood);
#else
void RicEntityHitByCutBlood(Entity* self) {
    u8 thickness;
    s16 variant;
    s32 timer;
    s16 x;
    s16 y;
    s16 rnd;
    s32 i;
    unkStr_8011E4BC* props;
    FakePrim* tilePrim;

    variant = (self->params) >> 8;
    props = D_us_801819AC[variant];
    x = self->posX.i.hi;
    y = self->posY.i.hi;
    timer = 0;
    thickness = 0;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, props->count + 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = props->flags;
        switch (props->unkA) {
        case 8:
            self->ext.et_8011E4BC.unk7C = 0x100;
            break;
        case 7:
            self->ext.et_8011E4BC.unk7C = 0x3F;
            break;
        case 4:
            self->ext.et_8011E4BC.unk7C = 0x2F;
            break;
        case 3:
        case 6:
            self->ext.et_8011E4BC.unk7C = 0x1F;
            break;
        }
        tilePrim = (FakePrim*)&g_PrimBuf[self->primIndex];
        i = 0;
        while (1) {
            tilePrim->drawMode = props->drawMode;
            tilePrim->priority = RIC.zPriority + props->priority;
            if (tilePrim->next == NULL) {
                tilePrim->w = 0;
                tilePrim->x0 = 0;
                tilePrim->y0 = 0;
                tilePrim->drawMode &= ~DRAW_HIDE;
                break;
            }
            tilePrim->posX.i.hi = x;
            tilePrim->posY.i.hi = y;
            tilePrim->posX.i.lo = tilePrim->posY.i.lo = 0;
            switch (props->unkA) {
            case 0:
                rnd = rand();
                tilePrim->velocityX.val = rcos(rnd) << ((rnd & 1) + 2);
                tilePrim->velocityY.val = -(rsin(rnd) << ((rnd & 1) + 2));
                break;
            case 1:
                tilePrim->velocityX.val = ((rand() & 0x1FF) - 0x100) << 8;
                tilePrim->velocityY.val = ((rand() & 0x1FF) - 0x100) << 8;
                break;
            case 3:
                tilePrim->posX.i.hi = x + (rand() & 0xF) - 7;
                tilePrim->posY.i.hi = y - (rand() & 0xF);
                tilePrim->velocityY.val = FIX(-0.75) - (rand() & 0x7FFF);
                if (self->ext.et_8011E4BC.parent != NULL) {
                    tilePrim->velocityX.val =
                        self->ext.et_8011E4BC.parent->velocityX;
                }
                tilePrim->delay = (rand() & 0xF) + 0x10;
                break;
            case 4:
                tilePrim->posX.i.hi = x + (rand() & 0x1F) - 0x10;
                tilePrim->posY.i.hi = y + (rand() & 0x1F) - 0x14;
                rnd = rand() & 0x1F;
                tilePrim->velocityX.val = D_us_801D11C8[rnd];
                tilePrim->velocityY.val = D_us_801D1248[rnd];
                break;
            case 6:
                tilePrim->posX.i.hi = self->posX.i.hi + (rand() & 0xF) - 8;
                tilePrim->posY.i.hi = self->posY.i.hi + (rand() & 0xF) - 4;
                tilePrim->velocityY.val = rand() + FIX(0.5);
                tilePrim->delay = (rand() & 0xF) + 0x10;
                break;
            case 7:
                tilePrim->posX.i.hi = x;
                tilePrim->posY.i.hi = y;
                if (i < 10) {
                    tilePrim->velocityY.val =
                        -((i * i * FIX(0.09375)) + FIX(0.125));
                } else {
                    tilePrim->velocityY.val =
                        (i - 10) * (i - 10) * FIX(0.09375) + FIX(0.125);
                }
                tilePrim->delay = 0x3F;
                break;
            case 8:
                tilePrim->posX.i.hi = x;
                tilePrim->posY.i.hi = y;
                tilePrim->velocityX.val = (rand() - FIX(0.25)) >> 1;
                tilePrim->velocityY.val = -((rand() & 0x1FFF) + FIX(0.375));
                tilePrim->timer = (i * 4);
                break;
            }
            tilePrim->x0 = tilePrim->posX.i.hi;
            tilePrim->y0 = tilePrim->posY.i.hi;
            tilePrim->r0 = props->r;
            tilePrim->g0 = props->g;
            tilePrim->b0 = props->b;
            tilePrim->w = props->w;
            tilePrim->h = props->h;
            i++;
            tilePrim = tilePrim->next;
        }
        self->step++;
        break;
    case 1:
        switch (props->unkA) {
        case 3:
        case 6:
        case 7:
        case 8:
            if (--self->ext.et_8011E4BC.unk7C == 0) {
                DestroyEntity(self);
                return;
            }
        case 4:
            if (--self->ext.et_8011E4BC.unk7C == 0) {
                DestroyEntity(self);
                return;
            }
            thickness = 3;
            timer = self->ext.et_8011E4BC.unk7C;
            if (timer < 9) {
                thickness--;
            }
            if (timer < 4) {
                thickness--;
            }
        }
        tilePrim = (FakePrim*)&g_PrimBuf[self->primIndex];
        while (1) {
            if (tilePrim->next == NULL) {
                tilePrim->w = 0;
                tilePrim->x0 = 0;
                tilePrim->y0 = 0;
                tilePrim->drawMode &= ~DRAW_HIDE;
                return;
            }
            tilePrim->posX.i.hi = tilePrim->x0;
            tilePrim->posY.i.hi = tilePrim->y0;
            switch (props->unkA) {
            case 0:
                tilePrim->posY.val += tilePrim->velocityY.val;
                tilePrim->posX.val += tilePrim->velocityX.val;
                tilePrim->r0 -= 6;
                tilePrim->g0 -= 6;
                tilePrim->b0 -= 6;
                if (tilePrim->r0 < 8) {
                    DestroyEntity(self);
                    return;
                }
                break;
            case 1:
                tilePrim->posY.val += tilePrim->velocityY.val;
                tilePrim->posX.val += tilePrim->velocityX.val;
                tilePrim->velocityY.val += FIX(0.15625);
                tilePrim->r0 -= 3;
                tilePrim->g0 -= 3;
                tilePrim->b0 -= 3;
                if (tilePrim->r0 < 8) {
                    DestroyEntity(self);
                    return;
                }
                break;
            case 3:
            case 6:
            case 7:
                tilePrim->posY.val += tilePrim->velocityY.val;
                if (--tilePrim->delay < 0) {
                    tilePrim->drawMode |= DRAW_HIDE;
                }
                break;
            case 4:
                tilePrim->posX.val += tilePrim->velocityX.val;
                tilePrim->posY.val += tilePrim->velocityY.val;
                tilePrim->velocityY.val -= tilePrim->velocityY.val >> 5;
                if (!(timer & 7)) {
                    tilePrim->velocityX.val >>= 1;
                    tilePrim->velocityY.val >>= 1;
                    if (timer & 0x20) {
                        tilePrim->velocityY.val >>= 1;
                    }
                    if (timer == 0x18) {
                        tilePrim->drawMode = DRAW_UNK02;
                    }
                }
                tilePrim->w = tilePrim->h = thickness;
                break;
            case 8:
                if (tilePrim->timer == 0) {
                    tilePrim->drawMode &= ~DRAW_HIDE;
                    tilePrim->r0 -= 1;
                    tilePrim->g0 -= 1;
                    tilePrim->b0 -= 1;
                    tilePrim->posY.val += tilePrim->velocityY.val;
                    tilePrim->posX.val += tilePrim->velocityX.val;
                    if (g_unkGraphicsStruct.D_80097448 == 0 ||
                        !(tilePrim->posY.i.hi >
                          (RIC.posY.i.hi - g_unkGraphicsStruct.D_80097448 +
                           0x19))) {
                        tilePrim->drawMode |= DRAW_HIDE;
                    }
                } else {
                    tilePrim->timer--;
                }
                break;
            }
            tilePrim->x0 = tilePrim->posX.i.hi;
            tilePrim->y0 = tilePrim->posY.i.hi;
            tilePrim = tilePrim->next;
        }
    }
}
#endif

extern AnimationFrame D_us_801818A8[];
extern AnimationFrame D_us_801819D0[];
extern AnimationFrame D_us_80181A0C[];

void func_us_801BD0B8(Entity* self) {
    s16 paramsLo = self->params & 0xFF;
    s16 paramsHi = (self->params >> 8) & 0xFF;

    switch (self->step) {
    case 0:
        if (paramsHi == 1) {
            self->scaleX = 0xC0;
            self->scaleY = 0xC0;
            self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
            self->animSet = ANIMSET_DRA(2);
            self->anim = D_us_80181A0C;
        }

        if ((paramsHi == 0) || (paramsHi == 2)) {
            if (paramsLo & 3) {
                self->anim = D_us_801819D0;
                self->scaleX = 0x120;
                self->scaleY = 0x120;
                self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
                self->animSet = ANIMSET_DRA(2);
            } else {
                self->animSet = ANIMSET_DRA(5);
                self->anim = D_us_801818A8;
                self->palette = PAL_FLAG(PAL_UNK_170);
            }
        }
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;

        if (rand() & 3) {
            self->zPriority = RIC.zPriority + 2;
        } else {
            self->zPriority = RIC.zPriority - 2;
        }

        if (paramsHi == 2) {
            self->posX.i.hi = RIC.posX.i.hi + (rand() % 44) - 22;
        } else {
            self->posX.i.hi = RIC.posX.i.hi + (rand() & 15) - 8;
        }

        self->posY.i.hi = RIC.posY.i.hi + RIC.hitboxOffY + (rand() & 31) - 16;
        self->velocityY = FIX(-0.5);
        self->velocityX = RIC.velocityX >> 2;
        self->step++;
        break;

    case 1:
        self->scaleX -= 4;
        self->scaleY -= 4;
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if ((self->pose == 8) && (self->anim != D_us_801818A8)) {
            self->blendMode = BLEND_TRANSP;
            if (!(paramsLo & 1) && (self->poseTimer == 1)) {
                RicCreateEntFactoryFromEntity(self, FACTORY(4, 4), 0);
            }
        }

        if ((self->pose == 16) && (self->anim == D_us_801818A8)) {
            self->blendMode = BLEND_TRANSP;
        }

        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

extern AnimationFrame D_us_80181A40[];

#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BD384);
#else
void func_us_801BD384(Entity* self) {
    switch (self->step) {
    case 0:
        self->animSet = ANIMSET_DRA(2);
        self->anim = D_us_80181A40;
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        self->zPriority = RIC.zPriority + 4;
        self->velocityY = (rand() & 0x3FFF) - FIX(1);
        self->step++;
        break;
    case 1:
        if (self->pose == 6 && self->poseTimer == self->step && (rand() & 1)) {
            RicCreateEntFactoryFromEntity(self, BP_EMBERS, 0);
        }
        self->posY.val += self->velocityY;
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}
#endif

// Returns true if another entity of the same kind and params is alive
#ifndef VERSION_PC
INCLUDE_ASM("boss/bo6/nonmatchings/us_39144", func_us_801BD47C);
#else
s32 func_us_801BD47C(Entity* self) {
    Entity* entity;
    s32 i;

    entity = &g_Entities[80];
    for (i = 80; i < 144; i++, entity++) {
        if (self->entityId == entity->entityId &&
            self->params == entity->params && entity != self) {
            return 1;
        }
    }
    return 0;
}
#endif

extern s32 D_us_80181A64[];
extern s16 D_us_80181AA4[][10];
extern s16* D_us_801A39B0[];
extern u8* richter_sprites[];

void RicEntityPlayerBlinkWhite(Entity* self) {
    u8 xMargin;
    u8 yMargin;
    s16 angle;
    u8 wSprite;
    u8 hSprite;
    s16 width;
    s16 height;
    s16 selfX;
    s16 selfY;
    s32 i;
    Primitive* prim;
    s16 xPivot;
    s16 yPivot;
    s16 plSpriteIndex;
    s16 upperParams;
    s16 angleRedIndex;
    s16* sp44;
    u8* plSprite;
    s16* dataPtr;
    s16 angleGreenIndex;
    s16 angleBlueIndex;
    s16 redDivide;
    s16 blueDivide;
    s16 greenDivide;
    void* dummy48;

    if (!RIC.animSet || !(RIC.animCurFrame & 0x7FFF)) {
        DestroyEntity(self);
        return;
    }
    self->posY.i.hi = RIC.posY.i.hi;
    self->posX.i.hi = RIC.posX.i.hi;
    self->facingLeft = RIC.facingLeft;
#if defined(VERSION_PSP)
    sp44 = D_92641C8[RIC.animCurFrame & 0x7FFF];
#endif
    sp44 = D_us_801A39B0[RIC.animCurFrame & 0x7FFF];
    plSpriteIndex = *sp44++;
    plSpriteIndex &= 0x7FFF;
    selfX = self->posX.i.hi;
    selfY = self->posY.i.hi;
    plSprite = richter_sprites[plSpriteIndex];
    xMargin = 4;
    yMargin = 1;
    wSprite = xMargin + plSprite[0];
    hSprite = yMargin + plSprite[1];
    width = wSprite - xMargin;
    height = hSprite - yMargin;
    xPivot = sp44[0] + plSprite[2];
    yPivot = sp44[1] + plSprite[3];

    self->rotate = RIC.rotate;
    self->drawFlags = RIC.drawFlags;
    self->scaleX = RIC.scaleX;
    self->scaleY = RIC.scaleY;
    self->rotPivotY = RIC.rotPivotY;
    self->rotPivotX = RIC.rotPivotX;
    upperParams = (self->params & 0x7F00) >> 8;
    dataPtr = D_us_80181AA4[upperParams & 0x3F];
    switch (self->step) {
    case 0:
        if (func_us_801BD47C(self) != 0) {
            DestroyEntity(self);
            return;
        }
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 8);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }

        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;

        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 8; i++) {
            D_us_80181A64[i] = i << 9;
            prim->tpage = 0x10;
            prim->clut = dataPtr[3];
            prim->r0 = prim->b0 = prim->g0 = prim->r1 = prim->b1 = prim->g1 =
                prim->r2 = prim->b2 = prim->g2 = prim->r3 = prim->b3 =
                    prim->g3 = 0x80;
            prim->priority = RIC.zPriority + 2;
            prim->drawMode = dataPtr[8] + DRAW_UNK_100 + DRAW_COLORS;
            prim = prim->next;
        }
        self->ext.playerBlink.unk8A = dataPtr[9];
        self->ext.playerBlink.unk90 = 0;
        self->step += 1;
        break;
    case 1:
        self->ext.playerBlink.unk90 += 0xA;
        if (self->ext.playerBlink.unk90 > 0x100) {
            self->ext.playerBlink.unk90 = 0x100;
            self->ext.playerBlink.unk80 = dataPtr[7];
            self->step += 1;
        }
        break;
    case 2:
        if (dataPtr[7] >= 0x7000) {
            switch ((u32)dataPtr[7]) {
            case 0x7000:
                if (g_Ric.timers[PL_T_POISON] == 0 ||
                    (RIC.step == PL_S_DEAD || RIC.step == PL_S_ENDING_3 ||
                     RIC.step == PL_S_ENDING_4)) {
                    self->step++;
                }
                break;
            case 0x7001:
                if (g_Ric.timers[PL_T_INVINCIBLE_SCENE] == 0) {
                    self->step++;
                }
                break;
            case 0x7007:
            case 0x7002:
                if (RIC.step != PL_S_HIT) {
                    self->step++;
                }
                break;
            case 0x7005:
            case 0x7006:
                if (RIC.step_s == 3) {
                    self->step++;
                }
                break;
            case 0x7003:
            case 0x7004:
            case 0x7008:

                break;
            }
            self->ext.playerBlink.unk80 = 8;
        }
        if (--self->ext.playerBlink.unk80 == 0) {
            self->step++;
        }
        break;
    case 3:
        self->ext.playerBlink.unk90 -= 10;
        if (self->ext.playerBlink.unk90 < 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    self->ext.playerBlink.unk82 += self->ext.playerBlink.unk8A;
    if (self->facingLeft) {
        selfX = selfX - xPivot;
    } else {
        selfX = selfX + xPivot;
    }
    selfY = selfY + yPivot;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 8; i++) {
        if (upperParams & 0x40) {
            switch (i) {
            case 0:
                if (self->facingLeft) {
                    prim->x0 = selfX;
                    prim->x1 = selfX - width / 2;
                    prim->u0 = xMargin;
                    prim->u1 = xMargin + width / 2;
                } else {
                    prim->x0 = selfX;
                    prim->x1 = selfX + width / 2;
                    prim->u0 = xMargin;
                    prim->u1 = xMargin + width / 2;
                }
                prim->y0 = prim->y1 = selfY;
                prim->v0 = prim->v1 = yMargin;
                break;
            case 1:
                if (self->facingLeft) {
                    prim->x0 = selfX - width / 2;
                    prim->x1 = selfX - width;
                    prim->u0 = xMargin + width / 2;
                    prim->u1 = xMargin + width;
                } else {
                    prim->x0 = selfX + width / 2;
                    prim->x1 = selfX + width;
                    prim->u0 = xMargin + width / 2;
                    prim->u1 = xMargin + width;
                }
                prim->y0 = prim->y1 = selfY;
                prim->v0 = prim->v1 = yMargin;
                break;
            case 2:
                if (self->facingLeft) {
                    prim->x0 = prim->x1 = selfX - width;
                    prim->u0 = prim->u1 = xMargin + width;
                } else {
                    prim->x0 = prim->x1 = selfX + width;
                    prim->u0 = prim->u1 = xMargin + width;
                }
                prim->y0 = selfY;
                prim->y1 = selfY + height / 2;
                prim->v0 = yMargin;
                prim->v1 = yMargin + height / 2;
                break;
            case 3:
                if (self->facingLeft) {
                    prim->x0 = prim->x1 = selfX - width;
                    prim->u0 = prim->u1 = xMargin + width;
                } else {
                    prim->x0 = prim->x1 = selfX + width;
                    prim->u0 = prim->u1 = xMargin + width;
                }
                prim->y0 = selfY + height / 2;
                prim->y1 = selfY + height;
                prim->v0 = yMargin + height / 2;
                prim->v1 = yMargin + height;
                break;
            case 4:
                if (self->facingLeft) {
                    prim->x0 = selfX - width;
                    prim->x1 = selfX - width / 2;
                    prim->u0 = xMargin + width;
                    prim->u1 = xMargin + width / 2;
                } else {
                    prim->x0 = selfX + width;
                    prim->x1 = selfX + width / 2;
                    prim->u0 = xMargin + width;
                    prim->u1 = xMargin + width / 2;
                }
                prim->y0 = prim->y1 = selfY + height;
                prim->v0 = prim->v1 = yMargin + height;
                break;
            case 5:
                if (self->facingLeft) {
                    prim->x0 = selfX - width / 2;
                    prim->x1 = selfX;
                    prim->u0 = xMargin + width / 2;
                    prim->u1 = xMargin;
                } else {
                    prim->x0 = selfX + width / 2;
                    prim->x1 = selfX;
                    prim->u0 = xMargin + width / 2;
                    prim->u1 = xMargin;
                }
                prim->y0 = prim->y1 = selfY + height;
                prim->v0 = prim->v1 = yMargin + height;
                break;
            case 6:
                if (self->facingLeft) {
                    prim->x0 = prim->x1 = selfX;
                    prim->u0 = prim->u1 = xMargin;
                } else {
                    prim->x0 = prim->x1 = selfX;
                    prim->u0 = prim->u1 = xMargin;
                }
                prim->y0 = selfY + height;
                prim->y1 = selfY + height / 2;
                prim->v0 = yMargin + height;
                prim->v1 = yMargin + height / 2;
                break;
            case 7:
                if (self->facingLeft) {
                    prim->x0 = prim->x1 = selfX;
                    prim->u0 = prim->u1 = xMargin;
                } else {
                    prim->x0 = prim->x1 = selfX;
                    prim->u0 = prim->u1 = xMargin;
                }
                prim->y0 = selfY + height / 2;
                prim->y1 = selfY;
                prim->v0 = yMargin + height / 2;
                prim->v1 = yMargin;
                break;
            }
            if (self->facingLeft) {
                prim->x2 = prim->x3 =
                    selfX - width / 2 +
                    (((rcos(self->ext.playerBlink.unk82) >> 4) * 3) >> 0xC);
            } else {
                prim->x2 = prim->x3 =
                    selfX + width / 2 +
                    (((rcos(self->ext.playerBlink.unk82) >> 4) * 3) >> 0xC);
            }
            prim->y2 = prim->y3 =
                (selfY + height / 2) -
                ((rsin(self->ext.playerBlink.unk82) >> 4) * 3 << 1 >> 8);
            prim->u2 = prim->u3 = xMargin + width / 2;
            prim->v2 = prim->v3 = yMargin + height / 2;
        } else {
            if (self->facingLeft) {
                prim->x0 = prim->x2 = (selfX - width) + 1;
                prim->x1 = prim->x3 = selfX + 1;
            } else {
                prim->x0 = prim->x2 = selfX;
                prim->x1 = prim->x3 = selfX + width;
            }
            prim->y0 = prim->y1 = selfY + height * i / 8;
            prim->y2 = prim->y3 = selfY + height * (i + 1) / 8;
            if (self->facingLeft) {
                prim->u0 = prim->u2 = wSprite - 1;
                prim->u1 = prim->u3 = xMargin - 1;
            } else {
                prim->u0 = prim->u2 = xMargin;
                prim->u1 = prim->u3 = wSprite;
            }
            prim->v0 = prim->v1 = yMargin + height * i / 8;
            prim->v2 = prim->v3 = yMargin + height * (i + 1) / 8;
        }
        angleRedIndex = dataPtr[0];
        angleGreenIndex = dataPtr[2];
        angleBlueIndex = dataPtr[1];
        redDivide = dataPtr[4];
        greenDivide = dataPtr[6];
        blueDivide = dataPtr[5];
        if (upperParams & 0x40) {
            angle = D_us_80181A64[(i + angleRedIndex) % 8];
            prim->r0 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / redDivide;
            angle = D_us_80181A64[(i + angleGreenIndex) % 8];
            prim->g0 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / greenDivide;
            angle = D_us_80181A64[(i + angleBlueIndex) % 8];
            prim->b0 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / blueDivide;
            angle = D_us_80181A64[(i + angleRedIndex + 1) % 8];
            prim->r1 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / redDivide;
            angle = D_us_80181A64[(i + angleGreenIndex + 1) % 8];
            prim->g1 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / greenDivide;
            angle = D_us_80181A64[(i + angleBlueIndex + 1) % 8];
            prim->b1 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / blueDivide;
            prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 = prim->b3 = 0;
            D_us_80181A64[i] += self->ext.playerBlink.unk8A;
        } else {
            angle = D_us_80181A64[(i + angleRedIndex) % 8];
            prim->r0 = prim->r1 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / redDivide);
            angle = D_us_80181A64[(i + angleGreenIndex) % 8];
            prim->g0 = prim->g1 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / greenDivide);
            angle = D_us_80181A64[(i + angleBlueIndex) % 8];
            prim->b0 = prim->b1 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / blueDivide);
            angle = D_us_80181A64[(i + angleRedIndex + 1) % 8];
            prim->r2 = prim->r3 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / redDivide);
            angle = D_us_80181A64[(i + angleGreenIndex + 1) % 8];
            prim->g2 = prim->g3 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / greenDivide);
            angle = D_us_80181A64[(i + angleBlueIndex + 1) % 8];
            prim->b2 = prim->b3 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / blueDivide);
            D_us_80181A64[i] += self->ext.playerBlink.unk8A;
        }
        prim->priority = RIC.zPriority + 2;
        prim = prim->next;
    }
    if ((upperParams & 0x3F) == 0 || (upperParams & 0x3F) == 7) {
        RicSetInvincibilityFrames(1, 10);
    }
}
