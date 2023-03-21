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
                     public IEventClientConnect
{
    struct PlayerData
    {
        std::string client_version;
        std::string prefered_RSA_key_version;
        std::vector<byte> payload;
    };

    std::string dirpath_public_keys_;
    std::map<std::string, EVP_PKEY*> map_cached_pkeys_;

    std::map<int, PlayerData> player_data_;

public:
    CVerificator();

    int ParsePublicKeys();
    void HandleNCLMVerificationRequest(edict_t* client);
    void HandleNCLMVerificationResponse(edict_t* client);

    void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) override;
    void OnClientConnect(int client) override;
};