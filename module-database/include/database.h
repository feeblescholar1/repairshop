/**
 * @file database.h
 * @brief Module header file. Include this to use the project database.
 */

#ifndef REPAIRSHOP_DATABASE_H
#define REPAIRSHOP_DATABASE_H

#include "objects.h"
#include "vector.h"

/**
 * @struct database database.h
 * @brief Primary data type used in cross-module data management.
 */
struct database {
        char name[NAME_SIZE + 1];       /**< The database's name */
        char desc[DESC_SIZE + 1];       /**< The database's description. */
        struct vector *cl;              /**< The database's client vector. */
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
