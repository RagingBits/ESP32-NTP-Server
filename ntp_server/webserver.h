#ifndef CONFIG_WEBPAGE_H
#define CONFIG_WEBPAGE_H

#include <Arduino.h>

extern String webSsid;
extern String webPass;
extern int8_t timeOffsetHours;
extern String time1;
extern String date1;
extern String time2;
extern String date2;
extern String statusAtomic;
extern String statusWifi;
extern String statusEth;

void webserverloop();
void webserverinit();

#endif
