/**
 * @file vector.c
 * @brief Generic vector implementation.
 * @details A vector implementation to manage an array of pointers. The managed
 *          data can point to anything, that is why \c void** is used. The type
 *          can also be different, however all data must be preallocated and
 *          properly cast by the caller. Deallocation (to some extent) is
 *          handled by the implementation, more specifically if the data inside
 *          is not itself is dynamically allocated.\n
 *          (e.g.: If there are nested vectors, the implementation doesn't know
 *          that, so it cannot free it. Freeing that is the caller's
 *          reponsibility.)
 */

#include "include/vector.h"

/**
 * @brief Allocates and initializes a vector on the heap.
 * @warning Does not initialize \c vector->items.
 * @return A struct vector* on success and \c EMEMNULL on failure.
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

/**
 * @brief Appends a memory block pointer to a vector.
 * @param v A vector pointer to add \c data to.
 * @param data Pointer to a preallocated memory block.
 * @retval 0 On success
 * @retval EINV If \c v or \c data is \c NULL
 * @retval EMALLOC If \c malloc() returns \c NULL.
 * @retval EREALLOC If the vector expansion fails.
 * @note Use to function to initialize \c v->items.
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

/**
 * @brief Checks if a given index exists in a vector.
 * @param v Pointer to the source vector.
 * @param pos The position to be checked.
 * @return \c true if it's inbounds, \c false if not.
 */
inline bool inbounds(const struct vector *v, idx pos)
{
        return (pos < v->size);
}

/**
 * @brief Inserts a memory block to a given position in a vector.
 * @param v Pointer to the vector to insert the \c data to.
 * @param data Pointer to a preallocated memory block.
 * @param pos The position to insert \c data to.
 * @warning This function CANNOT initialize \c v->items.\n
 *          See \c vct_push() for that.
 * @retval 0 On success.
 * @retval EINV If \c v or \c data is \c NULL.
 * @retval EOOB If the given \c pos is out of bounds.
 * @retval EREALLOC If the vector expansion fails.
 * @note Indexing starts from \c 0.
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

/**
 * @brief Returns a memory block pointer from a vector.
 * @param v Pointer to the vector to get the subpointer from.
 * @param pos Subpointer position.
 * @retval void* On success.
 * @retval NULL On failure.
 * @note The given pointer must not be freed. Use \c vct_rm() for that.
 */
void *vct_subptr(const struct vector *v, idx pos)
{
        if (!inbounds(v, pos))
                return NULL;

        return v->items[pos];
}

/**
 * @brief Frees and removes the last memory block pointer from a vector.
 * @param v Pointer to the source vector.
 * @retval 0 On success.
 * @retval EINV If \c v is \c NULL
 * @return EREALLOC If the vector shrinking fails.
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

/**
 * @brief Deallocates and removes a memory block pointer from a vector at the given position.
 * @param v Pointer to the source vector.
 * @param pos Position of the memory block pointer in \c v->items.
 * @retval 0 On success.
 * @retval EINV If \c v is \c NULL.
 * @retval EOOB If \c pos is out of range.
 * @retval REALLOC If the vector shrinking fails.
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

/**
 * @brief Frees all memory blocks and the vector.
 * @param v Pointer to the vector to be deleted.
 * @retval 0 On success.
 * @retval EINV If \c v is \c NULL.
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
