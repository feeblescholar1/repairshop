/**
 * @file errorcodes.h
 * @brief Project error codes. Chosen at random.
 */

#ifndef REPAIRSHOP_ERRORCODES_H
#define REPAIRSHOP_ERRORCODES_H

#define EMEMNULL NULL           /**< \c malloc() fails */
#define EINV (-1)               /**< Invalid function parameter */
#define EOOB 1                  /**< Out of bounds error */
#define EMALLOC 2               /**< \c malloc() fails */
#define EREALLOC EMALLOC        /**< \c realloc() fails */
#define EFPERM 4                /**< File permission error */

#endif //REPAIRSHOP_ERRORCODES_H
