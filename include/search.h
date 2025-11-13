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
struct sres {
        struct vector *map;      /**< Stores the database indexes */
        int err;                /**<  Error code */
};

struct sres search_cl(struct database *db, const char *term);
struct sres search_plate(struct database *db, const char *term);
struct sres search_expiration(struct database *db);

#endif //REPAIRSHOP_SEARCH_H
