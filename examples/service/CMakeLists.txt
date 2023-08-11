cmake_minimum_required(VERSION 3.16)

project(service)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_subdirectory(EchoService)
add_subdirectory(EchoControl)
add_subdirectory(TerminalService)

if(ANDROID)
    add_subdirectory(AndroidServiceTest)
endif()