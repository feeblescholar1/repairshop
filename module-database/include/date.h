#ifndef REPAIRSHOP_DATE_H
#define REPAIRSHOP_DATE_H

#include <time.h>
#include <stdio.h>

struct date {
        int y, mon, d, h, min;
};

struct date date_now();
struct date date_parse(const char *str);
void date_printf(const struct date *date, char *dst);

#endif //REPAIRSHOP_DATE_H