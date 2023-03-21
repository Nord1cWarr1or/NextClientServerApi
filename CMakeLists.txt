cmake_minimum_required(VERSION 3.10)

project(nextclientapi_amxx)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_CXX_STANDARD 14)

if(UNIX)
	set(NAPI_TARGET_PROPS
		SUFFIX "_i386.so"
		COMPILE_FLAGS "-m32" 
		LINK_FLAGS "-m32 -static-libstdc++ -static-libgcc -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/version.version"
		LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/version.version
	)
endif()

set(AMXXMODULE_SDK_MODULECONFIG_DIR
	${CMAKE_SOURCE_DIR}/public
)

find_package(Threads REQUIRED)

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
	${SRC_DIR}/rehlds_api.h
	${SRC_DIR}/rehlds_api.cpp

	${AMXXMODULE_SDK_INCLUDE_PATHS}
	${AMXXMODULE_SDK_RUNTIME_SOURCE_FILE}
	${CSSDK_RUNTIME_SOURCE_FILES}
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
	Threads::Threads
)

# custom build targets
add_custom_target(BUILD_ALL ALL)
add_dependencies(BUILD_ALL nextclientapi_amxx)

add_custom_target(INSTALL_ALL
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:nextclientapi_amxx> $ENV{nextclientapi_amxx_COPY_TO_PATH})

add_custom_target(BUILD_AND_INSTALL_ALL ALL)
add_dependencies(BUILD_AND_INSTALL_ALL BUILD_ALL)
add_dependencies(BUILD_AND_INSTALL_ALL INSTALL_ALL)
