@echo off
cd %~dp0
:start
echo SC-CL example.c
echo Press ENTER to launch
pause > nul
cls
"../../bin/SC-CL.exe" -emit-asm -platform=X360 -target=GTAIV -ivscr=None -out-dir="../../output/" -vcx="example.vcxproj" -- -I "../../include/"
goto start
