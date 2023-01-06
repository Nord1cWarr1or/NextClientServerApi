#include "natives.h"

cell AMX_NATIVE_CALL NativeCall_CvarSandbox_Begin(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_index
	};
	ASSERT_ARG_IS_PLAYER(arg_index);

	int client = params[arg_index];
	NAPI()->CvarSandbox()->Begin(client);

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_CvarSandbox_End(AMX* amx, cell* params) {
	NAPI()->CvarSandbox()->End();

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_CvarSandbox_WriteCvar(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_cvar,
		arg_value,
	};

	auto cvar = static_cast<SandboxCvar>(params[arg_cvar]);
	std::string value = MF_GetAmxString(amx, params[arg_value], 0, NULL);

	NAPI()->CvarSandbox()->WriteCvar(cvar, value);

	return TRUE;
}

AMX_NATIVE_INFO nativeInfoCvarSandbox[] = {
	{ "ncl_sandbox_cvar_begin", NativeCall_CvarSandbox_Begin },
	{ "ncl_sandbox_cvar_end", NativeCall_CvarSandbox_End },
	{ "ncl_write_sandbox_cvar", NativeCall_CvarSandbox_WriteCvar },

	{ nullptr, nullptr }
};

void AddNatives_CvarSandbox() {
	MF_AddNatives(nativeInfoCvarSandbox);
}