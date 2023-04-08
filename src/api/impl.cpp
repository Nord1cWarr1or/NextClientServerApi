#include "impl.h"

CNextClientApi::CNextClientApi()
{
    cvar_sandbox_ = std::make_unique<CCvarSandbox>();
    private_precache_ = std::make_unique<CPrivatePrecache>();
    viewmodel_fx_ = std::make_unique<CViewmodelFX>();
    verificator_ = std::make_unique<CVerificator>();
}

void CNextClientApi::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax)
{
    for (int i = 0; i <= clientMax; i++)
        players_[i] = PlayerData();

    if (!forward_api_ready_)
        forward_api_ready_ = MF_RegisterForward("ncl_client_api_ready", ET_IGNORE, FP_CELL, FP_DONE);

    cvar_sandbox_->OnServerActivated(pEdictList, edictCount, clientMax);
    viewmodel_fx_->OnServerActivated(pEdictList, edictCount, clientMax);
    verificator_->OnServerActivated(pEdictList, edictCount, clientMax);

    verificator_->ParsePublicKeys();

    message_SetFOVEx_ = utils::RegUserMsgSafe("SetFOVEx", -1);
}

IViewmodelFX* CNextClientApi::ViewmodelFX()
{
    return viewmodel_fx_.get();
}

IPrivatePrecache* CNextClientApi::PrivatePrecache()
{
    return private_precache_.get();
}

ICvarSandbox* CNextClientApi::CvarSandbox()
{
    return cvar_sandbox_.get();
}

bool CNextClientApi::ClientIsReady(int client)
{
    if (players_.count(client) == 0)
        return false;
    return players_[client].is_api_ready;
}

NextClientVersion CNextClientApi::GetNextClientVersion(int client)
{
    if (players_.count(client) == 0)
        return NextClientVersion::NOT_NEXTCLIENT;
    return players_[client].client_version;
}

void CNextClientApi::OnPlayerPostThink(int client)
{
    if (players_.count(client) == 0)
        return;

    auto data = &players_[client];
    if (!data->is_api_ready && data->client_version != NextClientVersion::NOT_NEXTCLIENT)
    {
        data->is_api_ready = true;

        MF_ExecuteForward(forward_api_ready_, client);
    }
}

void CNextClientApi::OnClientConnect(int client)
{
    if (players_.count(client) == 0)
        return;

    auto data = &players_[client];
    data->client_version = NextClientVersion::NOT_NEXTCLIENT;
    data->is_api_ready = false;
    auto value = INFOKEY_VALUE(GET_INFOKEYBUFFER(INDEXENT(client)), "_ncl");

    if (value[0] == '1')
    {
        if (value[1] == '8')
            data->client_version = NextClientVersion::V_2_1_8;
        else if (value[1] == '9')
            data->client_version = NextClientVersion::V_2_1_9;
        else
            data->client_version = NextClientVersion::V_2_1_7_OR_LOWER;
    }

    private_precache_->OnClientConnect(client);
    verificator_->OnClientConnect(client);
}

void CNextClientApi::OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode)
{
    auto verificator = verificator_.get();

    verificator->HandleNCLMVerificationRequest(client);

    switch (opcode)
    {
        case VERIFICATION_REQUEST:
            verificator->HandleNCLMVerificationRequest(client);
            break;

        case VERIFICATION_RESPONSE:
            verificator->HandleNCLMVerificationResponse(client);
            break;
    }
}

void CNextClientApi::ClientSetFOV(int client, int fov, float lerpTime)
{
    MESSAGE_BEGIN(MSG_ONE, message_SetFOVEx_, NULL, INDEXENT(client));
    WRITE_BYTE(fov & 0xFF);
    WRITE_LONG(lerpTime);
    MESSAGE_END();
}

void CNextClientApi::SetError(const std::string& text)
{
    message_error_ = text;
}

bool CNextClientApi::ReadAndClearError(std::string& text)
{
    if (message_error_.length() == 0)
        return false;

    text = message_error_;
    message_error_.clear();

    return true;
}

CNextClientApi* implNextClientApi = nullptr;

void NAPI_Install()
{
    implNextClientApi = new CNextClientApi;
}

INextClientAPI* NAPI()
{
    return implNextClientApi;
}

CNextClientApi* NAPIController()
{
    return implNextClientApi;
}