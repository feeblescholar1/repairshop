/*
 * objects.h - object definitions for the primary data structure
 */

#ifndef REPAIRSHOP_OBJECTS_H
#define REPAIRSHOP_OBJECTS_H

#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "vector.h"
#include "date.h"

/* struct size macros */

#define NAME_SIZE 100
#define EMAIL_SIZE 50
#define PLATE_SIZE 15
#define DESC_SIZE 100
#define PHNUM_SIZE 20

struct client {
        char name[NAME_SIZE + 1];       /* no format */
        char email[EMAIL_SIZE + 1];     /* format: usr@domain */
        char phone[PHNUM_SIZE + 1];     /* format: +1122334455 ... */
        struct vector *cars;            /* this clients car ptrs */
};

struct car {
        char name[NAME_SIZE + 1];       /* no format */
        char plate[PLATE_SIZE + 1];     /* format: ABC123 or ABCD123 */
        struct vector *operations;      /* this cars operation ptrs */
};

struct operation {
        char desc[DESC_SIZE + 1];       /* description, no format */
        double price;                   /* display format: .2f */
        struct date date_cr;            /* format: YYYY-MM-DD HH:MM */
        struct date date_exp;           /* format: YYYY-MM-DD HH:MM */
};

int obj_cl(struct vector *link, const char *name, const char *mail,
        const char *phone);
int obj_car(const struct client *link, const char *name,
        const char *plate);
int obj_op(const struct car *parent, const char *desc, double price,
        const char *date);

int obj_cl_mod(struct client *src, const char *name,
        const char *email, const char *phone);
int obj_car_mod(struct car *src, const char *name,
        const char *plate);
int obj_mod(struct operation *src, const char *desc, double price,
        const char *date);

int obj_op_rm(const struct car *src, idx pos);
int obj_car_rm(const struct client *src, idx pos);
int obj_cl_rm(struct vector *src, idx pos);

#endif //REPAIRSHOP_OBJECTS_H
