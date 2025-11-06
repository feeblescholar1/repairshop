/*
 * date.c - custom date functions
 */

#include "include/date.h"

/* Returns the current date. */
struct date date_now(void)
{
        const time_t now = time(NULL);
        const struct tm *src = localtime(&now);

        /*
         * struct tm handles time differently:
         * tm_year is 0 when its 1900
         * tm_mon is 0 when its January
         * We shift these values by 1900 and 1 respectively.
         */
        const struct date ret = {src->tm_year + 1900,
                src->tm_mon + 1, src->tm_mday,
                src->tm_hour, src->tm_min};

        return ret;
}

/* Parses str and returns the date. */
struct date date_parse(const char *str)
{
        struct date ret;
        sscanf(str, "%d-%d-%d %d:%d", &ret.y, &ret.mon, &ret.d, &ret.h,
                &ret.min);

        return ret;
}

/* 'Prints' date to dst for display */
void date_printf(const struct date *date, char *dst)
{
        /*
         * format: YYYY-MM-DD HH:MM
         * 12 characters + 3 separators + 1 whitespace + 1 null-terminator
         * Therefore sizeof(dst) has to be at least 17.
         */
        snprintf(dst, 17, "%d-%02d-%02d %02d:%02d", date->y, date->mon, date->d,
                date->h, date->min);
}