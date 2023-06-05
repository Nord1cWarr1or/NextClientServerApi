#pragma once

#include "events.h"
#include "main.h"

#include "cvar_sandbox.h"
#include "private_precache.h"
#include "verificator.h"
#include "viewmodelfx.h"

#include <memory>
#include <string>
#include <unordered_map>

#define THROW_NAPI_ERROR(message) NAPIController()->SetError(message)
#define THROW_NAPI_ERROR_AND_RETURN(message) \
    {                                        \
        THROW_NAPI_ERROR(message);           \
        return;                              \
    }

#define NAPI_ASSERT(predicate, message)       \
	if (!(predicate)) {                       \
		NAPIController()->SetError(message); \
		return;                               \
	}

#define NAPI_LOG_ASSERT(predicate, ...) \
    if (!(predicate)) {                 \
        MF_Log(__VA_ARGS__);            \
        return;                         \
    }

class CCvarSandbox;

class CPrivatePrecache;

class CViewmodelFX;

class CVerificator;

class CNextClientApi : public INextClientAPI,
					   public IEventPlayerPostThink,
					   public IEventClientConnect,
					   public IEventServerActivated,
					   public IEventHandleNCLMessage {
    struct PlayerData {
        NextClientVersion client_version;
        bool is_api_ready;
    };

	int forward_api_ready_{};
	int message_SetFOVEx_{};
    int message_HudSprite_{};

	std::unordered_map<int, PlayerData> players_;

	std::unique_ptr<CCvarSandbox> cvar_sandbox_;
	std::unique_ptr<CPrivatePrecache> private_precache_;
	std::unique_ptr<CViewmodelFX> viewmodel_fx_;
	std::unique_ptr<CVerificator> verificator_;

	std::string message_error_;

public:
    CNextClientApi();

	IViewmodelFX* ViewmodelFX() override;
	IPrivatePrecache* PrivatePrecache() override;
	ICvarSandbox* CvarSandbox() override;

	bool ClientIsReady(int client) override;
	NextClientVersion GetNextClientVersion(int client) override;
	void ClientSetFOV(int client, int fov, float lerpTime) override;

    void SendHudSprite(
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
    ) override;

    void SendHudSpriteFullScreen(
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
    ) override;

    void ClearHudSprite(int client, int channel) override;

	void OnPlayerPostThink(int client) override;
	void OnClientConnect(int client) override;
	void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) override;
	void OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode) override;

	void SetError(const std::string& text);
	bool ReadAndClearError(std::string& text);

    void OnClientDisconnect(int index);
};

void NAPI_Install();
INextClientAPI* NAPI();
CNextClientApi* NAPIController();
