/*
 * database.h - defines database structure, include this to use the project database.
 */

#ifndef REPAIRSHOP_DATABASE_H
#define REPAIRSHOP_DATABASE_H

#include "objects.h"
#include "vector.h"

struct database {
        char name[NAME_MAX_LEN + 1];
        char desc[DESCRIPTION_MAX_LEN + 1];
        struct vector *clients;
};

struct database *db_init(const char *name, const char *desc);

int db_add_client(const struct database *db, const char *name,
        const char *email, const char *phone);
int db_add_car(const struct database *db, index client,
        const char *name, const char *plate);
int db_add_op(const struct database *db, index client, index car,
        const char *desc, double price, const char *date);

struct client *db_get_client(const struct database *db, index client);
struct car *db_get_car(const struct database *db, index client, index car);
struct operation *db_get_op(const struct database *db, index client,
        index car, index op);

int db_modify_client(const struct database *db, index client_i,
        const char *new_name, const char *new_email, const char *new_phone);
int db_modify_car(const struct database *db, index client_i,
        index car_i, const char *new_name, const char *new_plate);
int db_modify_op(const struct database *db, index client_i, index car_i,
        index op_i, const char *new_desc, double new_price,
        const char *new_date);

int db_rm_client(const struct database *db, index pos);
int db_rm_car(const struct database *db, index client, index car);
int db_rm_op(const struct database *db, index client, index car, index op);

int db_del(struct database *db);
#endif //REPAIRSHOP_DATABASE_H
