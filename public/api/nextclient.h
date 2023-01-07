#pragma once

#include "cvar_sandbox.h"
#include "private_precache.h"
#include "viewmodelfx.h"

enum NextClientVersion {
	NOT_NEXTCLIENT = 0,
    V_2_1_7_OR_LOWER,
    V_2_1_8
};

class INextClientAPI {
public:
	~INextClientAPI() { }

	virtual IViewmodelFX* ViewmodelFX() = 0;
	virtual IPrivatePrecache* PrivatePrecache() = 0;
	virtual ICvarSandbox* CvarSandbox() = 0;

	virtual NextClientVersion ClientIsNextClient(int client) = 0;
	virtual bool ClientIsReady(int client) = 0;

	virtual void ClientSetFOV(int client, int fov, float lerpTime) = 0;
};