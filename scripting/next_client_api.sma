/*
 * Repository: https://github.com/Next21Team/NextClientServerApi
 */

#include <amxmodx>
#include <fakemeta>
#include <engine>
#include <next_client_api>

new g_SandboxCvarMsg;
new g_ViewModelFxMsg;
new g_SetFovExMsg;

new bool:g_IsNextClient[33];
new bool:g_IsClientApiReady[33];
new g_ClientApiReadyForward;

#define VMFX_MODE_SET 1
#define VMFX_MODE_RESET 2

enum _:structViewModelFx
{
    VMFX_PLAYER_ID  = 0,

    VMFX_RENDERMODE,
    VMFX_RENDERAMT,
    VMFX_RENDERCOLOR[3],
    VMFX_RENDERFX,
    VMFX_RENDERSKIN,
    VMFX_RENDERBODY,

    VMFX_RENDERMODE_MODE,
    VMFX_RENDERAMT_MODE,
    VMFX_RENDERCOLOR_MODE,
    VMFX_RENDERFX_MODE,
    VMFX_RENDERSKIN_MODE,
    VMFX_RENDERBODY_MODE,
}

new bool:g_IsViewModelMessageStarted;
new g_ViewModelData[structViewModelFx];

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

public native_ncl_write_rendermode(plugin_id, argc)
{
    new rendermode = get_param(1);

    g_ViewModelData[VMFX_RENDERMODE] = rendermode;
    g_ViewModelData[VMFX_RENDERMODE_MODE] = rendermode == -1 ? VMFX_MODE_RESET : VMFX_MODE_SET;
}

public native_ncl_write_renderamt(plugin_id, argc)
{
    new renderamt = get_param(1);

    g_ViewModelData[VMFX_RENDERAMT] = renderamt;
    g_ViewModelData[VMFX_RENDERAMT_MODE] = renderamt == -1 ? VMFX_MODE_RESET : VMFX_MODE_SET;
}

public native_ncl_write_rendercolor(plugin_id, argc)
{
    new r = get_param(1);
    new g = get_param(2);
    new b = get_param(3);

    g_ViewModelData[VMFX_RENDERCOLOR][0] = r;
    g_ViewModelData[VMFX_RENDERCOLOR][1] = g;
    g_ViewModelData[VMFX_RENDERCOLOR][2] = b;
    g_ViewModelData[VMFX_RENDERCOLOR_MODE] = r == -1 ? VMFX_MODE_RESET : VMFX_MODE_SET;
}

public native_ncl_write_renderfx(plugin_id, argc)
{
    new renderfx = get_param(1);

    g_ViewModelData[VMFX_RENDERFX] = renderfx;
    g_ViewModelData[VMFX_RENDERFX_MODE] = renderfx == -1 ? VMFX_MODE_RESET : VMFX_MODE_SET;
}

public native_ncl_write_renderskin(plugin_id, argc) 
{
    new renderskin = get_param(1);

    g_ViewModelData[VMFX_RENDERSKIN] = renderskin;
    g_ViewModelData[VMFX_RENDERSKIN_MODE] = renderskin == -1 ? VMFX_MODE_RESET : VMFX_MODE_SET;
}

public native_ncl_write_renderbody(plugin_id, argc)
{
    new renderbody = get_param(1);

    g_ViewModelData[VMFX_RENDERBODY] = renderbody;
    g_ViewModelData[VMFX_RENDERBODY_MODE] = renderbody == -1 ? VMFX_MODE_RESET : VMFX_MODE_SET;
}

public native_ncl_viewmodelfx_begin(plugin_id, argc)
{
    if (g_IsViewModelMessageStarted)
    {
        log_error(AMX_ERR_GENERAL, "Calling ncl_viewmodelfx_begin again without first calling ncl_viewmodelfx_end.");
        return;
    }

    new id = get_param(1);

    g_IsViewModelMessageStarted = true;
    arrayset(g_ViewModelData, 0, structViewModelFx);

    g_ViewModelData[VMFX_PLAYER_ID] = id;
}

public native_ncl_viewmodelfx_end(plugin_id, argc)
{
    if (!g_IsViewModelMessageStarted)
    {
        log_error(AMX_ERR_GENERAL, "Calling ncl_viewmodelfx_end without first calling ncl_viewmodelfx_begin.");
        return;
    }

    g_IsViewModelMessageStarted = false;

    new what_set;
    new what_reset;

    for (new idx = VMFX_RENDERMODE_MODE, i; idx <= VMFX_RENDERBODY_MODE; idx++, i++)
    {
        if (g_ViewModelData[idx] == VMFX_MODE_SET)
            what_set |= (1 << i);
        else if (g_ViewModelData[idx] == VMFX_MODE_RESET)
            what_reset |= (1 << i);
    }

    if (what_reset)
        what_set |= (1 << 6);

    engfunc(EngFunc_MessageBegin, MSG_ONE, g_ViewModelFxMsg, Float:{0.0,0.0,0.0}, g_ViewModelData[VMFX_PLAYER_ID]);

    write_byte(what_set);

    if (what_reset)
        write_byte(what_reset);

    if (g_ViewModelData[VMFX_RENDERMODE_MODE] == VMFX_MODE_SET || g_ViewModelData[VMFX_RENDERFX_MODE] == VMFX_MODE_SET)
        write_byte(g_ViewModelData[VMFX_RENDERMODE] | (g_ViewModelData[VMFX_RENDERFX] << 3));

    if (g_ViewModelData[VMFX_RENDERAMT_MODE] == VMFX_MODE_SET)
        write_byte(g_ViewModelData[VMFX_RENDERAMT]);

    if (g_ViewModelData[VMFX_RENDERCOLOR_MODE] == VMFX_MODE_SET)
    {
        write_byte(g_ViewModelData[VMFX_RENDERCOLOR][0]);
        write_byte(g_ViewModelData[VMFX_RENDERCOLOR][1]);
        write_byte(g_ViewModelData[VMFX_RENDERCOLOR][2]);
    }

    if (g_ViewModelData[VMFX_RENDERSKIN_MODE] == VMFX_MODE_SET)
        write_byte(g_ViewModelData[VMFX_RENDERSKIN]);

    if (g_ViewModelData[VMFX_RENDERBODY_MODE] == VMFX_MODE_SET)
        write_long(g_ViewModelData[VMFX_RENDERBODY]);
    
    message_end();
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

    register_native("ncl_write_rendermode", "native_ncl_write_rendermode");
    register_native("ncl_write_renderamt", "native_ncl_write_renderamt");
    register_native("ncl_write_rendercolor", "native_ncl_write_rendercolor");
    register_native("ncl_write_renderfx", "native_ncl_write_renderfx");
    register_native("ncl_write_renderskin", "native_ncl_write_renderskin");
    register_native("ncl_write_renderbody", "native_ncl_write_renderbody");
    register_native("ncl_viewmodelfx_begin", "native_ncl_viewmodelfx_begin");
    register_native("ncl_viewmodelfx_end", "native_ncl_viewmodelfx_end");

    register_native("ncl_setfov", "native_ncl_setfov");
}

public plugin_init()
{
    register_plugin("Next Client API", "1.0.2", "Next21 Team");
    register_sandbox_message();
    register_viewmodelfx_message();
    register_fovex_message();

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

register_viewmodelfx_message()
{
    g_ViewModelFxMsg = get_user_msgid("ViewModelFx");

    if (g_ViewModelFxMsg == 0)
        g_ViewModelFxMsg = engfunc(EngFunc_RegUserMsg, "ViewModelFx", -1);

    if (g_ViewModelFxMsg == 0)
        log_error(AMX_ERR_GENERAL, "Can't register ViewModelFx message");
}

register_fovex_message()
{
    g_SetFovExMsg = get_user_msgid("SetFOVEx");

    if (g_SetFovExMsg == 0)
        g_SetFovExMsg = engfunc(EngFunc_RegUserMsg, "SetFOVEx", -1);

    if (g_SetFovExMsg == 0)
        log_error(AMX_ERR_GENERAL, "Can't register SetFOVEx message");
}
