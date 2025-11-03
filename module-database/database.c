/*
 * database.c - main data structure function definitions
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
                return ERR_CALLOC_NULL;

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
        const char *desc, const double price, const char *date)
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
 * Returns an operation object pointer from a database at the given index.
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
 * Modifies a client's data at the given index.
 * This function does parameter validation.
 * Returns 0 on success and an error code on failure.
 */
int db_modify_client(const struct database *db, const index client_i,
        const char *new_name, const char *new_email, const char *new_phone)
{
        if (!db)
                return ERR_INV_PARAM;

        if (strlen(new_name) > NAME_MAX_LEN + 1 ||
        strlen(new_email) > EMAIL_MAX_LEN + 1 ||
        strlen(new_phone) > PHONENUM_MAX_LEN + 1)
                return ERR_INV_PARAM;

        struct client *client = db_get_client(db, client_i);
        if (!client)
                return ERR_OUT_OF_RANGE;

        return client_modify(client, new_name, new_email, new_phone);
}

/*
 * Modifies a car's data at the given index.
 * This function does parameter validation.
 * Returns 0 on success and an error code on failure.
 */
int db_modify_car(const struct database *db, const index client_i,
        const index car_i, const char *new_name, const char *new_plate)
{
        if (!db)
                return ERR_INV_PARAM;

        if (strlen(new_name) > NAME_MAX_LEN + 1 ||
        strlen(new_plate) > PLATE_MAX_LEN + 1)
                return ERR_INV_PARAM;

        struct car *car = db_get_car(db, client_i, car_i);
        if (!car)
                return ERR_OUT_OF_RANGE;

        return car_modify(car, new_name, new_plate);
}

/*
 * Modifies an operation's data at the given index.
 * This function does parameter validation.
 * Returns 0 on success and an error code on failure.
 */
int db_modify_op(const struct database *db, const index client_i,
        const index car_i, const index op_i, const char *new_desc,
        const double new_price, const char *new_date)
{
        if (!db)
                return ERR_INV_PARAM;

        if (strlen(new_desc) > NAME_MAX_LEN + 1)
                return ERR_INV_PARAM;

        struct operation *op = db_get_op(db, client_i, car_i, op_i);
        if (!op)
                return ERR_OUT_OF_RANGE;

        return op_modify(op, new_desc, new_price, new_date);
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