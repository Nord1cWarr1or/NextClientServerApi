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
    forward_api_ready_ = MF_RegisterForward("ncl_client_api_ready", ET_IGNORE, FP_CELL, FP_DONE);

    cvar_sandbox_->OnServerActivated(pEdictList, edictCount, clientMax);
    viewmodel_fx_->OnServerActivated(pEdictList, edictCount, clientMax);
    verificator_->OnServerActivated(pEdictList, edictCount, clientMax);

    verificator_->ParsePublicKeys();

    message_SetFOVEx_ = utils::RegUserMsgSafe("SetFOVEx", -1);
    message_HudSprite_ = utils::RegUserMsgSafe("HudSprite", -1);
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
    PlayerData data{};
    data.client_version = NextClientVersion::NOT_NEXTCLIENT;
    data.is_api_ready = false;

    std::string value = INFOKEY_VALUE(GET_INFOKEYBUFFER(INDEXENT(client)), "_ncl");

    if (!value.empty())
    {
        if (value == "20")
            data.client_version = NextClientVersion::V_2_2_0;
        else if (value == "18")
            data.client_version = NextClientVersion::V_2_1_8;
        else if (value == "19")
            data.client_version = NextClientVersion::V_2_1_9;
        else if (value == "110")
            data.client_version = NextClientVersion::V_2_1_10;
        else if (value == "111")
            data.client_version = NextClientVersion::V_2_1_11;
        else if (value == "112")
            data.client_version = NextClientVersion::V_2_1_12;
        else if (value[0] == '1')
            data.client_version = NextClientVersion::V_2_1_7_OR_LOWER;
    }

    players_[client] = data;

    private_precache_->OnClientConnect(client);
    verificator_->OnClientConnect(client);
}

void CNextClientApi::OnClientDisconnect(int index)
{
    players_.erase(index);
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

void CNextClientApi::SendHudSprite(
    int client,
    int channel,
    const char* spritePath,
    const byte spriteColor[3],
    byte alpha,
    int frame,
    float frameRate,
    float inTime,
    float holdTime,
    float outTime,
    float x,
    float y,
    const int spriteRect[4],
    float scaleX,
    float scaleY,
    int renderMode
)
{
    MESSAGE_BEGIN(MSG_ONE, message_HudSprite_, nullptr, INDEXENT(client));
    WRITE_BYTE(channel);
    WRITE_STRING(spritePath);
    WRITE_BYTE(0);
    WRITE_BYTE(spriteColor[0]);
    WRITE_BYTE(spriteColor[1]);
    WRITE_BYTE(spriteColor[2]);
    WRITE_BYTE(alpha);
    WRITE_SHORT(frame);
    WRITE_LONG(frameRate);
    WRITE_LONG(inTime);
    WRITE_LONG(holdTime);
    WRITE_LONG(outTime);
    WRITE_LONG(x);
    WRITE_LONG(y);
    WRITE_SHORT(spriteRect[0]);
    WRITE_SHORT(spriteRect[1]);
    WRITE_SHORT(spriteRect[2]);
    WRITE_SHORT(spriteRect[3]);
    WRITE_LONG(scaleX);
    WRITE_LONG(scaleY);
    WRITE_BYTE(renderMode);
    MESSAGE_END();
}

void CNextClientApi::SendHudSpriteFullScreen(
    int client,
    int channel,
    const char* spritePath,
    const byte spriteColor[3],
    byte alpha,
    int frame,
    float frameRate,
    float inTime,
    float holdTime,
    float outTime,
    int renderMode
)
{
    MESSAGE_BEGIN(MSG_ONE, message_HudSprite_, nullptr, INDEXENT(client));
    WRITE_BYTE(channel);
    WRITE_STRING(spritePath);
    WRITE_BYTE(1);
    WRITE_BYTE(spriteColor[0]);
    WRITE_BYTE(spriteColor[1]);
    WRITE_BYTE(spriteColor[2]);
    WRITE_BYTE(alpha);
    WRITE_SHORT(frame);
    WRITE_LONG(frameRate);
    WRITE_LONG(inTime);
    WRITE_LONG(holdTime);
    WRITE_LONG(outTime);
    WRITE_BYTE(renderMode);
    MESSAGE_END();
}

void CNextClientApi::ClearHudSprite(int client, int channel)
{
    MESSAGE_BEGIN(MSG_ONE, message_HudSprite_, nullptr, INDEXENT(client));
    WRITE_BYTE(channel);
    WRITE_STRING("");
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
