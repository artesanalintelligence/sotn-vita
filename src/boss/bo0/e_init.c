// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

// Entity table and init values. The PSX build still takes these from the
// asm data segment; the PC build compiles them from here.

void EntityBreakable(Entity* self);
void EntityExplosion(Entity* self);
void EntityPrizeDrop(Entity* self);
void EntityDamageDisplay(Entity* self);
void EntityRedDoor(Entity* self);
void EntityIntenseExplosion(Entity* self);
void EntitySoulStealOrb(Entity* self);
void EntityRoomForeground(Entity* self);
void EntityStageNamePopup(Entity* self);
void EntityEquipItemDrop(Entity* self);
void EntityRelicOrb(Entity* self);
void EntityPersistentItemDrop(Entity* self);
void EntityEnemyBlood(Entity* self);
void EntityMessageBox(Entity* self);
void EntityDummy(Entity* self);
void EntityBackgroundBlock(Entity* self);
void EntityLockCamera(Entity* self);
void EntityUnkId13(Entity* self);
void EntityExplosionVariants(Entity* self);
void EntityGreyPuff(Entity* self);
void EntitySkelerang(Entity* self);
void EntitySkelerangBoomerang(Entity* self);
void EntitySkelerangUnknown(Entity* self);
void func_us_801B3D8C(Entity* self);
void func_us_801B3F30(Entity* self);
void func_us_801B4148(Entity* self);
void func_us_801B41A4(Entity* self);
void func_us_801B4210(Entity* self);
void Entity3DBackgroundHouse(Entity* self);
void Entity3DHouseSpawner(Entity* self);
void func_us_801B4DA4(Entity* self);
void EntitySpikes(Entity* self);
void EntitySpikesParts(Entity* self);
void EntitySpikesDust(Entity* self);
void func_us_801ABFE0(Entity* self);
void func_us_801AC894(Entity* self);
void func_us_801AC54C(Entity* self);
void EntityMarionette(Entity* self);
void EntityBigRedFireball(Entity* self);
void EntityPrisoner(Entity* self);
void func_us_801B001C(Entity* self);
void func_us_801AD338(Entity* self);
void EntityOlroxAfterImage(Entity* self);
void func_us_801AFAF4(Entity* self);
void EntityPortal(Entity* self);
void EntityPortalSkulls(Entity* self);
void EntityBlastAttack(Entity* self);
void EntityGroundBlast(Entity* self);
void func_us_801B0930(Entity* self);
void func_us_801B365C(Entity* self);
void func_us_801B5470(Entity* self);
void func_us_801B551C(Entity* self);
void func_us_801B619C(Entity* self);
void func_us_801B6CA4(Entity* self);
void func_us_801B76E4(Entity* self);
void EntityLaserAttack(Entity* self);
void EntityFireballAttack(Entity* self);
void func_us_801B7BAC(Entity* self);
void func_us_801B7CC8(Entity* self);
void EntityRealOlroxDrool(Entity* self);
void func_us_801BA4AC(Entity* self);
void EntityLifeUpSpawn(Entity* self);
void EntityOwlKnight(Entity* self);
void EntityOwl(Entity* self);
void EntityOwlKnightSword(Entity* self);
void EntityOwlTarget(Entity* self);
void EntityBloodyZombie(Entity* self);
void EntityBloodDrips(Entity* self);
void EntityBloodSplatter(Entity* self);
void func_us_801B72E8(Entity* self);
void EntitySpikesDamage(Entity* self);
void EntityBossDoors(Entity* self);
void EntitySealedDoor(Entity* self);
void EntityChair(Entity* self);

PfnEntityUpdate EntityUpdates[] = {
    EntityBreakable,
    EntityExplosion,
    EntityPrizeDrop,
    EntityDamageDisplay,
    EntityRedDoor,
    EntityIntenseExplosion,
    EntitySoulStealOrb,
    EntityRoomForeground,
    EntityStageNamePopup,
    EntityEquipItemDrop,
    EntityRelicOrb,
    EntityPersistentItemDrop,
    EntityEnemyBlood,
    EntityMessageBox,
    EntityDummy,
    EntityDummy,
    EntityBackgroundBlock,
    EntityLockCamera,
    EntityUnkId13,
    EntityExplosionVariants,
    EntityGreyPuff,
    EntitySkelerang,
    EntitySkelerangBoomerang,
    EntitySkelerangUnknown,
    func_us_801B3D8C,
    func_us_801B3F30,
    func_us_801B4148,
    func_us_801B41A4,
    func_us_801B4210,
    Entity3DBackgroundHouse,
    Entity3DHouseSpawner,
    func_us_801B4DA4,
    EntitySpikes,
    EntitySpikesParts,
    EntitySpikesDust,
    func_us_801ABFE0,
    func_us_801AC894,
    func_us_801AC54C,
    EntityMarionette,
    EntityBigRedFireball,
    EntityPrisoner,
    func_us_801B001C,
    func_us_801AD338,
    EntityOlroxAfterImage,
    func_us_801AFAF4,
    EntityPortal,
    EntityPortalSkulls,
    EntityBlastAttack,
    EntityGroundBlast,
    func_us_801B0930,
    func_us_801B365C,
    func_us_801B5470,
    func_us_801B551C,
    func_us_801B619C,
    func_us_801B6CA4,
    func_us_801B76E4,
    EntityLaserAttack,
    EntityFireballAttack,
    func_us_801B7BAC,
    func_us_801B7CC8,
    EntityRealOlroxDrool,
    func_us_801BA4AC,
    EntityLifeUpSpawn,
    EntityOwlKnight,
    EntityOwl,
    EntityOwlKnightSword,
    EntityOwlTarget,
    EntityBloodyZombie,
    EntityBloodDrips,
    EntityBloodSplatter,
    func_us_801B72E8,
    EntitySpikesDamage,
    EntityBossDoors,
    EntitySealedDoor,
    EntityChair,
};

// animSet, animCurFrame, unk5A, palette, enemyID
// clang-format off
EInit g_EInitBreakable = {ANIMSET_DRA(3), 0x00, 0x00, PAL_NONE, 0x003};
EInit g_EInitObtainable = {ANIMSET_DRA(3), 0x00, 0x00, PAL_NONE, 0x001};
EInit g_EInitParticle = {ANIMSET_DRA(3), 0x00, 0x00, PAL_NONE, 0x002};
EInit g_EInitSpawner = {ANIMSET_DRA(0), 0x00, 0x00, PAL_NONE, 0x004};
EInit g_EInitInteractable = {ANIMSET_DRA(0), 0x00, 0x00, PAL_NONE, 0x005};
EInit g_EInitUnkId13 = {ANIMSET_DRA(0), 0x00, 0x00, PAL_NONE, 0x002};
EInit g_EInitLockCamera = {ANIMSET_DRA(0), 0x00, 0x00, PAL_NONE, 0x001};
EInit g_EInitCommon = {ANIMSET_DRA(0), 0x00, 0x00, PAL_NONE, 0x003};
EInit g_EInitDamageNum = {ANIMSET_DRA(0), 0x00, 0x00, PAL_NONE, 0x003};
EInit g_EInitSkelerang = {ANIMSET_OVL(1), 0x01, 0x48, 0x200, 0x00B};
EInit g_EInitSkelerangBoomerang = {ANIMSET_OVL(1), 0x2B, 0x48, 0x200, 0x00C};
EInit g_EInitEnvironment = {ANIMSET_OVL(2), 0x0B, 0x00, PAL_NONE, 0x003};
EInit g_EInitPrisoner = {ANIMSET_OVL(3), 0x01, 0x49, 0x203, 0x003};
EInit g_EInitMarionette = {ANIMSET_OVL(4), 0x00, 0x48, 0x205, 0x031};
EInit g_EInitOwlKnight = {ANIMSET_OVL(8), 0x10, 0x49, 0x21C, 0x014};
EInit g_EInitSlinger = {0x0000};
EInit g_EInitOwlKnightSword = {ANIMSET_OVL(8), 0x00, 0x49, 0x2CB, 0x015};
EInit g_EInitOwl = {ANIMSET_OVL(8), 0x01, 0x49, 0x21C, 0x016};
EInit g_EInitBloodyZombie = {ANIMSET_OVL(9), 0x01, 0x48, 0x223, 0x00D};
EInit g_EInitOlroxAfterImage = {ANIMSET_OVL(6), 0x01, 0x48, 0x209, 0x032};
EInit D_us_801806D8 = {ANIMSET_OVL(6), 0x3A, 0x48, 0x209, 0x033};
EInit D_us_801806E4 = {ANIMSET_OVL(6), 0x50, 0x48, 0x209, 0x034};
EInit D_us_801806F0 = {ANIMSET_OVL(6), 0x3F, 0x48, 0x209, 0x035};
EInit D_us_801806FC = {ANIMSET_OVL(0), 0x00, 0x00, PAL_NONE, 0x036};
EInit D_us_80180708 = {ANIMSET_OVL(7), 0x01, 0x50, 0x20B, 0x037};
EInit D_us_80180714 = {ANIMSET_OVL(7), 0x01, 0x50, 0x20B, 0x038};
EInit g_EInitLaserAttack = {ANIMSET_OVL(0), 0x00, 0x00, PAL_NONE, 0x03C};
EInit g_EInitFireballAttack = {ANIMSET_OVL(9), 0x01, 0x00, PAL_NONE, 0x03B};
EInit g_EInitBossDoors = {ANIMSET_OVL(2), 0x0E, 0x00, PAL_NONE, 0x005};
// clang-format on

u16 g_RedDoorTiles[][8] = {
    {0x06B4, 0x06B4, 0x06B4, 0x06B4, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x06B4, 0x06B4, 0x06B5, 0x06B6, 0x0000, 0x0000, 0x0593, 0x0595},
    {0x06B7, 0x06B8, 0x06B9, 0x06BA, 0x02EA, 0x02F2, 0x032F, 0x0331},
    {0x06BB, 0x06BC, 0x06BD, 0x06BE, 0x0467, 0x045F, 0x0459, 0x0454},
};

// Prize drops for breakables/candles
u16 PrizeDrops[] = {
    0x0008, 0x0008, 0x0008, 0x0009, 0x0008, 0x000A, 0x0008, 0x000B, 0xFFFF,
};

// Fireball attack animation and scale tables
u8 g_animFireballAttack1[] = {
    0x01, 0x01, 0x01, 0x02, 0x01, 0x03, 0x01, 0x04,
    0x01, 0x05, 0x01, 0x06, 0x01, 0x07, 0x01, 0x08,
    0x00, 0x00, 0x00, 0x00,
};
s16 g_animFireballAttack2[] = {0x0020, 0x0030, 0x0048, 0x0080};
