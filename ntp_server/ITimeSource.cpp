/*
* File: ITimeSource.h
* Description:
*   Interface abstraction for time sources. Intended for testability outside of the
*   Arduino environment.
* Author: Mooneer Salem <mooneer@gmail.com>
* License: New BSD License
*/

#include "ITimeSource.h"
#include "ESP32Time.h"

ESP32Time rtc(0);  // offset in seconds GMT+1

void ITimeSource::setOffsetInHours(int8_t ofset)
{
  int32_t of = ofset;
  of = (of>23)?23:of;
  of = (of<(-23))?(-23):of;
  of *= 3600;
  
   rtc.offset = of;  
}


uint32_t ITimeSource::millisToNtpFraction(uint32_t millis) {
    // multiply first, then divide for integer math
    return (uint64_t(millis) * 0x100000000ULL) / 1000;
}

void ITimeSource::getTimeDate(char* string_data, uint32_t *string_data_len)
{
  *string_data_len = rtc.getTimeDate().length()+1;
  rtc.getTime("%02H:%02M:%02S %02d/%02m/%04Y").toCharArray(string_data, *string_data_len);
}

void ITimeSource::updateFromInternalClock(uint8_t secs,
                                          uint8_t minutes,
                                          uint8_t hours,
                                          uint8_t days,
                                          uint8_t months,
                                          uint16_t years,
                                          char *refID)
{
  rtc.setTime(secs, minutes, hours, days, months, years);
  uint8_t counter = 5;
  while(counter--)
  {
      ref_id[counter] = 0;
  }
  
  counter = 0;
  while((counter<5) && (*refID != 0))
  {
      ref_id[counter++] = *refID++;
  }
}

char * ITimeSource::timeRefId(void)
{
  return ref_id;  
}

  
// example implementation
void ITimeSource::now(uint32_t *secs, uint32_t *fract) 
{
    *secs = rtc.getEpoch()+2208988800;/* Epoch to NTP*/
    *fract = millisToNtpFraction(rtc.getMillis());
}

uint32_t ITimeSource::timeRecv(uint32_t *secs, uint32_t *fract)
{
    *secs = 0;
    *fract = 0;
    return *secs;
}
