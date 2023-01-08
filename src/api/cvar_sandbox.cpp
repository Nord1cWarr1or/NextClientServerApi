#include "cvar_sandbox.h"

CCvarSandbox::CCvarSandbox() {
	this->messageSandboxCvar = utils::RegUserMsgSafe("SandboxCvar", -1);
}

void CCvarSandbox::Begin(int client) {
	NAPI_ASSERT(!this->isMessageBuilding, "Repeating of message beggining");

	MESSAGE_BEGIN(MSG_ONE, this->messageSandboxCvar, NULL, INDEXENT(client));
	WRITE_BYTE(255);

	this->isMessageBuilding = true;
}

void CCvarSandbox::End() {
	NAPI_ASSERT(this->isMessageBuilding, "Ending with no started message")

	MESSAGE_END();

	this->isMessageBuilding = false;
}

void CCvarSandbox::WriteCvar(SandboxCvar cvar, std::string value) {
	NAPI_ASSERT(this->isMessageBuilding, "Writing cvar with no started message")

	WRITE_BYTE(cvar);
	WRITE_STRING(value.c_str());
}