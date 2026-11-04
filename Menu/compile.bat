@echo off
cd %~dp0
:start
echo SC-CL example.c
echo Press ENTER to launch
pause > nul
cls
"../../bin/SC-CL.exe" -platform=XBOX -target=GTAV -out-dir="GTAV/X360/bin/" Cartel.c ../../include/common.c -- -I "../../include/"
goto start
