// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo1.h"

typedef struct {
    s16 x;
    s16 y;
    s16 params;
} Rbo1TilePlacement;

static Rbo1TilePlacement tile_placements[] = {
    {0x408, 0x168, 1}, {0x408, 0x178, 1}, {0x408, 0x188, 1}, {0x408, 0x198, 1},
    {-8, 0x068, 0},    {-8, 0x078, 0},    {-8, 0x088, 0},    {-8, 0x098, 0},
    {-8, 0x168, 0},    {-8, 0x178, 0},    {-8, 0x188, 0},    {-8, 0x198, 0},
};

extern EInit g_EInitInteractable;
extern s32 g_BossMgrState;

void EntityBossFightManager(Entity* self) {
    Entity* entity;
    Rbo1TilePlacement* placement;
    s32 offsetX;
    s32 offsetY;
    s32 i;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        entity = &self[1];
        placement = tile_placements;
        i = 0;
        while (i < LEN(tile_placements)) {
            CreateEntityFromCurrentEntity(E_ID(BOSS_ROOM_BLOCK), entity);
            entity->params = placement->params;
            entity->posX.i.hi = placement->x - g_Tilemap.scrollX.i.hi;
            entity->posY.i.hi = placement->y - g_Tilemap.scrollY.i.hi;
            i++;
            entity++;
            placement++;
        }
        entity = &g_Entities[80];
        CreateEntityFromCurrentEntity(E_ID(UNK_16), entity);
        entity->posX.i.hi = 0x240 - g_Tilemap.scrollX.i.hi;
        entity->posY.i.hi = 0xD8 - g_Tilemap.scrollY.i.hi;
        // fallthrough

    case 1:
        offsetX = PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi;
        if (offsetX > 0x18 && offsetX < 0x3D8) {
            g_BossMgrState |= BOSS_MGR_DOORS_CLOSE;
            g_api.PlaySfx(SET_UNK_90);
            self->step++;
        }
        break;

    case 2:
        offsetX = PLAYER.posX.i.hi + g_Tilemap.scrollX.i.hi;
        if (offsetX > 0x140 && offsetX < 0x320) {
            g_BossMgrState |= BOSS_MGR_FIGHT_ON;
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_BEELZEBUB_DEFEAT, TIMEATTACK_SET_VISITED);
            self->step++;
        }
        break;

    case 3:
        if (g_api.func_80131F68() == false) {
            stopMusicFlag = false;
            currentMusicId = MU_DEATH_BALLAD;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        // fallthrough

    case 4:
        if (g_BossMgrState & BOSS_MGR_DEFEAT) {
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_BEELZEBUB_DEFEAT, TIMEATTACK_SET_RECORD);
            g_api.PlaySfx(SET_UNK_90);
            currentMusicId = MU_FINAL_TOCATTA;
            self->step++;
        }
        break;

    case 5:
        if (g_BossMgrState & BOSS_MGR_TERMINATED) {
            self->step++;
        }
        break;

    case 6:
        offsetX = 0x80;
        offsetY = 0x180 - g_Tilemap.scrollY.i.hi;
        entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_ID(LIFE_UP_SPAWN), self, entity);
            entity->posX.i.hi = offsetX;
            entity->posY.i.hi = offsetY;
            entity->params = 5;
            stopMusicFlag = true;
            currentMusicId = MU_FINAL_TOCATTA;
            g_BossMgrState |= BOSS_MGR_DOORS_OPEN;
            self->step++;
        }
        break;

    case 7:
        if (g_api.func_80131F68() == false) {
            stopMusicFlag = false;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        break;
    }
}

#ifdef VERSION_PC
extern EInit D_us_801804DC;

// blocks that move to close the boss room
void EntityBossRoomBlock(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804DC);
        self->animCurFrame = 8;
        // fallthrough

    case 1:
        if (g_BossMgrState & BOSS_MGR_DOORS_CLOSE) {
            self->ext.GS_Props.timer = 16;
            self->step++;
        }
        break;

    case 2:
        if (self->params) {
            self->velocityX = FIX(-1);
        } else {
            self->velocityX = FIX(1);
        }
        MoveEntity();
        GetPlayerCollisionWith(self, 8, 8, 5);
        if (!(g_Timer & 3)) {
            g_api.PlaySfx(SFX_STONE_MOVE_B);
        }
        if (--self->ext.GS_Props.timer) {
            break;
        }
        self->step++;
        break;

    case 3:
        GetPlayerCollisionWith(self, 8, 8, 5);
        if (g_BossMgrState & BOSS_MGR_DOORS_OPEN) {
            self->step++;
        }
        break;

    case 4:
        self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
        if (self->params) {
            self->velocityX = FIX(1);
        } else {
            self->velocityX = FIX(-1);
        }
        MoveEntity();
        break;
    }
}
#else
INCLUDE_ASM("boss/rbo1/nonmatchings/e_boss_room", EntityBossRoomBlock);
#endif
