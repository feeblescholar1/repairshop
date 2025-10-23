/*
 * File: objects.h
 * Description: Objects definitions for the primary data structure. Wrapper
 *              declarations and its dependencies for those objects are also
 *              found here.
 */
#ifndef REPAIRSHOP_OBJECTS_H
#define REPAIRSHOP_OBJECTS_H
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"
struct client {
        char name[50 + 1];
        char email[30 + 1];
        char phone[20 + 1];
        struct vector *cars;
};
struct car {
        char name[100 + 1];
        char plate[30 + 1];
        struct vector *operations;
};
struct operation {
        char desc[100];
        double price;
        struct tm *date;
};
int client_create(struct vector *parent,
        const char *name,
        const char *email,
        const char *phone);
int car_create(const struct client *parent,
        const char *name,
        const char *plate);
int op_create(const struct car *parent,
        const char *desc,
        double price,
        struct tm *date);
int op_remove(const struct car *parent, size_t pos);
int car_remove(const struct client *parent, size_t pos);
int client_remove(struct vector *parent, size_t pos);
#endif //REPAIRSHOP_OBJECTS_H
