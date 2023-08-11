cmake_minimum_required(VERSION 3.16)

project(echoservice)

find_package(Qt6 COMPONENTS Core Service REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS
    echoservice.h
)

set(SOURCES
    main.cpp
    echoservice.cpp
)

set(QMAKE_SUBSTITUTES
    echoservice.service.in
    scinstall.bat.in
    echoservice.plist.in
)

set(DISTFILES
    ${QMAKE_SUBSTITUTES}
    echoservice.socket
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Service)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/service/EchoService)

foreach(file ${QMAKE_SUBSTITUTES})
    configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/service/EchoService)
endforeach()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/echoservice.socket DESTINATION ${CMAKE_INSTALL_PREFIX}/service/EchoService)