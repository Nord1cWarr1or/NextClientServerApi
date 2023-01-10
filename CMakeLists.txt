cmake_minimum_required(VERSION 3.10.0)

project(nextclientapi_amxx)

if(UNIX)
	set(NAPI_TARGET_PROPS
		SUFFIX "_i386.so"
		COMPILE_FLAGS "-m32" 
		LINK_FLAGS "-m32"
	)
endif()

set(AMXXMODULE_SDK_MODULECONFIG_DIR
	${CMAKE_SOURCE_DIR}/public
)

add_subdirectory(deps/amxxmodule)
add_subdirectory(deps/cssdk)
add_subdirectory(deps/metamod)
add_subdirectory(deps/openssl)
add_subdirectory(deps/rsa_keygen)

set(SRC_DIR src)

add_library(${PROJECT_NAME} SHARED
	${SRC_DIR}/api/events.h
	${SRC_DIR}/api/impl.h
	${SRC_DIR}/api/impl.cpp
	${SRC_DIR}/api/cvar_sandbox.h
	${SRC_DIR}/api/cvar_sandbox.cpp
	${SRC_DIR}/api/private_precache.h
	${SRC_DIR}/api/private_precache.cpp
	${SRC_DIR}/api/viewmodelfx.h
	${SRC_DIR}/api/viewmodelfx.cpp
	${SRC_DIR}/api/verificator.h
	${SRC_DIR}/api/verificator.cpp
	${SRC_DIR}/natives/cvar_sandbox.cpp
	${SRC_DIR}/natives/private_precache.cpp
	${SRC_DIR}/natives/viewmodelfx.cpp
	${SRC_DIR}/natives/nextclient.cpp
	${SRC_DIR}/natives/natives.cpp
	${SRC_DIR}/main.h
	${SRC_DIR}/main.cpp
	${SRC_DIR}/utilfuncs.cpp

	${AMXXMODULE_SDK_INCLUDE_PATHS}
	${AMXXMODULE_SDK_RUNTIME_SOURCE_FILE}
)

add_definitions(-DHAVE_STDINT_H)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CSSDK_INCLUDE_PATHS}
	${METAMOD_SDK_INCLUDE_PATHS}
	${AMXXMODULE_SDK_INCLUDE_PATHS}
	${OPENSSL_INCLUDE_PATHS}
	${SRC_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	PREFIX ""
	${NAPI_TARGET_PROPS}
)

target_link_libraries(${PROJECT_NAME} 
	${OPENSSL_STATIC_LIBS}
	${NAPI_LINK_LIBS}
)