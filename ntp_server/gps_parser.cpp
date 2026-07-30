#include "gps_parser.h"

#define NMEA_BUF_SIZE 128


static float fast_atof(const char* s) {
    float rez = 0.0f, fact = 1.0f;
    if (*s == '-') { s++; fact = -1.0f; }
    for (int point_seen = 0; *s; s++) {
        if (*s == '.') { point_seen = 1; continue; }
        int d = *s - '0';
        if (d >= 0 && d <= 9) {
            if (point_seen) fact /= 10.0f;
            rez = rez * 10.0f + (float)d;
        } else break;
    }
    return rez * fact;
}


static GPS_Data GPS_ParseRMC(char *nmea_line) {
    GPS_Data gps_data = {0};
    char *ptr = nmea_line;
    int field = 0;
    while (ptr != NULL) {
        char *token = ptr;
        ptr = strchr(ptr, ',');
        if (ptr) *ptr++ = '\0';
        if (strlen(token) > 0) {
            switch (field) {
                case 1:
                    if (strlen(token) >= 6) {
                        gps_data.hour = (token[0]-'0')*10 + (token[1]-'0');
                        gps_data.min  = (token[2]-'0')*10 + (token[3]-'0');
                        gps_data.sec  = (token[4]-'0')*10 + (token[5]-'0');
                    }
                    break;
                case 2: gps_data.is_valid = (token[0] == 'A'); break;
                case 3: gps_data.latitude = fast_atof(token); break;
                case 4: gps_data.lat_dir = token[0]; break;
                case 5: gps_data.longitude = fast_atof(token); break;
                case 6: gps_data.lon_dir = token[0]; break;
                case 9:
                    if (strlen(token) >= 6) {
                        gps_data.day   = (token[0]-'0')*10 + (token[1]-'0');
                        gps_data.month = (token[2]-'0')*10 + (token[3]-'0');
                        gps_data.year  = (token[4]-'0')*10 + (token[5]-'0');
                    }
                    break;
            }
        }
        field++;
    }
    return gps_data;
}

GPS_Data GPS_DataIn(const uint8_t rx_byte) {
    static char nmea_buffer[NMEA_BUF_SIZE];
    static uint32_t buf_idx = 0;
    GPS_Data gps_data = {0};

    if (rx_byte == '\n' || rx_byte == '\r') {
        if (buf_idx > 6) {
            nmea_buffer[buf_idx] = '\0';

            // Works for both $GPRMC and $GNRMC
            if (nmea_buffer[3] == 'R' && nmea_buffer[4] == 'M' && nmea_buffer[5] == 'C') {
                gps_data = GPS_ParseRMC(nmea_buffer);
            }
        }
        buf_idx = 0;
    } else {
        if (buf_idx < NMEA_BUF_SIZE - 1) {
            nmea_buffer[buf_idx++] = (char)rx_byte;
        }
        else
        {
            buf_idx = 0;
        }
    }
    return gps_data;
}
