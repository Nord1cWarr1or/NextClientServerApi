cmake_minimum_required(VERSION 3.0.0)

project(nextclient_api)

add_library(amxxmodule_sdk INTERFACE)
target_sources(amxxmodule_sdk INTERFACE
	${CMAKE_SOURCE_DIR}/deps/amxmodule/amxxmodule.cpp
	${CMAKE_SOURCE_DIR}/deps/amxmodule/amxxmodule.h
)

target_include_directories(amxxmodule_sdk INTERFACE
	${CMAKE_SOURCE_DIR}/public/
)