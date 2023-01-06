#pragma once

#include "cvar_sandbox.h"
#include "private_precache.h"
#include "viewmodelfx.h"

class INextClientAPI {
public:
	~INextClientAPI() { }

	virtual IViewmodelFX* ViewmodelFX() = 0;
	virtual IPrivatePrecache* PrivatePrecache() = 0;
	virtual ICvarSandbox* CvarSandbox() = 0;

	virtual bool ClientIsReady(int client) = 0;
	virtual bool ClientIsNextClient(int client) = 0;

	virtual void ClientSetFOV(int client, int fov, float lerpTime) = 0;
};