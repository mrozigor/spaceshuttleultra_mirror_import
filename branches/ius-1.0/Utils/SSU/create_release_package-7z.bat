@echo off

set WINRAR_DIR=C:\"Program Files"\7-Zip\7z.exe

echo input major version number:
set /P "MAJOR_VERSION="

echo input minor version number:
set /P "MINOR_VERSION="

echo input revision number:
set /P "REVISION="

cls

set OUTPUT_NAME=SSU_v%MAJOR_VERSION%.%MINOR_VERSION%r%REVISION%.zip

echo creating %OUTPUT_NAME% in orbiter base directory...
@echo on

cd ..\..

%WINRAR_DIR%

%WINRAR_DIR% a -mx5 %OUTPUT_NAME% @Utils\SSU\releasefilelist.txt

cd Utils\SSU