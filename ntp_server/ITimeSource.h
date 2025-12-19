/*
* File: ITimeSource.h
* Description:
*   Interface abstraction for time sources. Intended for testability outside of the
*   Arduino environment.
* Author: Mooneer Salem <mooneer@gmail.com>
* License: New BSD License
*/

#ifndef I_TIMESOURCE_H
#define I_TIMESOURCE_H

#include <stdint.h>

class ITimeSource
{
public:

	/*
	* Grabs latest time from the time source.
	*/
	void now(uint32_t *secs, uint32_t *fract);

	/*
	* Ethernet receive time.
	*/
	uint32_t timeRecv(uint32_t *secs, uint32_t *fract);

  char * timeRefId(void);
  // update sync
  void updateFromInternalClock(uint8_t secs,
                               uint8_t minutes,
                               uint8_t hours,
                               uint8_t days,
                               uint8_t months,
                               uint16_t years,
                               char * refID);

 void setOffsetInHours(int8_t ofset);

 void getTimeDate(char* string_data, uint32_t *string_data_len);
 
 private:
    // internal storage for last internal timestamp
    uint32_t lastSecs_;
    uint32_t lastFract_;
    char ref_id[5];
    uint32_t millisToNtpFraction(uint32_t millis);
    

};

#endif // I_TIMESOURCE_H
