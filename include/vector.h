/*
 * File: vector.h
 * Description: Vector definition and function declarations for the generic
 *              vector found in 'vector.c'. Include 'external/debugmalloc.h' for
 *              dynamic memory analysis.
 * Note: debugmalloc.h is an external header file not maintained by this project.
 *       Source: https://infoc.eet.bme.hu/debugmalloc/debugmalloc.h
 */
#ifndef REPAIRSHOP_VECTOR_H
#define REPAIRSHOP_VECTOR_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
        //#include "external/debugmalloc.h"
#include "errorcodes.h"
struct vector {
        void **items;
        size_t size;
};
struct vector *v_init(void);
int v_push_back(struct vector *vec, void *data);
int v_insert(struct vector *vec, void *data, size_t pos);
void *v_get_item_ptr(const struct vector *vec, size_t pos);
int v_pop_back(struct vector *vec);
int v_rm(struct vector *vec, size_t pos);
int v_del(struct vector *vec);
#endif //REPAIRSHOP_VECTOR_H
