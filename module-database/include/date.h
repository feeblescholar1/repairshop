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
struct date {
        int y;          /**< Year  */
        int mon;        /**< Month */
        int d;          /**< Day */
        int h;          /**< Hour */
        int min;        /**< Minute */
};

struct date date_now(void);
struct date date_parse(const char *str);
void date_printf(const struct date *date, char *dst);

#endif //REPAIRSHOP_DATE_H