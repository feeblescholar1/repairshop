/*
 * File: vector.c
 * Description: Generic vector implementation. Contains the function definitions
 *              found in 'vector.h'.
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
        if (vec->size == 0) {
                vec->items = calloc(1, sizeof(void*));
                if (vec->items == NULL) {
                        return ERR_MALLOC;
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
 * Checks if a given position is out of range.
 * For internal use only.
 */
bool vint_oob_check(const struct vector *vec, size_t pos)
{
        return (vec->size < pos);
}

/*
 * Insert a given pointer to the array into the given position.
 * Indexing starts from 0.
 * Unlike v_push_back() this function CANNOT initialize vec–>items.
 * The given pointer must be allocated by the caller.
 * Returns 0 on success and an error code on failure.
 */
int v_insert(struct vector *vec, void *data, size_t pos)
{
        if (vec == NULL || data == NULL)
                return ERR_INV_PARAM;
        if (vint_oob_check(vec, pos))
                return ERR_OUT_OF_RANGE;
        if (pos == vec->size)
                return v_push_back(vec, data);
        void **tmp = realloc(vec->items, (vec->size + 1) * sizeof(void*));
        if (tmp == NULL)
                return ERR_REALLOC;
        vec->items = tmp;
        vec->size++;
        for (size_t i = vec->size; i >= pos; i--) {
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
void *v_get_item_ptr(const struct vector *vec, size_t pos)
{
        if (vint_oob_check(vec, pos))
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
        if (vec == NULL)
                return ERR_INV_PARAM;
        free(vec->items[vec->size - 1]);
        vec->size--;
        if (vec->size == 0) {
                free(vec->items);
                free(vec);
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
int v_rm(struct vector *vec, size_t pos)
{
        if (vec == NULL)
                return ERR_INV_PARAM;
        if (vint_oob_check(vec, pos))
                return ERR_OUT_OF_RANGE;
        free(vec->items[pos]);
        vec->size--; // reduce the size first to avoid shifting in OOB values
        if (vec->size == 0) {
                free(vec->items);
                free(vec);
                return 0;
        }
        // this does not overindex because the realloc hasn't been made yet
        for (size_t i = pos; i < vec->size; i++) {
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
        for (size_t i = (vec->size - 1); i > 0; i--) {
                free(vec->items[i]);
        }
        free(vec->items[0]);
        free(vec->items);
        free(vec);
        return 0;
}
