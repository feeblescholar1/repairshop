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

#define idx size_t /* index */

struct vector {
        void **items;
        size_t size;
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
