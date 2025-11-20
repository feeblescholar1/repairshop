/**
 * @file search.h
 * @brief Search result structure definition and search function prototypes.
 */

#ifndef REPAIRSHOP_SEARCH_H
#define REPAIRSHOP_SEARCH_H

#include "../module-database/include/database.h"
#include "../module-database/include/vector.h"

/**
 * @struct sres search.h
 * @brief A structure for containing search results.
 */
typedef struct sres {
        vector *map;            /**< Stores the database indexes */
        int err;                /**<  Error code */
} sres;

sres search_cl(database *db, const char *term);
sres search_plate(database *db, const char *term);
sres search_expiration(database *db);

#endif //REPAIRSHOP_SEARCH_H
