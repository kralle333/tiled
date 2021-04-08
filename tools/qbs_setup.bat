@echo off
set QT_PATH=C:\Qt\Qt5.9.3\5.9.3\msvc2015_64\bin

if not exist "%QT_PATH%" (
echo "QT_PATH MISSING %QT_PATH%"
exit /b 2
)
set QT_CREATOR_PATH=C:\Qt\Qt5.9.3\Tools\QtCreator\bin

if NOT exist "%QT_CREATOR_PATH%" (
echo "Qt creator path not found %QT_CREATOR_PATH%!"
exit /b 2
)

set VS_KITS=C:\Program Files (x86)\Windows Kits\8.1\bin\x64
if NOT exist "%VS_KITS%" (
 echo "Visual studio windows kits installation missing %VS_KITS%!"
exit /b 2
)

set VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC
if NOT exist "%VS_PATH%" (
 echo "Visual studio installation missing %VS_PATH%!"
exit /b 2
) 

echo setting vcvars amd64
CALL "%VS_PATH%\vcvarsall.bat" amd64

%QT_CREATOR_PATH%\qbs-setup-toolchains --detect
%QT_CREATOR_PATH%\qbs-setup-qt --detect
%QT_CREATOR_PATH%\qbs config defaultProfile qt-5-9-3