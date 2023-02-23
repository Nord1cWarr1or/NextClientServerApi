#include "verificator.h"

CVerificator::CVerificator() {
	this->dirpathPublicKeys = MF_BuildPathname("%s/nextclient_api/pkeys/",
		MF_GetLocalInfo("amxx_datadir", "addons/amxmodx/data"));
}

void CVerificator::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) {
	for (int i = 0; i <= clientMax; i++) {
		this->playerData[i] = PlayerData();
	}
}

void CVerificator::OnClientConnect(int client) {
	if (this->playerData.count(client) == 0)
		return;

	this->playerData[client].payload.clear();
}

int CVerificator::ParsePublicKeys() {
	using std::ifstream;

	this->mapCachedPKeys.clear();

	ifstream fileActive(this->dirpathPublicKeys + "active.txt");
	if (!fileActive.is_open())
		return 0;

	std::string line;
	char buffer[1024];

	while (std::getline(fileActive, line)) {
		ifstream fileRSAKey(this->dirpathPublicKeys + line + ".pem");
		if (!fileRSAKey.is_open())
			continue;

		long long readed = 0;
		BIO* bio = BIO_new(BIO_s_mem());

		while (fileRSAKey.good()) {
			readed = fileRSAKey.rdbuf()->sgetn(buffer, 1024);
			BIO_write(bio, buffer, readed);

			if (readed != 1024)
				fileRSAKey.setstate(ifstream::eofbit | ifstream::failbit);
			else
				fileRSAKey.setstate(ifstream::goodbit);
		}
		this->mapCachedPKeys[line] = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
	}

	return this->mapCachedPKeys.size();
}

void CVerificator::HandleNCLMVerificationRequest(edict_t* client) {
	std::string clientVersion = MSG_ReadString();
	std::string rsaKeyVersion = MSG_ReadString();
	auto name = INFOKEY_VALUE(GET_INFOKEYBUFFER(client), "name");

	NAPI_LOG_ASSERT(!MSG_IsBadRead(), "%s: badread on %s", __FUNCTION__, name);

	int clientIndex = ENTINDEX(client);
	if (this->mapCachedPKeys.count(rsaKeyVersion) == 0 || this->playerData.count(clientIndex) == 0)
		return;

	auto playerData = &this->playerData[clientIndex];
	auto payload = playerData->payload;
	if (payload.size() != 0)
		return;

	payload.assign(NCLM_VERIF_PAYLOAD_SIZE, 0x00);
	playerData->clientVersion = clientVersion;
	playerData->preferedRSAKeyVersion = rsaKeyVersion;

	int res;
	auto ctx = EVP_PKEY_CTX_new(this->mapCachedPKeys[rsaKeyVersion], NULL);
	res = EVP_PKEY_encrypt_init(ctx);
	NAPI_LOG_ASSERT(res > 0, "Cannot initialize encrypt context (code %d)", res);

	byte out[RSA_KEY_LENGTH];
	size_t keySizeFromFile, keySizeWritten;

	res = EVP_PKEY_encrypt(ctx, NULL, &keySizeFromFile, payload.data(), payload.size());
	NAPI_LOG_ASSERT(res > 0, "Cannot perform encrypt operation (code %d)", res);
	NAPI_LOG_ASSERT(keySizeFromFile == RSA_KEY_LENGTH, "'%s' key length does not match (%d, but need %d)",
		rsaKeyVersion.c_str(), keySizeFromFile, RSA_KEY_LENGTH);

	RAND_bytes(payload.data(), payload.size());

	res = EVP_PKEY_encrypt(ctx, out, &keySizeWritten, payload.data(), payload.size());
	NAPI_LOG_ASSERT(res > 0, "Cannot perform encrypt operation 2 (code %d)", res);

	MESSAGE_BEGIN(MSG_ONE, SVC_NCL_MESSAGE, NULL, client);
	WRITE_BYTE(NCLM_S2C::VERIFICATION_PAYLOAD);
	for (int i = 0; i < NCLM_VERIF_PAYLOAD_SIZE; i += 4) {
		WRITE_LONG(*(long*)(out + i));
	}
	MESSAGE_END();
}

void CVerificator::HandleNCLMVerificationResponse(edict_t* client) {
	int clientIndex = ENTINDEX(client);
	if (this->playerData.count(clientIndex) == 0)
		return;

	auto playerData = &this->playerData[clientIndex];
	if (playerData->payload.size() == 0)
		return;

	auto name = INFOKEY_VALUE(GET_INFOKEYBUFFER(client), "name");
	auto rsaKeyVersion = playerData->preferedRSAKeyVersion;
	std::vector<byte> buffer;
	buffer.assign(NCLM_VERIF_PAYLOAD_SIZE, 0x00);
	MSG_ReadBuf(buffer.size(), buffer.data());

	NAPI_LOG_ASSERT(!MSG_IsBadRead(), "%s: badread on %s (%s)",
		__FUNCTION__, name, rsaKeyVersion.c_str());

	NAPI_LOG_ASSERT(buffer == playerData->payload, "%s: Decrypted payload body mismatch on %s (%s)",
		__FUNCTION__, name, rsaKeyVersion.c_str());

	MF_Log("Verificated user %s has joined the game!\n", name);
}