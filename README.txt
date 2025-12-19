Before Anything!

All the workers are Python 3.x based. Please have python installed beforehand.

Important!
The setup commands will ALWAYS be prioritised enabled in the Ethernet IP. ONLY if ethernet is disconnected, they are made enabled in the WiFi IP.

Setup WiFi:
    
    Find your NTP server IP in the server screen.
    Have your WiFi SSID and Password at hand.
    Once you have all these, just invoke the pyhton wifi_setup.py with the respective arguments.
     
        python3 wifi_setup.py --ip <NTP Server IP> --ssid <SSID> --password <Password>
        
        Ex.:
        python3 wifi_setup.py --ip 192.168.1.56 --ssid JohnHouseInternet --password JohnIsDaBest
    
    (Windows only)
    For a simpler use, batch files based of a configuration file, have been created.
    
    Open file "ntp_setup_conf.txt" and edit:
    Field "NTP_IP=" - Replace the IP with the one shown in the device display. 
                   For example if the device display shows "ETH: 192.168.1.25", the field will be NTP_IP=192.168.1.25
    Field "SSID=" - Replace "MY_WIFI_NAME" with your WiFi name (ssid). 
                 For example, if your WiFi is called "JohnHouseInternet", the field shall be set as SSID=JohnHouseInternet
                 
    Field "PASSWORD=" - Replace "MY_WIFI_PASSWORD" with your WiFi access password. 
                 For example, if your password for "JohnHouseInternet" is "JohnIsDaBest", the field shall be set as PASSWORD=JohnIsDaBest

    The ntp_setup_conf.txt file content shall be as per the examples:

    NTP_IP=192.168.1.25
    SSID=JohnHouseInternet
    PASSWORD=JohnIsDaBest
    TIME_OFFSET=0

    Once the configuration file is set, simply execute wifi_setup.bat.
    This batch file is a simple way to pick your settings and push them into the python worker that will configure the NTP server.
    
    
    
Setup Time Zone:    
    This will set the NTP Server with +/- N hours.
    
    Find your NTP server IP in the server screen.
    Find the offset in number of hours needed for the network.
    
    Once you this, just invoke the pyhton time_offset_setup.py with the respective arguments.
     
        python3 time_offset_setup.py --ip <NTP Server IP> --offset <+/- N hours>
        
        Ex.:
        python3 time_offset_setup.py --ip 192.168.1.56 --offset -2
    
    (Windows only)
    For a simpler use, batch files based of a configuration file, have been created.
    
    Open file "ntp_setup_conf.txt" and edit:
    Field "NTP_IP=" - Replace the IP with the one shown in the device display. 
                For example if the device display shows "ETH: 192.168.1.25", the field will be NTP_IP=192.168.1.25
                   
    Field "TIME_OFFSET=" - Replace the zero offset with the offset on the received time needed for the network. 
                For example if the device received time is advanced by one hour, set the offset to -1 hours. The field will then be TIME_OFFSET=-1
    
    
    The ntp_setup_conf.txt file content shall be as per the examples:

    NTP_IP=192.168.1.25
    SSID=JohnHouseInternet
    PASSWORD=JohnIsDaBest
    TIME_OFFSET=-1
    
    
    Once the configuration file is set, simply execute time_offset_setup.bat.
    This batch file is a simple way to pick your settings and push them into the python worker that will configure the NTP server.
    
    
    
ADVANCED

 Commands:
    The server will receive the setup commands via port 8080, always in the ethernet IP. If the ethernet is not present, then it will enable the command reception in the Wifi.
    The commands format is as such:
    
    [command indication][command type][\n][data][\n]
    
    Where
    command indication = "1234" plain text utf-8 format. 
    command type = "A" for wifi setup.
                   "B" for time offset setup.
    data = [text field 1]['\n'][text field 2]['\n'] ... [text field N]
    
    
    Ex.:
        Setup WiFi command:
            
            UDP to port 8080 content:   1234A\nJohnHouseInternet\nJohnIsDaBest\n
            
        Setup time offset:
            
            UDP to port 8080 content:   1234B\n-1\n
        
        
 ESP32 Tools:
    Some tools have been provided for those who want to further develop the module.
    The tools are batch files for windows user easly ported to linux. 
    They run of Config.txt configuration fields and are based of esp tools in "bin" folder.
        
    
    EraseChip - Fully Erases the ESP32 Wroom flash content.
    ReadFullImage - Copy the FULL ESP32 Wroom flash content into a file named by field PROG_SINGLE_BINARY.
    Programmer - It fills the ESP32 Wroom flash with the content of the file named by PROG_BINARY. 
                 The programming starts at address zero. 
                 It's used in tandem with ReadFullImage for an automated production process.
    
    
    
    
    
    
    
    
    
    