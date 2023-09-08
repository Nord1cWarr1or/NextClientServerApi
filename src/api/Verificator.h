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
#include "api/nclm_proto.h"

#define RSA_KEY_LENGTH 256

class Verificator : public IEventServerActivated,
                    public IEventClientConnect,
                    public IEventClientPutInServer,
                    public IEventHandleNCLMessage
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

public:
    Verificator();

    void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) override;
    void OnClientConnect(int client) override;
    void OnClientPutInServer(edict_t* pEntity) override;
    void OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode) override;

private:
    void HandleNCLMVerificationRequest(edict_t* client);
    void HandleNCLMVerificationResponse(edict_t* client);

    int ParsePublicKeys();
};