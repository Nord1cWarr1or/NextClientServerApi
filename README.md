# Description
NextClientServerApi is an amxmodx plugin that provides an API to interact with [Counter-Strike 1.6 NextClient](https://cs16nextclient.com).

See API functions in [next_client_api.inc](https://github.com/Next21Team/NextClientServerApi/blob/main/scripting/include/next_client_api.inc).

# Features

- The cvar sandbox. Allows you to set the value of a cvar to the client and locks it to be changed by the client. cvar value on the client is reset to the user's value after disconnecting from the server.
- Effects for viewmodel. Allows you to set rendering effects for viewmodels, as well as skin and body.
- Separate precaching for regular cs 1.6 client and NextClient.
- Precaching hud.txt and default sprites.

> ⚠️ **PLEASE DO NOT USE THE API TO GRANT PRIVILEGES** ⚠️ <br />
With the current implementation of the NextСlient authentication we cannot guarantee that `ncl_is_next_client`, `ncl_client_api_ready`, or `ncl_is_client_api_ready` will only work on genuine NextClients. Even a normal client without modifications can introduce itself as NextClient. Use these natives and forwards only to use existing API features like the cvars sandbox, viewmodel effects, etc.<br /><br />
In the near future, we will provide a reliable authentication method for NextClient, which can be trusted and used to grant game privileges or other things related to trust.

# NextClient specific cvars

| Cvar name | Default value | Min value | Max value | Available in sandbox | Description |
| --- | --- | --- | --- | --- | --- |
| viewmodel_disable_shift | 0 | 0 | 1 | Yes | Disable viewmodel shifting (when you looking up or down). |
| viewmodel_offset_x | 0 | no limit | no limit | Yes |  |
| viewmodel_offset_y | 0 | no limit | no limit | Yes |  |
| viewmodel_offset_z | 0 | no limit | no limit | Yes |  |
| cl_bobstyle | 0 | 0 | 2 | Yes | 0 for default bob, 1 for old style bob and 2 for CS:GO style bob. |
| cl_bobamt_vert | 0\.13 | no limit | no limit | Yes | Vertical scale for CS:GO style bob. |
| cl_bobamt_lat | 0\.32 | no limit | no limit | Yes | Lateral scale for CS:GO style bob. |
| cl_bob_lower_amt | 8 | no limit | no limit | Yes | Specifies how much the viewmodel moves inwards for CS:GO style bob. |
| cl_rollangle | 0 | no limit | no limit | Yes | Screen roll angle when strafing or looking (Quake effect). |
| cl_rollspeed | 200 | no limit | no limit | Yes | Screen roll speed when strafing or looking (Quake effect). |
| viewmodel_lag_scale | 0 | no limit | no limit | Yes | The value of the lag of the viewmodel from the crosshair (CS:GO effect). |
| viewmodel_lag_speed | 8 | no limit | no limit | Yes | The speed of the viewmodel following the crosshair (CS:GO effect). |
| fov_horplus | 0 | 0 | 1 | No | Enables Hor+ scaling for FOV. Fixes the FOV when playing with aspect ratios besides 4:3. |
| fov_angle | 90 | 70 | 100 | No (use ncl_setfov instead) |  |
| fov_lerp | 0 | 0 | no limit | No (use ncl_setfov instead) | FOV interpolation time in seconds. |
