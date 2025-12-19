

pause

REM @echo off
 
setlocal ENABLEDELAYEDEXPANSION

for /F "delims=" %%a in (ntp_setup_conf.txt) do (set "%%a")

python3 time_offset_setup.py --ip %NTP_IP% --offset %TIME_OFFSET%

echo %outs%

pause