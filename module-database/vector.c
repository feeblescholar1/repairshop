/*
 * vector.c - generic vector implementation
 */

#include "include/vector.h"

/*
 * Initializes a vector and return its pointer.
 * This does NOT initialize the array itself.
 * Returns NULL on failure.
 */
struct vector *v_init(void)
{
        struct vector *new = calloc(1, sizeof(struct vector));

        if (new == NULL)
                return ERR_MALLOC_NULL;

        new->items = NULL;
        new->size = 0;
        return new;
}

/*
 * Appends a pointer to the vector.
 * The given pointer must be allocated by the caller.
 * Use this function to initialize vec->items.
 * Returns 0 on success and an error code on failure.
 */
int v_push_back(struct vector *vec, void *data)
{
        if (vec == NULL || data == NULL)
                return ERR_INV_PARAM;

        /* vec->items is NULL, we have to allocate it */
        if (vec->size == 0) {
                vec->items = calloc(1, sizeof(void*));
                if (vec->items == NULL) {
                        return ERR_CALLOC;
                }
                vec->items[0] = data;
                vec->size++;
                return 0;
        }

        void **tmp = realloc(vec->items, (vec->size + 1) * sizeof(void*));
        if (tmp == NULL)
                return ERR_REALLOC;

        vec->items = tmp;
        vec->items[vec->size] = data;
        vec->size++;
        return 0;
}

/*
 * Checks if a given position is in vec->items.
 * For internal use only.
 */
inline bool vint_bounds_check(const struct vector *vec, const index pos)
{
        return (pos < vec->size);
}

/*
 * Insert a given pointer to the array into the given position.
 * Indexing starts from 0.
 * Unlike v_push_back() this function CANNOT initialize vec–>items.
 * The given pointer must be allocated by the caller.
 * Returns 0 on success and an error code on failure.
 */
int v_insert(struct vector *vec, void *data, index pos)
{
        if (!vec || !data)
                return ERR_INV_PARAM;

        if (!vint_bounds_check(vec, pos))
                return ERR_OUT_OF_RANGE;

        if (pos == vec->size)
                return v_push_back(vec, data);

        void **tmp = realloc(vec->items, (vec->size + 1) * sizeof(void*));
        if (!tmp)
                return ERR_REALLOC;

        vec->items = tmp;
        vec->size++;

        /* Shift the others to the right to make space at items[index]. */
        for (index i = vec->size; i >= pos; i--) {
                vec->items[i] = vec->items[i - 1];
        }

        vec->items[pos] = data;
        return 0;
}

/*
 * Returns a read only pointer from this given position.
 * Useful for obtaining subvectors.
 * The given pointer must not be freed. Use v_rm() for that.
 */
void *v_get_item_ptr(const struct vector *vec, index pos)
{
        if (!vint_bounds_check(vec, pos))
                return NULL;

        return vec->items[pos];
}

/*
 * Removes the last pointer from the array.
 * The allocated memory block is also freed.
 * Returns 0 on success and an error code on failure.
 */
int v_pop_back(struct vector *vec)
{
        if (!vec)
                return ERR_INV_PARAM;

        free(vec->items[vec->size - 1]);
        vec->size--;

        /* no items left, free the pointer array. */
        if (vec->size == 0) {
                free(vec->items);
                return 0;
        }

        void **tmp = realloc(vec->items, (vec->size + 1) * sizeof(void*));
        if (tmp == NULL)
                return ERR_REALLOC;

        vec->items = tmp;
        return 0;
}

/*
 * Removes a pointer from the array at the given position.
 * The allocated memory block is also freed.
 * Returns 0 on success and an error code on failure.
 */
int v_rm(struct vector *vec, index pos)
{
        if (vec == NULL)
                return ERR_INV_PARAM;

        if (!vint_bounds_check(vec, pos))
                return ERR_OUT_OF_RANGE;

        free(vec->items[pos]);
        /* reduce the size first to avoid shifting in OOB values later */
        vec->size--;

        /* no items left, free the pointer array. */
        if (vec->size == 0) {
                free(vec->items);
                return 0;
        }
        /* this is not buffer overflow because the resizing hasn't been made yet */
        for (index i = pos; i < vec->size; i++) {
                vec->items[i] = vec->items[i + 1];
        }

        void **tmp = realloc(vec->items, (vec->size + 1) * sizeof(void*));
        if (tmp == NULL)
                return ERR_REALLOC;

        vec->items = tmp;
        return 0;
}

/*
 * Removes all pointers from the array, then frees the vector pointer.
 * Frees all allocated memory blocks.
 */
int v_del(struct vector *vec)
{
        if (vec == NULL)
                return ERR_INV_PARAM;

        if (vec->size == 0){
                free(vec);
                return 0;
        }

        /*
         * We free everyone except items[0].
         * The reason behind this is we iterate with size_t to avoid
         * implementation defined behaviour.
         */
        for (index i = (vec->size - 1); i > 0; i--) {
                free(vec->items[i]);
        }

        free(vec->items[0]);
        free(vec->items);
        free(vec);
        return 0;
}
