// SPDX-License-Identifier: AGPL-3.0-or-later
#ifndef BO7_H
#define BO7_H

#include <stage.h>

#define STAGE_IS_BO7

enum Palettes {
    PAL_NONE,
    PAL_BREAKABLE_DEBRIS = 0x159,
    PAL_CERBEROS = 0x200,
    PAL_UNK_2E0 = 0x2E0,
};

typedef enum EntityID {
    E_NONE,
    E_BREAKABLE,             // EntityBreakable
    E_EXPLOSION,             // EntityExplosion
    E_PRIZE_DROP,            // EntityPrizeDrop
    E_DAMAGE_DISPLAY,        // EntityDamageDisplay
    E_RED_DOOR,              // EntityRedDoor
    E_INTENSE_EXPLOSION,     // EntityIntenseExplosion
    E_SOUL_STEAL_ORB,        // EntitySoulStealOrb
    E_ROOM_FOREGROUND,       // EntityRoomForeground
    E_STAGE_NAME_POPUP,      // EntityStageNamePopup
    E_EQUIP_ITEM_DROP,       // EntityEquipItemDrop
    E_RELIC_ORB,             // EntityRelicOrb
    E_PERSISTENT_ITEM_DROP,  // EntityPersistentItemDrop
    E_ENEMY_BLOOD,           // EntityEnemyBlood
    E_MESSAGE_BOX,           // EntityMessageBox
    E_DUMMY_F,               // EntityDummy
    E_DUMMY_10,              // EntityDummy
    E_BACKGROUND_BLOCK,      // EntityBackgroundBlock
    E_LOCK_CAMERA,           // EntityLockCamera
    E_UNK_ID13,              // EntityUnkId13
    E_EXPLOSION_VARIANTS,    // EntityExplosionVariants
    E_GREY_PUFF,             // EntityGreyPuff
    E_DEMON_SWITCH_WALL,     // EntityDemonSwitchWall
    E_DEMON_SWITCH,          // EntityDemonSwitch
    E_BREAKABLE_WALL,        // EntityBreakableWall
    E_BREAKABLE_WALL_DEBRIS, // EntityBreakableWallDebris
    E_BREAKABLE_DEBRIS,      // EntityBreakableDebris
    E_CERBERUS_GATE_DEBUG,   // EntityCerberusGateDebug
    E_UNK_1C,                // func_us_80194D3C
    E_UNK_1D,                // func_us_801959E0
    E_UNK_1E,                // func_us_80195AF0
    E_UNK_1F,                // func_us_80195C50
    E_UNK_20,                // func_us_801963D8
    E_LIFE_UP_SPAWN,         // EntityLifeUpSpawn
    NUM_ENTITIES,
};

Primitive* FindFirstUnkPrim(Primitive* prim);

#define g_EInitSecret D_us_80180454

// D_us_80180848: progress of the Cerberus fight, shared by its entities
extern s32 g_CerberusMgrState;
enum CerberusMgrState {
    CERBERUS_MGR_PLAYER_IN = 1,   // player entered the arena, gates drop
    CERBERUS_MGR_FIGHT_ON = 2,    // boss music started, Cerberus is active
    CERBERUS_MGR_DEAD = 4,        // Cerberus was defeated
    CERBERUS_MGR_DEATH_DONE = 8,  // death animation finished
    CERBERUS_MGR_REWARD = 0x10,   // life up spawned, gates rise
};

#endif // BO7_H
