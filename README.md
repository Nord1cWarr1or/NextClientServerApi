# Description
NextClientServerApi is an amxmodx plugin that provides an API to interact with [Counter-Strike 1.6 NextClient](https://client.next21.ru/).

See API functions in [next_client_api.inc](https://github.com/Next21Team/NextClientServerApi/blob/main/scripting/include/next_client_api.inc).

# Features

- The cvar sandbox. Allows you to set the value of a cvar to the client and locks it to be changed by the client. cvar value on the client is reset to the user's value after disconnecting from the server.
- Effects for viewmodel. Allows you to set rendering effects for viewmodels, as well as skin and body.

# NextClient specific cvars

| Cvar name | Default value | Min Value | Max Value | Description |
| --- | --- | --- | --- | --- |
| viewmodel_disable_shift | 0 | 0 | 1 | Disable viewmodel shifting (when you looking up or down) |
| viewmodel_offset_x | 0 | no limit | no limit |  |
| viewmodel_offset_y | 0 | no limit | no limit |  |
| viewmodel_offset_z | 0 | no limit | no limit |  |
| cl_bobstyle | 0 | 0 | 2 |  |
| cl_bobamt_vert | 0.13 | no limit | no limit |  |
| cl_bobamt_lat | 0.32 | no limit | no limit |  |
| cl_bob_lower_amt | 8 | no limit | no limit |  |
| cl_rollangle | 0 | no limit | no limit | Screen roll angle when strafing or looking (Quake effect) |
| cl_rollspeed | 200 | no limit | no limit | Screen roll speed when strafing or looking (Quake effect) |
| viewmodel_lag_scale | 0 | no limit | no limit | The value of the lag of the viewmodel from the crosshair (CS:GO effect) |
| viewmodel_lag_speed | 8 | no limit | no limit | The speed of the viewmodel following the crosshair (CS:GO effect) |
