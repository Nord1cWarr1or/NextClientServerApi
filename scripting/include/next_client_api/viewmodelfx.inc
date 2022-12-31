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

static g_ViewModelFxMsg;

static bool:g_IsViewModelMessageStarted;
static g_ViewModelData[structViewModelFx];

public viewmodelfx_register_natives()
{
    register_native("ncl_write_rendermode", "native_ncl_write_rendermode");
    register_native("ncl_write_renderamt", "native_ncl_write_renderamt");
    register_native("ncl_write_rendercolor", "native_ncl_write_rendercolor");
    register_native("ncl_write_renderfx", "native_ncl_write_renderfx");
    register_native("ncl_write_renderskin", "native_ncl_write_renderskin");
    register_native("ncl_write_renderbody", "native_ncl_write_renderbody");
    register_native("ncl_viewmodelfx_begin", "native_ncl_viewmodelfx_begin");
    register_native("ncl_viewmodelfx_end", "native_ncl_viewmodelfx_end");
}

public viewmodelfx_set_messageid(viewmodelfx_msg)
{
    g_ViewModelFxMsg = viewmodelfx_msg;
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

#undef VMFX_MODE_SET
#undef VMFX_MODE_RESET