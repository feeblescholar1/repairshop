#ifndef REPAIRSHOP_VECTOR_H
#define REPAIRSHOP_VECTOR_H
#include <stdlib.h>
#include <stdio.h>


#define ERR_VECTOR_CREATION_FAILURE NULL
#define ERR_INV_PARAM (-1)
#define ERR_OUT_OF_RANGE 1
#define ERR_MALLOC 2
#define ERR_REALLOC 3

struct vector {
        void **items;
        size_t size;
};

#endif //REPAIRSHOP_VECTOR_H