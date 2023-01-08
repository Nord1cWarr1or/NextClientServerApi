#include "main.h"

void OnAmxxAttach(void) {
	MF_PrintSrvConsole("[%s] Successfully loaded, version %s\n", MODULE_NAME, MODULE_VERSION);

	NAPI_Install();
	AddNatives_All();
}

void ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) {
	_NAPIController()->OnServerActivated(pEdictList, edictCount, clientMax);
}

void PlayerPostThink(edict_t* pEntity) {
	_NAPIController()->OnPlayerPostThink(ENTINDEX(pEntity));
}

void ClientDisconnect(edict_t* pEntity) {
	_NAPIController()->OnClientDisconnect(ENTINDEX(pEntity));
}

BOOL ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]) {
	_NAPIController()->OnClientConnect(ENTINDEX(pEntity));

	return true;
}