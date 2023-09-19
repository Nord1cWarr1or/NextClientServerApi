#pragma once

#include "EventManager.h"
#include <events.h>
#include <vector>
#include <rehlds_api_provider.h>

class NclmProtocol : public IEventHandleNclMessage {
	EventManager* event_manager_;

	void OnVerificationRequest(edict_t* client);
	void OnVerificationResponse(edict_t* client);

	sizebuf_t* GetClientReliableChannel(edict_t* client);
	sizebuf_t* GetClientUnrealibleChannel(edict_t* client);

public:
	NclmProtocol(EventManager* event_manager);
	void OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode) override;

	void SendVerificationPayload(edict_t* client, std::vector<uint8_t> payload);
};