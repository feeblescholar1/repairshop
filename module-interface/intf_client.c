/**
 * @file intf_client.c
 * @brief The client management menu's UI code.
 */
#include "include/intf_client.h"

/**
 * @brief Prints the client management menu's text to \c stdout .
 */
void intf_cl_txt(const struct database *db)
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
                        struct client *cl = db_cl_get(db, i);
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
int intf_cl(const struct database *db)
{
        bool submenu_active = true;
        while (submenu_active) {
                intf_cl_txt(db);
                int opt = intf_io_opt();
                int resp = 0;

                switch (opt) {
                        case 0:
                                submenu_active = false;
                                break;
                        case 1:
                                resp = intf_cl_add(db);
                                if (resp == EMALLOC)
                                        return EMALLOC;
                                break;
                        case 2:
                                printf("Ugyfelazonosito: ");
                                opt = intf_io_opt();

                                resp = intf_cl_mod(db, opt);
                                if (resp == EOOB)
                                        puts("\nAz ugyfel nem talalhato.");
                                break;

                        case 3:
                                printf("Ugyfelazonosito: ");
                                opt = intf_io_opt();

                                resp = interface_client_rm(db, opt);
                                if (resp == EOOB)
                                        puts("\nAz ugyfel nem talalhato.");
                                break;
                        case 4:
                                printf("Ugyfelzonosito: ");
                                opt = intf_io_opt();

                                resp = intf_car(db, opt);
                                if (resp == EOOB)
                                        puts("\nAz ugyfel nem talalhato.");
                                else if (resp == EMALLOC)
                                        return EMALLOC;
                                break;
                        default:
                                puts("\nErvenytelen opcio.");
                                break;
                }
        }
        return 0;
}

/**
 * @brief The frontend for client addition.
 * @param db The destination database.
 * @return \c db_cl_add() with the user given parameters.
 */
int intf_cl_add(const struct database *db)
{
        char name[NAME_SIZE + 1] = "\0";
        char email[EMAIL_SIZE + 1] = "\0";
        char phone[PHNUM_SIZE + 1] = "\0";

        printf("Ugyfel neve (max. %d karakter): ", NAME_SIZE);
        intf_io_fgets(name, NAME_SIZE + 1);

        printf("Ugyfel email cime (max. %d karater): ", EMAIL_SIZE);
        intf_io_fgets(email, EMAIL_SIZE + 1);

        printf("Ugyfel telefonszama (max. %d karakter): ", PHNUM_SIZE);
        intf_io_fgets(phone, PHNUM_SIZE + 1);

        return db_cl_add(db, name, email, phone);
}

/**
 * @brief The frontend for client modification.
 * @param db The destination database.
 * @param cl The client's index.
 * @return \c db_cl_mod() with the user given parameters.
 */
int intf_cl_mod(const struct database *db, idx cl)
{
        char name[NAME_SIZE + 1] = "\0";
        char email[EMAIL_SIZE + 1] = "\0";
        char phone[PHNUM_SIZE + 1] = "\0";

        printf("Ugyfel uj neve (max. %d karakter): ", NAME_SIZE);
        intf_io_fgets(name, NAME_SIZE + 1);

        printf("Ugyfel uj email cime (max. %d karater): ", EMAIL_SIZE);
        intf_io_fgets(email, EMAIL_SIZE + 1);

        printf("Ugyfel uj telefonszama (max. %d karakter): ", PHNUM_SIZE);
        intf_io_fgets(phone, PHNUM_SIZE + 1);


        return db_cl_mod(db, cl, name, email, phone);
}

/**
 * @brief The frontend for client removal.
 * @param db The destination database.
 * @param cl The client's index.
 * @return \c db_cl_rm() with the user given parameters.
 */
int interface_client_rm(const struct database *db, idx cl)
{
        return obj_cl_rm(db->cl, cl);
}
