#include "main.h"

void ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
    NAPIController()->OnServerActivated(pEdictList, edictCount, clientMax);
    SET_META_RESULT(MRES_IGNORED);
}

void PlayerPostThink(edict_t* pEntity)
{
    NAPIController()->OnPlayerPostThink(ENTINDEX(pEntity));
    SET_META_RESULT(MRES_IGNORED);
}

BOOL ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
    NAPIController()->OnClientConnect(ENTINDEX(pEntity));
    RETURN_META_VALUE(MRES_IGNORED, true);
}

void SV_HandleClientMessage(IRehldsHook_HandleNetCommand* hookchain, IGameClient* apiClient, int8 opcode)
{
    if (opcode == clc_stringcmd)
    {
        auto netMessage = g_RehldsFuncs->GetNetMessage();
        int* readcount = g_RehldsFuncs->GetMsgReadCount();

        if (*(uint32_t*) (netMessage->data + *readcount) == NCLM_C2S_HEADER)
        {
            auto nclm_opcode = static_cast<NCLM_C2S>(MSG_ReadByte());
            NAPIController()->OnHandleNCLMessage(apiClient->GetEdict(), nclm_opcode);
            *readcount = netMessage->cursize;
            return;
        }
    }

    hookchain->callNext(apiClient, opcode);
}

void OnAmxxAttach()
{
    if (!RehldsApi_Init())
        return;

    NAPI_Install();
    AddNatives_All();

    g_RehldsHookchains->HandleNetCommand()->registerHook(SV_HandleClientMessage);

    MF_PrintSrvConsole("[%s] Successfully loaded, version %s\n", MODULE_NAME, MODULE_VERSION);
}

void OnAmxxDetach()
{
    g_RehldsHookchains->HandleNetCommand()->unregisterHook(SV_HandleClientMessage);
}