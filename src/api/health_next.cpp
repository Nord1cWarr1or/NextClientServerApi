#include "health_next.h"
#include "utilfuncs.h"

constexpr auto kCommonErrorString = "extended hud health (more than 255) will not work";

CHealthNext* CHealthNext::instance_ = nullptr;

CHealthNext* CHealthNext::GetInstance()
{
    if (instance_ == nullptr)
        instance_ = new CHealthNext();

    return instance_;
}

void CHealthNext::DestroyInstance()
{
    if (instance_ != nullptr)
        delete instance_;
}

CHealthNext::~CHealthNext()
{
    instance_ = nullptr;

    if (message_begin_hook_ != nullptr)
        funchook_destroy(message_begin_hook_);
}

void CHealthNext::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax)
{
    message_HealthNEx_ = utils::RegUserMsgSafe("HealthNEx", -1);
    message_Health_ = GET_USER_MSG_ID(&Plugin_info, "Health", NULL);

    if (!hooks_installed_)
        hooks_installed_ = SetupMessagesHooks();
}

bool CHealthNext::SetupMessagesHooks()
{
    message_begin_hook_ = utils::CreateAndEnableHook((void*)g_engfuncs.pfnMessageBegin, (void*)CHealthNext::PF_MessageBegin_I_HookStatic, (void**)&message_begin_trampoline_);
    if (message_begin_hook_ == nullptr)
    {
        MF_Log("Failed create hook for pfnMessageBegin, %s.", kCommonErrorString);
        return false;
    }

    message_end_hook_ = utils::CreateAndEnableHook((void*)g_engfuncs.pfnMessageEnd, (void*)CHealthNext::PF_MessageEnd_I_HookStatic, (void**)&message_end_trampoline_);
    if (message_end_hook_ == nullptr)
    {
        funchook_destroy(message_begin_hook_);
        message_begin_hook_ = nullptr;

        MF_Log("Failed create hook for pfnMessageEnd, %s.", kCommonErrorString);
        return false;
    }

    return true;
}

void CHealthNext::PF_MessageBegin_I_Hook(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
    if (ed != nullptr
        && NAPI()->GetNextClientVersion(ENTINDEX(ed)) >= NextClientVersion::V_2_2_0
        && msg_type == message_Health_
        && ed->v.health > 255)
    {
        should_send_health_ex_ = true;

        msg_save_hp_ = ed->v.health;
        msg_save_dest_ = msg_dest;
        msg_save_ed_ = ed;
        if (pOrigin)
            memcpy(msg_save_origin_, pOrigin, sizeof(msg_save_origin_));
        else
            memset(msg_save_origin_, 0, sizeof(msg_save_origin_));
    }

    message_begin_trampoline_(msg_dest, msg_type, pOrigin, ed);
}

void CHealthNext::PF_MessageEnd_I_Hook()
{
    message_end_trampoline_();

    if (should_send_health_ex_)
    {
        should_send_health_ex_ = false;

        message_begin_trampoline_(msg_save_dest_, message_HealthNEx_, msg_save_origin_, msg_save_ed_);
        WRITE_LONG(std::min(msg_save_hp_, 999)); // hotfix: we limit it to 999, because as it turned out at the moment the client cannot display more than 999 correctly
        message_end_trampoline_();
    }
}

void CHealthNext::PF_MessageBegin_I_HookStatic(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
    instance_->PF_MessageBegin_I_Hook(msg_dest, msg_type, pOrigin, ed);
}

void CHealthNext::PF_MessageEnd_I_HookStatic()
{
    instance_->PF_MessageEnd_I_Hook();
}
