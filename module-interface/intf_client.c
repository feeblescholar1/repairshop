/**
 * @file intf_client.c
 * @brief The client management menu's UI code.
 */
#include "include/intf_client.h"

/**
 * @brief Prints the client management menu's text to \c stdout .
 */
void intf_cl_txt(const database *db)
{
        puts("------------------ Ugyfelek kezelese -----------------");
        puts("[0] Vissza");
        puts("[1] Ugyfel hozzadasa");
        puts("[2] Ugyfel adatainak modositasa");
        puts("[3] Ugyfel eltavolitasa");
        puts("[4] Ugyfel autoinak es szerviztortenetenek lekerdezese");
        puts("------------------------------------------------------");

        if (db->cl->size == 0) {
                puts("Nincsenek hozzaadott ugyfelek.\n");
        }
        else {
                for (idx i = 0; i < db->cl->size; i++) {
                        client *cl = db_cl_get(db, i);
                        printf("[%zu][%s][%s][%s][auto(k): %zu]\n", i,
                                cl->name, cl->email, cl->phone, cl->cars->size);
                }
        }

        puts("------------------------------------------------------");
        printf("\nOpcio: ");
}

/**
 * @brief The client management menu's driver code.
 * @param db The database pointer which the user will address.
 * @retval 0 If the user requests to go back.
 * @retval EMALLOC If a memory allocation failure is occured.
 */
int intf_cl(const database *db)
{
        bool submenu_active = true;
        while (submenu_active) {
                intf_cl_txt(db);
                int s = intf_io_opt();
                int retval = 0;

                switch (s) {
                        case 0:
                                submenu_active = false;
                                break;
                        case 1:
                                retval = intf_cl_add_mod(db, false, 0);
                                break;
                        case 2:
                                printf("Ugyfelazonosito: ");
                                s = intf_io_opt();

                                retval = intf_cl_add_mod(db, true, s);
                                break;

                        case 3:
                                printf("Ugyfelazonosito: ");
                                s = intf_io_opt();

                                retval = db_cl_rm(db, s);
                                break;
                        case 4:
                                printf("Ugyfelzonosito: ");
                                s = intf_io_opt();

                                retval = intf_car(db, s);
                                break;
                        default:
                                puts("\nErvenytelen opcio.");
                                break;
                }

                if (retval == EMALLOC)
                        return EMALLOC;

                if (retval == EOOB)
                        puts("\nAz ugyfel nem talalhato.");
        }
        return 0;
}

/**
 * @brief The frontend for client addition/modification.
 * @param db The destination database.
 * @param mod Set to \c true if the user requests modification.
 * @param cl The client's index if \c mod is \c true .
 * @return \c db_cl_add() with the user given parameters.
 */
int intf_cl_add_mod(const database *db, bool mod, idx cl)
{
        if (mod && !db_cl_get(db, cl))
                return EOOB;

        char name[NAME_SIZE + 1] = "\0";
        char email[EMAIL_SIZE + 1] = "\0";
        char phone[PHNUM_SIZE + 1] = "\0";

        printf("Ugyfel neve (max. %d karakter): ", NAME_SIZE);
        intf_io_fgets(name, NAME_SIZE + 1);

        printf("Ugyfel email cime (max. %d karater): ", EMAIL_SIZE);
        intf_io_fgets(email, EMAIL_SIZE + 1);

        printf("Ugyfel telefonszama (max. %d karakter): ", PHNUM_SIZE);
        intf_io_fgets(phone, PHNUM_SIZE + 1);

        if (mod)
                return db_cl_mod(db, cl, name, email, phone);

        return db_cl_add(db, name, email, phone);
}