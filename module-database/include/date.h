/**
 * @file date.h
 * @brief Date structure and function prototype definitons.
 */

#ifndef REPAIRSHOP_DATE_H
#define REPAIRSHOP_DATE_H

#include <time.h>
#include <stdio.h>

/**
 * @struct date date.h
 * @brief A custom date structure.
 */
typedef struct date {
        int y;          /**< Year  */
        int mon;        /**< Month */
        int d;          /**< Day */
        int h;          /**< Hour */
        int min;        /**< Minute */
} date;

date date_now(void);
date date_parse(const char *str);
void date_printf(const date *date, char *dst);
double date_diff(const date *d1, const date *d2);

#endif //REPAIRSHOP_DATE_H
