REM @echo off
 
setlocal ENABLEDELAYEDEXPANSION

for /F "delims=" %%a in (ntp_setup_conf.txt) do (set "%%a")

python3 wifi_setup.py --ip %NTP_IP% --ssid %SSID% --password %PASSWORD%

echo %outs%

pause

