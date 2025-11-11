/**
 * @file main.c
 * @brief Program entry.
 * @note This file is \b the \b most \b important, since it's the main function.
 *       In reality, it contains nearly \c 0 information about how the program
 *       works. Visit the individual module documentation for more info.
 */

#include <stdio.h>

#include "module-database/include/database.h"
#include "module-interface/include/intf.h"
#include "module-filehandler/include/fh.h"

/**
 * @brief The (fake) program entry. We all know that the program starts at
 *        the label \c _start . Documented for completeness’ sake.
 * @return Returns 0 on success and whatever error code the modules throw.
 */
int main(void)
{
        struct database *db = db_init("", "");
        fh_import(db);  /* import the database from export.txt */
        intf_main(db);  /* call the mainloop */
        fh_export(db);  /* export the database into export.txt */
        db_del(db);     /* clean up the allocated memory */
        return 0;
}
