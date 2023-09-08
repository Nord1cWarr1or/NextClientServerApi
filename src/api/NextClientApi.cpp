#include "NextClientApi.h"
#include <utility>
#include "utilfuncs.h"

NextClientApi::NextClientApi()
{
    event_manager_ = std::make_unique<EventManager>();

    cvar_sandbox_ = std::make_shared<::CvarSandbox>();
    event_manager_->AddEventListener(cvar_sandbox_);

    private_precache_ = std::make_shared<::PrivatePrecache>();
    event_manager_->AddEventListener(private_precache_);

    viewmodel_fx_ = std::make_shared<::ViewmodelFX>();
    event_manager_->AddEventListener(viewmodel_fx_);

    health_next_ = std::make_shared<HealthNext>();
    event_manager_->AddEventListener(health_next_);

    nclm_protocol_ = std::make_shared<NclmProtocol>(event_manager_.get());

    verificator_ = std::make_shared<Verificator>(nclm_protocol_.get());
    event_manager_->AddEventListener(verificator_);
}

IViewmodelFX* NextClientApi::ViewmodelFX()
{
    return viewmodel_fx_.get();
}

IPrivatePrecache* NextClientApi::PrivatePrecache()
{
    return private_precache_.get();
}

ICvarSandbox* NextClientApi::CvarSandbox()
{
    return cvar_sandbox_.get();
}

bool NextClientApi::ClientIsReady(int client)
{
    if (players_.count(client) == 0)
        return false;

    return players_[client].is_api_ready;
}

NextClientVersion NextClientApi::GetNextClientVersion(int client)
{
    if (players_.count(client) == 0)
        return NextClientVersion::NOT_NEXTCLIENT;

    return players_[client].client_version;
}

void NextClientApi::ClientSetFOV(int client, int fov, float lerpTime)
{
    MESSAGE_BEGIN(MSG_ONE, message_SetFOVEx_, NULL, INDEXENT(client));
    WRITE_BYTE(fov & 0xFF);
    WRITE_LONG(lerpTime);
    MESSAGE_END();
}

void NextClientApi::SendHudSprite(
    int client,
    int channel,
    const char* spritePath,
    const uint8_t spriteColor[3],
    uint8_t alpha,
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

void NextClientApi::SendHudSpriteFullScreen(
    int client,
    int channel,
    const char* spritePath,
    const uint8_t spriteColor[3],
    uint8_t alpha,
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

void NextClientApi::ClearHudSprite(int client, int channel)
{
    MESSAGE_BEGIN(MSG_ONE, message_HudSprite_, nullptr, INDEXENT(client));
    WRITE_BYTE(channel);
    WRITE_STRING("");
    MESSAGE_END();
}

void NextClientApi::OnPlayerPostThink(int client)
{
    if (players_.count(client) == 0)
        return;

    auto data = &players_[client];
    if (!data->is_api_ready && data->client_version != NextClientVersion::NOT_NEXTCLIENT)
    {
        data->is_api_ready = true;

        MF_ExecuteForward(forward_api_ready_, client);
    }

    event_manager_->OnPlayerPostThink(client);
}

void NextClientApi::OnClientConnect(int client)
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

    event_manager_->OnClientConnect(client);
}

void NextClientApi::OnClientPutInServer(edict_t* pEntity) {
    event_manager_->OnClientPutInServer(pEntity);
}

void NextClientApi::OnClientDisconnect(int index)
{
    event_manager_->OnClientDisconnect(index);
    players_.erase(index);
}

void NextClientApi::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax)
{
    forward_api_ready_ = MF_RegisterForward("ncl_client_api_ready", ET_IGNORE, FP_CELL, FP_DONE);

    message_SetFOVEx_ = utils::RegUserMsgSafe("SetFOVEx", -1);
    message_HudSprite_ = utils::RegUserMsgSafe("HudSprite", -1);

    event_manager_->OnServerActivated(pEdictList, edictCount, clientMax);
}

void NextClientApi::OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode)
{
    nclm_protocol_->OnHandleNCLMessage(client, opcode);
}

void NextClientApi::OnMessageBeginPost(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed)
{
    event_manager_->OnMessageBeginPost(msg_dest, msg_type, pOrigin, ed);
}

void NextClientApi::OnMessageEndPost()
{
    event_manager_->OnMessageEndPost();
}
