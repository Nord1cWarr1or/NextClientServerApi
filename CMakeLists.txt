cmake_minimum_required(VERSION 3.0.0)

project(nextclientapi_amxx)

set(AMXXMODULE_SDK_MODULECONFIG_DIR
	${CMAKE_SOURCE_DIR}/public
)

add_subdirectory(deps/amxxmodule)
add_subdirectory(deps/cssdk)
add_subdirectory(deps/metamod)

set(SRC_DIR src)

add_library(${PROJECT_NAME} SHARED
	${AMXXMODULE_SDK_RUNTIME_SOURCE_FILE}
	${SRC_DIR}/api/impl.h
	${SRC_DIR}/api/impl.cpp
	${SRC_DIR}/api/cvar_sandbox.h
	${SRC_DIR}/api/cvar_sandbox.cpp
	${SRC_DIR}/api/private_precache.h
	${SRC_DIR}/api/private_precache.cpp
	${SRC_DIR}/natives/cvar_sandbox.cpp
	${SRC_DIR}/natives/private_precache.cpp
	${SRC_DIR}/natives/viewmodelfx.cpp
	${SRC_DIR}/natives/nextclient.cpp
	${SRC_DIR}/natives/natives.cpp
	${SRC_DIR}/main.h
	${SRC_DIR}/main.cpp
	${SRC_DIR}/utilfuncs.cpp

	${AMXXMODULE_SDK_INCLUDE_PATHS}
)

add_compile_definitions(HAVE_STDINT_H)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CSSDK_INCLUDE_PATHS}
	${METAMOD_SDK_INCLUDE_PATHS}
	${AMXXMODULE_SDK_INCLUDE_PATHS}
	${SRC_DIR}
)