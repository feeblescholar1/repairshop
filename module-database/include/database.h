/*
 * database.h - defines database structure, include this to use the project database.
 */

#ifndef REPAIRSHOP_DATABASE_H
#define REPAIRSHOP_DATABASE_H

#include "objects.h"
#include "vector.h"

struct database {
        char name[NAME_SIZE + 1];
        char desc[DESC_SIZE + 1];
        struct vector *cl;              /* database clients */
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
        const char *new_desc, double new_price, const char *new_date);

int db_cl_rm(const struct database *db, idx cl);
int db_car_rm(const struct database *db, idx cl, idx car);
int db_op_rm(const struct database *db, idx cl, idx car, idx op);

int db_del(struct database *db);
#endif //REPAIRSHOP_DATABASE_H
