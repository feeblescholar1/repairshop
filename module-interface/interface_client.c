/*
 * interface_client.c - defines the client submenu and handles client database calls
 */
#include "include/interface_client.h"

void interface_client_text(const struct database *db)
{
        printf("\n------------------ Ugyfelek kezelese ------------------\n");
        printf("[0] Vissza\n");
        printf("[1] Ugyfel hozzadasa\n");
        printf("[2] Ugyfel eltavolitasa\n");
        printf("[3] Ugyfel autoinak es szerviztortenetenek lekerdezese\n");
        printf("-------------------------------------------------------\n\n");

        if (db->clients->size == 0) {
                printf("Nincsenek hozzaadott ugyfelek.\n");
        }
        else {
                for (index i = 0; i < db->clients->size; i++) {
                        struct client *client = db_get_client(db, i);
                        printf("[%lu][%s][%s][%s][auto(k): %lu]\n", i,
                                client->name, client->email, client->phone,
                                client->cars->size);
                }
        }

        printf("\n-------------------------------------------------------\n");
        printf("\nOpcio: ");
};

int interface_client(const struct database *db)
{
        bool submenu_active = true;
        while (submenu_active) {
                interface_client_text(db);
                int opt = interface_io_get_opt();
                int response = 0;

                switch (opt) {
                        case 0:
                                submenu_active = false;
                                break;
                        case 1:
                                response = interface_client_add(db);
                                if (response == ERR_CALLOC)
                                        return ERR_CALLOC;
                                break;
                        case 2:
                                printf("Ugyfelazonosito: ");
                                opt = interface_io_get_opt();

                                response = interface_client_rm(db, opt);
                                if (response == ERR_OUT_OF_RANGE)
                                        printf("\nAz ugyfel nem talalhato.\n");
                                break;
                        case 3:
                                printf("Ugyfelzonosito: ");
                                opt = interface_io_get_opt();

                                response = interface_car_op(db, opt);
                                if (response == ERR_OUT_OF_RANGE)
                                        printf("\nAz ugyfel nem talalhato.\n");
                                else if (response == ERR_CALLOC)
                                        return ERR_CALLOC;
                                break;
                        default:
                                printf("\nErvenytelen opcio.\n");
                                break;
                }
        }
        return 0;
}
int interface_client_add(const struct database *db)
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

int interface_client_rm(const struct database *db, const index client_i)
{
        return client_remove(db->clients, client_i);
}
