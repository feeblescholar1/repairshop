/*
 * objects.c - vector wrappers for managing clients, cars and operations
 */

#include "include/objects.h"

/*
 * Allocates and fills a client struct and links it to its given parent vector.
 * String length validation is done by the caller.
 * Returns 0 on success and an error code on failure.
 */
int client_create(struct vector *parent, const char *name, const char *email,
        const char *phone)
{
        if (!parent)
                return ERR_INV_PARAM;

        struct client *c = calloc(1, sizeof(struct client));
        if (!c)
                return ERR_CALLOC;

        strcpy(c->name, name);
        strcpy(c->email, email);
        strcpy(c->phone, phone);
        c->cars = v_init();
        return v_push_back(parent, c);
}

/*
 * Allocates and fills a car struct and links it to its given client struct.
 * String length validation is done by the caller.
 * Return 0 on success and an error code on failure.
 */
int car_create(const struct client *parent, const char *name,
        const char *plate)
{
        if (!parent)
                return ERR_INV_PARAM;

        struct car *c = calloc(1, sizeof(struct car));
        if (!c)
                return ERR_CALLOC;

        strcpy(c->name, name);
        strcpy(c->plate, plate);
        c->operations = v_init();
        return v_push_back(parent->cars, c);
}

/*
 * Allocates and fills an operation struct and links it to its given car struct.
 * String length validation is done by the caller.
 * Pass NULL to date for the current date.
 * Return 0 on success and an error code on failure.
 */
int op_create(const struct car *parent, const char *desc, const double price,
        const struct tm *date)
{
        if (!parent)
                return ERR_INV_PARAM;

        struct operation *op = calloc(1, sizeof(struct operation));
        if (!op)
                return ERR_CALLOC;

        strcpy(op->desc, desc);
        op->price = price;

        if (!date) {
                const time_t now = time(NULL);
                op->date = localtime(&now);
        }
        else {
                op->date = date;
        }

        return v_push_back(parent->operations, op);
}

/*
 * Removes an operation struct at the given pos in a car struct.
 * The given object is also deallocated.
 * Returns 0 on success and an error code on failure.
 */
int op_remove(const struct car *parent, const index pos)
{
        return v_rm(parent->operations, pos);
}

/*
 * Removes a car struct at the given position in a client struct.
 * All operation structs and the car struct will be deallocated.
 * Returns 0 on success and error code on failure.
 */
int car_remove(const struct client *parent, const index pos)
{
        struct car *car = v_get_item_ptr(parent->cars, pos);
        if (!car)
                return ERR_OUT_OF_RANGE;

        v_del(car->operations);
        return v_rm(parent->cars, pos);
}

/*
 * Removes a client struct at the given position in a vector.
 * All car structs and its operation structs and the client struct will be freed.
 * Returns 0 on success and error code on failure.
 */
int client_remove(struct vector *parent, const index pos)
{
        const struct client *client = v_get_item_ptr(parent, pos);
        if (!client)
                return ERR_OUT_OF_RANGE;

        const index client_cars = client->cars->size;
        for (size_t i = 0; i < client_cars; i++) {
                car_remove(client, 0);
        }

        v_del(client->cars);

        return v_rm(parent, pos);
}
