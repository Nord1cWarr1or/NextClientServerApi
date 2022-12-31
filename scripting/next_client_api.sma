/*
 * Repository: https://github.com/Next21Team/NextClientServerApi
 */

#include <amxmodx>
#include <amxmisc>
#include <fakemeta>
#include <engine>
#include <next_client_api>

#pragma semicolon 1

#include <next_client_api/viewmodelfx>
#include <next_client_api/private_precache>

new g_SandboxCvarMsg;
new g_SetFovExMsg;

new eNclVersion:g_NextClientVersion[33];
new bool:g_IsClientApiReady[33];
new g_ClientApiReadyForward;

public native_ncl_is_client_api_ready(plugin_id, argc)
{
    new id = get_param(1);

    return g_IsClientApiReady[id];
}

public native_ncl_is_next_client(plugin_id, argc)
{
    new id = get_param(1);

    return g_NextClientVersion[id];
}

public native_ncl_sandbox_cvar_begin(plugin_id, argc)
{
    new id = get_param(1);

    engfunc(EngFunc_MessageBegin, MSG_ONE, g_SandboxCvarMsg, Float:{0.0,0.0,0.0}, id);
    write_byte(255);
}

public native_ncl_sandbox_cvar_end(plugin_id, argc)
{
    message_end();
}

public native_ncl_write_sandbox_cvar(plugin_id, argc)
{
    new eSandboxCvar:cvar_id = eSandboxCvar:get_param(1);
    new value[64];
    get_string(2, value, charsmax(value));

    write_byte(cvar_id);
    write_string(value);
}

public native_ncl_setfov(plugin_id, argc)
{
    new id = get_param(1);
    new fov = get_param(2);
    new Float:lerp_time = get_param_f(3);

    engfunc(EngFunc_MessageBegin, MSG_ONE, g_SetFovExMsg, Float:{0.0,0.0,0.0}, id);
    write_byte(fov);
    write_long(_:lerp_time);
    message_end();
}

public plugin_natives()
{
    register_native("ncl_is_client_api_ready", "native_ncl_is_client_api_ready");
    register_native("ncl_is_next_client", "native_ncl_is_next_client");

    register_native("ncl_write_sandbox_cvar", "native_ncl_write_sandbox_cvar");
    register_native("ncl_sandbox_cvar_begin", "native_ncl_sandbox_cvar_begin");
    register_native("ncl_sandbox_cvar_end", "native_ncl_sandbox_cvar_end");

    register_native("ncl_setfov", "native_ncl_setfov");

    viewmodelfx_register_natives();
    private_precache_init();
}

public plugin_init()
{
    register_plugin("Next Client API", "1.0.3-beta", "Next21 Team");
    
    g_SandboxCvarMsg = register_custom_message("SandboxCvar");
    viewmodelfx_set_messageid(register_custom_message("ViewModelFx"));
    g_SetFovExMsg = register_custom_message("SetFOVEx");

    g_ClientApiReadyForward = CreateMultiForward("ncl_client_api_ready", ET_IGNORE, FP_CELL);
}

public client_connect(id)
{
    new keyValData[3];
    engfunc(EngFunc_InfoKeyValue, engfunc(EngFunc_GetInfoKeyBuffer, id), "_ncl", keyValData, charsmax(keyValData));

    new eNclVersion:version = NCLV_NOT_NEXT_CLIENT;
    if (keyValData[0] == '1' && keyValData[1] == '8')
        version = NCLV_2_1_8;
    else if (keyValData[0] == '1')
        version = NCLV_2_1_7_OR_LOWER;

    g_NextClientVersion[id] = version; 

    if (g_NextClientVersion[id] >= NCLV_2_1_8)
        private_precache_send_res_location(id);
}

public client_putinserver(id)
{
    new data[1];
    data[0] = id;

    set_task(0.1, "invoke_client_api_ready_forward", 0, data, sizeof(data));
}

public client_disconnected(id, bool:drop, message[], maxlen)
{
    g_NextClientVersion[id] = NCLV_NOT_NEXT_CLIENT;
    g_IsClientApiReady[id] = false;
}

public invoke_client_api_ready_forward(arg[], id)
{
    new id = arg[0];

    g_IsClientApiReady[id] = is_user_connected(id) && g_NextClientVersion[id];
    
    if (g_IsClientApiReady[id])
    {
        new ret_val;
        ExecuteForward(g_ClientApiReadyForward, ret_val, id);
    }
}

register_custom_message(const message[])
{
    new msgid = get_user_msgid(message);

    if (msgid == 0)
        msgid = engfunc(EngFunc_RegUserMsg, message, -1);

    if (msgid == 0)
        log_error(AMX_ERR_GENERAL, "Can't register %s message", message);
    
    return msgid;
}
