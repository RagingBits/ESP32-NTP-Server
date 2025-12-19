
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <Ticker.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ITimeSource.h"
#include "NtpPacket.h"
#include "NtpServer.h"
/*

Raging Bits fully fledged NTP server.
An example project on how to use Raging Bits Atomic Clock Receiver Decoder UART module.

Based of the following example:

****************************************************************************************************************************
  UDPSendReceive.ino - Simple Arduino web server sample for ESP8266/ESP32 AT-command shield

  For Ethernet shields using WT32_ETH01 (ESP32 + LAN8720)

  WebServer_WT32_ETH01 is a library for the Ethernet LAN8720 in WT32_ETH01 to run WebServer

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/WebServer_WT32_ETH01
  Licensed under MIT license
 ********************************************************************a********************************************************
 
 */

#define EEPROM_SSID         0
#define EEPROM_PASS         1
#define EEPROM_OFFSET       2
#define EEPROM_SSID_LEN     50
#define EEPROM_PASS_LEN     50
#define EEPROM_OFFSET_LEN   1

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define MAX_RECEIVING_TIME_LEN  100

#define DEBUG_ETHERNET_WEBSERVER_PORT     Serial2

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_       0

#include <WebServer_WT32_ETH01.h>


#define RXD1 17  // for loopback jumper these pins
#define TXD1 5

enum
{
  MEM_SSID,  
  MEM_PASS
};

unsigned int localPort = 1883;    //10002;  // local port to listen on
int8_t time_adjust = 0;
ITimeSource my_time;
NtpServer my_server(my_time);
WiFiUDP udp;
bool eth_connected;
bool wifi_connected;
IPAddress eth_ip;
IPAddress wifi_ip;
uint8_t secs = 0;
uint8_t minutes =0;
uint8_t hours = 0;
uint8_t days =0;
uint8_t months =0;
uint16_t years = 0;
uint32_t query_counter = 0;
char modulation[10];
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
uint8_t receiving_time[MAX_RECEIVING_TIME_LEN];
uint8_t receiving_time_index = 0;


void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      eth_ip = ETH.localIP();

      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      udp.stop();
      udp.begin(8080);
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      if(wifi_connected)
      {
        udp.stop();
        udp.begin(WiFi.localIP(),8080);
      }
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        //When connected set 
        Serial.print("WiFi connected! IP address: ");
        Serial.println(WiFi.localIP());  
        wifi_connected = true;
        wifi_ip = WiFi.localIP();
        //initializes the UDP state
        //This initializes the transfer buffer
        //udp.begin(WiFi.localIP(),8080);
        if(!eth_connected)
        {
           udp.stop();
           udp.begin(WiFi.localIP(),8080);
        }
        
        break;
        
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        //WiFi.disconnect();
        wifi_connected = false;
        break;
        
    default:
      break;
  }
}



void disp_start(void)
{
  Wire.begin(14,4);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  

  // Clear the buffer
  display.clearDisplay();  

  update_disp();
}

void update_disp(void)
{
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text

  display.clearDisplay();  
  display.setCursor(0,0);             // Start at top-left corner
  
  display.println(F("  -- Raging Bits --"));
  //display.println(F("---------------------"));
  display.println(F(" "));
  
  display.print(F("WiF "));
  if(wifi_connected)
  {
    display.println(wifi_ip);
  }
  else
  {
    display.println(F("---.---.---.---"));
  }

  display.print(F("ETH "));
  if(eth_connected)
  {
    display.println(eth_ip);
  }
  else
  {
    display.println(F("---.---.---.---"));
  }
  //display.println(F("_____________________"));
  //display.println(F("---------------------"));
  display.println(F(" "));
  char temp[100];
  sprintf(temp, " %02d:%02d:%02d %02d/%02d/%04d ",hours,minutes,secs,days,months,years);
  display.println(temp);
  //display.println(F("¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨"));
  //display.println(F("---------------------"));
  display.println(F(" "));
  display.print(modulation);
  display.print(F("  REQ "));
  display.println(query_counter);
  display.display();
}

void setup()  
{

  disp_start();
  
    
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);
  //Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  while (!Serial);


  // Using this if Serial debugging is not necessary or not using Serial port
  //while (!Serial && (millis() < 3000));

  // To be called before ETH.begin()
  //WT32_ETH01_onEvent();

  //bool begin(uint8_t phy_addr=ETH_PHY_ADDR, int power=ETH_PHY_POWER, int mdc=ETH_PHY_MDC, int mdio=ETH_PHY_MDIO,
  //           eth_phy_type_t type=ETH_PHY_TYPE, eth_clock_mode_t clk_mode=ETH_CLK_MODE);
  //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_LAN8720);

  // Static IP, leave without this line to get IP via DHCP
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  //ETH.config(myIP, myGW, mySN, myDNS);

  //WT32_ETH01_waitForConnect();
  //or
  WiFi.onEvent(WiFiEvent);
  //or nothing

  Serial.println(F("\nStarting connection to server..."));
  // if you get a connection, report back via serial:
  

  Serial.print(F("Listening on port "));
  Serial.println(localPort);

  EepromStart();

  char my_pass[100];
  uint8_t my_pass_len = EEPROM_PASS_LEN;
  char my_ssid[100];
  uint8_t my_ssid_len = EEPROM_SSID_LEN;
  uint8_t my_offset_len = EEPROM_OFFSET_LEN;
  EepromRead(EEPROM_SSID, (uint8_t*)my_ssid, &my_ssid_len);
  EepromRead(EEPROM_PASS, (uint8_t*)my_pass, &my_pass_len);
  EepromRead(EEPROM_OFFSET, (uint8_t*)&time_adjust, &my_offset_len);
  
  Serial.println("NTP server offset:");
  char temp[100];
  sprintf(temp,"%i:\n",time_adjust);
  Serial.print(temp);
  
  my_server.timeSource_.setOffsetInHours(time_adjust);
  
  Serial.println("Starting WIFI:");
  Serial.println(my_ssid);
  WiFi.begin(my_ssid, my_pass);
  

  //udp.begin(8080);
  my_server.beginListening();
  
}



void loop()
{
  static uint32_t wifi_status = 0;

  // if there's data available, read a packet
  int packetSize = udp.parsePacket();
  char temp[100];
  uint32_t temp_len = 100;
  static bool lastlink = false;
  bool nowlink = ETH.linkUp();

  if(packetSize)
  {
    
     char packetBuffer[255];
     uint8_t ReplyBuffer[] = "NACK";
     char my_ssid[255] = {0};
     char my_pass[255] = {0};

     uint8_t counter = 0;
     char *p = packetBuffer+6;
     int len = packetSize;
     len=len>255?255:len;
     udp.read(packetBuffer, len);   
    
    //Serial.println(packetBuffer);
    
     uint8_t *check_p = (uint8_t *)packetBuffer;
    if((check_p[0] == '1')&&
       (check_p[1] == '2')&&
       (check_p[2] == '3')&&
       (check_p[3] == '4')&& ('\n' == check_p[5]))
    {
      if('A' == check_p[4])
      {    
        Serial.print("New wifi set: ");
        
        counter = 0;
        while(counter < 100 && *p!= '\n' && *p!= '\r' && len--)
        {
           my_ssid[counter] = *p++;
           counter++;
        }
        my_ssid[counter] = '\0';
        while((*p == '\n' || *p == '\r') && len--)p++;
    
        counter = 0;
        while(counter < 100 && *p!= '\n' && len--)
        {
           my_pass[counter] = *p++;
           counter++;
        }
        my_pass[counter] = '\0';
    
        EepromWrite(EEPROM_SSID, (uint8_t*)my_ssid, strlen(my_ssid)+1);
        EepromWrite(EEPROM_PASS, (uint8_t*)my_pass, strlen(my_pass)+1);

        Serial.print(my_ssid);

        Serial.println(" ***");
        
         WiFi.disconnect();
         WiFi.begin(my_ssid, my_pass);
         ReplyBuffer[0] = ' ';
      }
      else
      if('B' == check_p[4])
      {
         Serial.print("New time offset set: ");
         int8_t val = atoi(p);
         val = val>23?23:val;
         val = val<-23?-23:val;

         time_adjust = val;

         EepromWrite(EEPROM_OFFSET, (uint8_t*)&time_adjust, 1);
         
         char temp[100];
         sprintf(temp,"%i:\n",time_adjust);
         Serial.print(temp);
         
         my_server.timeSource_.setOffsetInHours(time_adjust);
         
         ReplyBuffer[0] = ' ';
      }
  
       udp.beginPacket(udp.remoteIP(), udp.remotePort());
       udp.write(ReplyBuffer, sizeof(ReplyBuffer));
       udp.endPacket();
       //udp.begin(8080);
    }
      
  }


  
  if (nowlink != lastlink)
  {
    lastlink = nowlink;
    
    if (nowlink)
    {
      Serial.print("Connected\r\n");
    }
    else
    {
      Serial.print("Disconnected\r\n");
    }
  }

  if(wifi_status != WiFi.status())
  {
    wifi_status = WiFi.status();
    
    if(WL_CONNECTED == WiFi.status())
    {
      Serial.print("Connected: ");
      Serial.println(WiFi.localIP());
    }   
  }

  while (Serial1.available() > 0)
  {
    // read the incoming byte:
    receiving_time[receiving_time_index] = Serial1.read();
    if (receiving_time_index >= MAX_RECEIVING_TIME_LEN)
    {
      receiving_time_index = 0;
      receiving_time[0] = 0;
    }
    else if (receiving_time[receiving_time_index] == '\n')
    {
      //full time is here

      memcpy(modulation,&receiving_time[20],5);
      
      uint32_t t_secs = atoi((const char*)&receiving_time[6]);
      uint32_t t_minutes = atoi((const char*)&receiving_time[3]);
      uint32_t t_hours = atoi((const char*)&receiving_time[0]);
      uint32_t t_days = atoi((const char*)&receiving_time[9]);
      uint32_t t_months = atoi((const char*)&receiving_time[12]);
      uint32_t t_years = atoi((const char*)&receiving_time[15]);

      if((secs != 0) && (memcmp(modulation,"UNKN",4)))
      {
        my_server.timeSource_.updateFromInternalClock(t_secs, t_minutes, t_hours, t_days, t_months, t_years, modulation);
      }

      receiving_time_index = 0;
      receiving_time[0] = 0;
    }
    else
    {
      receiving_time_index++;
    }

  }

  my_server.timeSource_.getTimeDate(temp,&temp_len);
  uint32_t temp_secs = atoi((const char*)&temp[6]);
  static uint32_t last_secs = 0;

  if(last_secs != temp_secs)
  {
      secs = temp_secs;
      minutes = atoi((const char*)&temp[3]);
      hours = atoi((const char*)&temp[0]);
      days = atoi((const char*)&temp[9]);
      months = atoi((const char*)&temp[12]);
      years = atoi((const char*)&temp[15]);

      
      last_secs = temp_secs;
      my_server.timeSource_.getTimeDate(temp,&temp_len);
      Serial.print(temp);Serial.print(" ");
      Serial.println(modulation);
       
      update_disp();
  }
  
  if (my_server.processOneRequest())
  {
    query_counter++;
    //Serial.printf("NTP Query\r\n");

  }

}
