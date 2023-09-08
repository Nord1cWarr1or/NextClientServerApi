#include "Verificator.h"
#include "../rehlds_api.h"
#include "asserts.h"
#include <iostream>

Verificator::Verificator()
{
    dirpath_public_keys_ = MF_BuildPathname("%s/nextclient_api/pkeys/", MF_GetLocalInfo("amxx_datadir", "addons/amxmodx/data"));
}

void Verificator::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax)
{
    for (int i = 0; i <= clientMax; i++)
    {
        player_data_[i] = PlayerData();
    }

    ParsePublicKeys();
}

void Verificator::OnClientConnect(int client)
{
    if (player_data_.count(client) == 0)
        return;

    player_data_[client].payload.clear();

    std::cout << "Sending onconnect NCLM_S2C::IS_SERVER_SUPPORT_NEXTCLIENT to client " 
        << INFOKEY_VALUE(GET_INFOKEYBUFFER(INDEXENT(client)), "name") << std::endl;

    MESSAGE_BEGIN(MSG_ONE, SVC_NCL_MESSAGE, NULL, INDEXENT(client));
    WRITE_LONG(NCLM_C2S_HEADER);

    const char str[] = {0x1, 0x1, 0x2, 0x3, 'n', 'c', 'l', 'm', 0xFA, 0xFB, 0x00};
    WRITE_STRING(str);
    //WRITE_STRING(R"(_0f#s5?C3[|Y8F"$F[=oq5;C,[4Y)FQ!_HmS74FE3[xX+HZRxtVEF5=Ctgx`9fj/JvuQQs<PTI!W[SAu!{?CNlg^@[4Y)FQ!_Hf#s5~Cihg}nBk&bv>`+4"Dj>CS9fk/DveQF5?C}f&]qU9/Rj;CQs~}?kUXLIk&bv>`r5IDihg}nBp(!{fSq5`Cyha:~|!/vuWQ8)Dihg}nBp(!{fSq5`Cyha:~|!/vuWQ8)Dihg}nBp(!{fSq5`Cyha:~|!/vuWQ8)Dihg}nBp(!{fSq5`Cyha:~|!/vuWQ8)Dihg}nBp(!{fSq5`Cyha:~|!/vuWQ8)Dihg}nBp(!{fSq5`Cyha:~|!/vuWQ8)");
    MESSAGE_END();
}

void Verificator::OnClientPutInServer(edict_t* pEntity) {
    // std::cout << "Sending NCLM_S2C::IS_SERVER_SUPPORT_NEXTCLIENT to client " 
    //     << INFOKEY_VALUE(GET_INFOKEYBUFFER(pEntity), "name") << std::endl;

    // MESSAGE_BEGIN(MSG_ONE, SVC_NCL_MESSAGE, NULL, pEntity);
    // WRITE_LONG(NCLM_C2S_HEADER);
    // WRITE_BYTE(NCLM_S2C::IS_SERVER_SUPPORT_NEXTCLIENT);
    // MESSAGE_END();
}

void Verificator::OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode)
{   
    switch (opcode)
    {
        case VERIFICATION_REQUEST:
            HandleNCLMVerificationRequest(client);
            break;

        case VERIFICATION_RESPONSE:
            HandleNCLMVerificationResponse(client);
            break;
    }
}

void Verificator::HandleNCLMVerificationRequest(edict_t* client)
{
    std::string clientVersion = MSG_ReadString();
    std::string rsaKeyVersion = MSG_ReadString();
    auto name = INFOKEY_VALUE(GET_INFOKEYBUFFER(client), "name");

    std::cout << "HandleNCLMVerificationRequest: " << clientVersion << " " << rsaKeyVersion << std::endl;

    NAPI_LOG_ASSERT(!MSG_IsBadRead(), "%s: badread on %s", __FUNCTION__, name);

    int clientIndex = ENTINDEX(client);
    if (map_cached_pkeys_.count(rsaKeyVersion) == 0 || player_data_.count(clientIndex) == 0)
        return;

    auto playerData = &player_data_[clientIndex];
    auto payload = playerData->payload;
    if (!payload.empty())
        return;

    payload.assign(NCLM_VERIF_PAYLOAD_SIZE, 0x00);
    playerData->client_version = clientVersion;
    playerData->prefered_RSA_key_version = rsaKeyVersion;

    int res;
    auto ctx = EVP_PKEY_CTX_new(map_cached_pkeys_[rsaKeyVersion], NULL);
    res = EVP_PKEY_encrypt_init(ctx);
    NAPI_LOG_ASSERT(res > 0, "Cannot initialize encrypt context (code %d)", res);

    uint8_t out[RSA_KEY_LENGTH];
    size_t keySizeFromFile, keySizeWritten;

    res = EVP_PKEY_encrypt(ctx, NULL, &keySizeFromFile, payload.data(), payload.size());
    NAPI_LOG_ASSERT(res > 0, "Cannot perform encrypt operation (code %d)", res);
    NAPI_LOG_ASSERT(keySizeFromFile == RSA_KEY_LENGTH, "'%s' key length does not match (%d, but need %d)",
                    rsaKeyVersion.c_str(), keySizeFromFile, RSA_KEY_LENGTH);

    RAND_bytes(payload.data(), payload.size());

    res = EVP_PKEY_encrypt(ctx, out, &keySizeWritten, payload.data(), payload.size());
    NAPI_LOG_ASSERT(res > 0, "Cannot perform encrypt operation 2 (code %d)", res);

    std::cout << "Sending NCLM_S2C::VERIFICATION_PAYLOAD to " << name << std::endl;

    MESSAGE_BEGIN(MSG_ONE, SVC_NCL_MESSAGE, NULL, client);
    WRITE_LONG(NCLM_C2S_HEADER);
    WRITE_BYTE(NCLM_S2C::VERIFICATION_PAYLOAD);
    for (int i = 0; i < NCLM_VERIF_PAYLOAD_SIZE; i += 4)
    {
        WRITE_LONG(*(long*) (out + i));
    }
    MESSAGE_END();
}

void Verificator::HandleNCLMVerificationResponse(edict_t* client)
{
    int clientIndex = ENTINDEX(client);
    if (player_data_.count(clientIndex) == 0)
        return;

    auto playerData = &player_data_[clientIndex];
    if (playerData->payload.empty())
        return;

    auto name = INFOKEY_VALUE(GET_INFOKEYBUFFER(client), "name");
    auto rsaKeyVersion = playerData->prefered_RSA_key_version;
    std::vector<uint8_t> buffer;
    buffer.assign(NCLM_VERIF_PAYLOAD_SIZE, 0x00);
    MSG_ReadBuf(buffer.size(), buffer.data());

    NAPI_LOG_ASSERT(!MSG_IsBadRead(), "%s: badread on %s (%s)",
                    __FUNCTION__, name, rsaKeyVersion.c_str());

    NAPI_LOG_ASSERT(buffer == playerData->payload, "%s: Decrypted payload body mismatch on %s (%s)",
                    __FUNCTION__, name, rsaKeyVersion.c_str());

    MF_Log("Verificated user %s has joined the game!\n", name);
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
