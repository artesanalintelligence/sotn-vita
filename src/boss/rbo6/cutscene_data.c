// SPDX-License-Identifier: AGPL-3.0-or-later
// On PSX these scripts are part of the overlay raw data
#include "rbo6.h"
#include <cutscene.h>

// Alucard meets Dracula (father) and the epilogue with Lisa (mother); the
// scripts and their events are contiguous, as SET_EVENTS refers to them by
// their PSX address
u8 cutscene_script[] = {
#include "gen/cutscene_script_father.h"
#include "gen/cutscene_script_mother.h"
#include "gen/cutscene_events_father.h"
#include "gen/cutscene_events_mother.h"
};

STATIC_ASSERT(sizeof(cutscene_script) == 0x80181D32 - 0x80181720,
              "cutscene script size mismatch");
