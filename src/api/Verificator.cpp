#include "Verificator.h"
#include "asserts.h"
#include <iostream>

Verificator::Verificator(NclmProtocol* protocol, EventManager* event_manager) 
    : protocol_(protocol), event_manager_(event_manager)
{
    dirpath_public_keys_ = MF_BuildPathname("%s/nextclient_api/pkeys/", MF_GetLocalInfo("amxx_datadir", "addons/amxmodx/data"));
}

void Verificator::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax)
{
    for (int i = 0; i <= clientMax; i++)
        player_data_[i] = PlayerData();

    ParsePublicKeys();
}

void Verificator::OnClientConnect(int client)
{
    if (player_data_.count(client) == 0)
        return;

    player_data_[client].payload.clear();
}

void Verificator::OnNclmVerificationRequest(edict_t* client, std::string rsaKeyVersion) {
    int clientIndex = ENTINDEX(client);
    if (map_cached_pkeys_.count(rsaKeyVersion) == 0 || player_data_.count(clientIndex) == 0)
        return;

    auto playerData = &player_data_[clientIndex];
    auto payload = &playerData->payload;
    if (!payload->empty())
        return;

    payload->assign(NCLM_VERIF_PAYLOAD_SIZE, 0x00);
    playerData->prefered_RSA_key_version = rsaKeyVersion;

    int res;
    auto ctx = EVP_PKEY_CTX_new(map_cached_pkeys_[rsaKeyVersion], NULL);
    res = EVP_PKEY_encrypt_init(ctx);
    NAPI_LOG_ASSERT(res > 0, "Cannot initialize encrypt context (code %d)", res);

    size_t outLen;
    std::vector<uint8_t> out(NCLM_VERIF_ENCRYPTED_PAYLOAD_SIZE);

    RAND_bytes(payload->data(), payload->size());

    res = EVP_PKEY_encrypt(ctx, NULL, &outLen, payload->data(), payload->size());
    NAPI_LOG_ASSERT(res > 0, "Cannot perform encrypt operation (code %d)", res);
    NAPI_LOG_ASSERT(outLen == NCLM_VERIF_ENCRYPTED_PAYLOAD_SIZE, "'%s' key length does not match (%d, but need %d)",
                    rsaKeyVersion.c_str(), outLen, NCLM_VERIF_ENCRYPTED_PAYLOAD_SIZE);

    res = EVP_PKEY_encrypt(ctx, out.data(), &outLen, payload->data(), payload->size());
    NAPI_LOG_ASSERT(res > 0, "Cannot perform encrypt operation 2 (code %d)", res);

    protocol_->SendVerificationPayload(client, out);
}

void Verificator::OnNclmVerificationResponse(edict_t* client, std::string clientVersion, std::vector<uint8_t> payload) {
    int clientIndex = ENTINDEX(client);
    if (player_data_.count(clientIndex) == 0)
        return;

    auto playerData = &player_data_[clientIndex];
    if (playerData->payload.empty())
        return;

    auto name = INFOKEY_VALUE(GET_INFOKEYBUFFER(client), "name");
    auto rsaKeyVersion = playerData->prefered_RSA_key_version;

    NAPI_LOG_ASSERT(payload == playerData->payload, "%s: Decrypted payload body mismatch on %s (%s)",
                    __FUNCTION__, name, rsaKeyVersion.c_str());

    event_manager_->OnClientVerificated(client, clientVersion, rsaKeyVersion);
}

int Verificator::ParsePublicKeys()
{
    using std::ifstream;

    map_cached_pkeys_.clear();

    ifstream fileActive(dirpath_public_keys_ + "active.txt");
    if (!fileActive.is_open())
        return 0;

    std::string line;
    char buffer[1024];

    while (std::getline(fileActive, line))
    {
        ifstream fileRSAKey(dirpath_public_keys_ + line + ".pem");
        if (!fileRSAKey.is_open())
            continue;

        long long readed = 0;
        BIO* bio = BIO_new(BIO_s_mem());

        while (fileRSAKey.good())
        {
            readed = fileRSAKey.rdbuf()->sgetn(buffer, 1024);
            BIO_write(bio, buffer, readed);

            if (readed != 1024)
                fileRSAKey.setstate(ifstream::eofbit | ifstream::failbit);
            else
                fileRSAKey.setstate(ifstream::goodbit);
        }
        map_cached_pkeys_[line] = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    }

    return map_cached_pkeys_.size();
}
