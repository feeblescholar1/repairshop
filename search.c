#include "include/search.h"

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