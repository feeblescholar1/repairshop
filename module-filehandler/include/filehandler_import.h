#ifndef REPAIRSHOP_FILEHANDLER_IMPORT_H
#define REPAIRSHOP_FILEHANDLER_IMPORT_H

/*
 * Due to the line format (ID> DATA | DATA | ...) we have to make some space for
 * this in the read buffer:
 * 1 ID char + '>' char + 1 whitespace char + 2 pipes (at most) + 1 null
 */
#define FORMAT_REQUIREMENT (1 + 1 + 1 + 2 + 1)
#define LONGEST_VALID_LINE (NAME_MAX_LEN + EMAIL_MAX_LEN + PHONENUM_MAX_LEN + FORMAT_REQUIREMENT)

#include "../../module-database/include/database.h"
#include "../../include/errorcodes.h"

int fh_import(struct database *dst);

#endif //REPAIRSHOP_FILEHANDLER_IMPORT_H