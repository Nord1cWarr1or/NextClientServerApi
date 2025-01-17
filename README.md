# Description
NextClientServerApi is an amxmodx module that provides an API to interact with [Counter-Strike 1.6 NextClient](https://cs16nextclient.com).

See API functions in [next_client_api.inc](https://github.com/Next21Team/NextClientServerApi/blob/main/addons/amxmodx/scripting/include/next_client_api.inc).

# Features

- The cvar sandbox. Allows you to set the value of a cvar to the client and locks it to be changed by the client. cvar value on the client is reset to the user's value after disconnecting from the server.
- Effects for viewmodel. Allows you to set rendering effects for viewmodels, as well as skin and body.
- Sprite management api. Allows you to display sprites on the screen.
- Extended fov message. Allows you to set the interpolation time of the fov change.
- Override of deathmsg weapon sprites. Allows you to customize weapon icons in kill feed.

Coming soon:
- Separate precaching for regular cs 1.6 client and NextClient.
- Precaching hud.txt and default sprites.

> 💎 **USING OF THE API TO GRANT PRIVILEGES** <br />
Since `NextClientServerApi 1.4.0` we introduced a verification method for players playing with nextclient by using `ncl_is_using_nextclient` native and some [RSA public keys](https://github.com/Next21Team/NextClientServerApi/tree/main/addons/amxmodx/data/nextclient_api/pkeys) which should be installed on the server to make verification work.<br />
The verification process is carried out only by nextclient since version `2.3.0`, where support for the new client-server protocol was introduced. For earlier versions, it is unfortunately impossible to reliably determine whether a player uses nextclient.<br />
Here is an example of using verification to provide access flags for players playing with updated nextclient:
```c++
#include <amxmodx>
#include <next_client_api>

const VIP_FLAGS[] = "f"; // the vip flags we are providing

public ncl_client_api_ready(id) {
	// Check that the player has verified nextclient
	if(ncl_is_using_nextclient(id) == NCL_USING_VERIFICATED)
		set_user_flags(id, read_flags(VIP_FLAGS));
}
```

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

## Building

Building NextClientServerApi requires CMake 3.10+ and GCC or MSVC compiler with C++11 support. Tested compilers are:

* GCC 9.4.0
* MSVC 2019

Building the library is done using CMake. You can run the CMake GUI to configure the library or use the command line:

```
mkdir Release
cd Release
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .. --target nextclientapi_amxx
```
