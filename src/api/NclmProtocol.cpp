#include "NclmProtocol.h"
#include "SizeBufWriter.h"
#include "api/nclm_proto.h"
#include "rehlds_api_provider.h"
#include "asserts.h"
#include <string>
#include <map>

class NclmSizeBufWriter : public SizeBufWriter {
public:
	NclmSizeBufWriter(sizebuf_t* output_buf, size_t maxsize)
		: SizeBufWriter(output_buf, maxsize) {
		WriteByte(SVC_NCL_MESSAGE);
		WriteLong(NCLM_C2S_HEADER);
	}

	std::map<uint8_t, uint8_t> escaping_symbols_ = {
		{ 0x0, '0' },
		{ 0xFF, 'm' },
		{ '^', '^' }
	};

	void Send() override {
		auto slice = GetTempBufCurSizeSlice();

		for (auto it = slice.begin(); it != slice.end(); it++) {
			auto symbol = *it;
			if (escaping_symbols_.count(symbol)) {
				*it = '^';
				it = slice.insert(it + 1, escaping_symbols_[symbol]);
			}
		}
		slice.push_back(NULL);

		ReplaceTempBufWithSlice(slice);
		SizeBufWriter::Send();
	}
};

NclmProtocol::NclmProtocol(EventManager* event_manager)
	: event_manager_(event_manager) { }

void NclmProtocol::OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode) {
	switch (opcode) {
	case VERIFICATION_REQUEST:
		OnVerificationRequest(client);
		break;

	case VERIFICATION_RESPONSE:
		OnVerificationResponse(client);
		break;
	}
}

void NclmProtocol::OnVerificationRequest(edict_t* client) {
	std::string clientVersion = MSG_ReadString();
	std::string rsaKeyVersion = MSG_ReadString();

	auto name = INFOKEY_VALUE(GET_INFOKEYBUFFER(client), "name");
	NAPI_LOG_ASSERT(!MSG_IsBadRead(), "%s: badread on %s", __FUNCTION__, name);

	event_manager_->OnNclmVerificationRequest(client, clientVersion, rsaKeyVersion);
}

void NclmProtocol::OnVerificationResponse(edict_t* client) {
	std::vector<uint8_t> payload;

	payload.assign(NCLM_VERIF_PAYLOAD_SIZE, 0x00);
	auto written = MSG_ReadBuf(payload.size(), payload.data());

	auto name = INFOKEY_VALUE(GET_INFOKEYBUFFER(client), "name");
	auto netMessage = g_RehldsFuncs->GetNetMessage();
    int readcount = *g_RehldsFuncs->GetMsgReadCount();
	NAPI_LOG_ASSERT(!MSG_IsBadRead(), "%s: badread on %s (%d/%d/%d)", 
		__FUNCTION__, name, readcount, readcount + NCLM_VERIF_PAYLOAD_SIZE, netMessage->cursize);

	event_manager_->OnNclmVerificationResponse(client, payload);
}

sizebuf_t* NclmProtocol::GetClientReliableChannel(edict_t* client) {
	auto cl = g_RehldsApi->GetServerStatic()->GetClient(ENTINDEX(client) - 1);
	return cl->GetNetChan()->GetMessageBuf();
}

sizebuf_t* NclmProtocol::GetClientUnrealibleChannel(edict_t* client) {
	auto cl = g_RehldsApi->GetServerStatic()->GetClient(ENTINDEX(client) - 1);
	return cl->GetDatagram();
}

void NclmProtocol::SendIsServerSupportNextclient(edict_t* client) {
	NclmSizeBufWriter message(GetClientReliableChannel(client), 0x32);

	message.WriteByte(NCLM_S2C::IS_SERVER_SUPPORT_NEXTCLIENT)->Send();
}

void NclmProtocol::SendVerificationPayload(edict_t* client, std::vector<uint8_t> payload) {
	NclmSizeBufWriter message(GetClientReliableChannel(client), 0x140);

	message.WriteByte(NCLM_S2C::VERIFICATION_PAYLOAD)
		->WriteBuf(payload)
		->Send();
}