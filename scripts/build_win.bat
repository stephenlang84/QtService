@echo off
setlocal

REM Define paths
set QT_PATH=C:\Qt
set QT_TOOLS_PATH=%QT_PATH%\Tools
set QT_TOOLS_CMAKE_PATH=%QT_TOOLS_PATH%\CMake_64\bin
set QT_TOOLS_NINJA_PATH=%QT_TOOLS_PATH%\Ninja
set QT_VERSION=6.6.0
set QT_PREFIX_PATH=%QT_PATH%\%QT_VERSION%\msvc2019_64
set QT_SERVICE_INSTALL_PATH=%CD%\build\install\lib\cmake\Qt6Service

REM Add Qt Tools to PATH
set PATH=%PATH%;%QT_TOOLS_PATH%
set PATH=%PATH%;%QT_TOOLS_CMAKE_PATH%
set PATH=%PATH%;%QT_TOOLS_NINJA_PATH%
set PATH=%PATH%;%QT_SERVICE_INSTALL_PATH%

REM set MSVC compiler environment variables for 64-bit builds
CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

REM create destination folders
set BUILD_DIR=build
set INSTALL_DIR="%QT_PREFIX_PATH%"
set INSTALL_HEADER_DIR="%INSTALL_DIR%\include\QtService"
set INSTALL_LIB_DIR="%INSTALL_DIR%\lib"
set INSTALL_LIB_CMAKE_DIR="%INSTALL_LIB_DIR%\cmake"
set INSTALL_PLUGINS_DIR="%INSTALL_DIR%\plugins"
set MODULES_DIR=modules

mkdir "%BUILD_DIR%"
mkdir "%INSTALL_LIB_DIR%"
mkdir "%INSTALL_LIB_CMAKE_DIR%"
mkdir "%INSTALL_PLUGINS_DIR%"

REM copy files in modules folder
xcopy /E /Y "%MODULES_DIR%\" "%INSTALL_HEADER_DIR%\"

REM copy QtService header files
xcopy /Y "%CD%\src\service\qtservice_global.h" "%INSTALL_HEADER_DIR%\"
xcopy /Y "%CD%\src\service\qtservice_helpertypes.h" "%INSTALL_HEADER_DIR%\"
xcopy /Y "%CD%\src\service\service.h" "%INSTALL_HEADER_DIR%\"
xcopy /Y "%CD%\src\service\servicebackend.h" "%INSTALL_HEADER_DIR%\"
xcopy /Y "%CD%\src\service\servicecontrol.h" "%INSTALL_HEADER_DIR%\"
xcopy /Y "%CD%\src\service\serviceplugin.h" "%INSTALL_HEADER_DIR%\"
xcopy /Y "%CD%\src\service\terminal.h" "%INSTALL_HEADER_DIR%\"

REM build QtService
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_PREFIX_PATH=%QT_PREFIX_PATH%\lib\cmake -DCMAKE_INSTALL_PREFIX=%QT_PREFIX_PATH% .. --debug-output -DQT_DEBUG_FIND_PACKAGE=ON
ninja
ninja install