#pragma once

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