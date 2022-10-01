/*
 * Repository: https://github.com/Next21Team/NextClientServerApi
 */

#include <amxmodx>
#include <fakemeta>
#include <engine>
#include <next_client_api>

new g_SandboxCvarMsg;

new bool:g_IsNextClient[33];
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

    return g_IsNextClient[id];
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

public plugin_natives()
{
    register_native("ncl_is_client_api_ready", "native_ncl_is_client_api_ready");
    register_native("ncl_is_next_client", "native_ncl_is_next_client");
    register_native("ncl_sandbox_cvar_begin", "native_ncl_sandbox_cvar_begin");
    register_native("ncl_sandbox_cvar_end", "native_ncl_sandbox_cvar_end");
    register_native("ncl_write_sandbox_cvar", "native_ncl_write_sandbox_cvar");
}

public plugin_init()
{
    register_plugin("Next Client API", "1.0.0", "Next21 Team");
    register_sandbox_message();

    g_ClientApiReadyForward = CreateMultiForward("ncl_client_api_ready", ET_IGNORE, FP_CELL);
}

public client_connect(id)
{
    new keyValData[2];
    engfunc(EngFunc_InfoKeyValue, engfunc(EngFunc_GetInfoKeyBuffer, id), "_ncl", keyValData, charsmax(keyValData));

    g_IsNextClient[id] = keyValData[0] == '1';
}

public client_putinserver(id)
{
    new data[1];
    data[0] = id;

    set_task(0.1, "invoke_client_api_ready_forward", 0, data, sizeof(data));
}

public client_disconnected(id, bool:drop, message[], maxlen)
{
    g_IsNextClient[id] = false;
    g_IsClientApiReady[id] = false;
}

public invoke_client_api_ready_forward(arg[], id)
{
    new id = arg[0];

    g_IsClientApiReady[id] = is_user_connected(id) && g_IsNextClient[id];
    
    if (g_IsClientApiReady[id])
    {
        new ret_val;
        ExecuteForward(g_ClientApiReadyForward, ret_val, id);
    }
}

register_sandbox_message()
{
    g_SandboxCvarMsg = get_user_msgid("SandboxCvar");

    if (g_SandboxCvarMsg == 0)
        g_SandboxCvarMsg = engfunc(EngFunc_RegUserMsg, "SandboxCvar", -1);

    if (g_SandboxCvarMsg == 0)
        log_error(AMX_ERR_GENERAL, "Can't register SandboxCvar message");
}
