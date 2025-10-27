/*
 * vector.h - generic vector struct definition, can be debugged with debugmalloc.h
 * Note: debugmalloc.h is an external header file not maintained by this project.
 *       Source: https://infoc.eet.bme.hu/debugmalloc/debugmalloc.h
 */
#ifndef REPAIRSHOP_VECTOR_H
#define REPAIRSHOP_VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../../include/errorcodes.h"
#include "../../include/external/debugmalloc.h"

#define index size_t

struct vector {
        void **items;
        size_t size;
};

struct vector *v_init(void);
bool vint_bounds_check(const struct vector *vec, index pos);
int v_push_back(struct vector *vec, void *data);
int v_insert(struct vector *vec, void *data, index pos);
void *v_get_item_ptr(const struct vector *vec, index pos);
int v_pop_back(struct vector *vec);
int v_rm(struct vector *vec, index pos);
int v_del(struct vector *vec);
#endif //REPAIRSHOP_VECTOR_H
