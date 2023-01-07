#pragma once

#include "main.h"
#include <string>

class CCvarSandbox : public ICvarSandbox {
	int messageSandboxCvar;

public:
	CCvarSandbox();

	void Begin(int client);
	void End();

	void WriteCvar(SandboxCvar cvar, std::string value);
};