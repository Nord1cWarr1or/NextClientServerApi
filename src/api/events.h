#pragma once

#include "amxxmodule.h"

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

class IEventServerActivated {
public:
	virtual void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) = 0;
};

class IEventHandleNCLMessage {
public:
	virtual void OnHandleNCLMessage(edict_t* client) = 0;
};