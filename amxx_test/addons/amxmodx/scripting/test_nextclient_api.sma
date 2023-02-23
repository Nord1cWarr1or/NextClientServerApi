#include <amxmodx>
#include <fakemeta>
#include <reapi>

#include <next_client_api>

new LOG_FILE[] = "test_nextclient_api.log";

// Uncomment this if you do not want to check the values of the sandbox CVars yourself
#define AUTO_RESTORE_CVAR_VALUES

#define TEST_PRECACHE_AND_REPLACE_DEFAULT_MODELS

#define TEST_PRECACHE_AND_REPLACE_DEFAULT_SOUNDS

// Test of replace default hud.txt and HUD sprites
#define TEST_PRECACHE_AND_REPLACE_HUD

#define TEST_PRECACHE_AND_REPLACE_CUSTOM_MODELS

#define TEST_PRECACHE_AND_REPLACE_CUSTOM_SOUNDS

public plugin_init() {
    register_plugin("Test NCL API", "1.1.1", "Nordic Warrior");

    register_clcmd("ncl_is_next_client",            "cmd_ncl_is_next_client",           ADMIN_ALL);
    register_clcmd("ncl_is_client_api_ready",       "cmd_ncl_is_client_api_ready",      ADMIN_ALL);
    register_clcmd("ncl_test_sandbox_cvars",        "cmd_ncl_test_sandbox_cvars",       ADMIN_ALL);
    register_clcmd("ncl_test_viewmodelfx_skin",     "cmd_ncl_test_viewmodelfx_skin",    ADMIN_ALL);
    register_clcmd("ncl_test_viewmodelfx_body",     "cmd_ncl_test_viewmodelfx_body",    ADMIN_ALL);
    register_clcmd("ncl_test_viewmodelfx_render",   "cmd_ncl_test_viewmodelfx_render",  ADMIN_ALL);
    register_clcmd("ncl_restore_viewmodelfx",       "cmd_ncl_restore_viewmodelfx",      ADMIN_ALL);
    register_clcmd("ncl_setfov",                    "cmd_ncl_setfov",                   ADMIN_ALL);

    register_clcmd("ncl_restore_cvars_values", "cmd_ncl_restore_cvars_values", ADMIN_ALL);
}

/* <== FORWARDS ==> */

public ncl_client_api_ready(id) {
    log_to_file(LOG_FILE, "FORWARD <ncl_client_api_ready> called for player: %n", id);
    return PLUGIN_HANDLED;
}

/* <=== END FORWARDS ===> */

/* <== NATIVES ==> */

public cmd_ncl_is_next_client(id) {
    log_to_file(LOG_FILE, "NATIVE <ncl_is_next_client> testing called by player: %n", id);
    log_to_file(LOG_FILE, "Result: Is player NextClient? %s.", ncl_is_next_client(id) > NCLV_NOT_NEXT_CLIENT ? "Yes" : "No");
    return PLUGIN_HANDLED;
}

/* <=======> */

public cmd_ncl_is_client_api_ready(id) {
    log_to_file(LOG_FILE, "NATIVE <ncl_is_client_api_ready> testing called by player: %n", id);
    log_to_file(LOG_FILE, "Result: API ready? %s.", ncl_is_client_api_ready(id)  ? "Yes" : "No");
    return PLUGIN_HANDLED;
}

/* <=======> */

new const CVAR_STRING[eSandboxCvar][] = {
    "cl_forwardspeed",          "cl_backspeed",         "cl_sidespeed",
    "sensitivity",              "gl_fog",               "cl_minmodels",
    "viewmodel_disable_shift",  "viewmodel_offset_x",   "viewmodel_offset_y",
    "viewmodel_offset_z",       "cl_bobstyle",          "cl_bobcycle",
    "cl_bobup",                 "cl_bob",               "cl_bobamt_vert",
    "cl_bobamt_lat",            "cl_bob_lower_amt",     "cl_rollangle",
    "cl_rollspeed",             "viewmodel_lag_scale",  "viewmodel_lag_speed",
    "cl_crosshair_type",        "cl_crosshair_size",    "cl_crosshair_color",
    "cl_crosshair_translucent", "cl_weather"
};

new const CVAR_VALUES[eSandboxCvar][] = {
    "888",      "888",      "888",
    "3.33",     "2",        "1",
    "1",        "1.11",     "1.11",
    "1.11",     "1",        "0.888",
    "0.66",     "0.011",    "0.333",
    "0.444",    "10",       "5",
    "300",      "6.66",     "9.99",
    "2",        "auto",     "177 177 177",
    "1",        "3"
};

public cmd_ncl_test_sandbox_cvars(id) {
    log_to_file(LOG_FILE, "SANDBOX CVAR testing called by player: %n", id);

    ncl_sandbox_cvar_begin(id);
    for(new eSandboxCvar:i; i < eSandboxCvar; i++) {
        ncl_write_sandbox_cvar(i, CVAR_VALUES[any:i]);
    }
    ncl_sandbox_cvar_end();

    for(new eSandboxCvar:i; i < eSandboxCvar; i++) {
        query_client_cvar(id, CVAR_STRING[i], "checkCvarValue");
    }

    return PLUGIN_HANDLED;
}

public checkCvarValue(id, const cvar[], const value[], const param[]) {
    static any:successfulTest;
    static any:testsPassed;

    for(new eSandboxCvar:i; i < eSandboxCvar; i++) {
        if(strcmp(cvar, CVAR_STRING[i]) == 0) {
            log_to_file(LOG_FILE, "CVAR <%s> EXPECTED VALUE: %s ACTUAL VALUE: %s.", cvar, CVAR_VALUES[i], value);

            if(strcmp(value, CVAR_VALUES[i]) == 0) {
                successfulTest++;
            }
        }
    }

    testsPassed++;

    if(successfulTest == eSandboxCvar && testsPassed == eSandboxCvar) {
        log_to_file(LOG_FILE, "<SANDBOX CVAR testing was SUCCESSFUL>", id);
        successfulTest = 0;
        testsPassed = 0;

        #if defined AUTO_RESTORE_CVAR_VALUES
        cmd_ncl_restore_cvars_values(id);
        #endif
    }
    else if(successfulTest != eSandboxCvar && testsPassed == eSandboxCvar) {
        log_to_file(LOG_FILE, "[!] SANDBOX CVAR testing FAILED [!]", id);
        successfulTest = 0;
        testsPassed = 0;

        #if defined AUTO_RESTORE_CVAR_VALUES
        cmd_ncl_restore_cvars_values(id);
        #endif
    }
}

public cmd_ncl_restore_cvars_values(id) {
    log_to_file(LOG_FILE, "CVARS from SANDBOX are restored for player: %n", id);

    ncl_sandbox_cvar_begin(id);
    for(new eSandboxCvar:i; i < eSandboxCvar; i++) {
        ncl_write_sandbox_cvar(i, "");
    }
    ncl_sandbox_cvar_end();

    return PLUGIN_HANDLED;
}

/* <=======> */

new const TEST_SKIN_MODEL[] = "models/v_ak47_with_skins.mdl";
const SKIN = 1;

public cmd_ncl_test_viewmodelfx_skin(id) {
    log_to_file(LOG_FILE, "NATIVE <ncl_write_renderbody> testing called by player: %n", id);
    log_to_file(LOG_FILE, "* You should manually check a skin changing of view model. If you see not a standart model, the test was successful.");

    rg_give_item(id, "weapon_ak47");
    engclient_cmd(id, "weapon_ak47");
    set_entvar(id, var_viewmodel, TEST_SKIN_MODEL);

    ncl_viewmodelfx_begin(id);
    ncl_write_renderskin(SKIN);
    ncl_viewmodelfx_end();

    return PLUGIN_HANDLED;
}

/* <=======> */

new const TEST_BODY_MODEL[] = "models/v_m4a1_with_body.mdl";
const BODY = 7;

public cmd_ncl_test_viewmodelfx_body(id) {
    log_to_file(LOG_FILE, "NATIVE <ncl_write_renderbody> testing called by player: %n", id);
    log_to_file(LOG_FILE, "* You should manually check a body changing of view model. If you see not a standart model, the test was successful.");

    rg_give_item(id, "weapon_m4a1");
    engclient_cmd(id, "weapon_m4a1");
    set_entvar(id, var_viewmodel, TEST_BODY_MODEL);

    ncl_viewmodelfx_begin(id);
    ncl_write_renderbody(BODY);
    ncl_viewmodelfx_end();

    return PLUGIN_HANDLED;
}

/* <=======> */

public cmd_ncl_test_viewmodelfx_render(id) {
    log_to_file(LOG_FILE, "NATIVES <ncl_write_renderfx, ncl_write_rendercolor, ncl_write_rendermode, ncl_write_renderamt> testing called by player: %n", id);
    log_to_file(LOG_FILE, "* You should manually check the glowing of view model.");

    ncl_viewmodelfx_begin(id);
    ncl_write_renderfx(kRenderFxGlowShell);
    ncl_write_rendercolor(255, 0, 0);
    ncl_write_rendermode(kRenderNormal);
    ncl_write_renderamt(10);
    ncl_viewmodelfx_end();

    return PLUGIN_HANDLED;
}

/* <=======> */

public cmd_ncl_restore_viewmodelfx(id) {
    log_to_file(LOG_FILE, "All viewmodelfx have been reset for player: %n", id);

    ncl_viewmodelfx_begin(id);
    ncl_write_renderfx(kRenderFxNone);
    ncl_write_rendercolor(0, 0, 0);
    ncl_write_rendermode(kRenderNormal);
    ncl_write_renderamt(0);
    ncl_write_renderskin(0);
    ncl_write_renderbody(0);
    ncl_viewmodelfx_end();

    return PLUGIN_HANDLED;
}

/* <=======> */
const Float:FOV_TIME = 2.0;

public cmd_ncl_setfov(id) {
    log_to_file(LOG_FILE, "NATIVE <ncl_setfov> testing called by player: %n", id);
    log_to_file(LOG_FILE, "* You should manually check a FOV changing.");

    ncl_setfov(id, 120, FOV_TIME);
    set_task(FOV_TIME, "restoreFOV", id);

    return PLUGIN_HANDLED;
}

public restoreFOV(id) {
    log_to_file(LOG_FILE, "FOV automatically restored to 90 for player: %n", id);

    ncl_setfov(id, 90, FOV_TIME);
}

/* <=== END NATIVES ===> */

/* <== PRECACHE ==> */

public plugin_precache() {
    precache_model(TEST_SKIN_MODEL);
    precache_model(TEST_BODY_MODEL);

#if defined TEST_PRECACHE_AND_REPLACE_DEFAULT_MODELS
    test_ncl_precache_and_replace_default_model();
#endif

#if defined TEST_PRECACHE_AND_REPLACE_DEFAULT_SOUNDS
    test_ncl_precache_and_replace_default_sound();
#endif

#if defined TEST_PRECACHE_AND_REPLACE_HUD
    test_ncl_precache_client_only();
#endif

#if defined TEST_PRECACHE_AND_REPLACE_CUSTOM_MODELS
    test_ncl_precache_and_replace_custom_model();
#endif

#if defined TEST_PRECACHE_AND_REPLACE_CUSTOM_SOUNDS
    test_ncl_precache_and_replace_custom_sound();
#endif
}

/* <=======> */

public test_ncl_precache_and_replace_default_model() {
    log_to_file(LOG_FILE, "~ Testing ncl_precache_model() for DEFAULT models is active.");

    ncl_precache_model("models/v_glock18.mdl",  "models/test_nextclient/v_glock18.mdl"  );
    ncl_precache_model("models/v_usp.mdl",      "models/test_nextclient/v_usp.mdl"      );
}

/* <=======> */

public test_ncl_precache_and_replace_default_sound() {
    log_to_file(LOG_FILE, "~ Testing ncl_precache_sound() for DEFAULT sounds is active.");

    ncl_precache_sound("weapons/glock18-1.wav",   "weapons/test_nextclient/glock18-1.wav"   );
    ncl_precache_sound("weapons/glock18-2.wav",   "weapons/test_nextclient/glock18-2.wav"   );
    ncl_precache_sound("weapons/usp_unsil-1.wav", "weapons/test_nextclient/usp_unsil-1.wav" );
    ncl_precache_sound("weapons/usp_unsil-2.wav", "weapons/test_nextclient/usp_unsil-2.wav" );
    ncl_precache_sound("weapons/usp1.wav",        "weapons/test_nextclient/usp1.wav"        );
    ncl_precache_sound("weapons/usp2.wav",        "weapons/test_nextclient/usp2.wav"        );

    ncl_precache_sound("player/pl_step1.wav", "player/test_nextclient/pl_step1.wav" );
    ncl_precache_sound("player/pl_step2.wav", "player/test_nextclient/pl_step2.wav" );
    ncl_precache_sound("player/pl_step3.wav", "player/test_nextclient/pl_step3.wav" );
    ncl_precache_sound("player/pl_step4.wav", "player/test_nextclient/pl_step4.wav" );
}

/* <=======> */

public test_ncl_precache_client_only() {
    log_to_file(LOG_FILE, "~ Testing ncl_precache_client_only() for hud.txt and HUD sprites is active.");

    ncl_precache_client_only("sprites/TBW1.spr",    "sprites/test_nextclient/TBW1.spr"  );
    ncl_precache_client_only("sprites/TBW2.spr",    "sprites/test_nextclient/TBW2.spr"  );
    ncl_precache_client_only("sprites/TBW3.spr",    "sprites/test_nextclient/TBW3.spr"  );
    ncl_precache_client_only("sprites/TBW4.spr",    "sprites/test_nextclient/TBW4.spr"  );
    ncl_precache_client_only("sprites/TBW5.spr",    "sprites/test_nextclient/TBW5.spr"  );
    ncl_precache_client_only("sprites/TBW6.spr",    "sprites/test_nextclient/TBW6.spr"  );
    ncl_precache_client_only("sprites/TBW7.spr",    "sprites/test_nextclient/TBW7.spr"  );
    ncl_precache_client_only("sprites/TDW.spr",     "sprites/test_nextclient/TDW.spr"   );
    ncl_precache_client_only("sprites/THUD.spr",    "sprites/test_nextclient/THUD.spr"  );

    ncl_precache_client_only("sprites/radar640.spr",        "sprites/test_nextclient/radar640.spr"          ); 
    ncl_precache_client_only("sprites/radaropaque640.spr",  "sprites/test_nextclient/radaropaque640.spr"    );

    for(new WeaponIdType:weaponID = WEAPON_P228; weaponID <= WEAPON_P90; weaponID++) {
        if(weaponID == WEAPON_GLOCK) {
            continue;
        }

        new weaponName[MAX_NAME_LENGTH];
        rg_get_weapon_info(weaponID, WI_NAME, weaponName, charsmax(weaponName));

        ncl_precache_client_only(fmt("sprites/%s.txt", weaponName), fmt("sprites/test_nextclient/%s.txt", weaponName));\
    }

    ncl_precache_client_only("sprites/weapon_shieldgun.txt", "sprites/test_nextclient/weapon_shieldgun.txt");

    ncl_precache_hudtxt("sprites/test_nextclient/hud.txt");
}

/* <=======> */

new g_chickModelIndex;

public test_ncl_precache_and_replace_custom_model() {
    log_to_file(LOG_FILE, "~ Testing ncl_precache_model() for CUSTOM models is active.");

    g_chickModelIndex = ncl_precache_model("models/test_nextclient/chick.mdl", "models/test_nextclient/chick_HD.mdl");

    RegisterHookChain(RG_CBasePlayer_Spawn, "RG_CBasePlayer_Spawn_post", true);
}

public RG_CBasePlayer_Spawn_post(id) {
    if(!is_user_alive(id)) {
        return;
    }

    CreateChick(id);
}

CreateChick(id) {
    new chick = rg_create_entity("info_target");

    set_entvar(chick, var_modelindex, g_chickModelIndex);

    new Float:origin[3];
    get_CoordPointInDirView(id, 64.0, origin);

    engfunc(EngFunc_SetOrigin, chick, origin);
    engfunc(EngFunc_DropToFloor, chick);
}

// https://dev-cs.ru/threads/222/page-9#post-81453
stock get_CoordPointInDirView(const index, const Float:dist, Float:output[3]) {
    new Float:start_origins[3];
    pev(index, pev_origin, start_origins);

    new Float:angles[3];
    pev(index, pev_v_angle, angles);

    new Float:end_origins[3];
    end_origins[0] = start_origins[0] + dist * floatcos(angles[1], degrees) * floatabs(floatcos(angles[0], degrees));
    end_origins[1] = start_origins[1] + dist * floatsin(angles[1], degrees) * floatabs(floatcos(angles[0], degrees));
    end_origins[2] = start_origins[2];

    engfunc(EngFunc_TraceLine, start_origins, end_origins, DONT_IGNORE_MONSTERS, index, 0);
    get_tr2(0, TR_vecEndPos, end_origins);

    output[0] = end_origins[0];
    output[1] = end_origins[1];
    output[2] = end_origins[2];
}

/* <=======> */

public test_ncl_precache_and_replace_custom_sound() {
    log_to_file(LOG_FILE, "~ Testing ncl_precache_sound() for CUSTOM sounds is active.");
    
    ncl_precache_sound("test_nextclient/sound_for_all.wav", "test_nextclient/sound_for_nextclient.wav");

    set_task(1.0, "sendSound", .flags = "b");
}

public sendSound() {
    for(new player = 1; player <= MaxClients; player++) {
        if(!is_user_connected(player)) {
            continue;
        }

        rg_send_audio(player, "test_nextclient/sound_for_all.wav");
    }
}

/* <=== END PRECACHE ===> */
