

#ifndef TIME_HELP_H
#define TIME_HELP_H

#include <stdio.h>
#include <stdbool.h>



typedef enum { DCF77, MSF60, WWVB, JJY } TimeSource;

typedef struct {
    int second, minute, hour, day, month, year;
} DateTime;


DateTime convert_to_utc(DateTime local, TimeSource source, int wwvb_tz_offset);



#endif /*TIME_HELP_H*/
