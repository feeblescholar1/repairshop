/**
 * @file main.c
 * @brief Program entry.
 * @note This file is \b the \b most \b important, since it has the the main function. In reality, it contains nearly
 *       \c 0 information about how the program works. Visit the individual module documentation for more info.
 */

#include <stdio.h>

#include "module-database/include/database.h"
#include "module-interface/include/intf.h"
#include "module-filehandler/include/fh.h"

/**
 * @brief Cleans up the allocated memory and exits the program with the given error code.
 * @param db Pointer the main database.
 * @param err_code Error code sent to the operating system.
 * @note File closing is handled by the filehandler module.
 */
void err_cleanup(database *db, int err_code)
{
        fprintf(stderr, "Takaritas es kilepes...\n");
        db_del(db);
        exit(err_code);
}

/**
 * @brief Error handled function caller. Used only in \c main().
 * @param function Pointer to the function to be called.
 * @param db Pointer to the database that will be passed to function.
 * @return \c 0 if there were no errors, non-zero if there was an error.
 */
int errh_call(int (*function)(database *), database *db)
{
        int error_code = function(db);

        switch (error_code) {
                case EMALLOC:
                        fprintf(stderr, "\nMemoriakezelesi hiba.\n");

                        /* If this is not a read error, try to save to a file.*/
                        if (function != fh_import)
                                fh_export(db);

                        err_cleanup(db, error_code);
                        break;
                case EINV:
                        /*
                         * Only fh_import() will return EINV. (as of now)
                         * Other functions ignore EINV.
                         */
                        fprintf(stderr, "\nA fajl formatuma nem megfelelo.\n");
                        err_cleanup(db, error_code);
                        break;
                default:
                        break;
        }

        return error_code;
}

/**
 * @brief The (fake) program entry. We all know that the program starts at the label \c _start .
 */
int main(void)
{
        setbuf(stdout, NULL);
        database *db = db_init("(nincs nev)", "(nincs leiras)\n");
        if (!db) {
                fprintf(stderr, "\nNem lehet letrehozni az adatbazist.\n");
                return EMALLOC;
        }

        errh_call(fh_import, db);
        errh_call(intf_main, db);
        errh_call(fh_export, db);

        db_del(db);
        return 0;
}
