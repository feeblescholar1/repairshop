/*
 * objects.c - vector wrappers for managing clients, cars and operations
 */

#include "include/objects.h"

/*
 * Allocates and fills a client struct and links it to its given parent vector.
 * String length validation is done by the caller.
 */
int obj_cl(struct vector *link, const char *name, const char *mail,
        const char *phone)
{
        if (!link)
                return EINV;

        struct client *c = malloc(sizeof(struct client));
        if (!c)
                return EMALLOC;

        strcpy(c->name, name);
        strcpy(c->email, mail);
        strcpy(c->phone, phone);
        c->cars = vct();
        return vct_push(link, c);
}

/*
 * Allocates and fills a car struct and links it to its given client struct.
 * String length validation is done by the caller.
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

/*
 * Allocates and fills an operation struct and links it to its given car struct.
 * String length validation is done by the caller.
 * Pass NULL to date for the current date.
 */
int obj_op(const struct car *parent, const char *desc, double price,
        const char *date)
{
        if (!parent)
                return EINV;

        struct operation *op = malloc(sizeof(struct operation));
        if (!op)
                return EMALLOC;

        strcpy(op->desc, desc);
        op->price = price;

        if (!date)
                op->date = date_now();
        else
                op->date = date_parse(date);

        return vct_push(parent->operations, op);
}

/*
 * Modifies a client's data.
 * String length validation is done by the caller.
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

/*
 * Modifies a car's data.
 * String length validation is done by the caller.
 */
int obj_car_mod(struct car *src, const char *name, const char *plate)
{
        if (!src)
                return EINV;

        strcpy(src->name, name);
        strcpy(src->plate, plate);

        return 0;
}

/*
 * Modifies an operation's data.
 * String length validation is done by the caller.
 */
int obj_mod(struct operation *src, const char *desc, double price,
        const char *date)
{
        if (!src)
                return EINV;

        strcpy(src->desc, desc);
        src->price = price;
        if (!date)
                src->date = date_now();
        else {
                src->date = date_parse(date);
        }

        return 0;
}


/*
 * Removes an operation struct at the given pos in a car struct.
 * The given object is also deallocated.
 */
int obj_op_rm(const struct car *src, idx pos)
{
        return vct_rm(src->operations, pos);
}

/*
 * Removes a car struct at the given position in a client struct.
 * All operation structs and the car struct will be deallocated.
 */
int obj_car_rm(const struct client *src, idx pos)
{
        struct car *car = vct_subptr(src->cars, pos);
        if (!car)
                return EOOB;

        vct_del(car->operations);
        return vct_rm(src->cars, pos);
}

/*
 * Removes a client struct at the given position in a vector.
 * All car structs and its operation structs and the client struct will be freed.
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
