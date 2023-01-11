#pragma once

#include "amxxmodule.h"
#include <extdll.h>
#include <eiface.h>
#include <model.h>
#include <pm_defs.h>
#include <rehlds_api.h>

extern IRehldsApi* g_RehldsApi;
extern const RehldsFuncs_t* g_RehldsFuncs;
extern IRehldsServerData* g_RehldsData;
extern IRehldsHookchains* g_RehldsHookchains;
extern IRehldsServerStatic* g_RehldsSvs;

extern bool RehldsApi_Init();