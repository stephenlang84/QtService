cmake_minimum_required(VERSION 3.10)

project(qwindows)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Service REQUIRED)

set(HEADERS
    windowsserviceplugin.h
    windowsservicebackend.h
    windowsservicecontrol.h
)

set(SOURCES
    windowsserviceplugin.cpp
    windowsservicebackend.cpp
    windowsservicecontrol.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    PLUGIN_TYPE "servicebackends"
    PLUGIN_EXTENDS "service"
    PLUGIN_CLASS_NAME "WindowsServicePlugin"
)

target_link_libraries(${PROJECT_NAME} Qt6::Service advapi32)