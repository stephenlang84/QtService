cmake_minimum_required(VERSION 3.16)

project(EchoControl)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt6 COMPONENTS Service REQUIRED)

set(SOURCES
    main.cpp
    controlwidget.cpp
)

set(HEADERS
    controlwidget.h
)

set(FORMS
    controlwidget.ui
)

qt_wrap_ui(FORMS_HEADERS ${FORMS})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FORMS_HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Service)
