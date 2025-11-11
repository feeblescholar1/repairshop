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
 * @brief Prints out an error message to \c stderr .
 * @param err_code An errorcode defined in \c errorcodes.h
 * @return The \c err_code parameter.
 */
int err_msg(int err_code)
{
        switch (err_code) {
                case EMALLOC:
                case EREALLOC:
                        fprintf(stderr, "Memoriakezelesi hiba. Kilepes...\n");
                        break;
                case EFPERM:
                        fprintf(stderr, "A faljt nem lehet megnyitni irasra.");
                        break;
                default:
                        break;
        }

        return err_code;
}

/**
 * @brief The (fake) program entry. We all know that the program starts at
 *        the label \c _start . Documented for completeness’ sake.
 * @return Returns 0 on success or whatever error code the modules throw.
 */
int main(void)
{
        struct database *db = db_init("", "");

        int err_code = 0;

        err_code = fh_import(db);
        if (err_code == EMALLOC)
                goto cleanup;

        err_code = intf_main(db);
        if (err_code)
                goto cleanup;

        err_code = fh_export(db);
        if (err_code)
                goto cleanup;

        err_code = db_del(db);
        return 0;

        cleanup:
                db_del(db);
                return err_msg(err_code);
}
