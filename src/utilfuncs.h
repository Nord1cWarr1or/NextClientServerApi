#pragma once

#include "main.h"
#include <funchook.h>

namespace utils
{
	CBasePlayer *PlayerByIndex(int playerIndex);
	int RegUserMsgSafe(const char *pszName, int iSize);

	size_t FileSize(const std::string& path);
	bool CRC_File(const std::string& path, CRC32_t* crc);

    funchook_t* CreateAndEnableHook(void* target_func, void* hook_func, void** trampoline_func);
}
