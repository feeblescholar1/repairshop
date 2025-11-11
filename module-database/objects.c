/**
 * @file objects.c
 * @brief Function definitions for the objects used in the project.
 * @details The functions defined here help initialize and manage objects on
 *          the heap using the vectors defined in \c vector.h. The objects are:
 *          clients, cars and operations. They have the same hierarchy as
 *          mentioned.
 *          Operaitions are linked to cars and cars are linked to clients.
 *          Clients can be linked to any vector. This type of arrangement will
 *          allow the program to link multiple objects to the same, higher
 *          precedence one without any duplicate data.
 * @warning Do not use these functions. Use the functions in \c database.c instead.
 */

#include "include/objects.h"

/**
 * @brief Allocates and initializes a client structure and links it to vector.
 * @param link Pointer to the parent vector.
 * @param name The client's name.
 * @param email The client's email address.
 * @param phone The client's phone number.
 * @warning String parameters are not validated, it is the caller's
 *          responsibility to make sure they fit into the structure.
 * @return \c vct_push() - hands it over to the parent vector for linkage
 * @retval 0 On success.
 * @retval EINV If \c link is \c NULL .
 * @retval EMALLOC If the client cannot be allocated.
 */
int obj_cl(struct vector *link, const char *name, const char *email,
           const char *phone)
{
        if (!link)
                return EINV;

        struct client *c = malloc(sizeof(struct client));
        if (!c)
                return EMALLOC;

        strcpy(c->name, name);
        strcpy(c->email, email);
        strcpy(c->phone, phone);
        c->cars = vct();
        return vct_push(link, c);
}

/**
 * @brief Allocates and initializes a car structure and links it to a client.
 * @param link Pointer to the parent client structure.
 * @param name The car's model.
 * @param plate The car's plate number.
 * @warning String parameters are not validated, it is the caller's
 *          responsibility to make sure they fit into the structure.
 * @return \c vct_push() - hands it over to the parent vector for linkage
 * @retval 0 On success.
 * @retval EINV If \c link is \c NULL .
 * @retval EMALLOC If the car cannot be allocated.
 */
int obj_car(const struct client *link, const char *name, const char *plate)
{
        if (!link)
                return EINV;

        struct car *c = malloc(sizeof(struct car));
        if (!c)
                return EMALLOC;

        strcpy(c->name, name);
        strcpy(c->plate, plate);
        c->operations = vct();
        return vct_push(link->cars, c);
}

/**
 * @brief Allocates and initializes an operation structure and links it to a car.
 * @param link Pointer to the parent car structure.
 * @param desc The operation's description.
 * @param price The operation's price.
 * @param date The expiration date which will be parsed to \c op->date_exp . If
 *             it's not needed, pass \c NULL.
 * @warning String parameters are not validated, it is the caller's
 *          responsibility to make sure they fit into the structure.
 * @return \c vct_push() - hands it over to the parent vector for linkage
 * @retval 0 On success.
 * @retval EINV If \c link is \c NULL .
 * @retval EMALLOC If the operation cannot be allocated.
 * @note The creation date (\c op->date_cr) will always be the current date.
 */
int obj_op(const struct car *link, const char *desc, double price,
           const char *date)
{
        if (!link)
                return EINV;

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

        return vct_push(link->operations, op);
}

/**
 * @brief Modifies an existing client's data.
 * @param src A pointer to a client structure to be modified.
 * @param name The client's new name.
 * @param email The client's new email address.
 * @param phone The client's new phone number.
 * @warning String parameters are not validated, it is the caller's
 *          responsibility to make sure they fit into the structure.
 * @retval 0 On success.
 * @retval EINV If \c src is \c NULL .
 */
int obj_cl_mod(struct client *src, const char *name, const char *email,
               const char *phone)
{
        if (!src)
                return EINV;

        strcpy(src->name, name);
        strcpy(src->email, email);
        strcpy(src->phone, phone);

        return 0;
}

/**
 * @brief Modifies an existing car's data.
 * @param src A pointer to a car structure to be modified.
 * @param name The car's new name.
 * @param plate The car's new plate number.
 * @warning String parameters are not validated, it is the caller's
 *          responsibility to make sure they fit into the structure.
 * @retval 0 On success.
 * @retval EINV If \c src is \c NULL .
 */
int obj_car_mod(struct car *src, const char *name, const char *plate)
{
        if (!src)
                return EINV;

        strcpy(src->name, name);
        strcpy(src->plate, plate);

        return 0;
}

/**
 * @brief Modifies an existing operation's data.
 * @param src A pointer to an operation structure to be modified.
 * @param desc The operation's new description.
 * @param price The operation's new price.
 * @param date The operation's new expiration data.
 * @warning String parameters are not validated, it is the caller's
 *          responsibility to make sure they fit into the structure.
 * @retval 0 On success.
 * @retval EINV If \c src is \c NULL .
 */
int obj_mod(struct operation *src, const char *desc, double price,
        const char *date)
{
        if (!src)
                return EINV;

        strcpy(src->desc, desc);
        src->price = price;
        if (!date)
                src->date_cr = date_now();
        else {
                src->date_cr = date_parse(date);
        }

        return 0;
}


/**
 * @brief Removes an operation from a car at the given index.
 * @param src The pointer to the parent car structure.
 * @param pos The index of the operation.
 * @return \c vct_rm()
 */
int obj_op_rm(const struct car *src, idx pos)
{
        return vct_rm(src->operations, pos);
}

/**
 * @brief Removes a car from a client at the given index.
 * @param src The pointer to the parent client structure.
 * @param pos The index of the car.
 * @return \c vct_rm() or \c EOOB if \c pos is out of range.
 * @note This deletes all operations related to the deleted car.
 */
int obj_car_rm(const struct client *src, idx pos)
{
        struct car *car = vct_subptr(src->cars, pos);
        if (!car)
                return EOOB;

        vct_del(car->operations);
        return vct_rm(src->cars, pos);
}

/**
 * @brief Removes a client from a vector at the given index.
 * @param src The pointer to the parent vector structure.
 * @param pos The index of the client.
 * @return \c vct_rm() or \c EOOB if \c pos is out of range.
 * @note This deletes all cars and its operations related to the deleted client.
 */
int obj_cl_rm(struct vector *src, idx pos)
{
        const struct client *client = vct_subptr(src, pos);
        if (!client)
                return EOOB;

        idx client_cars = client->cars->size;
        for (size_t i = 0; i < client_cars; i++) {
                obj_car_rm(client, 0);
        }

        vct_del(client->cars);
        return vct_rm(src, pos);
}
