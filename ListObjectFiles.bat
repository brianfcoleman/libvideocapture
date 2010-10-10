@echo off
setlocal enabledelayedexpansion
set OUTPUT_MAKEFILE=%1
set SOURCE_DIR=%2
set OBJECT_DIR=%3
set CPP_EXT=%4
set OBJECT_EXT=%5
set ENV_VAR_NAME=%6
if exist %OUTPUT_MAKEFILE% del %OUTPUT_MAKEFILE%
set OBJECT_FILES=
for %%i in (%SOURCE_DIR%\*.%CPP_EXT%) do (
  set OBJECT_FILES=!OBJECT_FILES! %OBJECT_DIR%\%%~ni.%OBJECT_EXT% )
echo %ENV_VAR_NAME%=%OBJECT_FILES% > %OUTPUT_MAKEFILE%
exit 1