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
        if (strlen(name) > NAME_SIZE + 1 || strlen(desc) > DESC_SIZE + 1)
                return NULL;

        struct database *db = malloc(sizeof(struct database));
        if (!db)
                return EMEMNULL;

        strcpy(db->name, name);
        strcpy(db->desc, desc);
        db->cl = vct();
        return db;
}

/*
 * Creates a client object and links it to a database.
 * This function validates its parameters.
 */
int db_cl_add(const struct database *db, const char *name, const char *email,
        const char *phone)
{
        if (!db)
                return EINV;

        if (strlen(name) > NAME_SIZE + 1 || strlen(email) > EMAIL_SIZE + 1
                || strlen(phone) > PHNUM_SIZE + 1)
                return EINV;

        return obj_cl(db->cl, name, email, phone);
}

/*
 * Creates a car object and links it to a database.
 * Car objects can only be linked to an existing client.
 * Throws an error if the client does not exist at the given index.
 * This function validates its parameters.
 */
int db_car_add(const struct database *db, idx cl, const char *name,
        const char *plate)
{
        if (!db)
                return EINV;

        if (strlen(name) > NAME_SIZE + 1 || strlen(plate) > PLATE_SIZE + 1)
                return EINV;

        const struct client *client_ = vct_subptr(db->cl, cl);
        if (client_ == NULL)
                return EOOB;

        return obj_car(client_, name, plate);
}

/*
 * Creates an operation object and links it to a database.
 * Operation objects can only be linked to an existing car object.
 * Throws an error if the client or the car does not exist at the given index.
 * This function validates its parameters.
 */
int db_op_add(const struct database *db, idx cl, idx car,
        const char *desc, double price, const char *date)
{
        if (!db)
                return EINV;

        if (strlen(desc) > DESC_SIZE)
                return EINV;

        const struct car *car_ = db_car_get(db, cl, car);
        if (!car_)
                return EOOB;

        return obj_op(car_, desc, price, date);
}

/*
 * Returns a client object pointer from a database at the given index.
 * Returns NULL on failure.
 */
struct client *db_cl_get(const struct database *db, idx cl)
{
        return vct_subptr(db->cl, cl);
}


/*
 * Returns a car object pointer from a database at the given index.
 * Returns NULL on failure.
 */
struct car *db_car_get(const struct database *db, idx cl, idx car)
{
        const struct client *client_ = db_cl_get(db, cl);
        if (!client_)
                return NULL;

        return vct_subptr(client_->cars, car);
}

/*
 * Returns an operation object pointer from a database at the given index.
 * Returns NULL on failure.
 */
struct operation *db_op_get(const struct database *db, idx cl, idx car, idx op)
{
        const struct car *car_ = db_car_get(db, cl, car);
        if (!car_)
                return NULL;

        return vct_subptr(car_->operations, op);
}

/* Modifies a client's data at the given index. */
int db_cl_mod(const struct database *db, idx cl, const char *name,
        const char *email, const char *phone)
{
        if (!db)
                return EINV;

        if (strlen(name) > NAME_SIZE + 1 || strlen(email) > EMAIL_SIZE + 1 ||
                strlen(phone) > PHNUM_SIZE + 1)
                return EINV;

        struct client *client = db_cl_get(db, cl);
        if (!client)
                return EOOB;

        return obj_cl_mod(client, name, email, phone);
}

/* Modifies a car's data at the given index. */
int db_car_mod(const struct database *db, idx cl, idx car, const char *name,
        const char *plate)
{
        if (!db)
                return EINV;

        if (strlen(name) > NAME_SIZE + 1 || strlen(plate) > PLATE_SIZE + 1)
                return EINV;

        struct car *car_ = db_car_get(db, cl, car);
        if (!car_)
                return EOOB;

        return obj_car_mod(car_, name, plate);
}

/* Modifies an operation's data at the given index. */
int db_op_mod(const struct database *db, idx cl, idx car, idx op,
        const char *new_desc, double new_price, const char *new_date)
{
        if (!db)
                return EINV;

        if (strlen(new_desc) > NAME_SIZE + 1)
                return EINV;

        struct operation *tmp = db_op_get(db, cl, car, op);
        if (!tmp)
                return EOOB;

        return obj_mod(tmp, new_desc, new_price, new_date);
}

/* Removes a client from a database at the given index. */
int db_cl_rm(const struct database *db, idx cl)
{
        return obj_cl_rm(db->cl, cl);
}

/*
 * Removes a car from a database at the given index.
 * The car object will be deallocated.
 */
int db_car_rm(const struct database *db, idx cl, idx car)
{
        const struct client *client_ = db_cl_get(db, cl);
        if (!client_)
                return EOOB;

        return obj_car_rm(client_, car);
}

/*
 * Removes an operation from a database at the given index.
 * The operation object will be deallocated.
 */
int db_op_rm(const struct database *db, idx cl, idx car, idx op)
{
        struct car *car_ = db_car_get(db, cl, car);
        if (!car_)
                return EOOB;

        return obj_op_rm(car_, op);
}

/*
 * Deallocates a given database.
 * Returns 0 on success and error code on failure.
 */
int db_del(struct database *db)
{
        if (!db)
                return EINV;

        const size_t cl_size = db->cl->size;
        for (idx cl = 0; cl < cl_size; cl++) {
                db_cl_rm(db, 0);
        }

        vct_del(db->cl);
        free(db);
        return 0;
}