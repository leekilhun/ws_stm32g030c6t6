@echo off
setlocal

:: 설정
set COMPILER=cl
set CFLAGS=/Zi /EHsc /std:c++20 /nologo
set INCLUDES=/Isrc /Isrc/core /Isrc/core/ap /Isrc/core/sys /Isrc/core/hw /Isrc/core/common /Isrc/core/common/util /Isrc/mcc_lib /Isrc/core/common/_inc_hw
set OBJ_DIR=./build
set OUTPUT_NAME=project.exe
set OUTPUT_PATH=%OBJ_DIR%/%OUTPUT_NAME%
set SOURCE_PATH=src/main.cpp src/core/core.cpp src/core/ap/ap.cpp src/core/hw/hw.cpp src/core/sys/sys.cpp src/core/common/util/utils.cpp

if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

%COMPILER% %CFLAGS% %INCLUDES% /Fo"%OBJ_DIR%/" /Fd"%OBJ_DIR%/vc140.pdb" /Fe"%OBJ_DIR%/%OUTPUT_NAME%" %SOURCE_PATH% /link /INCREMENTAL:NO


echo.
echo Running the executable...
::"%OUTPUT_PATH%"
if %ERRORLEVEL% NEQ 0 echo Failed to execute %OUTPUT_NAME%. Error level: %ERRORLEVEL%.
endlocal
