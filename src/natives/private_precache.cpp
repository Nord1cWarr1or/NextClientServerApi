#include "natives.h"

cell AMX_NATIVE_CALL NativeCall_PrivatePrecache_PrecacheModel(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_filepath,
		arg_nclFilepath
	};

	std::string filepath = MF_GetAmxString(amx, params[arg_filepath], 0, NULL);
	std::string nclFilepath = MF_GetAmxString(amx, params[arg_nclFilepath], 1, NULL);

	NAPI()->PrivatePrecache()->PrecacheModel(filepath, nclFilepath);

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_PrivatePrecache_PrecacheSound(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_filepath,
		arg_nclFilepath
	};

	std::string filepath = MF_GetAmxString(amx, params[arg_filepath], 0, NULL);
	std::string nclFilepath = MF_GetAmxString(amx, params[arg_nclFilepath], 1, NULL);
	
	NAPI()->PrivatePrecache()->PrecacheSound(filepath, nclFilepath);

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_PrivatePrecache_PrecacheClientOnly(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_filepath,
		arg_nclFilepath
	};

	std::string filepath = MF_GetAmxString(amx, params[arg_filepath], 0, NULL);
	std::string nclFilepath = MF_GetAmxString(amx, params[arg_nclFilepath], 1, NULL);
	
	NAPI()->PrivatePrecache()->PrecacheClientOnly(filepath, nclFilepath);

	return TRUE;
}

AMX_NATIVE_INFO nativeInfoPrivatePrecache[] = {
	{ "ncl_precache_model", NativeCall_PrivatePrecache_PrecacheModel },
	{ "ncl_precache_sound", NativeCall_PrivatePrecache_PrecacheSound },
	{ "ncl_precache_client_only", NativeCall_PrivatePrecache_PrecacheClientOnly },

	{ nullptr, nullptr }
};

void AddNatives_PrivatePrecache() {
	MF_AddNatives(nativeInfoPrivatePrecache);
}