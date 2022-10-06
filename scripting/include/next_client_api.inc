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
    SC_viewmodel_lag_speed,
    // since Next Client v2.1.4
    SC_cl_crosshair_type,
    SC_cl_crosshair_size,
    SC_cl_crosshair_color,
    SC_cl_crosshair_translucent
}

/**
 * Called when all api functionality is ready to use. Called only for Next Clients.
 *
 * @param id    Client index
 *
 * @noreturn
 */
forward ncl_client_api_ready(id);

/**
 * Checks if client is Next Client and its ready to interact with server api.
 *
 * @param id    Client index
 *
 * @return      True or false
 */
native bool:ncl_is_client_api_ready(id);

/**
 * Checks if client is Next Client. 
 *
 * @note Unreliable player identification, use only to check cvars sandbox abailability.
 *
 * @param id    Client index
 *
 * @return      True or false
 */
native bool:ncl_is_next_client(id);

/**
 * Marks the beginning of a sandbox cvar message.
 *
 * @note Be careful, this function does not work in client_connect, client_putinserver, etc. If you want to change player cvars after connecting, call this function in forward ncl_client_api_ready.
 *
 * @param id    Client index
 *
 * @noreturn
 */
native ncl_sandbox_cvar_begin(id);

/**
 * Ends sandbox cvar message that was started with ncl_sandbox_cvar_begin().
 * If the function is called without using ncl_sandbox_cvar_begin() first, the server will crash immediately.
 *
 * @param id    Client index
 *
 * @noreturn
 */
native ncl_sandbox_cvar_end();

/**
 * Writes a cvar value to a sandbox cvar message.
 *
 * @param cvar  Cvar index from eSandboxCvar
 * @param value Cvar value
 *
 * @noreturn
 */
native ncl_write_sandbox_cvar(eSandboxCvar:cvar, const value[]);
