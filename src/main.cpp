#include "main.h"

void ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) {
	_NAPIController()->OnServerActivated(pEdictList, edictCount, clientMax);
}

void PlayerPostThink(edict_t* pEntity) {
	_NAPIController()->OnPlayerPostThink(ENTINDEX(pEntity));
}

BOOL ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]) {
	_NAPIController()->OnClientConnect(ENTINDEX(pEntity));

	return true;
}

void SV_HandleClientMessage(IRehldsHook_HandleNetCommand* hookchain, IGameClient* apiClient, int8 opcode) {
	if (opcode == clc_stringcmd) {
		auto netMessage = g_RehldsFuncs->GetNetMessage();
		int* readcount = g_RehldsFuncs->GetMsgReadCount();

		if (*(uint32_t*)(netMessage->data + *readcount) == NCLM_C2S_HEADER) {
			auto opcode = static_cast<NCLM_C2S>(MSG_ReadByte());
			_NAPIController()->OnHandleNCLMessage(apiClient->GetEdict(), opcode);
			*readcount = netMessage->cursize;
			return;
		}
	}

	hookchain->callNext(apiClient, opcode);
}

void OnAmxxAttach(void) {
	MF_PrintSrvConsole("[%s] Successfully loaded, version %s\n", MODULE_NAME, MODULE_VERSION);

	RehldsApi_Init();
	NAPI_Install();
	AddNatives_All();

	g_RehldsHookchains->HandleNetCommand()->registerHook(SV_HandleClientMessage);
}

void OnAmxxDetach(void) {
	g_RehldsHookchains->HandleNetCommand()->unregisterHook(SV_HandleClientMessage);
}