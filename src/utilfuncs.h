#pragma once

#include "main.h"

namespace utils {
	CBasePlayer *PlayerByIndex(int playerIndex);
	int RegUserMsgSafe(const char *pszName, int iSize);

	size_t FileSize(std::string path);
	bool CRC_File(std::string path, CRC32_t* crc);
}
