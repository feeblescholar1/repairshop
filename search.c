/**
 * @file search.c
 * @brief Functions definitions for searching a given database.
 * @note These functions return \b exact \b matches . Wildcards (\c * and \c ?)
 *       are \b not supported.
 */
#include "include/search.h"

/**
 * @brief Searches a database by a client's name.
 * @param db The pointer to the database to search in.
 * @param term The search term.
 * @return A \c sres structure containing the result.
 */
struct sres search_cl(struct database *db, const char *term)
{
        struct sres res = {.map = vct(), .err = 0};

        for (idx i = 0; i < db->cl->size; i++) {
                struct client *cl = db_cl_get(db, i);
                if (!strcmp(cl->name, term)) {
                        idx *db_index = malloc(sizeof(idx));
                        if (!db_index) {
                                res.err = EMALLOC;
                                vct_del(res.map);
                                break;
                        }

                        *db_index = i;

                        if (vct_push(res.map, db_index) == EREALLOC) {
                                res.err = EREALLOC;
                                vct_del(res.map);
                                break;
                        }
                }
        }

        return res;
}

/**
 * @brief Searches a database by car plate number.
 * @param db The pointer to the database to search in.
 * @param term The search term.
 * @return A \c sres structure containing the result.
 * @note In this case \c res.map->items points to an \c idx \b array with 2
 *       values: the client index and the car index.
 */
struct sres search_plate(struct database *db, const char *term)
{
        struct sres res = {.map = vct(), .err = 0};

        for (idx i = 0; i < db->cl->size; i++) {
                struct client *cl = db_cl_get(db, i);

                for (idx j = 0; j < cl->cars->size; j++) {
                        struct car *car = db_car_get(db, i, j);

                        if (!strcmp(car->plate, term)) {
                                idx *db_index = malloc(2 * sizeof(idx));
                                if (!db_index) {
                                        res.err = EMALLOC;
                                        vct_del(res.map);
                                        break;
                                }

                                db_index[0] = i;
                                db_index[1] = j;

                                if (vct_push(res.map, db_index) == EREALLOC) {
                                        res.err = EREALLOC;
                                        vct_del(res.map);
                                        break;
                                }
                        }
                }
        }

        return res;
}

/**
 * @brief Looks for those operations, which have date_exp due in 30 days.
 * @param db The pointer to the database to search in.
 * @return A \c sres structure containing the result.
 * @note In this case \c res.map->items points to an \c idx \b array with 2
 *       values: the client index and the car index.
 */
struct sres search_expiration(struct database *db)
{
        struct sres res = {.map = vct(), .err = 0};

        struct date now = date_now();

        /* Start the database iteration [O(n^3) oof.]*/
        for (idx i = 0; i < db->cl->size; i++) {
                struct client *cl = db_cl_get(db, i);

                for (idx j = 0; j < cl->cars->size; j++) {
                        struct car *car = db_car_get(db, i, j);

                        for (idx k = 0; k < car->operations->size; k++) {
                                struct operation *op = db_op_get(db, i, j, k);
                                if (op->date_exp.y == 0)
                                        continue;

                                double diff = date_diff(&op->date_exp, &now);

                                if (0 < diff && diff < 30.0) {

                                        idx *db_index = malloc(3 * sizeof(idx));
                                        if (!db_index) {
                                                res.err = EMALLOC;
                                                vct_del(res.map);
                                                break;
                                        }

                                        db_index[0] = i;
                                        db_index[1] = j;
                                        db_index[2] = k;

                                        if (vct_push(res.map, db_index) == EREALLOC) {
                                                res.err = EREALLOC;
                                                vct_del(res.map);
                                                break;
                                        }
                                }
                        }
                }
        }

        return res;
}