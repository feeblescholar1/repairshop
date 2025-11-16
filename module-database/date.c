/**
 * @file date.c
 * @brief Custom date functions.
 * @details The functions defined in here manage the custom date structure, which is based on \c struct \c tm .
 */

#include "include/date.h"

/**
 * @brief Gets the current date and time and creates a date structure.
 * @return A newly created date structure.
 * @note Unlike the objects in the database in \c database.h this object is \b not allocated on the heap.
 */
date date_now(void)
{
        const time_t now = time(NULL);
        const struct tm *src = localtime(&now);
        /*
         * struct tm handles time differently:
         * tm_year is 0 when its 1900
         * tm_mon is 0 when its January
         * We shift these values by 1900 and 1 respectively.
         */
        const date ret = {src->tm_year + 1900,src->tm_mon + 1, src->tm_mday,src->tm_hour, src->tm_min};
        return ret;
}

/**
 * @brief Parses str to a new date structure.
 * @param str A date to be parsed in a YYYY-MM-DD HH:MM format.
 * @warning It's the caller's responsibility to ensure the correct format.
 * @return A newly created date structure basen on str.
 * @note Unlike the objects in the database in \c database.h this object is \b not allocated on the heap.
 * @warning If the parsing fails, itt will mark the date as uninitialized.
 */
date date_parse(const char *str)
{
        date ret;
        if (sscanf(str, "%d-%d-%d %d:%d", &ret.y, &ret.mon, &ret.d, &ret.h, &ret.min) != 5)
                ret.y = 0;
        return ret;
}

/**
 * @brief Similarly to \c asctime() , this function makes a user-readable string from a date.
 * @param date The date to be 'printed'.
 * @param dst The destination string. The output format will be: YYYY-MM-DD HH:MM.
 * @warning The size of \c dst must be at least 17.
 * @return -
 */
void date_printf(const date *date, char *dst)
{
        snprintf(dst, 17, "%d-%02d-%02d %02d:%02d", date->y, date->mon, date->d, date->h, date->min);
}

/**
 * @brief Calculates the difference between date and date2.
 * @return The time difference in days.
 */
double date_diff(const date *d1, const date *d2)
{
        struct tm t1 = {0};
        t1.tm_year = d1->y - 1900;
        t1.tm_mon = d1->mon - 1;
        t1.tm_mday = d1->d;
        t1.tm_hour = d1->h;
        t1.tm_min = d1->min;

        struct tm t2 = {0};
        t2.tm_year = d2->y - 1900;
        t2.tm_mon = d2->mon - 1;
        t2.tm_mday = d2->d;
        t2.tm_hour = d2->h;
        t2.tm_min = d2->min;

        double ret = difftime(mktime(&t1), mktime(&t2)) / (60 * 60 * 24);
        return ret;
}
