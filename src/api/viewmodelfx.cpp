#include "viewmodelfx.h"

CViewmodelFX::CViewmodelFX() {
	this->messageViewmodelFX = utils::RegUserMsgSafe("ViewModelFx", -1);
	memset(&this->stateVFX, 0, sizeof(VFXState));
}

void CViewmodelFX::WriteRenderMode(int rendermode) {
	NAPI_ASSERT(this->isMessageBuilding, "Writing value with no started message");

	if (rendermode == -1)
		this->StateReset(VMFX_RENDERMODE);
	else {
		this->stateVFX.rendermode = rendermode;
		this->StateSet(VMFX_RENDERMODE);
	}
}

void CViewmodelFX::WriteRenderAmt(int renderamt) {
	NAPI_ASSERT(this->isMessageBuilding, "Writing value with no started message");

	if (renderamt == -1)
		this->StateReset(VMFX_RENDERAMT);
	else {
		this->stateVFX.renderamt = renderamt;
		this->StateSet(VMFX_RENDERAMT);
	}
}

void CViewmodelFX::WriteRenderColor(int r, int g, int b) {
	NAPI_ASSERT(this->isMessageBuilding, "Writing value with no started message");

	if (r == -1)
		this->StateReset(VMFX_RENDERCOLOR);
	else {
		auto color = &this->stateVFX.rendercolor;
		color->r = r;
		color->g = g;
		color->b = b;
		this->StateSet(VMFX_RENDERCOLOR);
	}
}

void CViewmodelFX::WriteRenderFX(int renderfx) {
	NAPI_ASSERT(this->isMessageBuilding, "Writing value with no started message");

	if (renderfx == -1)
		this->StateReset(VMFX_RENDERFX);
	else {
		this->stateVFX.renderfx = renderfx;
		this->StateSet(VMFX_RENDERFX);
	}
}

void CViewmodelFX::WriteSkin(int skin) {
	NAPI_ASSERT(this->isMessageBuilding, "Writing value with no started message");

	if (skin == -1)
		this->StateReset(VMFX_SKIN);
	else {
		this->stateVFX.skin = skin;
		this->StateSet(VMFX_SKIN);
	}
}

void CViewmodelFX::WriteBody(int body) {
	NAPI_ASSERT(this->isMessageBuilding, "Writing value with no started message");

	if (body == -1)
		this->StateReset(VMFX_BODY);
	else {
		this->stateVFX.body = body;
		this->StateSet(VMFX_BODY);
	}
}

void CViewmodelFX::StateSet(VFX state) {
	this->bitStateSet |= (1 << state);
}

void CViewmodelFX::StateReset(VFX state) {
	this->bitStateReset |= (1 << state);
}

bool CViewmodelFX::StateIsSet(VFX state) {
	return (this->bitStateSet & (1 << state));
}

void CViewmodelFX::Begin(int client) {
	NAPI_ASSERT(!this->isMessageBuilding, "Repeating of message beggining");

	memset(&this->stateVFX, 0, sizeof(VFXState));

	this->client = client;
	this->bitStateSet = 0;
	this->bitStateReset = 0;
	this->isMessageBuilding = true;
}

void CViewmodelFX::End() {
	NAPI_ASSERT(this->isMessageBuilding, "Ending with no started message");

	MESSAGE_BEGIN(MSG_ONE, this->messageViewmodelFX, NULL, INDEXENT(this->client));

	if (this->bitStateReset)
		this->bitStateSet |= (1 << 6);

	WRITE_BYTE(this->bitStateSet);

	if (this->bitStateReset)
		WRITE_BYTE(this->bitStateReset);

	if (this->StateIsSet(VMFX_RENDERMODE) || this->StateIsSet(VMFX_RENDERFX))
		WRITE_BYTE(this->stateVFX.rendermode | (this->stateVFX.renderfx << 3));

	if (this->StateIsSet(VMFX_RENDERAMT))
		WRITE_BYTE(this->stateVFX.renderamt);

	if (this->StateIsSet(VMFX_RENDERCOLOR)) {
		auto color = &this->stateVFX.rendercolor;

		WRITE_BYTE(color->r);
		WRITE_BYTE(color->g);
		WRITE_BYTE(color->b);
	}

	if (this->StateIsSet(VMFX_SKIN))
		WRITE_BYTE(this->stateVFX.skin);

	if (this->StateIsSet(VMFX_BODY))
		WRITE_LONG(this->stateVFX.body);

	MESSAGE_END();

	this->isMessageBuilding = false;
}