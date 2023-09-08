#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include <events.h>
#include "NclmProtocol.h"

#define RSA_KEY_LENGTH 256

class Verificator : public IEventServerActivated,
                    public IEventClientConnect,
                    public IEventNclmVerificationRequest,
                    public IEventNclmVerificationResponse
{
    struct PlayerData
    {
        std::string client_version;
        std::string prefered_RSA_key_version;
        std::vector<uint8_t> payload;
    };

    std::string dirpath_public_keys_;
    std::map<std::string, EVP_PKEY*> map_cached_pkeys_;
    std::map<int, PlayerData> player_data_;

    NclmProtocol* protocol_;

    void OnNclmVerificationRequest(edict_t* client, std::string clientVersion, std::string rsaKeyVersion) override;
    void OnNclmVerificationResponse(edict_t* client, std::vector<uint8_t> payload) override;
    void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) override;
    void OnClientConnect(int client) override;

    int ParsePublicKeys();

public:
    Verificator(NclmProtocol* protocol);
};