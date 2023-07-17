#pragma once
#include <funchook.h>
#include "events.h"

class CHealthNext : public IEventServerActivated
{
    int message_HealthNEx_{};
    int message_Health_{};

    bool hooks_installed_{};
    funchook_t* message_begin_hook_{};
    funchook_t* message_end_hook_{};
    decltype(enginefuncs_s::pfnMessageBegin) message_begin_trampoline_{};
    decltype(enginefuncs_s::pfnMessageEnd) message_end_trampoline_{};

    bool should_send_health_ex_{};
    int msg_save_hp_{};
    int msg_save_dest_{};
    float msg_save_origin_[3]{};
    edict_t* msg_save_ed_{};

    static CHealthNext* instance_;

public:
    static CHealthNext* GetInstance();
    static void DestroyInstance();
    ~CHealthNext();

    void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) override;

private:
    explicit CHealthNext() = default;
    CHealthNext(const CHealthNext&) = delete;
    CHealthNext& operator=(const CHealthNext&) = delete;
    CHealthNext(CHealthNext&&) = delete;
    CHealthNext& operator=(CHealthNext&&) = delete;

    bool SetupMessagesHooks();
    void PF_MessageBegin_I_Hook(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
    void PF_MessageEnd_I_Hook();

    static void PF_MessageBegin_I_HookStatic(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
    static void PF_MessageEnd_I_HookStatic();
};
