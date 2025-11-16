/**
 * @file database.c
 * @brief Functions for the project database which manages objects.
 * @details The main purpose of the database is to abstract object management
 *          away from the programmer. It also handles memory management, which
 *          means we don't have to worry about memory leaks and other memory
 *          related errors/bugs. Just use \c db_del() to clean up and to avoid
 *          leaks.\n
*           The functions defined here help initialize and manage objects on
 *          the heap using the vectors defined in \c vector.h. The objects are:
 *          clients, cars and operations. They have the same hierarchy as
 *          mentioned.\n
 *          Operaitions are linked to cars and cars are linked to clients.
 *          Clients can be linked to any vector. This type of arrangement will
 *          allow the program to link multiple objects to the same, higher
 *          precedence one without any duplicate data.
 * @note Not all function return values are documented. Visit \c vector.c and
 *       \c date.c for all possible return values.
 */

#include "include/database.h"

/**
 * @brief Allocates and initializes a database on the heap.
 * @param name The database's name.
 * @param desc The database's description.
 * @retval db On success.
 * @retval NULL If \c name or \c desc is too large.
 * @retval EMEMNULL If the database cannot be allocated.
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

/**
 * @brief Adds a client to the database.
 * @param db The pointer of the destination database.
 * @param name The client's name.
 * @param email The client's email address. Format: 'username@domain'
 * @param phone The client's phone number. Format: '+11222333334444'
 * @retval 0 On success.
 * @retval EINV If \c db is \c NULL or at least 1 string is too large.
 * @retval EMALLOC If the new client cannot be allocated.
 */
int db_cl_add(const struct database *db, const char *name, const char *email,
              const char *phone)
{
        if (!db || strlen(name) > NAME_SIZE + 1
                || strlen(email) > EMAIL_SIZE + 1
                || strlen(phone) > PHNUM_SIZE + 1)
                return EINV;

        struct client *c = malloc(sizeof(struct client));
        if (!c)
                return EMALLOC;

        strcpy(c->name, name);
        strcpy(c->email, email);
        strcpy(c->phone, phone);
        c->cars = vct();

        return vct_push(db->cl, c);
}

/**
 * @brief Adds a car to the database.
 * @param db The pointer of the destination database.
 * @param cl The client's index in the database to link the car to.
 * @param name The car's name.
 * @param plate The car's plate number. Format: 'ABC123' or 'ABCD123'.
 * @retval 0 On success.
 * @retval EINV If \c db is \c NULL or at least 1 string is too large.
 * @retval EOOB If the client doesn't exist in the database.
 * @retval EMALLOC If the new car cannot be allocated.
 */
int db_car_add(const struct database *db, idx cl, const char *name,
        const char *plate)
{
        if (!db || strlen(name) > NAME_SIZE + 1
                || strlen(plate) > PLATE_SIZE + 1)
                return EINV;

        struct client *client_ = vct_subptr(db->cl, cl);
        if (!client_)
                return EOOB;

        struct car *c = malloc(sizeof(struct car));
        if (!c)
                return EMALLOC;

        strcpy(c->name, name);
        strcpy(c->plate, plate);
        c->operations = vct();

        return vct_push(client_->cars, c);
}

/**
 * @brief Adds an operation to the database.
 * @param db The pointer to the destination database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @param desc The operation's description.
 * @param price The operation's price.
 * @param date The expiration date (if applicable). Pass to \c NULL to ignore.
 *             Format: 'YYYY:MM:DD HH:MM'
 * @retval 0 On success.
 * @retval EINV If \c db is \c NULL or at least 1 string is too large.
 * @retval EOOB If the client or the car doesn't exist in the database.
 * @retval EMALLOC If the new operation cannot be allocated.
 */
int db_op_add(const struct database *db, idx cl, idx car,
              const char *desc, double price, const char *date)
{
        if (!db || strlen(desc) > DESC_SIZE)
                return EINV;

        struct car *car_ = db_car_get(db, cl, car);
        if (!car_)
                return EOOB;

        struct operation *op = malloc(sizeof(struct operation));
        if (!op)
                return EMALLOC;

        strcpy(op->desc, desc);
        op->price = price;

        if (date)
                op->date_exp = date_parse(date);
        else
                /* Set the first element to 0 to know this is not used. */
                op->date_exp.y = 0;


        op->date_cr = date_now();

        return vct_push(car_->operations, op);
}

/**
 * @brief Looks for a client in the database.
 * @param db The pointer to the source database.
 * @param cl The client's index in the database.
 * @return A cast \c vct_subptr() .
 * @retval client* On success.
 * @retval NULL On failure.
 */
struct client *db_cl_get(const struct database *db, idx cl)
{
        return vct_subptr(db->cl, cl);
}


/**
 * @brief Looks for a car in the database.
 * @param db The pointer to the source database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @return A cast \c vct_subptr() .
 * @retval car* On success.
 * @retval NULL On failure.
 */
struct car *db_car_get(const struct database *db, idx cl, idx car)
{
        const struct client *client_ = db_cl_get(db, cl);
        if (!client_)
                return NULL;

        return vct_subptr(client_->cars, car);
}

/**
 * @brief Looks for an operation in the database.
 * @param db The pointer to the source database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @param op The operation's index in the database.
 * @return A cast \c vct_subptr() .
 * @retval car* On success.
 * @retval NULL On failure.
 */
struct operation *db_op_get(const struct database *db, idx cl, idx car, idx op)
{
        const struct car *car_ = db_car_get(db, cl, car);
        if (!car_)
                return NULL;

        return vct_subptr(car_->operations, op);
}

/**
 * @brief Looks for and modifies a client in the database.
 * @param db The pointer to the source database.
 * @param cl The client's index in the database.
 * @param name The client's new name.
 * @param email The client's new email address.
 * @param phone The client's new phone number.
 * @retval 0 On success.
 * @retval EINV If \c db is \c NULL or at least 1 string is too large.
 * @retval EOOB If the client doesn't exist in the database.
 * @note For input formattting see \c db_cl_add() .
 */
int db_cl_mod(const struct database *db, idx cl, const char *name,
        const char *email, const char *phone)
{
        if (!db || strlen(name) > NAME_SIZE + 1
                || strlen(email) > EMAIL_SIZE + 1
                || strlen(phone) > PHNUM_SIZE + 1)
                return EINV;

        struct client *client = db_cl_get(db, cl);
        if (!client)
                return EOOB;

        strcpy(client->name, name);
        strcpy(client->email, email);
        strcpy(client->phone, phone);

        return 0;
}

/**
 * @brief Looks for and modifies a car in the database.
 * @param db The pointer to the source database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @param name The car's new name.
 * @param plate The car's new plate number.
 * @retval 0 On success.
 * @retval EINV If \c db is \c NULL or at least 1 string is too large.
 * @retval EOOB If the client or the car doesn't exist in the database.
 * @note For input formattting see \c db_car_add() .
 */
int db_car_mod(const struct database *db, idx cl, idx car, const char *name,
        const char *plate)
{
        if (!db || strlen(name) > NAME_SIZE + 1
                || strlen(plate) > PLATE_SIZE + 1)
                return EINV;

        struct car *car_ = db_car_get(db, cl, car);
        if (!car_)
                return EOOB;

        strcpy(car_->name, name);
        strcpy(car_->plate, plate);

        return 0;
}

/**
 * @brief Looks for and modifies an operation in the database.
 * @param db The pointer to the destination database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @param op The operation's index in the database.
 * @param desc The operation's new description.
 * @param price The operation's new price.
 * @param date The new expiration date (if applicable). Pass to \c NULL to ignore.
 * @retval 0 On success.
 * @retval EINV If \c db is \c NULL or at least 1 string is too large.
 * @retval EOOB If the client/car/operation doesn't exist in the database.
 * @note For input formattting see \c db_op_add() .
 */
int db_op_mod(const struct database *db, idx cl, idx car, idx op,
        const char *desc, double price, const char *date)
{
        if (!db || strlen(desc) > NAME_SIZE + 1)
                return EINV;

        struct operation *op_ = db_op_get(db, cl, car, op);
        if (!op_)
                return EOOB;

        strcpy(op_->desc, desc);
        op_->price = price;

        if (date)
                op_->date_exp = date_parse(date);
        else
                /* Set the first element to 0 to know this is not used. */
                op_->date_exp.y = 0;

        return 0;
}

/**
 * @brief Removes a client from the database.
 * @param db The pointer to the source database.
 * @param cl The client's index in the database.
 * @retval 0 On success.
 * @retval EOOB If the client doesn't exist.
 */
int db_cl_rm(const struct database *db, idx cl)
{
        struct client *client = db_cl_get(db, cl);
        if (!client)
                return EOOB;

        idx client_cars = client->cars->size;
        for (size_t i = 0; i < client_cars; i++) {
                db_car_rm(db, cl, 0);
        }

        vct_del(client->cars);
        return vct_rm(db->cl, cl);
}

/**
 * @brief Removes a car from the database.
 * @param db The pointer to the source database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @return \c obj_car_rm() with the proper paramaters.
 * @retval 0 On success.
 * @retval EOOB If the client or the car doesn't exist.
 */
int db_car_rm(const struct database *db, idx cl, idx car)
{
        struct client *client = db_cl_get(db, cl);
        struct car *car_ = db_car_get(db, cl, car);
        if (!client || !car_)
                return EOOB;

        vct_del(car_->operations);
        return vct_rm(client->cars, car);
}

/**
 * @brief Removes an operation from the database.
 * @param db The pointer to the source database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @param op The operation's index in the database.
 * @return \c obj_op_rm() with the proper paramaters.
 * @retval 0 On success.
 * @retval EOOB If the car doesn't exist.
 */
int db_op_rm(const struct database *db, idx cl, idx car, idx op)
{
        struct car *car_ = db_car_get(db, cl, car);
        if (!car_)
                return EOOB;

        return vct_rm(car_->operations, op);
}

/**
 * @brief Deletes a database. Use this to clean up all allocated blocks.
 * @param db The pointer to the database to be destroyed.
 * @retval 0 On success
 * @retval EINV If \c db is \c NULL .
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

        /* To avoid calling this function twice, set db to NULL.*/
        db = NULL;
        return 0;
}
