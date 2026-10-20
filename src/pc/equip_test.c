// SPDX-License-Identifier: AGPL-3.0-or-later
// Headless test harness for the weapon and familiar overlays. It is only
// built when configuring with -DPC_EQUIP_TEST=ON and is driven by:
//   SOTN_TEST_WEAPON=<id>   equip both hands with the first item using the
//                           weapon overlay <id> (loads w0_<id> and w1_<id>)
//   SOTN_TEST_SERVANT=<id>  activate the relic that summons familiar <id>
//   SOTN_TEST_FRAMES=<n>    in-game frames to run before quitting
//   SOTN_TEST_TRACE         when set, log the player state every 5 frames
// While in game it alternates attacks with both hands and some movement.
#ifdef PC_EQUIP_TEST
#include <game.h>
#include <psyz/input.h>
#include <psyz/log.h>
#include <psyz/video.h>
#include "pc.h"
#include "dra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern bool g_IsQuitRequested;

static int EnvInt(const char* name, int def) {
    const char* env = getenv(name);
    return env ? atoi(env) : def;
}

static void ApplyEquipment(int weapon, int servant) {
    s32 i;

    if (weapon >= 0) {
        for (i = 0; i < NUM_HAND_ITEMS; i++) {
            if (g_EquipDefs[i].weaponId == weapon) {
                break;
            }
        }
        if (i == NUM_HAND_ITEMS && g_EquipDefs[0xD8].weaponId == weapon) {
            // only reachable through the Axe Lord armor
            g_Status.equipment[ARMOR_SLOT] = ITEM_AXE_LORD_ARMOR;
            INFOF("equip test: Axe Lord armor uses weapon overlay %d", weapon);
        } else if (i == NUM_HAND_ITEMS) {
            ERRORF("no hand item uses weapon overlay %d", weapon);
        } else {
            g_Status.equipment[LEFT_HAND_SLOT] = i;
            g_Status.equipment[RIGHT_HAND_SLOT] = i;
            INFOF("equip test: item %d uses weapon overlay %d", i, weapon);
        }
    }
    if (servant > 0) {
        for (i = 0; i < LEN(g_Status.relics); i++) {
            if (g_RelicDefs[i].unk0C == servant) {
                g_Status.relics[i] = RELIC_FLAG_FOUND | RELIC_FLAG_ACTIVE;
                INFOF("equip test: relic %d summons familiar %d", i, servant);
                break;
            }
        }
    }
}

void EquipTest_OnFrame(void) {
    static int weapon = -2;
    static int servant;
    static int maxFrames;
    static bool applied;
    static unsigned playFrames;
    static u8 seen[256];
    char frame[PSYZ_PAD_BUF_LEN];
    u32 pressed;
    s32 i;

    if (weapon == -2) {
        weapon = EnvInt("SOTN_TEST_WEAPON", -1);
        servant = EnvInt("SOTN_TEST_SERVANT", 0);
        maxFrames = EnvInt("SOTN_TEST_FRAMES", 900);
        if (weapon >= 0 || servant > 0) {
            Psyz_VideoSetVsyncMode(PSYZ_VSYNC_LIMITLESS);
        }
    }
    if (weapon < 0 && servant <= 0) {
        return;
    }

    // equipment is reset by InitStatsAndGear right before Play_Unk11, where
    // the weapon overlays get loaded
    if (!applied && g_GameState == Game_NowLoading &&
        g_GameStep == Play_Unk11) {
        ApplyEquipment(weapon, servant);
        applied = true;
    }
    if (!applied || g_GameState != Game_Play || g_GameStep != Play_Default) {
        return;
    }

    // track which entity kinds appear in the player-owned entity slots, so
    // the log shows whether the weapon entities were actually spawned
    for (i = 1; i < 64; i++) {
        seen[g_Entities[i].entityId] = 1;
    }

    if (getenv("SOTN_TEST_TRACE") && !(playFrames % 5)) {
        INFOF("trace f=%u step=%d/%d anim=%d pressed=%04X unk46=%d cs=%d "
              "pos=%d,%d",
              playFrames, PLAYER.step, PLAYER.step_s, PLAYER.ext.player.anim,
              g_pads[0].pressed, g_Player.unk46, g_CutsceneHasControl,
              PLAYER.posX.i.hi, PLAYER.posY.i.hi);
    }

    playFrames++;
    switch ((playFrames / 10) % 12) {
    case 1:
        pressed = PAD_SQUARE;
        break;
    case 3:
        pressed = PAD_CIRCLE;
        break;
    case 5:
        pressed = PAD_RIGHT | PAD_SQUARE;
        break;
    case 7:
        pressed = PAD_LEFT | PAD_CIRCLE;
        break;
    case 9:
        pressed = PAD_UP | PAD_SQUARE;
        break;
    case 10:
        pressed = PAD_CROSS | PAD_CIRCLE;
        break;
    default:
        pressed = 0;
        break;
    }
    memset(frame, 0, sizeof(frame));
    frame[1] = (char)PSYZ_CTRL_DIGITAL_PAD;
    frame[2] = (char)(~(pressed >> 8) & 0xFF);
    frame[3] = (char)(~pressed & 0xFF);
    Psyz_PadsSet(0, frame, sizeof(frame));

    if (playFrames >= (unsigned)maxFrames) {
        char ids[256 * 3 + 1];
        int len = 0;
        for (i = 1; i < 256; i++) {
            if (seen[i]) {
                len += snprintf(ids + len, sizeof(ids) - len, " %02X", i);
            }
        }
        INFOF("equip test: finished after %u frames, hp=%d/%d, servant=%d",
              playFrames, g_Status.hp, g_Status.hpMax, g_Servant);
        INFOF("equip test: entity ids seen:%s", ids);
        g_IsQuitRequested = true;
    }
}
#endif
