@echo off
setlocal

REM Define paths
set QT_PATH=C:\Qt
set QT_TOOLS_PATH=%QT_PATH%\Tools
set QT_TOOLS_CMAKE_PATH=%QT_TOOLS_PATH%\CMake_64\bin
set QT_TOOLS_NINJA_PATH=%QT_TOOLS_PATH%\Ninja
set QT_VERSION=6.6.0
set QT_PREFIX_PATH=%QT_PATH%\%QT_VERSION%\msvc2019_64

REM Add Qt Tools to PATH
set PATH=%PATH%;%QT_TOOLS_PATH%
set PATH=%PATH%;%QT_TOOLS_CMAKE_PATH%
set PATH=%PATH%;%QT_TOOLS_NINJA_PATH%

REM set MSVC compiler environment variables for 64-bit builds
CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

REM create destination folders
set BUILD_DIR=build
set INSTALL_DIR="%BUILD_DIR%\install"
set INSTALL_HEADER_DIR="%INSTALL_DIR%\include\QtService"
set INSTALL_LIB_DIR="%INSTALL_DIR%\lib"
set INSTALL_LIB_CMAKE_DIR="%INSTALL_LIB_DIR%\cmake"
set INSTALL_PLUGINS_DIR="%INSTALL_DIR%\plugins"

mkdir "%BUILD_DIR%"
mkdir "%INSTALL_DIR%"
mkdir "%INSTALL_LIB_DIR%"
mkdir "%INSTALL_LIB_CMAKE_DIR%"
mkdir "%INSTALL_PLUGINS_DIR%"

REM copy QtService header files
xcopy /Y "%CD%\src\service\qtservice_global.h" "%INSTALL_HEADER_DIR%\"
xcopy /Y "%CD%\src\service\qtservice_helpertypes.h" "%INSTALL_HEADER_DIR%\"
xcopy /Y "%CD%\src\service\service.h" "%INSTALL_HEADER_DIR%\"

REM build QtService
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=%QT_PREFIX_PATH%\lib\cmake -DCMAKE_INSTALL_PREFIX=install .. --debug-output
ninja
cd ..

REM copy QtService libraries
REM C:\Project\QtService\build\src\service\Qt6Service.lib
IF NOT EXIST "%BUILD_DIR%\src\service\Qt6Service.lib" (
    echo "Error no found Qt6Service.lib library"
    exit /b 1
)
xcopy /Y "%BUILD_DIR%\src\service\Qt6Service.lib" "%INSTALL_LIB_DIR%\"