#pragma once

#include "events.h"
#include "main.h"

#include "cvar_sandbox.h"
#include "private_precache.h"

#include <unordered_map>
#include <memory>

class CCvarSandbox;
class CPrivatePrecache;

class CNextClientApi : public INextClientAPI,
					   public IEventPlayerPostThink,
					   public IEventClientConnect,
					   public IEventClientDisconnect {
	int forwardApiReady;
	int messageSetFOVEx;

	struct PlayerData {
		NextClientVersion clientVersion;
		bool isApiReady;

		PlayerData()
			: clientVersion(NOT_NEXTCLIENT)
			, isApiReady(false) {};
	};

	std::unordered_map<int, PlayerData> playerData;
	
	std::auto_ptr<CCvarSandbox> apiCvarSandbox;
	std::auto_ptr<CPrivatePrecache> apiPrivatePrecache;
public:
	CNextClientApi();

	IViewmodelFX* ViewmodelFX();
	IPrivatePrecache* PrivatePrecache();
	ICvarSandbox* CvarSandbox();

	bool ClientIsReady(int client);
	NextClientVersion ClientIsNextClient(int client);
	void ClientSetFOV(int client, int fov, float lerpTime);

	void OnPlayerPostThink(int client);
	void OnClientDisconnect(int client);
	void OnClientConnect(int client);
};

void NAPI_Install();
INextClientAPI* NAPI();
CNextClientApi* _NAPIController();