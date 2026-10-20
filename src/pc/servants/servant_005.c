// SPDX-License-Identifier: AGPL-3.0-or-later
#include <psyz/module.h>
#include "../../servant/tt_002/faerie.h"
#include "../../pc/stages/overlay.h"
#include <string.h>

// TT_005 shares the servant descriptor layout (and init.c) with TT_002
extern ServantDesc faerie_ServantDesc;
void Psyz_ModuleStart(void* param) {
    ServantDesc* o = param;
    memcpy(o, &faerie_ServantDesc, sizeof(ServantDesc));
}

void Psyz_ModuleStop(void) {}
