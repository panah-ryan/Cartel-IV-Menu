@echo off
cd %~dp0
:start
echo SC-CL test.c
echo Press ENTER to launch
pause > nul
cls
"../../bin/SC-CL.exe" -extra-arg-before="-Werror=implicit-function-declaration" -platform=XBOX -target=GTAIV -scr=None -emit-asm -out-dir="MNCLA/X360/bin/" test.c ../../include/common.c -- -I "../../include/"
goto start
