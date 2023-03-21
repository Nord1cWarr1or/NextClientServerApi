#pragma once

#include "main.h"
#include <string>

class CCvarSandbox : public ICvarSandbox
{
    int message_sandbox_cvar_;
    bool is_message_building_{};

public:
    CCvarSandbox();
    void Begin(int client) override;
    void End() override;
    void WriteCvar(SandboxCvar cvar, const std::string& value) override;
};