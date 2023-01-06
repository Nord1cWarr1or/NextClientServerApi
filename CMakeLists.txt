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
	${SRC_DIR}/main.cpp
)

add_compile_definitions(HAVE_STDINT_H)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CSSDK_INCLUDE_PATHS}
	${METAMOD_SDK_INCLUDE_PATHS}
	${AMXXMODULE_SDK_INCLUDE_PATHS}
)