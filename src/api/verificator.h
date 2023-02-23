#pragma once

#include "events.h"
#include "main.h"
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#define RSA_KEY_LENGTH 256

class CVerificator : public IEventServerActivated,
					 public IEventClientConnect {
	std::string dirpathPublicKeys;
	std::map<std::string, EVP_PKEY*> mapCachedPKeys;

	struct PlayerData {
		std::string clientVersion;
		std::string preferedRSAKeyVersion;
		std::vector<byte> payload;

		PlayerData()
			: clientVersion("")
			, preferedRSAKeyVersion("")
			, payload(std::vector<byte>()) {};
	};

	std::map<int, PlayerData> playerData;

public:
	CVerificator();

	int ParsePublicKeys();
	void HandleNCLMVerificationRequest(edict_t* client);
	void HandleNCLMVerificationResponse(edict_t* client);

	void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax);
	void OnClientConnect(int client);
};