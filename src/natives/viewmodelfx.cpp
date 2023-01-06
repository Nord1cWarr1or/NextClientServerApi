#include "natives.h"

cell AMX_NATIVE_CALL NativeCall_ViewmodelFX_Begin(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_index
	};
	ASSERT_ARG_IS_PLAYER(arg_index);

	NAPI()->ViewmodelFX()->Begin(params[arg_index]);

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_ViewmodelFX_End(AMX* amx, cell* params) {
	NAPI()->ViewmodelFX()->End();

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_ViewmodelFX_WriteRenderMode(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_rendermode
	};

	NAPI()->ViewmodelFX()->WriteRenderMode(params[arg_rendermode]);

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_ViewmodelFX_WriteRenderAmt(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_renderamt
	};

	NAPI()->ViewmodelFX()->WriteRenderAmt(params[arg_renderamt]);

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_ViewmodelFX_WriteRenderFX(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_renderfx
	};

	NAPI()->ViewmodelFX()->WriteRenderFX(params[arg_renderfx]);

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_ViewmodelFX_WriteSkin(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_skin
	};

	NAPI()->ViewmodelFX()->WriteSkin(params[arg_skin]);

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_ViewmodelFX_WriteBody(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_body
	};

	NAPI()->ViewmodelFX()->WriteBody(params[arg_body]);

	return TRUE;
}

cell AMX_NATIVE_CALL NativeCall_ViewmodelFX_WriteRenderColor(AMX* amx, cell* params) {
	enum args_e {
		arg_count,
		arg_r,
		arg_g,
		arg_b
	};

	NAPI()->ViewmodelFX()->WriteRenderColor(params[arg_r], params[arg_g], params[arg_b]);

	return TRUE;
}

AMX_NATIVE_INFO nativeInfoViewmodelFX[] = {
	{ "ncl_viewmodelfx_begin", NativeCall_ViewmodelFX_Begin },
	{ "ncl_viewmodelfx_end", NativeCall_ViewmodelFX_End },
	{ "ncl_write_rendermode", NativeCall_ViewmodelFX_WriteRenderMode },
	{ "ncl_write_renderamt", NativeCall_ViewmodelFX_WriteRenderAmt },
	{ "ncl_write_renderfx", NativeCall_ViewmodelFX_WriteRenderFX },
	{ "ncl_write_renderskin", NativeCall_ViewmodelFX_WriteSkin },
	{ "ncl_write_renderbody", NativeCall_ViewmodelFX_WriteBody },
	{ "ncl_write_rendercolor", NativeCall_ViewmodelFX_WriteRenderColor },

	{ nullptr, nullptr }
};

void AddNatives_ViewmodelFX() {
	MF_AddNatives(nativeInfoViewmodelFX);
}