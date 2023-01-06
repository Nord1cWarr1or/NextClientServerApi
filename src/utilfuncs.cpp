#pragma once

#include "utilfuncs.h"

namespace utils {
	CBasePlayer *UTIL_PlayerByIndex(int playerIndex) {
		return (CBasePlayer *)GET_PRIVATE(INDEXENT(playerIndex));
	}
}
