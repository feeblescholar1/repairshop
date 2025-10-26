/*
 * database.c - primary data structure function
 */

#include "include/database.h"

/*
 * Allocates and initializes a database.
 * Returns the pointer to the new database on success and NULL on failure.
 */
struct database *db_init(const char *name, const char *desc)
{
        if (strlen(name) > NAME_MAX_LEN + 1 ||
                strlen(desc) > DESCRIPTION_MAX_LEN + 1)
                return NULL;

        struct database *db = calloc(1, sizeof(struct database));
        if (db == NULL)
                return ERR_MALLOC_NULL;

        strcpy(db->name, name);
        strcpy(db->desc, desc);
        db->clients = v_init();
        return db;
}

/*
 * Creates a client object and links it to a database.
 * This function validates its parameters.
 * Returns 0 on success and an error code on failure.
 */
int db_add_client(const struct database *db, const char *name,
        const char *email, const char *phone)
{
        if (!db)
                return ERR_INV_PARAM;

        if (strlen(name) > NAME_MAX_LEN + 1 || strlen(email) > EMAIL_MAX_LEN + 1
                || strlen(phone) > PHONENUM_MAX_LEN + 1)
                return ERR_INV_PARAM;

        return client_create(db->clients, name, email, phone);
}

/*
 * Creates a car object and links it to a database.
 * Car objects can only be linked to an existing client.
 * Throws an error if the client does not exist at the given index.
 * This function validates its parameters.
 * Returns 0 on success and an error code on failure.
 */
int db_add_car(const struct database *db, const index client,
        const char *name, const char *plate)
{
        if (!db)
                return ERR_INV_PARAM;

        if (strlen(name) > NAME_MAX_LEN + 1 || strlen(plate) > PLATE_MAX_LEN + 1)
                return ERR_INV_PARAM;

        const struct client *client_ = v_get_item_ptr(db->clients, client);
        if (client_ == NULL)
                return ERR_OUT_OF_RANGE;

        return car_create(client_, name, plate);
}

/*
 * Creates an operation object and links it to a database.
 * Operation objects can only be linked to an existing car object.
 * Throws an error if the client or the car does not exist at the given index.
 * This function validates its parameters.
 * Returns 0 on success and an error code on failure.
 */
int db_add_op(const struct database *db, const index client, const index car,
        const char *desc, const double price, const struct tm *date)
{
        if (!db)
                return ERR_INV_PARAM;

        if (strlen(desc) > DESCRIPTION_MAX_LEN)
                return ERR_INV_PARAM;

        const struct car *car_ = db_get_car(db, client, car);
        if (!car_)
                return ERR_OUT_OF_RANGE;

        return op_create(car_, desc, price, date);
}

/*
 * Returns a client object pointer from a database at the given index.
 * Returns NULL on failure.
 */
struct client *db_get_client(const struct database *db, const index client)
{
        return v_get_item_ptr(db->clients, client);
}

/*
 * Returns a car object pointer from a database at the given index.
 * Returns NULL on failure.
 */
struct car *db_get_car(const struct database *db, const index client,
        const index car)
{
        const struct client *client_ = db_get_client(db, client);
        if (!client_)
                return NULL;

        return v_get_item_ptr(client_->cars, car);
}

/*
 * Returns a operation object pointer from a database at the given index.
 * Returns NULL on failure.
 */
struct operation *db_get_op(const struct database *db, const index client,
        const index car, const index op)
{
        const struct car *car_ = db_get_car(db, client, car);
        if (!car_)
                return NULL;

        return v_get_item_ptr(car_->operations, op);
}

/*
 * Removes a client from a database at the given index.
 * The client object will be deallocated.
 * Returns 0 on success and an error code on failure.
 */
int db_rm_client(const struct database *db, const index pos)
{
        return client_remove(db->clients, pos);
}

/*
 * Removes a car from a database at the given index.
 * The car object will be deallocated.
 * Returns 0 on success and an error code on failure.
 */
int db_rm_car(const struct database *db, const index client, const index car)
{
        const struct client *client_ = db_get_client(db, client);
        if (!client_)
                return ERR_OUT_OF_RANGE;

        return car_remove(client_, car);
}

/*
 * Removes an operation from a database at the given index.
 * The operation object will be deallocated.
 * Returns 0 on success and an error code on failure.
 */
int db_rm_op(const struct database *db, const index client, const index car,
        const index op)
{
        const struct car *car_ = db_get_car(db, client, car);
        if (!car_)
                return ERR_OUT_OF_RANGE;

        return op_remove(car_, op);
}

/*
 * Deallocates a given database.
 * Returns 0 on success and error code on failure.
 */
int db_del(struct database *db)
{
        if (!db)
                return ERR_INV_PARAM;

        const size_t client_num = db->clients->size;
        for (index client_i = 0; client_i < client_num; client_i++) {
                db_rm_client(db, 0);
        }

        v_del(db->clients);
        free(db);
        return 0;
}