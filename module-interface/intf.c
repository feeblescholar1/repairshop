/*
 * interface.c - mainloop UI
 */

#include "include/intf.h"

/* Walls of text */
void intf_main_txt()
{
        puts("\n-------------- carrepairshop --------------");
        puts("[0] Kilepes");
        puts("[1] Ugyfelek kezelese");
        puts("[2] Kereses");
        puts("-------------------------------------------");
        printf("Opcio: ");
}

/* Mainloop driver code */
int intf_main(struct database *db)
{
        bool mainloop_active = true;
        while (mainloop_active) {
                intf_main_txt();
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