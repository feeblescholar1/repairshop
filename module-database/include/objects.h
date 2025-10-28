/*
 * objects.h - object definitions for the primary data structure
 */

#ifndef REPAIRSHOP_OBJECTS_H
#define REPAIRSHOP_OBJECTS_H

#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "vector.h"

#define NAME_MAX_LEN 100
#define EMAIL_MAX_LEN 50
#define PLATE_MAX_LEN 15
#define DESCRIPTION_MAX_LEN 100
#define PHONENUM_MAX_LEN 20

struct client {
        char name[NAME_MAX_LEN + 1];
        char email[EMAIL_MAX_LEN + 1];
        char phone[PHONENUM_MAX_LEN + 1];
        struct vector *cars;
};

struct car {
        char name[NAME_MAX_LEN + 1];
        char plate[PLATE_MAX_LEN + 1];
        struct vector *operations;
};

struct operation {
        char desc[DESCRIPTION_MAX_LEN + 1];
        double price;
        const struct tm *date;
};

int client_create(struct vector *parent, const char *name, const char *email,
        const char *phone);
int car_create(const struct client *parent, const char *name,
        const char *plate);
int op_create(const struct car *parent, const char *desc, double price,
        const struct tm *date);

int client_modify(struct client *client, const char *new_name,
        const char *new_email, const char *new_phone);
int car_modify(struct car *car, const char *new_name,
        const char *new_plate);
int op_modify(struct operation *op, const char *new_desc, double new_price,
        const struct tm *date);

int op_remove(const struct car *parent, index pos);
int car_remove(const struct client *parent, index pos);
int client_remove(struct vector *parent, index pos);

#endif //REPAIRSHOP_OBJECTS_H
