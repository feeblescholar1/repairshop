/*
 * vector.c - generic vector implementation
 */

#include "include/vector.h"

/*
 * Initializes a vector and return its pointer.
 * This does NOT initialize the array itself.
 * Returns NULL on failure.
 */
struct vector *vct(void)
{
        struct vector *new = malloc(sizeof(struct vector));

        if (!new)
                return EMEMNULL;

        new->items = NULL;
        new->size = 0;
        return new;
}

/*
 * Appends a pointer to the vector.
 * The given pointer must be allocated by the caller.
 * Use this function to initialize vec->items.
 */
int vct_push(struct vector *v, void *data)
{
        if (!v || !data)
                return EINV;

        /* vec->items is NULL, we have to allocate it */
        if (v->size == 0) {
                v->items = malloc(sizeof(void*));
                if (!v->items) {
                        return EMALLOC;
                }
                v->items[0] = data;
                v->size++;
                return 0;
        }

        void **tmp = realloc(v->items, (v->size + 1) * sizeof(void*));
        if (tmp == NULL)
                return EREALLOC;

        v->items = tmp;
        v->items[v->size] = data;
        v->size++;
        return 0;
}

/*
 * Checks if a given position is in vec->items.
 * For internal use only.
 */
inline bool inbounds(const struct vector *v, idx pos)
{
        return (pos < v->size);
}

/*
 * Insert a given pointer to the array into the given position.
 * Indexing starts from 0.
 * Unlike vector_push() this function CANNOT initialize vec–>items.
 * The given pointer must be allocated by the caller.
 */
int vct_insert(struct vector *v, void *data, idx pos)
{
        if (!v || !data)
                return EINV;

        if (!inbounds(v, pos))
                return EOOB;

        if (pos == v->size)
                return vct_push(v, data);

        void **tmp = realloc(v->items, (v->size + 1) * sizeof(void*));
        if (!tmp)
                return EREALLOC;

        v->items = tmp;
        v->size++;

        /* Shift the others to the right to make space at items[index]. */
        for (idx i = v->size; i >= pos; i--) {
                v->items[i] = v->items[i - 1];
        }

        v->items[pos] = data;
        return 0;
}

/*
 * Returns a pointer from a given position.
 * Useful for obtaining subvectors.
 * The given pointer must not be freed. Use vct_rm() for that.
 */
void *vct_subptr(const struct vector *v, idx pos)
{
        if (!inbounds(v, pos))
                return NULL;

        return v->items[pos];
}

/*
 * Removes the last pointer from the array.
 * The allocated memory block is also freed.
 */
int vct_pop(struct vector *v)
{
        if (!v)
                return EINV;

        free(v->items[v->size - 1]);
        v->size--;

        /* no items left, free the pointer array. */
        if (v->size == 0) {
                free(v->items);
                return 0;
        }

        void **tmp = realloc(v->items, (v->size + 1) * sizeof(void*));
        if (tmp == NULL)
                return EREALLOC;

        v->items = tmp;
        return 0;
}

/*
 * Removes a pointer from the array at the given position.
 * The allocated memory block is also freed.
 */
int vct_rm(struct vector *v, idx pos)
{
        if (v == NULL)
                return EINV;

        if (!inbounds(v, pos))
                return EOOB;

        free(v->items[pos]);
        /* reduce the size first to avoid shifting in OOB values later */
        v->size--;

        /* no items left, free the pointer array. */
        if (v->size == 0) {
                free(v->items);
                return 0;
        }

        for (idx i = pos; i < v->size; i++) {
                v->items[i] = v->items[i + 1];
        }

        void **tmp = realloc(v->items, (v->size + 1) * sizeof(void*));
        if (tmp == NULL)
                return EREALLOC;

        v->items = tmp;
        return 0;
}

/*
 * Removes all pointers from the array, then frees the vector pointer.
 * Frees all allocated memory blocks.
 */
int vct_del(struct vector *v)
{
        if (v == NULL)
                return EINV;

        if (v->size == 0){
                free(v);
                return 0;
        }

        /*
         * We free everyone except items[0].
         * The reason behind this is we iterate with size_t to avoid
         * implementation defined behaviour.
         */
        for (idx i = (v->size - 1); i > 0; i--) {
                free(v->items[i]);
        }

        free(v->items[0]);
        free(v->items);
        free(v);
        return 0;
}
