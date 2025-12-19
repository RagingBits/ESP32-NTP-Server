REM @echo off
 
setlocal ENABLEDELAYEDEXPANSION

for /F "delims=" %%a in (Config.txt) do (set "%%a")
	
cd bin
esptool.exe --chip auto --baud %PROG_BAUD% --port %PROG_PORT% write_flash 0 ..\%PROG_BINARY%

REM @echo on
echo %outs%

PAUSE