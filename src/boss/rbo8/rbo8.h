// SPDX-License-Identifier: AGPL-3.0-or-later
#ifndef RBO8_H
#define RBO8_H

#include <stage.h>

#define STAGE_IS_RBO8

enum Palettes {
    PAL_NONE,
    PAL_GALAMOTH = 0x200,
    PAL_UNK_21B = 0x21B,
};

typedef enum EntityID {
    E_NONE,
    E_BREAKABLE,            // EntityBreakable
    E_EXPLOSION,            // EntityExplosion
    E_PRIZE_DROP,           // EntityPrizeDrop
    E_DAMAGE_DISPLAY,       // EntityDamageDisplay
    E_RED_DOOR,             // EntityRedDoor
    E_INTENSE_EXPLOSION,    // EntityIntenseExplosion
    E_SOUL_STEAL_ORB,       // EntitySoulStealOrb
    E_ROOM_FOREGROUND,      // EntityRoomForeground
    E_STAGE_NAME_POPUP,     // EntityStageNamePopup
    E_EQUIP_ITEM_DROP,      // EntityEquipItemDrop
    E_RELIC_ORB,            // EntityRelicOrb
    E_PERSISTENT_ITEM_DROP, // EntityPersistentItemDrop
    E_ENEMY_BLOOD,          // EntityEnemyBlood
    E_MESSAGE_BOX,          // EntityMessageBox
    E_DUMMY_F,              // EntityDummy
    E_DUMMY_10,             // EntityDummy
    E_BACKGROUND_BLOCK,     // EntityBackgroundBlock
    E_LOCK_CAMERA,          // EntityLockCamera
    E_UNK_ID13,             // EntityUnkId13
    E_EXPLOSION_VARIANTS,   // EntityExplosionVariants
    E_GREY_PUFF,            // EntityGreyPuff
    E_UNK_16,               // func_us_80195D80
    E_UNK_17,               // func_us_80197B1C
    E_UNK_18,               // func_us_801980E4
    E_UNK_19,               // func_us_80198210
    E_UNK_1A,               // func_us_801983EC
    E_UNK_1B,               // func_us_8019C7B8_from_rcen
    E_UNK_1C,               // func_us_80198964
    E_UNK_1D,               // func_us_801991D4
    E_MINOTAUR_SPIT_LIQUID, // EntityMinotaurSpitLiquid
    E_UNK_1F,               // func_us_8019921C
    E_UNK_20,               // func_us_8019943C
    E_UNK_21,               // func_us_8019953C
    E_UNK_22,               // func_us_80199A58
    E_LIFE_UP_SPAWN,        // EntityLifeUpSpawn
    NUM_ENTITIES,
};

// hitbox of each Galamoth body part, indexed by animCurFrame
typedef struct {
    s8 offsetX;
    s8 offsetY;
    u8 width;
    u8 height;
} GalamothHitbox;

// Galamoth and its body parts use the Giant Brothers part system
// (ext.GH_Props). This is the same layout with the padding bytes named,
// so both views can be used on the same entity on every platform.
typedef struct {
    /* 0x7C */ struct Primitive* prim;
    /* 0x80 */ s16 timer;
    /* 0x82 */ s16 : 16;
    /* 0x84 */ u8 unk84;
    /* 0x85 */ s32 : 24;
    /* 0x88 */ u8 unk88;
    /* 0x89 */ u8 unk89;
    /* 0x8A */ u8 unk8A;
    /* 0x8B */ u8 unk8B;
    /* 0x8C */ u8 unk8C;
    /* 0x8D */ u8 unk8D;
    /* 0x8E */ u8 unk8E;
    /* 0x8F */ u8 : 8;
    /* 0x90 */ s32 : 32;
    /* 0x94 */ s32 : 32;
    /* 0x98 */ s32 : 32;
    /* 0x9C */ s16 rotate;
    /* 0x9E */ s16 length;
    /* 0xA0 */ struct Entity* parent;
    /* 0xA4 */ s16 unkA4;
    /* 0xA6 */ s16 rotVel;
    /* 0xA8 */ u8 unkA8;
    /* 0xA9 */ u8 : 8;
    /* 0xAA */ u16 : 16;
    /* 0xAC */ s32 : 32;
    /* 0xB0 */ u16 unkB0[2];
    /* 0xB4 */ u16 unkB4[2];
} ET_Galamoth;
#define GALAMOTH(entity) ((ET_Galamoth*)&(entity)->ext)

#ifdef VERSION_PC
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, prim);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, timer);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, unk84);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, unk8C);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, unk8D);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, unk8E);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, rotate);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, length);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, parent);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, unkA4);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, rotVel);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, unkA8);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, unkB0);
SYNC_FIELD(ET_Galamoth, ET_GurkhaHammer, unkB4);
#endif

// lightning bolts (func_us_801983EC, func_us_8019C7B8_from_rcen)
typedef struct {
    /* 0x7C */ struct Primitive* prim;
    /* 0x80 */ struct Primitive* current;
    /* 0x84 */ s16 angle;
    /* 0x86 */ s16 angleStep;
    /* 0x88 */ u8 unk88;
    /* 0x89 */ u8 : 8;
    /* 0x8A */ u16 : 16;
    /* 0x8C */ s16 segmentsBeforeAim;
    /* 0x8E */ s16 offset;
    /* 0x90 */ s32 : 32;
    /* 0x94 */ s32 : 32;
    /* 0x98 */ s32 : 32;
    /* 0x9C */ s32 : 32;
    /* 0xA0 */ struct Entity* parent;
} ET_GalamothBolt;
#define GALAMOTH_BOLT(entity) ((ET_GalamothBolt*)&(entity)->ext)

#endif // RBO8_H
