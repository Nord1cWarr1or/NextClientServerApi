#include "impl.h"

CNextClientApi::CNextClientApi() {
	this->messageSetFOVEx = utils::RegUserMsgSafe("SetFOVEx", -1);
	
	this->apiCvarSandbox.reset(new CCvarSandbox);
	this->apiPrivatePrecache.reset(new CPrivatePrecache);
	this->apiViewmodelFX.reset(new CViewmodelFX);
}

void CNextClientApi::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) {
	for (int i = 0; i <= clientMax; i++) {
		this->playerData[i] = PlayerData();
	}

	this->forwardApiReady = MF_RegisterForward("ncl_client_api_ready", ET_IGNORE, FP_CELL, FP_DONE);
}

IViewmodelFX* CNextClientApi::ViewmodelFX() {
	return this->apiViewmodelFX.get();
}

IPrivatePrecache* CNextClientApi::PrivatePrecache() {
	return this->apiPrivatePrecache.get();
}

ICvarSandbox* CNextClientApi::CvarSandbox() {
	return this->apiCvarSandbox.get();
}

bool CNextClientApi::ClientIsReady(int client) {
	if (this->playerData.count(client) == 0)
		return false;
	return this->playerData[client].isApiReady;
}

NextClientVersion CNextClientApi::ClientIsNextClient(int client) {
	if (this->playerData.count(client) == 0)
		return NOT_NEXTCLIENT;
	return this->playerData[client].clientVersion;
}

void CNextClientApi::OnPlayerPostThink(int client) {
	if (this->playerData.count(client) == 0)
		return;

	auto data = &this->playerData[client];
	if (!data->isApiReady && data->clientVersion != NOT_NEXTCLIENT) {
		data->isApiReady = true;

		MF_ExecuteForward(this->forwardApiReady, client);
	}
}

void CNextClientApi::OnClientDisconnect(int client) {
	if (this->playerData.count(client) == 0)
		return;

	auto data = &this->playerData[client];
	data->clientVersion = NOT_NEXTCLIENT;
	data->isApiReady = false;
}

void CNextClientApi::OnClientConnect(int client) {
	this->apiPrivatePrecache->OnClientConnect(client);

	if (this->playerData.count(client) == 0)
		return;

	auto data = &this->playerData[client];
	auto value = INFOKEY_VALUE(GET_INFOKEYBUFFER(INDEXENT(client)), "_ncl");

	if (value[0] == '1') {
		if (value[1] == '8')
			data->clientVersion = V_2_1_8;
		else
			data->clientVersion = V_2_1_7_OR_LOWER;
	}
}

void CNextClientApi::ClientSetFOV(int client, int fov, float lerpTime) {
	MESSAGE_BEGIN(MSG_ONE, this->messageSetFOVEx, NULL, INDEXENT(client));
	WRITE_BYTE(fov & 0xFF);
	WRITE_LONG(lerpTime);
	MESSAGE_END();
}

CNextClientApi* implNextClientApi = nullptr;

void NAPI_Install() {
	implNextClientApi = new CNextClientApi;
}

INextClientAPI* NAPI() {
	return implNextClientApi;
}

CNextClientApi* _NAPIController() {
	return implNextClientApi;
}