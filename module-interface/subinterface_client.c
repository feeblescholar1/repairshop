/*
 * subinterface_client.c - defines the client submenu and handles client database calls
 */
#include "include/subinterface_client.h"

void subinterface_client_text(struct database *db)
{
        printf("\n------------- Ugyfelek kezelese -------------\n");
        printf("[0] Vissza\n");
        printf("[1] Ugyfel hozzadasa\n");
        printf("[2] Ugyfel eltavolitasa\n");
        printf("[3] Ugyfeltortenet lekerdezese\n");
        printf("------------------------------\n\n");

        for (index i = 0; i < db->clients->size; i++) {
                struct client *client = db_get_client(db, i);
                printf("[%lu][%s][email: %s][tel.: %s][auto(k): %lu]\n", i,
                        client->name, client->email, client->phone,
                        client->cars->size);
        }

        printf("\n---------------------------------------------\n");
        printf("\nOpcio: ");
};

int subinterface_client(struct database *db)
{
        bool submenu_active = true;
        while (submenu_active) {
                subinterface_client_text(db);
                int opt = interface_io_get_opt();

                if (opt == 0) {
                        submenu_active = false;
                }
                else if (opt == 1) {
                        if (subinterface_client_add(db) == ERR_CALLOC)
                                return ERR_CALLOC;
                }
                else if (opt == 2) {
                        if (subinterface_client_rm(db) == ERR_OUT_OF_RANGE)
                                printf("Az ugyfel nem talalhato.\n");
                }
        }
        return 0;
}
int subinterface_client_add(struct database *db)
{
        char name_buf[NAME_MAX_LEN + 1];
        char email_buf[EMAIL_MAX_LEN + 1];
        char phone_buf[PHONENUM_MAX_LEN + 1];

        printf("Ugyfel neve (max. %d karakter): ", NAME_MAX_LEN);
        interface_io_fgets_clean(name_buf, NAME_MAX_LEN + 1);

        printf("Ugyfel email cime (max. %d karater): ", EMAIL_MAX_LEN);
        interface_io_fgets_clean(email_buf, EMAIL_MAX_LEN + 1);

        printf("Ugyfel telefonszama (max. %d karakter): ", PHONENUM_MAX_LEN);
        interface_io_fgets_clean(phone_buf, PHONENUM_MAX_LEN + 1);

        return db_add_client(db, name_buf, email_buf, phone_buf);
}

int subinterface_client_rm(struct database *db)
{
        printf("Azonosito (elso szam a listaban): ");
        int opt = interface_io_get_opt();

        return client_remove(db->clients, opt);
}
