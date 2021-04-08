CALL qbs_setup.bat

cd %~dp0\..
%QT_CREATOR_PATH%\qbs -d build\tiled-release buildVariant:release