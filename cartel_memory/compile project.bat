@echo off
cd %~dp0
:start
echo SC-CL test.c
echo Press ENTER to launch
pause > nul
cls
"../../bin/SC-CL.exe" -platform=X360 -target=GTAV -out-dir="GTAV/X360/bin/" -vcx="test.vcxproj" -- -I "../../include/"
goto start