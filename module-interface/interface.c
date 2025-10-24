/*
 * interface.c - mainloop definition for the ui, which handles calls to subinterfaces
 */

#include "include/interface.h"

void text_mainloop()
{
        printf("\n-------------- carrepairshop --------------\n");
        printf("[0] Kilepes\n");
        printf("[1] Ugyfelek kezelese\n");
        printf("[2] Kereses");
        printf("\n-------------------------------------------\n");
        printf("Opcio: ");
}

int mainloop(struct database *db)
{
        bool mainloop_active = true;
        while (mainloop_active) {
                text_mainloop();
                int opt = interface_io_get_opt();

                switch (opt) {
                        case 0:
                                mainloop_active = false;
                                break;
                        case 1:
                                subinterface_client(db);
                                break;
                        default:
                                printf("Nincs ilyen opcio. \n");
                                break;
                }
        }
        return 0;
}