#include "HealthNext.h"
#include "utilfuncs.h"
#include "main.h"

void HealthNext::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax)
{
    message_HealthNEx_ = utils::RegUserMsgSafe("HealthNEx", -1);
    message_Health_ = GET_USER_MSG_ID(&Plugin_info, "Health", NULL);
}

void HealthNext::OnMessageBeginPost(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
    if (msg_type == message_Health_
        && ed != nullptr
        && ed->v.health > 255
        && NAPI()->GetNextClientVersion(ENTINDEX(ed)) >= NextClientVersion::V_2_2_0)
    {
        should_send_health_ex_ = true;

        msg_save_hp_ = (int)ed->v.health;
        msg_save_dest_ = msg_dest;
        msg_save_ed_ = ed;
        if (pOrigin)
            memcpy(msg_save_origin_, pOrigin, sizeof(msg_save_origin_));
        else
            memset(msg_save_origin_, 0, sizeof(msg_save_origin_));
    }
}

void HealthNext::OnMessageEndPost()
{
    if (should_send_health_ex_)
    {
        should_send_health_ex_ = false;

        MESSAGE_BEGIN(msg_save_dest_, message_HealthNEx_, msg_save_origin_, msg_save_ed_);
        WRITE_LONG(std::min(msg_save_hp_, 999)); // hotfix: we limit it to 999, because as it turned out at the moment the client cannot display more than 999 correctly
        MESSAGE_END();
    }
}
