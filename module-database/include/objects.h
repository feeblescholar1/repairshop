/**
 * @file objects.h
 * @brief Object structure definitions and size macros.
 */

#ifndef REPAIRSHOP_OBJECTS_H
#define REPAIRSHOP_OBJECTS_H

#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "vector.h"
#include "date.h"

#define NAME_SIZE 100   /**< Size of a name string. */
#define EMAIL_SIZE 50   /**< Size of an email string. */
#define PLATE_SIZE 15   /**< Size of a car plate string. */
#define DESC_SIZE 100   /**< Size of a description string. */
#define PHNUM_SIZE 20   /**< Size of a phone number string. */

/**
 * @struct client objects.h
 * @brief A client structure with user data and a car vector.
 */
struct client {
        char name[NAME_SIZE + 1];       /**< The client's name */
        char email[EMAIL_SIZE + 1];     /**< The client's email address. */
        char phone[PHNUM_SIZE + 1];     /**< The client's phone number. */
        struct vector *cars;            /**< This client's car vector. */
};

/**
 * @struct car objects.h
 * @brief A car structure with user data and an operation vector.
 * @note Must be linked to an existing client.
 */
struct car {
        char name[NAME_SIZE + 1];       /**< The car's name/model. */
        char plate[PLATE_SIZE + 1];     /**< The car's plate number. */
        struct vector *operations;      /**< This car's operation vector. */
};

/**
 * @struct operation objects.h
 * @brief An operation structure with user data and an operation vector.
 * @note Must be linked to an existing car.
 */
struct operation {
        char desc[DESC_SIZE + 1];       /**< The operation's description. */
        double price;                   /**< The operation's price. */
        struct date date_cr;            /**< The date of creation. */
        struct date date_exp;           /**< The date of expiration (if applicable) */
};

int obj_cl(struct vector *link, const char *name, const char *email,
        const char *phone);
int obj_car(const struct client *link, const char *name,
        const char *plate);
int obj_op(const struct car *link, const char *desc, double price,
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
