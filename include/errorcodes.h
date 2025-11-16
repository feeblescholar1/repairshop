/**
 * @file errorcodes.h
 * @brief Project error codes. Chosen at random.
 */

#ifndef REPAIRSHOP_ERRORCODES_H
#define REPAIRSHOP_ERRORCODES_H

#define EMEMNULL NULL           /**< \c malloc() fails */
#define EINV 1                  /**< Invalid function parameter */
#define EOOB 2                  /**< Out of bounds error */
#define EMALLOC 3               /**< \c malloc() fails */
#define EREALLOC EMALLOC        /**< \c realloc() fails */
#define EFPERM 4                /**< File permission error */

#endif //REPAIRSHOP_ERRORCODES_H
