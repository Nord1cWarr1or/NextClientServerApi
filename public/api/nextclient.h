#pragma once

#include "const.h"
#include "cvar_sandbox.h"
#include "private_precache.h"
#include "viewmodelfx.h"

enum class NextClientVersion {
	NOT_NEXTCLIENT   = 0,
    V_2_1_7_OR_LOWER = 1,
    V_2_1_8          = 2,
    V_2_1_9          = 3
};

class INextClientAPI {
public:
	virtual ~INextClientAPI() = default;

	virtual IViewmodelFX* ViewmodelFX() = 0;
	virtual IPrivatePrecache* PrivatePrecache() = 0;
	virtual ICvarSandbox* CvarSandbox() = 0;

	virtual NextClientVersion GetNextClientVersion(int client) = 0;
	virtual bool ClientIsReady(int client) = 0;

	virtual void ClientSetFOV(int client, int fov, float lerpTime) = 0;
};