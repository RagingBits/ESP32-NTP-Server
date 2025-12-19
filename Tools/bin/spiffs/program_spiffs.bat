@echo off
 
setlocal ENABLEDELAYEDEXPANSION

for /F "delims=" %%a in (program_spiffs_config.txt) do (set "%%a")
	

start ../esptool.exe --chip auto --baud %PROG_BAUD% --port %PROG_PORT% write_flash 0x00290000 %SPIFFS_FILE%

pause
rem PAUSE