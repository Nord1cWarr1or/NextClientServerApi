#pragma once

#include "events.h"
#include "main.h"
#include <cstring>

class CViewmodelFX : public IViewmodelFX {
	int messageViewmodelFX;
	int client;
	int bitStateSet = 0;
	int bitStateReset = 0;

	enum VFX {
		VMFX_RENDERMODE,
		VMFX_RENDERAMT,
		VMFX_RENDERCOLOR,
		VMFX_RENDERFX,
		VMFX_SKIN,
		VMFX_BODY,
	};

	struct VFXState {
		int rendermode;
		int renderamt;
		color24 rendercolor;
		int renderfx;
		int skin;
		int body;
	} stateVFX;

	void StateSet(VFX state);
	bool StateIsSet(VFX state);
	void StateReset(VFX state);

public:
	CViewmodelFX();

	void Begin(int client);
	void End();

	void WriteRenderMode(int rendermode);
	void WriteRenderAmt(int renderamt);
	void WriteRenderColor(int r, int g, int b);
	void WriteRenderFX(int renderfx);
	void WriteSkin(int skin);
	void WriteBody(int body);
};