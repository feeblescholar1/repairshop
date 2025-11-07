#ifndef REPAIRSHOP_FH_EXPORT_H
#define REPAIRSHOP_FH_EXPORT_H
#include "../../module-database/include/database.h"
#include "../../include/errorcodes.h"
#include "module-interface/include/intf_io.h"

/*
 * Due to the line format (ID> DATA | DATA | ...) we have to make some space for
 * this in the read buffer:
 * 1 ID char + 4 US (at most) + 1 null
 */

#define FORMAT_RQ (1 + 4 + 1)
#define LONGEST_VALID_LINE (NAME_SIZE + EMAIL_SIZE + PHNUM_SIZE + FORMAT_RQ)
#define US 0x1F /* the ASCII unit separator character */

int fh_import(struct database *dst);
int fh_export(const struct database *db);

#endif //REPAIRSHOP_FH_EXPORT_H