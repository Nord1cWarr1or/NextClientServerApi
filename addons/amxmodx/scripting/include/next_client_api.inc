#if defined _next_client_api_included
    #endinput
#endif
#define _next_client_api_included

#if AMXX_VERSION_NUM >= 175
#pragma reqlib nextclientapi
 #if !defined AMXMODX_NOAUTOLOAD
  #pragma loadlib nextclientapi
 #endif
#else
#pragma library nextclientapi
#endif

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
    SC_cl_crosshair_translucent,
    // since Next Client v2.1.8
    SC_cl_weather,
}

enum eNclVersion
{
    NCLV_NOT_NEXT_CLIENT = 0,
    NCLV_2_1_7_OR_LOWER,
    NCLV_2_1_8,
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
 * @note Unreliable player identification! Use only to check features availability.
 *
 * @param id    Client index
 *
 * @return      Next Client version
 */
native eNclVersion:ncl_is_next_client(id);

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
 * To restore the user cvar value and unlock the client's ability to change cvar, pass in a value an empty string.
 *
 * @param cvar  Cvar index from eSandboxCvar
 * @param value Cvar value
 *
 * @noreturn
 */
native ncl_write_sandbox_cvar(eSandboxCvar:cvar, const value[]);


/**
 * Writes a render mode to a viewmodelfx message.
 *
 * @param rendermode One of kRender* constant
 *
 * @noreturn
 */
native ncl_write_rendermode(rendermode);

/**
 * Writes a render amount to a viewmodelfx message.
 *
 * @param renderamt Render amount (0 to 255)
 *
 * @noreturn
 */
native ncl_write_renderamt(renderamt);

/**
 * Writes a render color to a viewmodelfx message.
 *
 * @param r The amount of red color (0 to 255)
 * @param g The amount of green color (0 to 255)
 * @param b The amount of blue color (0 to 255)
 *
 * @noreturn
 */
native ncl_write_rendercolor(r, g, b);

/**
 * Writes a rendering effect to a viewmodelfx message.
 *
 * @param renderfx One of kRenderFx* constants
 *
 * @noreturn
 */
native ncl_write_renderfx(renderfx);

/**
 * Writes a viewmodel skin to viewmodelfx message.
 *
 * @param renderskin     
 *
 * @noreturn
 */
native ncl_write_renderskin(renderskin);

/**
 * Writes a viewmodel body to viewmodelfx message.
 *
 * @param renderbody     
 *
 * @noreturn
 */
native ncl_write_renderbody(renderbody);

/**
 * Marks the beginning of a viewmodelfx message.
 * You can write the message parameters in any order. 
 * It is also not necessary to specify all the parameters, write down only those you want to change.
 *
 * @note Be careful, this function does not work in client_connect, client_putinserver, etc. If you want to change player viewmodelfx after connecting, call this function in forward ncl_client_api_ready.
 *
 * @param id    Client index
 *
 * @noreturn
 */
native ncl_viewmodelfx_begin(id);

/**
 * Ends viewmodelfx message that was started with ncl_viewmodelfx_begin().
 * If the function is called without using ncl_viewmodelfx_begin() error log will be printed.
 *
 * @param id    Client index
 *
 * @noreturn
 */
native ncl_viewmodelfx_end();

/**
 * This message sets the specified field of view and interpolate time.
 *
 * @note Be careful, this function does not work in client_connect, client_putinserver, etc. If you want to change player fov after connecting, call this function in forward ncl_client_api_ready.
*
 * @param id    Client index
 * @param fov   FOV angle
 * @param time  Inertpolate time
 *
 * @noreturn
 */
native ncl_setfov(id, fov, Float:time);

/**
 * Separate precaching of the model for regular cs 1.6 client and NextClient.
 * For NextClient: 
 * - allows to overwrite default models;
 * - allows to overwrite an existing model when it changed on the server.
 *
 * @note Can only be used inside of the plugin_precache() forward.
 * 
 * @param filepath       Path to the model file
 * @param ncl_filepath   Path to the model file which will be preached to NextClient players (will be saved as filepath)
 *
 * @return               Unique cache id of the model
 * @error                If called outside of the plugin_precache() forward, an error is thrown.
 */
native ncl_precache_model(const filepath[], const ncl_filepath[]);

/**
 * Separate precaching of the sound for regular cs 1.6 client and NextClient. 
 * For NextClient: 
 * - allows to overwrite default sounds;
 * - allows to overwrite an existing sound when it changed on the server.
 *
 * @note Can only be used inside of the plugin_precache() forward.
 * 
 * @param filepath       Path to the sound file
 * @param ncl_filepath   Path to the sound file which will be preached to NextClient players (will be saved as filepath)
 *
 * @return               Unique cache id of the sound
 * @error                If called outside of the plugin_precache() forward, an error is thrown.
 */
native ncl_precache_sound(const filepath[], const ncl_filepath[]);

/**
 * Client only precaching, allows to override default resources, such hud.txt and default sprites.
 *
 * @note For security reason at this moment allows to precache only hud.txt and sprites.
 * 
 * @param filepath       Saving path on client (regarding the mod directory)
 * @param ncl_filepath   Downloading path on server (regarding the mod directory)
 *
 * @noreturn
 * @error                If called outside of the plugin_precache() forward, an error is thrown.
 */
native ncl_precache_client_only(const filepath[], const ncl_filepath[]);

stock ncl_precache_hudtxt(const ncl_filepath[])
{
    ncl_precache_client_only("sprites/hud.txt", ncl_filepath);
}
