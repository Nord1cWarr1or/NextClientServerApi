#include "natives.h"

cell AMX_NATIVE_CALL ncl_is_client_api_ready(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_index
	};
	ASSERT_ARG_IS_PLAYER(arg_index);

	cell result = NAPI()->ClientIsReady(params[arg_index]);

	ASSERT_NO_NAPI_ERRORS();
	return result;
}

cell AMX_NATIVE_CALL ncl_is_next_client(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_index
	};
	ASSERT_ARG_IS_PLAYER(arg_index);

	cell result = NAPI()->ClientIsNextClient(params[arg_index]);

	ASSERT_NO_NAPI_ERRORS();
	return result;
}

cell AMX_NATIVE_CALL ncl_setfov(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_index,
		arg_fov,
		arg_lerpTime
	};
	ASSERT_ARG_IS_PLAYER(arg_index);

	NAPI()->ClientSetFOV(params[arg_index], params[arg_fov], params[arg_lerpTime]);

	ASSERT_NO_NAPI_ERRORS();
	return TRUE;
}

AMX_NATIVE_INFO nativeInfoNextClient[] = {
	{ "ncl_is_client_api_ready", ncl_is_client_api_ready },
	{ "ncl_is_next_client", ncl_is_next_client },

	{ "ncl_setfov", ncl_setfov },

	{ nullptr, nullptr }
};

void AddNatives_NextClient() {
	MF_AddNatives(nativeInfoNextClient);
}