#pragma once

#include "main.h"

class CNextClientApi: public INextClientAPI {
public:
	CNextClientApi();

	IViewmodelFX* ViewmodelFX();
	IPrivatePrecache* PrivatePrecache();
	ICvarSandbox* CvarSandbox();

	bool ClientIsReady(int client);
	bool ClientIsNextClient(int client);
	void ClientSetFOV(int client, int fov, float lerpTime);
};

INextClientAPI* NAPI();