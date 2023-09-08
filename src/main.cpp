#include "main.h"
#include "rehlds_api_provider.h"
#include "EventManager.h"
#include "api/NextClientApi.h"
#include "natives/natives.h"

std::unique_ptr<NextClientApi> g_NextClientApi;

INextClientAPI* NAPI()
{
    return g_NextClientApi.get();
}

void ServerActivate_Post(edict_t* pEdictList, int edictCount, int clientMax)
{
    if (g_NextClientApi)
        g_NextClientApi->OnServerActivated(pEdictList, edictCount, clientMax);

    SET_META_RESULT(MRES_IGNORED);
}

void PlayerPostThink(edict_t* pEntity)
{
    if (g_NextClientApi)
        g_NextClientApi->OnPlayerPostThink(ENTINDEX(pEntity));

    SET_META_RESULT(MRES_IGNORED);
}

void ClientPutInServer_Post(edict_t *pEntity)
{
   if (g_NextClientApi)
        g_NextClientApi->OnClientPutInServer(pEntity);

    SET_META_RESULT(MRES_IGNORED);
}

BOOL ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
    if (g_NextClientApi)
        g_NextClientApi->OnClientConnect(ENTINDEX(pEntity));

    RETURN_META_VALUE(MRES_IGNORED, true);
}

void ClientDisconnect(edict_t *pEntity)
{
    if (g_NextClientApi)
        g_NextClientApi->OnClientDisconnect(ENTINDEX(pEntity));

    SET_META_RESULT(MRES_IGNORED);
}

void MessageBegin_Post(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
    if (g_NextClientApi)
        g_NextClientApi->OnMessageBeginPost(msg_dest, msg_type, pOrigin, ed);

    SET_META_RESULT(MRES_IGNORED);
}

void MessageEnd_Post()
{
    if (g_NextClientApi)
        g_NextClientApi->OnMessageEndPost();

    SET_META_RESULT(MRES_IGNORED);
}

#include <iostream>

void SV_HandleClientMessage(IRehldsHook_HandleNetCommand* hookchain, IGameClient* apiClient, int8 opcode)
{
    if (opcode == clc_ncl_message)
    {
        auto netMessage = g_RehldsFuncs->GetNetMessage();
        int* readcount = g_RehldsFuncs->GetMsgReadCount();

        if (*(uint32_t*) (netMessage->data + *readcount) == NCLM_C2S_HEADER)
        {
            *readcount += 4;
            auto nclm_opcode = static_cast<NCLM_C2S>(MSG_ReadByte());
            std::cout << "got nclm message, opcode " << nclm_opcode << std::endl;

            if (g_NextClientApi)
                g_NextClientApi->OnHandleNCLMessage(apiClient->GetEdict(), nclm_opcode);
                
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

    g_NextClientApi = std::make_unique<NextClientApi>();
    g_RehldsHookchains->HandleNetCommand()->registerHook(SV_HandleClientMessage);

    AddNatives_All();

    MF_PrintSrvConsole("[%s] Successfully loaded, version %s\n", MODULE_NAME, MODULE_VERSION);
}

void OnAmxxDetach()
{
    g_NextClientApi = nullptr;
    g_RehldsHookchains->HandleNetCommand()->unregisterHook(SV_HandleClientMessage);
}