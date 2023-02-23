#include "natives.h"

cell AMX_NATIVE_CALL ncl_precache_model(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_filepath,
		arg_nclFilepath
	};

	std::string filepath = MF_GetAmxString(amx, params[arg_filepath], 0, NULL);
	std::string nclFilepath = MF_GetAmxString(amx, params[arg_nclFilepath], 1, NULL);

	NAPI()->PrivatePrecache()->PrecacheModel(filepath, nclFilepath);

	ASSERT_NO_NAPI_ERRORS();
	return TRUE;
}

cell AMX_NATIVE_CALL ncl_precache_sound(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_filepath,
		arg_nclFilepath
	};

	std::string filepath = MF_GetAmxString(amx, params[arg_filepath], 0, NULL);
	std::string nclFilepath = MF_GetAmxString(amx, params[arg_nclFilepath], 1, NULL);
	
	NAPI()->PrivatePrecache()->PrecacheSound(filepath, nclFilepath);

	ASSERT_NO_NAPI_ERRORS();
	return TRUE;
}

cell AMX_NATIVE_CALL ncl_precache_client_only(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_filepath,
		arg_nclFilepath
	};

	std::string filepath = MF_GetAmxString(amx, params[arg_filepath], 0, NULL);
	std::string nclFilepath = MF_GetAmxString(amx, params[arg_nclFilepath], 1, NULL);
	
	NAPI()->PrivatePrecache()->PrecacheClientOnly(filepath, nclFilepath);

	ASSERT_NO_NAPI_ERRORS();
	return TRUE;
}

AMX_NATIVE_INFO nativeInfoPrivatePrecache[] = {
	{ "ncl_precache_model", ncl_precache_model },
	{ "ncl_precache_sound", ncl_precache_sound },
	{ "ncl_precache_client_only", ncl_precache_client_only },

	{ nullptr, nullptr }
};

void AddNatives_PrivatePrecache() {
	MF_AddNatives(nativeInfoPrivatePrecache);
}