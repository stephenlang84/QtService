cmake_minimum_required(VERSION 3.16)

project(QtService)
set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(QConsoleDir ${LIBS_DIR}/QConsole)
set(QConsoleHeader ${QConsoleDir}/qconsole.h)
set(QConsoleSource ${QConsoleDir}/qconsole.cpp)
if (WIN32)
    list(APPEND QConsoleHeader ${QConsoleDir}/readthread_win.h)
    list(APPEND QConsoleSource ${QConsoleDir}/readthread_win.cpp)
endif()


set(QCtrlSignalsDir ${LIBS_DIR}/QCtrlSignals/src) 
set(QCtrlSignalsHeader ${QCtrlSignalsDir}/qctrlsignalhandler.h 
                       ${QCtrlSignalsDir}/qctrlsignalhandler_p.h)

set(QCtrlSignalsSource ${QCtrlSignalsDir}/qctrlsignalhandler.cpp)
  
if (UNIX)
    list(APPEND QCtrlSignalsHeader ${QCtrlSignalsDir}/qctrlsignalhandler_unix.h)   
    list(APPEND QCtrlSignalsSource ${QCtrlSignalsDir}/qctrlsignalhandler_unix.cpp)
elseif (WIN32)
    list(APPEND QCtrlSignalsHeader ${QCtrlSignalsDir}/qctrlsignalhandler_win.h)   
    list(APPEND QCtrlSignalsSource ${QCtrlSignalsDir}/qctrlsignalhandler_win.cpp)
endif()


set(QtServiceInclude ${CMAKE_CURRENT_SOURCE_DIR}/build/install/include)

include_directories(${QtServiceInclude} 
                    ${QConsoleDir} 
                    ${QCtrlSignalsDir})

add_subdirectory(src/service)