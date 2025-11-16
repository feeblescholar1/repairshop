/**
 * @file database.h
 * @brief Module header file. Include this to use the project database.
 */

#ifndef REPAIRSHOP_DATABASE_H
#define REPAIRSHOP_DATABASE_H

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
 * @struct database database.h
 * @brief Primary data type used in cross-module data management.
 */
struct database {
        char name[NAME_SIZE + 1];       /**< The database's name */
        char desc[DESC_SIZE + 1];       /**< The database's description. */
        struct vector *cl;              /**< The database's client vector. */
};

/**
 * @struct client database.h
 * @brief A client structure with user data and a car vector.
 */
struct client {
        char name[NAME_SIZE + 1];       /**< The client's name */
        char email[EMAIL_SIZE + 1];     /**< The client's email address. */
        char phone[PHNUM_SIZE + 1];     /**< The client's phone number. */
        struct vector *cars;            /**< This client's car vector. */
};

/**
 * @struct car database.h
 * @brief A car structure with user data and an operation vector.
 * @note Must be linked to an existing client.
 */
struct car {
        char name[NAME_SIZE + 1];       /**< The car's name/model. */
        char plate[PLATE_SIZE + 1];     /**< The car's plate number. */
        struct vector *operations;      /**< This car's operation vector. */
};

/**
 * @struct operation database.h
 * @brief An operation structure with user data and an operation vector.
 * @note Must be linked to an existing car.
 */
struct operation {
        char desc[DESC_SIZE + 1];       /**< The operation's description. */
        double price;                   /**< The operation's price. */
        struct date date_cr;            /**< The date of creation. */
        struct date date_exp;           /**< The date of expiration (if applicable) */
};

struct database *db_init(const char *name, const char *desc);

int db_cl_add(const struct database *db, const char *name,
        const char *email, const char *phone);
int db_car_add(const struct database *db, idx cl,
        const char *name, const char *plate);
int db_op_add(const struct database *db, idx cl, idx car,
        const char *desc, double price, const char *date);

struct client *db_cl_get(const struct database *db, idx cl);
struct car *db_car_get(const struct database *db, idx cl, idx car);
struct operation *db_op_get(const struct database *db, idx cl, idx car, idx op);

int db_cl_mod(const struct database *db, idx cl, const char *name,
        const char *email, const char *phone);
int db_car_mod(const struct database *db, idx cl, idx car, const char *name,
        const char *plate);
int db_op_mod(const struct database *db, idx cl, idx car, idx op,
        const char *desc, double price, const char *date);

int db_cl_rm(const struct database *db, idx cl);
int db_car_rm(const struct database *db, idx cl, idx car);
int db_op_rm(const struct database *db, idx cl, idx car, idx op);

int db_del(struct database *db);
#endif //REPAIRSHOP_DATABASE_H
