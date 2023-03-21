#include "cvar_sandbox.h"

CCvarSandbox::CCvarSandbox()
{
    message_sandbox_cvar_ = utils::RegUserMsgSafe("SandboxCvar", -1);
}

void CCvarSandbox::Begin(int client)
{
    NAPI_ASSERT(!is_message_building_, "Repeating of message beggining");

    MESSAGE_BEGIN(MSG_ONE, message_sandbox_cvar_, NULL, INDEXENT(client));
    WRITE_BYTE(255);

    is_message_building_ = true;
}

void CCvarSandbox::End()
{
    NAPI_ASSERT(is_message_building_, "Ending with no started message")

    MESSAGE_END();

    is_message_building_ = false;
}

void CCvarSandbox::WriteCvar(SandboxCvar cvar, const std::string& value)
{
    NAPI_ASSERT(is_message_building_, "Writing cvar with no started message")

    WRITE_BYTE(cvar);
    WRITE_STRING(value.c_str());
}
