#ifndef REPAIRSHOP_SEARCH_H
#define REPAIRSHOP_SEARCH_H

#include "module-database/include/database.h"
#include "module-database/include/vector.h"

struct sres {
        struct vector *map;      /* stores the database indexes */
        int err;                /* error code */
};

struct sres search_cl(struct database *db, const char *term);


#endif //REPAIRSHOP_SEARCH_H