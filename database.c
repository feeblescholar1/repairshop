#include "include/database.h"

struct database *db_init(const char *name, const char *desc)
{
        if (strlen(name) > NAME_MAX_LEN || strlen(desc) > DESCRIPTION_MAX_LEN)
                return NULL;

        struct database *db = calloc(1, sizeof(struct database));
        if (db == NULL)
                return ERR_MALLOC_NULL;

        strcpy(db->name, name);
        strcpy(db->desc, desc);
        db->clients = v_init();
        return db;
}

int db_add_client(const struct database *db, const char *name,
        const char *email, const char *phone)
{
        if (!db)
                return ERR_INV_PARAM;

        if (strlen(name) > NAME_MAX_LEN || strlen(email) > EMAIL_MAX_LEN
                || strlen(phone) > PHONENUM_MAX_LEN)
                return ERR_INV_PARAM;

        return client_create(db->clients, name, email, phone);
}

int db_add_car(const struct database *db, const index client,
        const char *name, const char *plate)
{
        if (!db)
                return ERR_INV_PARAM;

        if (strlen(name) > NAME_MAX_LEN || strlen(plate) > PLATE_MAX_LEN)
                return ERR_INV_PARAM;

        const struct client *client_ = v_get_item_ptr(db->clients, client);
        if (client_ == NULL)
                return ERR_OUT_OF_RANGE;

        return car_create(client_, name, plate);
}

int db_add_op(const struct database *db, const index client, const index car,
        const char *desc, const double price, const struct tm *date)
{
        if (!db)
                return ERR_INV_PARAM;

        if (strlen(desc) > DESCRIPTION_MAX_LEN)
                return ERR_INV_PARAM;

        const struct client *client_ = v_get_item_ptr(db->clients, client);
        if (!client_)
                return ERR_OUT_OF_RANGE;

        const struct car *car_ = v_get_item_ptr(client_->cars, car);
        if (!car_)
                return ERR_OUT_OF_RANGE;

        return op_create(car_, desc, price, date);
}

struct client *db_get_client(const struct database *db, const index client)
{
        return v_get_item_ptr(db->clients, client);
}

struct car *db_get_car(const struct database *db, const index client,
        const index car)
{
        const struct client *client_ = db_get_client(db, client);
        if (!client_)
                return NULL;

        return v_get_item_ptr(client_->cars, car);
}

struct operation *db_get_op(const struct database *db, const index client,
        const index car, const index op)
{
        const struct car *car_ = db_get_car(db, client, car);
        if (!car_)
                return NULL;

        return v_get_item_ptr(car_->operations, op);
}

int db_rm_client(const struct database *db, const index pos)
{
        return client_remove(db->clients, pos);
}

int db_car_rm(const struct database *db, const index client, const index car)
{
        const struct client *client_ = db_get_client(db, client);
        if (!client_)
                return ERR_OUT_OF_RANGE;

        return car_remove(client_, car);
}

int db_rm_op(const struct database *db, const index client, const index car,
        const index op)
{
        const struct car *car_ = db_get_car(db, client, car);
        if (!car_)
                return ERR_OUT_OF_RANGE;

        return op_remove(car_, op);
}
