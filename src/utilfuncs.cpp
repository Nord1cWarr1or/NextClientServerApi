#pragma once

#include "utilfuncs.h"
#include <fstream>
#include <vector>
#include <string>

namespace utils {
CBasePlayer* PlayerByIndex(int playerIndex) {
	return (CBasePlayer*)GET_PRIVATE(INDEXENT(playerIndex));
}

int RegUserMsgSafe(const char* pszName, int iSize) {
	int messageid = GET_USER_MSG_ID(&Plugin_info, pszName, NULL);
	if (messageid == NULL)
		messageid = REG_USER_MSG(pszName, -1);
	if (messageid == NULL)
		MF_PrintSrvConsole("[%s] Unable to register %s message", MODULE_NAME, pszName);

	return messageid;
}

using std::ifstream;

size_t FileSize(std::string path) {
	ifstream file(path, ifstream::binary);
	file.seekg(0, ifstream::end);
	return file.tellg();
}

bool CRC_File(std::string path, CRC32_t* crc) {
	ifstream file(path, ifstream::binary);
	std::vector<char> buffer(1024);

	while (file.good()) {
		/*
		file.read(buffer.data(), buffer.size());
		CRC32_PROCESS_BUFFER(crc, buffer.data(), file.gcount());
		*/
		CRC32_PROCESS_BYTE(crc, file.get());
	}

	return !file.fail();
}
}
