#include "vector.h"

struct vector *vec_create(void)
{
        struct vector *new = malloc(sizeof(struct vector));
        if (new == NULL)
                return ERR_VECTOR_CREATION_FAILURE;
        new->items = NULL;
        new->size = 0;
        return new;
}

int vec_push_back(struct vector *vec, void *data)
{
        if (vec == NULL || data == NULL)
                return ERR_INV_PARAM;
        if (vec->size == 0) {
                vec->items = malloc(sizeof(data));
                if (vec->items == ERR_VECTOR_CREATION_FAILURE) {
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

int vec_insert(struct vector *vec, void *data, const int pos)
{
        if (vec == NULL || data == NULL)
                return ERR_INV_PARAM;
        if (pos < 0 || pos > vec->size)
                return ERR_OUT_OF_RANGE;
        if (pos == vec->size)
                return vec_push_back(vec, data);
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

int vec_pop_back(struct vector *vec)
{
        if (vec == NULL)
                return ERR_INV_PARAM;
        free(vec->items[vec->size - 1]);
        vec->size--;
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

int vec_remove(struct vector *vec, const int pos)
{
        if (vec == NULL)
                return ERR_INV_PARAM;
        if (pos < 0 || pos > vec->size)
                return ERR_OUT_OF_RANGE;
        free(vec->items[pos]);
        for (size_t i = pos; i < vec->size; i++) {
                vec->items[i] = vec->items[i + 1];
        }
        vec->size--;
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

int vec_destroy(struct vector *vec)
{
        if (vec == NULL)
                return ERR_INV_PARAM;
        for (size_t i = (vec->size - 1); i > 0; i--) {
                free(vec->items[i]);
        }
        vec->size = 0;
        free(vec);
        return 0;
}

void vec_debug_print(struct vector *vec)
{
        printf("\n[vec addr.][pos]: [ptr. addr.]->[value]\n\n");
        for (int i = 0; i < vec->size; i++) {
                printf("[%p][%d]: [%p]->[%d]\n", vec, i, vec->items[i], *(int*)(vec->items[i]));
        }
        printf("\n[size]: %llu\n", vec->size);
}