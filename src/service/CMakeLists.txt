cmake_minimum_required(VERSION 3.16)

project(Qt6Service VERSION 6.6.0) 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS Core Network) 

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

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::CorePrivate Qt6::Network QCtrlSignals QConsole)

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_BUILD_SERVICE_LIB)

# target_compile_definitions(${PROJECT_NAME} PRIVATE Q_CTRL_SIGNALS_EXPORT= Q_CONSOLE_EXPORT=)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION "1.0"
        AUTHOR "Skycoder42"
        COPYRIGHT "Felix Barz"
    )
endif()

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
               @ONLY)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Config.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)

export(EXPORT ${PROJECT_NAME}Targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
)