#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  using WebServerClass = ESP8266WebServer;
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
  using WebServerClass = WebServer;
#endif

#include <time.h>             // Core standard C library for time processing
#include "webserver.h"
#include "background.h"
#include "webpage.h"


#define EEPROM_SSID         0
#define EEPROM_PASS         1
#define EEPROM_OFFSET       2
#define EEPROM_TIME_SOURCE  3
#define EEPROM_NTP          4

#define EEPROM_SSID_LEN     50
#define EEPROM_PASS_LEN     50
#define EEPROM_OFFSET_LEN   1
#define EEPROM_TIME_SOURCE_LEN   1
#define EEPROM_NTP_LEN     50

enum
{
  MEM_SSID,  
  MEM_PASS
};

// Define shared variables declared 'extern' inside ConfigWebpage.h
String webSsid = "";
String webPass = "";
int8_t timeOffsetHours = 0;      // Loaded directly via the form input box

String time1 = "--:--:--";
String date1 = "--.--.----";
String time2 = "--:--:--";
String date2 = "--.--.----";

uint8_t timeSource = 0;      // 0=GPS 1=Radio 2=Manual
uint8_t timeSourceUpdated = 0; 

String manualDate = "";
String manualTime = "";
String ntpServer = "";
String statusAtomic = "Receiver: UNK";
String statusWifi   = "WiFi: Diconnected";
String statusEth    = "ETH: Disconnected";


// Change these to your initial router credentials to get the board online
const char* initial_ssid = "YOUR_ROUTER_SSID";
const char* initial_password = "YOUR_ROUTER_PASSWORD";


WebServerClass server(80);
unsigned long lastUpdateTicker = 0;

// Restore the clean, RAM-safe handleRoot
void handleRoot() {
  server.send(200, "text/html", INDEX_HTML); 
}

void handleBackgroundImage() {
  // Use the built-in flash memory streaming constructor.
  // This bypasses RAM entirely and handles all network chunking automatically.
  server.send_P(200, "image/jpeg", (const char*)background_jpg, background_jpg_len);
}
void handleLogoImage() {
  // Uses the high-efficiency flash memory wrapper to stream the logo data safely
  server.send_P(200, "image/png", (const char*)logo_jpg, logo_jpg_len);
}

void handleLiveData() {
  // Generates the lightweight JSON package the background JavaScript fetches every second
  String json = "{";
  json += "\"t1\":\"" + time1 + "\",";
  json += "\"d1\":\"" + date1 + "\",";
  json += "\"t2\":\"" + time2 + "\",";
  json += "\"d2\":\"" + date2 + "\",";
  json += "\"saved_ssid\":\"" + webSsid + "\",";
  json += "\"saved_pass\":\"" + webPass + "\",";
  json += "\"saved_offset\":" + String(timeOffsetHours) + ",";
  
  // Maps your ready-to-go network and atomic status strings
  json += "\"s_atom\":\"" + statusAtomic + "\",";
  json += "\"s_wifi\":\"" + statusWifi + "\",";
  json += "\"s_eth\":\"" + statusEth + "\"";

  json += ",\"saved_source\":" + String(timeSource);
  json += ",\"saved_mdate\":\"" + manualDate + "\"";
  json += ",\"saved_mtime\":\"" + manualTime + "\"";

  json += "}";
  
  server.send(200, "application/json", json);
}


unsigned long lastSaveTimestamp = 0; 

// UPDATE THIS ROUTE IN YOUR MAIN CODE FILE
void handleSave() {
  unsigned long currentMillis = millis();

  // THE HARDWARE REJECTION GUARD: If less than 5000ms has passed since the last save, drop it
  if (currentMillis - lastSaveTimestamp < 5000) {
    Serial.println("[WARNING] Save request rejected! Frequency too high.");
    server.send(429, "text/plain", "Too Many Requests"); // Send a standard HTTP 429 Throttle status code
    return;
  }

  // If validation passes, lock in the new timestamp marker
  lastSaveTimestamp = currentMillis;

        
        

  // Process your variables safely now that the rate is verified secure
  extern void EepromWrite(uint8_t data_type, uint8_t *data_in, uint8_t data_length);
  if (server.hasArg("ssid"))     {webSsid = server.arg("ssid");EepromWrite(EEPROM_SSID, (uint8_t*)webSsid.c_str(), webSsid.length()+1);}
  if (server.hasArg("password")) {webPass = server.arg("password");EepromWrite(EEPROM_PASS, (uint8_t*)webPass.c_str(), webPass.length()+1);}
  if (server.hasArg("offset"))   {timeOffsetHours = server.arg("offset").toInt();EepromWrite(EEPROM_OFFSET, (uint8_t*)&timeOffsetHours, 1);}
     
  Serial.println("\n[SYSTEM] Variable parameters updated. Frequency constraint clear.");
  server.send(200, "text/plain", "OK");
  server.close();
  delay(1000);
  ESP.restart(); 
}


void handleSaveTime() {
  unsigned long currentMillis = millis();

  // THE HARDWARE REJECTION GUARD: If less than 5000ms has passed since the last save, drop it
  if (currentMillis - lastSaveTimestamp < 5000) {
    Serial.println("[WARNING] Save request rejected! Frequency too high.");
    server.send(429, "text/plain", "Too Many Requests"); // Send a standard HTTP 429 Throttle status code
    return;
  }

Serial.println("---- SAVE ARGS ----");

for (int i = 0; i < server.args(); i++) {
    Serial.print(server.argName(i));
    Serial.print(" = ");
    Serial.println(server.arg(i));
}

Serial.println("-------------------");

  // If validation passes, lock in the new timestamp marker
  lastSaveTimestamp = currentMillis;


  // Process your variables safely now that the rate is verified secure
  extern void EepromWrite(uint8_t data_type, uint8_t *data_in, uint8_t data_length);
  
  if (server.hasArg("time_source")) {

    String source = server.arg("time_source");

    if (source == "gps")
        timeSource = GPS;
    else if (source == "radio")
        timeSource = Radio;
    else if (source == "manual")
        timeSource = Manual;
    else if (source == "ntp")
        timeSource = Ntp;
        
    timeSourceUpdated = 1;
    EepromWrite(EEPROM_TIME_SOURCE, (uint8_t*)&timeSource, 1);
}

  if(Manual == timeSource)
  {
  
    if (server.hasArg("manual_date")) {manualDate = server.arg("manual_date");}
    if (server.hasArg("manual_time")) {manualTime = server.arg("manual_time");}
       
    Serial.println("\n[SYSTEM] Time manually updated. ");
    
  }
  else if (GPS == timeSource)
  {
    Serial.println("\n[SYSTEM] Time source set for GPS UTC."); 
  }
  else if(Ntp == timeSource)
  {
    Serial.println("\n[SYSTEM] Time source set for remote NTP."); 
    if (server.hasArg("ntp_server"))  {ntpServer = server.arg("ntp_server");EepromWrite(EEPROM_NTP, (uint8_t*)ntpServer.c_str(), ntpServer.length()+1);}
  }
  else
  {
    Serial.println("\n[SYSTEM] Time source set for Radio Receiver."); 
  }
  
  server.send(200, "text/plain", "OK");
}

void webserverinit() {
  
  // Initialize internal clock engine using the current user hour offset

  // Set up URL routing targets
  server.on("/", handleRoot);
  server.on("/live-data", handleLiveData);
  server.on("/save", HTTP_POST, handleSave);  
  server.on("/save-sync", HTTP_POST, handleSaveTime);
  server.on("/background.jpg", handleBackgroundImage);
  server.on("/logo.jpg", handleLogoImage); 
  server.begin();
}

void webserverloop() {
  server.handleClient();

}
