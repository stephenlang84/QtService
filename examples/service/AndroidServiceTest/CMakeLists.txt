cmake_minimum_required(VERSION 3.16)

project(AndroidServiceTest)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Quick Service REQUIRED)

set(HEADERS
    testservice.h
    controlhelper.h
)

set(SOURCES
    main.cpp
    testservice.cpp
    controlhelper.cpp
)

set(RESOURCES
    qml.qrc
)

set(DISTFILES
    android/AndroidManifest.xml
    android/res/values/libs.xml
    android/build.gradle
    android/src/de/skycoder42/qtservice/test/TestServiceHelper.java
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES} ${DISTFILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Quick Qt6::Service)
