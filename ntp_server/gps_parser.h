
/*
 * gps_parser.h
 *
 *  Created on: Mar 28, 2026
 *      Author: eng_m
 */

#ifndef INC_GPS_PARSER_H_
#define INC_GPS_PARSER_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t hour, min, sec;
    uint8_t day, month, year;
    float latitude, longitude;
    char lat_dir, lon_dir;
    uint8_t is_valid;
    uint8_t receiver_present;
} GPS_Data;





GPS_Data GPS_DataIn(const uint8_t data);

#endif /* INC_GPS_PARSER_H_ */
