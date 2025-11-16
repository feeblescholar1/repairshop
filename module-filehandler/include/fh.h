/**
 * @file fh.h
 * @brief Module header file. Include this file to be able store databases in a
 *        file.
 */

#ifndef REPAIRSHOP_FH_EXPORT_H
#define REPAIRSHOP_FH_EXPORT_H

#include "../../module-database/include/database.h"
#include "../../include/errorcodes.h"
#include "../../module-interface/include/intf_io.h"

/** Format requirement: 1 ID char + 1 \c > char + 3 \c | chars + 1 \c \0 at end */
#define FORMAT_RQ (1 + 1 + 3 + 1)
/** A constant for the \c read_buffer maximum. */
#define LONGEST_VALID_LINE (NAME_SIZE + EMAIL_SIZE + PHNUM_SIZE + FORMAT_RQ)

int fh_import(struct database *dst);
int fh_export(struct database *db);

#endif //REPAIRSHOP_FH_EXPORT_H
