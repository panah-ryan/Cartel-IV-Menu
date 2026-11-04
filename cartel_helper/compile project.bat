@echo off
cd %~dp0
:start
echo SC-CL cartel_helper.c
echo Press ENTER to launch
pause > nul
cls
"../../bin/SC-CL.exe" -emit-asm -platform=XBOX -target=GTAIV -out-dir="../../output/" -vcx="cartel_helper.vcxproj" -- -I "../../include/"
goto start