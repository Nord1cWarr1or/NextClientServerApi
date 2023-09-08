#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <events.h>
#include <EventManager.h>

#include "CvarSandbox.h"
#include "PrivatePrecache.h"
#include "Verificator.h"
#include "ViewmodelFX.h"
#include "HealthNext.h"

class NextClientApi : public INextClientAPI
{
    struct PlayerData {
        NextClientVersion client_version;
        bool is_api_ready;
    };

	std::unique_ptr<EventManager> event_manager_;

    int forward_api_ready_{};
	int message_SetFOVEx_{};
    int message_HudSprite_{};

	std::unordered_map<int, PlayerData> players_;

	std::shared_ptr<::CvarSandbox> cvar_sandbox_;
	std::shared_ptr<::PrivatePrecache> private_precache_;
	std::shared_ptr<::ViewmodelFX> viewmodel_fx_;
	std::shared_ptr<Verificator> verificator_;
    std::shared_ptr<HealthNext> health_next_;

public:
    explicit NextClientApi();

	IViewmodelFX* ViewmodelFX() override;
	IPrivatePrecache* PrivatePrecache() override;
	ICvarSandbox* CvarSandbox() override;

	bool ClientIsReady(int client) override;
	void ClientSetFOV(int client, int fov, float lerpTime) override;
	NextClientVersion GetNextClientVersion(int client) override;

    void SendHudSprite(
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
    ) override;

    void SendHudSpriteFullScreen(
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
    ) override;

    void ClearHudSprite(int client, int channel) override;

    // events
	void OnPlayerPostThink(int client);
	void OnClientDisconnect(int client);
	void OnClientConnect(int client);
    void OnClientPutInServer(edict_t* pEntity);
	void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax);
	void OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode);
    void OnMessageBeginPost(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
    void OnMessageEndPost();
};
