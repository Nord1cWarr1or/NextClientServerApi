#include "viewmodelfx.h"

void CViewmodelFX::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) 
{
    message_viewmodelFX_ = utils::RegUserMsgSafe("ViewModelFx", -1);
}

void CViewmodelFX::WriteRenderMode(int rendermode)
{
    NAPI_ASSERT(is_message_building_, "Writing value with no started message");

    if (rendermode == -1)
    {
        StateReset(VFX::VMFX_RENDERMODE);
    }
    else
    {
        stateVFX_.rendermode = rendermode;
        StateSet(VFX::VMFX_RENDERMODE);
    }
}

void CViewmodelFX::WriteRenderAmt(int renderamt)
{
    NAPI_ASSERT(is_message_building_, "Writing value with no started message");

    if (renderamt == -1)
    {
        StateReset(VFX::VMFX_RENDERAMT);
    }
    else
    {
        stateVFX_.renderamt = renderamt;
        StateSet(VFX::VMFX_RENDERAMT);
    }
}

void CViewmodelFX::WriteRenderColor(int r, int g, int b)
{
    NAPI_ASSERT(is_message_building_, "Writing value with no started message");

    if (r == -1)
    {
        StateReset(VFX::VMFX_RENDERCOLOR);
    }
    else
    {
        auto color = &stateVFX_.rendercolor;
        color->r = r;
        color->g = g;
        color->b = b;
        StateSet(VFX::VMFX_RENDERCOLOR);
    }
}

void CViewmodelFX::WriteRenderFX(int renderfx)
{
    NAPI_ASSERT(is_message_building_, "Writing value with no started message");

    if (renderfx == -1)
    {
        StateReset(VFX::VMFX_RENDERFX);
    }
    else
    {
        stateVFX_.renderfx = renderfx;
        StateSet(VFX::VMFX_RENDERFX);
    }
}

void CViewmodelFX::WriteSkin(int skin)
{
    NAPI_ASSERT(is_message_building_, "Writing value with no started message");

    if (skin == -1)
    {
        StateReset(VFX::VMFX_SKIN);
    }
    else
    {
        stateVFX_.skin = skin;
        StateSet(VFX::VMFX_SKIN);
    }
}

void CViewmodelFX::WriteBody(int body)
{
    NAPI_ASSERT(is_message_building_, "Writing value with no started message");

    if (body == -1)
    {
        StateReset(VFX::VMFX_BODY);
    }
    else
    {
        stateVFX_.body = body;
        StateSet(VFX::VMFX_BODY);
    }
}

void CViewmodelFX::StateSet(VFX state)
{
    bit_state_set_ |= (1 << (int)state);
}

void CViewmodelFX::StateReset(VFX state)
{
    bit_state_reset_ |= (1 << (int)state);
}

bool CViewmodelFX::StateIsSet(VFX state) const
{
    return (bit_state_set_ & (1 << (int)state));
}

void CViewmodelFX::Begin(int client)
{
    NAPI_ASSERT(!is_message_building_, "Repeating of message beggining");

    memset(&stateVFX_, 0, sizeof(VFXState));

    client_ = client;
    bit_state_set_ = 0;
    bit_state_reset_ = 0;
    is_message_building_ = true;
}

void CViewmodelFX::End()
{
    NAPI_ASSERT(is_message_building_, "Ending with no started message");

    MESSAGE_BEGIN(MSG_ONE, message_viewmodelFX_, NULL, INDEXENT(client_));

    if (bit_state_reset_)
        bit_state_set_ |= (1 << 6);

    WRITE_BYTE(bit_state_set_);

    if (bit_state_reset_)
        WRITE_BYTE(bit_state_reset_);

    if (StateIsSet(VFX::VMFX_RENDERMODE) || StateIsSet(VFX::VMFX_RENDERFX))
        WRITE_BYTE(stateVFX_.rendermode | (stateVFX_.renderfx << 3));

    if (StateIsSet(VFX::VMFX_RENDERAMT))
        WRITE_BYTE(stateVFX_.renderamt);

    if (StateIsSet(VFX::VMFX_RENDERCOLOR))
    {
        auto color = &stateVFX_.rendercolor;

        WRITE_BYTE(color->r);
        WRITE_BYTE(color->g);
        WRITE_BYTE(color->b);
    }

    if (StateIsSet(VFX::VMFX_SKIN))
        WRITE_BYTE(stateVFX_.skin);

    if (StateIsSet(VFX::VMFX_BODY))
        WRITE_LONG(stateVFX_.body);

    MESSAGE_END();

    is_message_building_ = false;
}