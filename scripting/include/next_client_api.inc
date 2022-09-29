#if defined _next_client_api_included
#endinput
#endif
#define _next_client_api_included

enum eSandboxCvar
{
    SC_cl_forwardspeed = 0,
    SC_cl_backspeed,
    SC_cl_sidespeed,
    SC_sensitivity,
    SC_gl_fog,
    SC_cl_minmodels,
    SC_viewmodel_disable_shift,
    SC_viewmodel_offset_x,
    SC_viewmodel_offset_y,
    SC_viewmodel_offset_z,
    SC_cl_bobstyle,
    SC_cl_bobcycle,
    SC_cl_bobup,
    SC_cl_bob,
    SC_cl_bobamt_vert,
    SC_cl_bobamt_lat,
    SC_cl_bob_lower_amt,
    SC_cl_rollangle,
    SC_cl_rollspeed,
    SC_viewmodel_lag_scale,
    SC_viewmodel_lag_speed
}

native nclapi_is_next_client(id);

native nclapi_sandbox_cvar_begin(id);
native nclapi_write_sandbox_cvar(eSandboxCvar:cvar, const value[]);
native nclapi_sandbox_cvar_end();
