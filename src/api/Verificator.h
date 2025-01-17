#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <stddef.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include <events.h>
#include "NclmProtocol.h"
#include "EventManager.h"

class Verificator : public IEventServerActivated,
                    public IEventClientConnect,
                    public IEventNclmVerificationRequest,
                    public IEventNclmVerificationResponse
{
    struct PlayerData
    {
        std::string prefered_RSA_key_version;
        std::vector<uint8_t> payload;
    };

    std::string dirpath_public_keys_;
    std::map<std::string, EVP_PKEY*> map_cached_pkeys_;
    std::map<int, PlayerData> player_data_;

    NclmProtocol* protocol_;
    EventManager* event_manager_;

    void OnNclmVerificationRequest(edict_t* client, std::string rsaKeyVersion) override;
    void OnNclmVerificationResponse(edict_t* client, std::string clientVersion, std::vector<uint8_t> payload) override;
    void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) override;
    void OnClientConnect(int client) override;

    int ParsePublicKeys();

public:
    Verificator(NclmProtocol* protocol, EventManager* event_manager);
};