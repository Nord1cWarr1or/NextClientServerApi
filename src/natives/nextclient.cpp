#include "natives.h"

cell AMX_NATIVE_CALL NativeCall_ClientIsReady(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_index
	};
	ASSERT_ARG_IS_PLAYER(arg_index);
	
	return NAPI()->ClientIsReady(params[arg_index]);
}

cell AMX_NATIVE_CALL NativeCall_ClientIsNextClient(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_index
	};
	ASSERT_ARG_IS_PLAYER(arg_index);

	return NAPI()->ClientIsNextClient(params[arg_index]);
}

cell AMX_NATIVE_CALL NativeCall_ClientSetFOV(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_index,
		arg_fov,
		arg_lerpTime
	};
	ASSERT_ARG_IS_PLAYER(arg_index);

	NAPI()->ClientSetFOV(params[arg_index], params[arg_fov], params[arg_lerpTime]);

	return TRUE;
}

AMX_NATIVE_INFO nativeInfoNextClient[] = {
	{ "ncl_is_client_api_ready", NativeCall_ClientIsReady },
	{ "ncl_is_next_client", NativeCall_ClientIsNextClient },

	{ "ncl_setfov", NativeCall_ClientSetFOV },

	{ nullptr, nullptr }
};

void AddNatives_NextClient() {
	MF_AddNatives(nativeInfoNextClient);
}