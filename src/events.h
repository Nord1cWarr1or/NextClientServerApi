#pragma once

#include "amxxmodule.h"
#include "api/INextClientAPI.h"
#include <vector>
#include <string>

class IEventPlayerPostThink {
public:
	virtual void OnPlayerPostThink(int client) = 0;
};

class IEventClientDisconnect {
public:
	virtual void OnClientDisconnect(int client) = 0;
};

class IEventClientConnect {
public:
	virtual void OnClientConnect(int client) = 0;
};

class IEventClientFirstFrame {
public:
	virtual void OnClientFirstFrame(int client) = 0;
};

class IEventClientPutInServer {
public:
	virtual void OnClientPutInServer(edict_t* pEntity) = 0;
};

class IEventServerActivated {
public:
	virtual void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) = 0;
};

class IEventMessageBeginPost {
public:
    virtual void OnMessageBeginPost(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) = 0;
};

class IEventMessageEndPost {
public:
    virtual void OnMessageEndPost() = 0;
};

class IEventHandleNclMessage {
public:
	virtual void OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode) = 0;
};

class IEventNclmVerificationRequest {
public:
	virtual void OnNclmVerificationRequest(edict_t* client, std::string rsaKeyVersion) = 0;
};

class IEventNclmVerificationResponse {
public:
	virtual void OnNclmVerificationResponse(edict_t* client, std::string clientVersion, std::vector<uint8_t> payload) = 0;
};

class IEventClientVerificated {
public:
	virtual void OnClientVerificated(edict_t* client, std::string clientVersion, std::string rsaKeyVersion) = 0;
};

class IEventSendServerinfo {
public:
	virtual void OnSendServerInfo(edict_t* client) = 0;
};