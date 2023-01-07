#include "cvar_sandbox.h"

CCvarSandbox::CCvarSandbox() {
	this->messageSandboxCvar = utils::RegUserMsgSafe("SandboxCvar", -1);
}

void CCvarSandbox::Begin(int client) {
	MESSAGE_BEGIN(MSG_ONE, this->messageSandboxCvar, NULL, INDEXENT(client));
	WRITE_BYTE(255);
}

void CCvarSandbox::End() {
	MESSAGE_END();
}

void CCvarSandbox::WriteCvar(SandboxCvar cvar, std::string value) {
	WRITE_BYTE(cvar);
	WRITE_STRING(value.c_str());
}