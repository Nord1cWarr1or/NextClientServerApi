#include <amxmodx>
#include <fakemeta>
#include <engine>
#include <next_client_api>

public plugin_init()
{
    register_plugin("Next Client API Example", "1.0.0", "Polarhigh");
}

public ncl_client_api_ready(id)
{
    set_task(6.0, "reset_vm_fx", id);
    set_task(6.0, "set_fovex", id);
}

send_vm_fx(id)
{
    ncl_sandbox_cvar_begin(id);
    ncl_write_sandbox_cvar(SC_cl_crosshair_color, "35 45 255");
    ncl_sandbox_cvar_end();

    ncl_viewmodelfx_begin(id);
    ncl_write_renderfx(kRenderFxGlowShell);
    ncl_write_renderamt(2);
    ncl_write_rendercolor(255, 0, 0);
    ncl_viewmodelfx_end();
}

public reset_vm_fx(id)
{
    ncl_viewmodelfx_begin(id);
    ncl_write_renderfx(kRenderFxNone);
    ncl_write_renderamt(0);
    ncl_write_rendercolor(0, 0, 0);
    ncl_viewmodelfx_end();
}

public set_fovex(id)
{
    ncl_setfov(id, 120, 2.0);
}
