REM @echo off
 
setlocal ENABLEDELAYEDEXPANSION

for /F "delims=" %%a in (Config.txt) do (set "%%a")

cd bin
esptool.exe --port %PROG_PORT% --baud %PROG_BAUD% read_flash 0 0x00400000 ../%SAVE_SINGLE_BINARY%

REM @echo on
echo %outs%
pause