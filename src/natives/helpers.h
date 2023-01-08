#pragma once

#include "main.h"

#define PARAMS_COUNT (params[0] / sizeof(cell))
#define ASSERT_ARG_IS_PLAYER(x)                                                                            \
	if (params[x] <= 0 || params[x] > gpGlobals->maxClients) {                                             \
		MF_LogError(amx, AMX_ERR_NATIVE, "%s: invalid player index %i [%s]", __FUNCTION__, params[x], #x); \
		return FALSE;                                                                                      \
	}                                                                                                      \
	if (utils::PlayerByIndex(params[x]) == NULL) {                                                         \
		MF_LogError(amx, AMX_ERR_NATIVE, "%s: disconnected player %i [%s]", __FUNCTION__, params[x], #x);  \
		return FALSE;                                                                                      \
	}

#define ASSERT_NO_NAPI_ERRORS()                                                         \
	std::string _errorString;                                                           \
	if (_NAPIController()->ReadAndClearError(_errorString)) {                           \
		MF_LogError(amx, AMX_ERR_NATIVE, "%s: %s", __FUNCTION__, _errorString.c_str()); \
		return FALSE;                                                                   \
	}