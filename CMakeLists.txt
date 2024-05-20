cmake_minimum_required(VERSION 3.16)

project(QtService)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# set(QConsoleDir ${LIBS_DIR}/QConsole)
# set(QConsoleHeader ${QConsoleDir}/qconsole.h)
# set(QConsoleSource ${QConsoleDir}/qconsole.cpp)
# if (WIN32)
#     list(APPEND QConsoleHeader ${QConsoleDir}/readthread_win.h)
#     list(APPEND QConsoleSource ${QConsoleDir}/readthread_win.cpp)
# endif()


# set(QCtrlSignalsDir ${LIBS_DIR}/QCtrlSignals/src) 
# set(QCtrlSignalsHeader ${QCtrlSignalsDir}/qctrlsignalhandler.h 
#                        ${QCtrlSignalsDir}/qctrlsignalhandler_p.h)

# set(QCtrlSignalsSource ${QCtrlSignalsDir}/qctrlsignalhandler.cpp)
  
# if (UNIX)
#     list(APPEND QCtrlSignalsHeader ${QCtrlSignalsDir}/qctrlsignalhandler_unix.h)   
#     list(APPEND QCtrlSignalsSource ${QCtrlSignalsDir}/qctrlsignalhandler_unix.cpp)
# elseif (WIN32)
#     list(APPEND QCtrlSignalsHeader ${QCtrlSignalsDir}/qctrlsignalhandler_win.h)   
#     list(APPEND QCtrlSignalsSource ${QCtrlSignalsDir}/qctrlsignalhandler_win.cpp)
# endif()

# MESSAGE(STATUS "QConsoleSource: " ${QConsoleSource})
# MESSAGE(STATUS "QCtrlSignalsSource: " ${QCtrlSignalsSource})


set(QtServiceInclude ${QT_PREFIX_PATH}/include)

include_directories(${QtServiceInclude})

add_subdirectory(${LIBS_DIR}/QCtrlSignals)
add_subdirectory(${LIBS_DIR}/QConsole)
add_subdirectory(src/service)

if (WIN32)
    # add_subdirectory(src/plugins/servicebackends/windows)
endif()