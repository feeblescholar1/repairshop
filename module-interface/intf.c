/**
 * @file intf.c
 * @brief Mainloop UI code.
 */

#include "include/intf.h"

/**
 * @brief Prints the main menu's text to \c stdout .
 */
void intf_main_txt(struct database *db)
{
        puts("--------------------- repairshop ---------------------");
        printf("Adatbazis: %s | %s", db->name, db->desc);
        puts("------------------------------------------------------");
        puts("[0] Kilepes");
        puts("[1] Ugyfelek kezelese");
        puts("[2] Kereses");
        puts("------------------------------------------------------");
        printf("Opcio: ");
}

/**
 * @brief The main menu's driver code.
 * @param db The database pointer which the user will address.
 * @retval 0 If the user requests the program to exit.
 * @retval EMALLOC If a memory allocation failure is occured.
 */
int intf_main(struct database *db)
{
        bool mainloop_active = true;
        while (mainloop_active) {
                intf_main_txt(db);
                int opt = intf_io_opt();
                int resp = 0;

                switch (opt) {
                        case 0:
                                mainloop_active = false;
                                break;
                        case 1:
                                resp = intf_cl(db);
                                if (resp == EMALLOC)
                                        return EMALLOC;
                                break;
                        case 2:
                                resp = intf_search(db);
                                if (resp == EMALLOC)
                                        return EMALLOC;
                                break;
                        default:
                                puts("Nincs ilyen opcio.");
                                break;
                }
        }

        return 0;
}
