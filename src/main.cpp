#include "main.h"

void OnAmxxAttach(void) {
	MF_PrintSrvConsole("[%s] Successfully loaded, version %s\n", MODULE_NAME, MODULE_VERSION);

	AddNatives_All();
}