#pragma once

#include "events.h"
#include "main.h"
#include <cstring>

class CViewmodelFX : public IViewmodelFX
{
    enum class VFX
    {
        VMFX_RENDERMODE,
        VMFX_RENDERAMT,
        VMFX_RENDERCOLOR,
        VMFX_RENDERFX,
        VMFX_SKIN,
        VMFX_BODY,
    };

    int message_viewmodelFX_;
    int client_{};
    int bit_state_set_{};
    int bit_state_reset_{};
    bool is_message_building_{};

    struct VFXState
    {
        int rendermode;
        int renderamt;
        color24 rendercolor;
        int renderfx;
        int skin;
        int body;
    } stateVFX_{};

public:
    CViewmodelFX();

    void Begin(int client) override;
    void End() override;
    void WriteRenderMode(int rendermode) override;
    void WriteRenderAmt(int renderamt) override;
    void WriteRenderColor(int r, int g, int b) override;
    void WriteRenderFX(int renderfx) override;
    void WriteSkin(int skin) override;
    void WriteBody(int body) override;

private:
    void StateSet(VFX state);
    bool StateIsSet(VFX state) const;
    void StateReset(VFX state);
};