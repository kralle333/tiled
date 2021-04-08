CALL build_release.bat

rem %BUILD_TOOLS%\tiled
xcopy build\tiled-release\default\install-root %BUILD_TOOLS%\tiled /H /S /Y /D
