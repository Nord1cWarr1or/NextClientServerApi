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

#define THROW_NAPI_ERROR(message) _NAPIController()->SetError(message)
#define THROW_NAPI_ERROR_AND_RETURN(message) \
	{                                        \
		THROW_NAPI_ERROR(message);           \
		return;                              \
	}

#define NAPI_ASSERT(predicate, message)       \
	if (!(predicate)) {                       \
		_NAPIController()->SetError(message); \
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
	int forwardApiReady {};
	int messageSetFOVEx {};

	struct PlayerData {
		NextClientVersion clientVersion;
		bool isApiReady;

		PlayerData()
			: clientVersion(NOT_NEXTCLIENT)
			, isApiReady(false) {};
	};

	std::unordered_map<int, PlayerData> playerData;

	std::unique_ptr<CCvarSandbox> apiCvarSandbox;
	std::unique_ptr<CPrivatePrecache> apiPrivatePrecache;
	std::unique_ptr<CViewmodelFX> apiViewmodelFX;
	std::unique_ptr<CVerificator> apiVerificator;

	std::string messageError = "";

public:
	CNextClientApi();

	IViewmodelFX* ViewmodelFX();
	IPrivatePrecache* PrivatePrecache();
	ICvarSandbox* CvarSandbox();

	bool ClientIsReady(int client);
	NextClientVersion GetNextClientVersion(int client);
	void ClientSetFOV(int client, int fov, float lerpTime);

	void OnPlayerPostThink(int client);
	void OnClientConnect(int client);
	void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax);
	void OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode);

	void SetError(std::string text);
	bool ReadAndClearError(std::string& text);
};

void NAPI_Install();
INextClientAPI* NAPI();
CNextClientApi* _NAPIController();