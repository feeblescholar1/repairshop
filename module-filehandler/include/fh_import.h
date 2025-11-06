#ifndef REPAIRSHOP_FH_IMPORT_H
#define REPAIRSHOP_FH_IMPORT_H

/*
 * Due to the line format (ID> DATA | DATA | ...) we have to make some space for
 * this in the read buffer:
 * 1 ID char + '>' char + 1 whitespace char + 2 pipes (at most) + 1 null
 */
#define FORMAT_RQ (1 + 1 + 1 + 2 + 1)
#define LONGEST_VALID_LINE (NAME_SIZE + EMAIL_SIZE + PHNUM_SIZE + FORMAT_RQ)

#include "../../module-database/include/database.h"
#include "../../include/errorcodes.h"

int fh_import(struct database *dst);

#endif //REPAIRSHOP_FH_IMPORT_H