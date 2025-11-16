/**
 * @file vector.h
 * @brief Vector struct definition and function prototypes.
 * @details Defines the vector and its funtion prototypes used in \c vector.c.
 *          Include \c debugmalloc.h for memory analysis.
 * @note \c debugmalloc.h is an external library not maintained by this project:
 *       \htmlonly <a href=https://infoc.eet.bme.hu/debugmalloc/>Documentation (Hungarian)</a>\endhtmlonly |
 *       \htmlonly<a href=https://infoc.eet.bme.hu/debugmalloc/debugmalloc.h>File mirror</a>\endhtmlonly
 */

#ifndef REPAIRSHOP_VECTOR_H
#define REPAIRSHOP_VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../../include/errorcodes.h"
#include "../../include/external/debugmalloc.h"

#define idx size_t /**< Macro for size_t */

/**
 * @struct vector vector.h
 * @brief A vector for storing pointers.
 */
struct vector {
        void **items; /**< Generic dynamically allocated pointer array. */
        size_t size; /**< The size of the vector */
};

struct vector *vct(void);

bool inbounds(const struct vector *v, idx pos);

int vct_push(struct vector *v, void *data);
int vct_insert(struct vector *v, void *data, idx pos);

void *vct_subptr(const struct vector *v, idx pos);

int vct_pop(struct vector *v);
int vct_rm(struct vector *v, idx pos);

int vct_del(struct vector *v);
#endif //REPAIRSHOP_VECTOR_H
