/*
 * File: objects.c
 * Description: Wrapper definitions found in 'objects.h'.
 */
#include "include/objects.h"
/*
 * Allocates and fills a client struct and links it to its given parent vector.
 * Parameter validation is done by the caller.
 * Returns 0 on success and an error code on failure.
 */
int client_create(struct vector *parent,
        const char *name,
        const char *email,
        const char *phone)
{
        struct client *c = calloc(1, sizeof(struct client));
        if (c == NULL)
                return ERR_MALLOC;
        strcpy(c->name, name);
        strcpy(c->email, email);
        strcpy(c->phone, phone);
        c->cars = v_init();
        return v_push_back(parent, c);
}

/*
 * Allocates and fills a car struct and links it to its given client struct.
 * Parameter validation is done by the caller.
 * Return 0 on success and an error code on failure.
 */
int car_create(const struct client *parent,
        const char *name,
        const char *plate)
{
        struct car *c = calloc(1, sizeof(struct car));
        strcpy(c->name, name);
        strcpy(c->plate, plate);
        c->operations = v_init();
        return v_push_back(parent->cars, c);
}

/*
 * Allocates and fills a operation struct and links it to its given car struct.
 * Parameter validation is done by the caller.
 * Pass NULL to date for the current date.
 * Return 0 on success and an error code on failure.
 */
int op_create(const struct car *parent,
        const char *desc,
        const double price,
        struct tm *date)
{
        struct operation *op = calloc(1, sizeof(struct operation));
        strcpy(op->desc, desc);
        op->price = price;
        if (date == NULL) {
                time_t now = time(NULL);
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
int op_remove(const struct car *parent, const size_t pos)
{
        return v_rm(parent->operations, pos);
}

/*
 * Removes a car struct at the given position in a client struct.
 * All operation structs and the car struct will be deallocated.
 * Returns 0 on success and error code on failure.
 */
int car_remove(const struct client *parent, const size_t pos)
{
        struct car *car = v_get_item_ptr(parent->cars, pos);
        if (car == NULL)
                return ERR_OUT_OF_RANGE;
        v_del(car->operations);
        return v_rm(parent->cars, pos);
}

/*
 * Removes a client struct at the given position in a vector.
 * All car structs and its operation structs and the client struct will be freed.
 * Returns 0 on success and error code on failure.
 */
int client_remove(struct vector *parent, const size_t pos)
{
        const struct client *client = v_get_item_ptr(parent, pos);
        if (client == NULL)
                return ERR_OUT_OF_RANGE;
        const size_t client_cars = client->cars->size;
        for (size_t i = 0; i < client_cars; i++) {
                car_remove(client, 0);
        }
        if (client_cars == 0) // the client has 0 cars, but has a vector
                v_del(client->cars);
        return v_rm(parent, pos);
}
