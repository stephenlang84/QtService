cmake_minimum_required(VERSION 3.16)

project(Qt6Service)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS Core) # special case

set(HEADER_FILES
    terminalserver_p.h
    terminalclient_p.h
)

set(SOURCE_FILES
    service.cpp
    servicebackend.cpp
    servicecontrol.cpp
    terminal.cpp
    terminalserver.cpp
    terminalclient.cpp
    serviceplugin.cpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${QCtrlSignalsSource} ${QConsoleSource})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${QConsoleDir} ${QCtrlSignalsDir} ${QtServiceInclude})  

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::CorePrivate)

target_compile_definitions(${PROJECT_NAME} PRIVATE Q_CTRL_SIGNALS_EXPORT= Q_CONSOLE_EXPORT=)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION "1.0"
        AUTHOR "Skycoder42"
        COPYRIGHT "Felix Barz"
    )
endif()