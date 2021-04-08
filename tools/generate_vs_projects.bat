CALL qbs_setup.bat

cd %~dp0\..
%QT_CREATOR_PATH%\qbs generate -d build/tiled-vs-debug -g visualstudio2015
%QT_CREATOR_PATH%\qbs generate -d build/tiled-vs-release -g visualstudio2015 buildVariant:release