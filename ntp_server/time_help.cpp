


#include "time_help.h"






// Helper to find the day of the week (0=Sun, 6=Sat) using Sakamoto's algorithm
int get_weekday(int d, int m, int y) {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (m < 3) y -= 1;
    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

// Helper to find the Nth or Last Sunday of a month
int find_sunday(int year, int month, int occurrence) {
    if (occurrence == -1) { // "Last Sunday"
        int d = 31;
        while (get_weekday(d, month, year) != 0) d--;
        return d;
    } else { // "Nth Sunday" (e.g., 2nd Sunday)
        int d = 1;
        int count = 0;
        while (count < occurrence) {
            if (get_weekday(d, month, year) == 0) count++;
            if (count < occurrence) d++;
        }
        return d;
    }
}

bool is_summer_time(int hour, int day, int month, int year, TimeSource source) {
    if (source == JJY) return false; // Japan has no DST

    int start_day, end_day, start_hour, end_hour;

    if (source == DCF77 || source == MSF60) {
        // EU Rules: Last Sun March to Last Sun Oct
        start_day = find_sunday(year, 3, -1);
        end_day = find_sunday(year, 10, -1);
        // EU changes at 01:00 UTC. DCF77 is UTC+1 (changes at 02:00 local). 
        // MSF60 is UTC+0 (changes at 01:00 local).
        start_hour = (source == DCF77) ? 2 : 1; 
        end_hour = (source == DCF77) ? 3 : 2; // End is 01:00 UTC -> 02:00/03:00 local
    } else if (source == WWVB) {
        // US Rules: 2nd Sun March to 1st Sun Nov at 02:00 local
        start_day = find_sunday(year, 3, 2);
        end_day = find_sunday(year, 11, 1);
        start_hour = 2;
        end_hour = 2;
    }

    // Month check
    if (month > 3 && month < 10) return true; // Definitely summer (April-Sept)
    if (month < 3 || (month > 11 && source == WWVB) || (month > 10 && source != WWVB)) return false;

    // Boundary month checks
    if (month == 3) {
        if (day > start_day) return true;
        if (day < start_day) return false;
        return hour >= start_hour;
    }
    if (month == 10 || month == 11) {
        if (day < end_day) return true;
        if (day > end_day) return false;
        return hour < end_hour;
    }

    return false;
}

// source: DCF77, MSF60, WWVB, JJY
// is_dst: result from your previous function
// wwvb_tz_offset: For WWVB, the hours behind UTC (e.g., 5 for Eastern, 8 for Pacific)
DateTime convert_to_utc(DateTime local, TimeSource source,  int wwvb_tz_offset) {
    int offset = 0;
    bool is_dst = is_summer_time(local.hour,local.day,local.month,local.year, source);

    switch (source) {
        case DCF77: offset = is_dst ? -2 : -1; break;
        case MSF60: offset = is_dst ? -1 : 0;  break;
        case JJY:   offset = -9; break;
        case WWVB:  offset = 0;/*wwvb_tz_offset - (is_dst ? 1 : 0); */break;
    }

    local.hour += offset;

    // Simple wrap-around for hours (Note: Does not handle day/month rollover)
    if (local.hour < 0) {
        local.hour += 24;
        local.day -= 1; 
    } else if (local.hour >= 24) {
        local.hour -= 24;
        local.day += 1;
    }

    return local;
}
