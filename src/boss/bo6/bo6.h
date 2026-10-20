// SPDX-License-Identifier: AGPL-3.0-or-later
#ifndef BO6_H
#define BO6_H

#include <stage.h>

enum Palettes {
    PAL_NONE = 0,
};

typedef enum EntityID {
    /* 0x00 */ E_NONE,
    /* 0x01 */ E_FACTORY,
    /* 0x02 */ E_EXPLOSION,
    /* 0x03 */ E_PRIZE_DROP,
    /* 0x04 */ E_NUMERIC_DAMAGE,
    /* 0x05 */ E_RED_DOOR,
    /* 0x06 */ E_INTENSE_EXPLOSION,
    /* 0x07 */ E_SOUL_STEAL_ORB,
    /* 0x08 */ E_ROOM_FOREGROUND,
    /* 0x09 */ E_STAGE_NAME_POPUP,
    /* 0x0A */ E_EQUIP_ITEM_DROP,
    /* 0x0B */ E_RELIC_ORB,
    /* 0x0C */ E_PERSISTENT_ITEM_DROP,
    /* 0x0D */ E_ENEMY_BLOOD,
    /* 0x0E */ E_MESSAGE_BOX,
    /* 0x13 */ E_ID_13,
    /* 0x14 */ E_EXPLOSION_VARIANTS = 0x14,
    /* 0x15 */ E_GREY_PUFF,
    /* 0x17 */ E_ID_17 = 0x17,

    /* 0x21 */ E_ID_21 = 0x21,
    /* 0x23 */ E_ID_23 = 0x23,
    /* 0x24 */ E_ID_24 = 0x24,

    /* 0x40 */ E_RICHTER = STAGE_ENTITY_START,
    /* 0x41 */ E_ID_41,
    /* 0x42 */ E_ID_42,
    /* 0x43 */ E_ID_43,
    /* 0x44 */ E_ID_44,
    /* 0x90 */ E_ID_90 = 0x90,
} EntityID;

#ifdef VERSION_PSP
extern s32 D_pspeu_0927BAF8;
#undef E_ID
#define E_ID(ID_17) D_pspeu_0927BAF8
#endif

// same as `RicSteps`, but like Doppleganger, offset by 1
typedef enum RicSteps {
    /* 0x01 */ PL_S_STAND = 1,
    /* 0x02 */ PL_S_WALK,
    /* 0x03 */ PL_S_CROUCH,
    /* 0x04 */ PL_S_FALL,
    /* 0x05 */ PL_S_JUMP,
    /* 0x06 */ PL_S_5, // unused
    /* 0x07 */ PL_S_6, // unused
    /* 0x08 */ PL_S_7, // unused
    /* 0x09 */ PL_S_HIGHJUMP,
    /* 0x0A */ PL_S_9, // unused
    /* 0x0B */ PL_S_HIT,
    /* 0x0C */ PL_S_11, // unused
    /* 0x0D */ PL_S_BOSS_GRAB,
    /* 0x0E */ PL_S_13, // unused
    /* 0x0F */ PL_S_14, // unused
    /* 0x10 */ PL_S_15, // unused
    /* 0x11 */ PL_S_DEAD,
    /* 0x12 */ PL_S_17, // unused
    /* 0x13 */ PL_S_STAND_IN_AIR,
    /* 0x14 */ PL_S_FLAME_WHIP,
    /* 0x15 */ PL_S_HYDROSTORM,
    /* 0x16 */ PL_S_THROW_DAGGERS,
    /* 0x17 */ PL_S_DEAD_PROLOGUE,
    /* 0x18 */ PL_S_SLIDE,
    /* 0x19 */ PL_S_BLADEDASH,
    /* 0x1A */ PL_S_RUN,
    /* 0x1B */ PL_S_SLIDE_KICK,
    /* 0x1C */ PL_S_SUBWPN_CRASH,
    /* 0x1D */ PL_S_28, // unused
    /* 0x1E */ PL_S_29, // unused
    /* 0x1F */ PL_S_30, // unused
    /* 0x20 */ PL_S_31, // unused
    /* 0x21 */ PL_S_INIT,
    // various endings
    /* 0x40 */ PL_S_ENDING_1 = 0x40,
    /* 0x50 */ PL_S_ENDING2 = 0x50,
    /* 0x60 */ PL_S_ENDING_3 = 0x60,
    /* 0x70 */ PL_S_ENDING_4 = 0x70,
    /* 0xF0 */ PL_S_DEBUG = 0xF0,
} BO6_RicSteps;

#include "../../ric/ric_shared.h"

#define RIC g_Entities[STAGE_ENTITY_START]

extern PlayerState g_Ric;

#ifdef VERSION_PC
// Prototypes of the functions defined in this overlay, so that 64-bit builds
// never fall back to implicit int declarations.
void func_us_801A7DC0(Entity* self);
void func_us_801A9084(Entity* self);
void func_us_801A9208(Entity* self);
void func_us_801A95F4(Entity* self);
void func_us_801A9944(Entity* self);
void func_us_801B4BD0(void);
void func_us_801B4EAC(void);
void CheckHighJumpInput(void);
void RicMain(void);
void func_us_801B5A14(s32 step);
void RichterThinking(void);
void func_us_801B6998(void);
void EntityRichter(Entity* self);
void RicStepStand(void);
void RicStepWalk(void);
void RicStepRun(void);
void RicStepJump(void);
void RicStepFall(void);
void RicStepCrouch(void);
void RicResetPose(void);
void func_us_801B77D8(void);
void RicStepHit(s32 damageEffect, u32 damageKind, s16 prevStep, s32 prevStepS);
void RicStepDead(s32 damageEffects, s32 damageKind, s32 prevStep, s32 prevStepS);
void RicStepStandInAir(void);
void RicStepEnableFlameWhip(void);
void RicStepHydrostorm(void);
void RicStepGenericSubwpnCrash(void);
void RicStepThrowDaggers(void);
void RicStepSlide(void);
void RicStepSlideKick(void);
void RicStepBladeDash(void);
void func_us_801B8E80(s32 arg0);
void RicStepHighJump(void);
void func_us_801B9144(void);
void func_us_801B9340(void);
void func_us_801B94CC(void);
void func_us_801B96F4(void);
void RicSetStep(s16 step);
void RicSetAnimation(AnimationFrame* anim);
s32 RicCheckFacing(void);
void RicSetSpeedX(s32 speed);
void func_us_801B9ACC(s32 speed);
void RicSetInvincibilityFrames(s32 kind, s16 invincibilityFrames);
void DisableAfterImage(s32 resetAnims, s32 time);
void func_us_801B9C14(void);
void RicSetCrouch(s32 kind, s32 velocityX);
void RicSetStand(s32 velocityX);
void func_us_801B9D74(void);
void func_us_801B9DE4(void);
void func_us_801B9E70(void);
void RicSetFall(void);
void func_us_801BA050(void);
s32 RicDoSubweapon(void);
s32 RicDoAttack(void);
s32 RicDoCrash(void);
void RicSetSlide(void);
void RicSetSlideKick(void);
void func_us_801BA9D0(void);
s32 RicCheckInput(s32 checks);
Entity* RicGetFreeEntity(s16 start, s16 end);
Entity* RicGetFreeEntityReverse(s16 start, s16 end);
void func_us_801BB314(s32 arg0);
void func_us_801BB370(Entity* entity);
s32 RicCheckSubweapon(SubweaponDef* actualSubwpn, s32 isItemCrash, s32 useHearts);
s32 func_us_801BB5BC(Primitive* prim, s16 posX, s16 posY);
void RicEntityHitByHoly(Entity* entity);
void RicEntityHitByDark(Entity* entity);
void func_us_801BBBD0(void);
Entity* RicCreateEntFactoryFromEntity(Entity* source, u32 factoryParams, s32 arg2);
void RicEntityFactory(Entity* self);
void func_us_801BC2F0(Entity* self);
void func_us_801BC3E0(Entity* self);
void func_us_801BC4F8(Entity* self);
void func_us_801BC5C8(Entity* self);
void func_us_801BC678(Entity* self);
void RicEntityHitByCutBlood(Entity* self);
void func_us_801BD0B8(Entity* self);
void func_us_801BD384(Entity* self);
s32 func_us_801BD47C(Entity* self);
void RicEntityPlayerBlinkWhite(Entity* self);
void func_us_801BE79C(Entity* self);
void RicEntityShrinkingPowerUpRing(Entity* self);
void RicEntityHitByIce(Entity* self);
void RicEntityHitByLightning(Entity* self);
void func_us_801C03E8(Entity* self);
void EntityShaft(Entity* self);
void func_us_801C0FE8(Entity* self);
void func_us_801C13A8(Entity* self);
void RicEntityWhip(Entity* self);
void RicEntityArmBrandishWhip(Entity* entity);
void func_us_801C2688(Entity* entity);
void RicEntitySubwpnHolyWaterBreakGlass(Entity* self);
void RicEntityCrashHydroStorm(Entity* self);
s32 RicCheckHolyWaterCollision(s16 height, s16 width);
void RicEntitySubwpnHolyWater(Entity* self);
void RicEntitySubwpnHolyWaterFlame(Entity* self);
void RicEntitySubwpnCrashCross(Entity* self);
void RicEntitySubwpnCross(Entity* self);
void func_us_801C488C(Entity* self);
void RicEntitySubwpnCrossTrail(Entity* self);
void RicEntitySubwpnCrashCrossParticles(Entity* self);
void RicEntitySubwpnThrownAxe(Entity* self);
void RicEntityCrashAxe(Entity* self);
void RicEntitySubwpnKnife(Entity* self);
void RicEntitySubwpnReboundStone(Entity* self);
void RicEntitySubwpnThrownVibhuti(Entity* self);
u8 BO6_PrimDecreaseBrightness(Primitive* prim, u8 amount);
void RicEntitySubwpnAgunea(Entity* self);
void RicEntityAguneaHitEnemy(Entity* self);
void RicEntityVibhutiCrashCloud(Entity* self);
void RicEntityCrashVibhuti(Entity* self);
void func_us_801C8590(Entity* self);
void func_us_801C8618(Entity* self);
void RicEntityCrashReboundStoneExplosion(Entity* self);
void RicEntityCrashReboundStone(Entity* entity);
void RicEntityCrashBibleBeam(Entity* self);
void RicEntityCrashBible(Entity* self);
void func_us_801C9DE8(Entity* self);
void func_us_801CA340(Entity* self);
void RicEntityAguneaLightning(Entity* self);
void RicEntityAguneaCircle(Entity* self);
void RicEntitySubwpnStopwatchCircle(Entity* self);
void EntityStopWatch(Entity* self);
void RicEntitySubwpnBibleTrail(Entity* entity);
void RicEntitySubwpnBible(Entity* self);
void RicEntityCrashCrossBeam(Entity* self);
#endif

#endif // BO6_H
