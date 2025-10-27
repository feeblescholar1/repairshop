#include "include/interface_car_op.h"

void interface_car_op_text(const struct database *db, const index client_i)
{
        struct client *client = db_get_client(db, client_i);
        printf("\n----- Autok kezelese ------\n");
        printf("[0] Vissza\n");
        printf("[1] Auto hozzadasa\n");
        printf("[2] Auto eltavolitasa\n");
        printf("[3] Javitas hozzadasa\n");
        printf("[4] Javitas eltavolitasa\n");
        printf("-----------------------------\n");
        printf("[%lu][%s][%s][%s]\n", client_i, client->name, client->email,
                client->phone);
        printf("-----------------------------\n\n");
        if (client->cars->size == 0) {
                printf("Ennek az ugyfelnek nincsenek hozzadott autoi.\n");
        }
        else {
                for (index car_i = 0; car_i < client->cars->size; car_i++) {
                        struct car *car = db_get_car(db, client_i, car_i);
                        printf("[%lu][%s][%s]\n", car_i, car->name, car->plate);
                        for (index op_i = 0; op_i < car->operations->size; op_i++) {
                                struct operation *op = db_get_op(db, client_i,
                                        car_i, op_i);
                                printf("\t\t[Javitas][%lu][%s][%f][%s]\n", op_i,
                                        op->desc, op->price, asctime(op->date));
                        }
                }
        }
        printf("\n-----------------------------\n");
        printf("Opcio: ");

}

int interface_car_op(const struct database *db, const index client_i)
{
        if (db_get_client(db, client_i) == NULL)
                return ERR_OUT_OF_RANGE;

        bool submenu_active = true;
        while (submenu_active) {
                interface_car_op_text(db, client_i);
                int opt = interface_io_get_opt();
                int opt2 = 0;
                int response = 0;

                switch (opt) {
                        case 0:
                                submenu_active = false;
                                break;
                        case 1:
                                response = interface_car_add(db, client_i);
                                if (response == ERR_CALLOC)
                                        return ERR_CALLOC;
                                break;
                        case 2:
                                printf("Auto sorszama: ");
                                opt = interface_io_get_opt();

                                response = interface_car_rm(db, client_i, opt);
                                if (response == ERR_OUT_OF_RANGE)
                                        printf("\nAz auto nem talalhato\n");
                                break;
                        case 3:
                                printf("Auto sorszama: ");
                                opt = interface_io_get_opt();

                                response = interface_op_add(db, client_i, opt);
                                if (response == ERR_OUT_OF_RANGE)
                                        printf("\nAz auto nem talalhato.\n");
                                else if (response == ERR_CALLOC)
                                        return ERR_CALLOC;
                                break;
                        case 4:
                                printf("Auto sorszama: ");
                                opt = interface_io_get_opt();

                                printf("Javitas sorszama: ");
                                opt2 = interface_io_get_opt();

                                response = interface_op_rm(db, client_i, opt,
                                        opt2);
                                if (response == ERR_OUT_OF_RANGE)
                                        printf("Az auto/javitas nem talalhato.");
                                break;
                        default:
                                printf("\nErvenytelen opcio.\n");
                                break;
                }
        }
        return 0;
}

int interface_car_add(const struct database *db, const index client_i)
{
        char name_buffer[NAME_MAX_LEN + 1] = "\0";
        char plate_buffer[PLATE_MAX_LEN + 1] = "\0";

        printf("Auto tipusa (max. %d karakter): ", NAME_MAX_LEN);
        interface_io_fgets_clean(name_buffer, NAME_MAX_LEN + 1);

        printf("Auto rendszama (max. %d karakter): ", PLATE_MAX_LEN);
        interface_io_fgets_clean(plate_buffer, PLATE_MAX_LEN + 1);

        return db_add_car(db, client_i, name_buffer, plate_buffer);
}

int interface_op_add(const struct database *db, const index client_i,
        const index car_i)
{
        if (db_get_car(db, client_i, car_i) == NULL)
                return ERR_OUT_OF_RANGE;

        char desc_buffer[DESCRIPTION_MAX_LEN + 1] = "\0";
        char price_buffer[DEFAULT_BUFFER_SIZE + 1] = "\0";
        double price = 0;

        printf("Javitas leirasa (max. %d karakter): ", DESCRIPTION_MAX_LEN);
        interface_io_fgets_clean(desc_buffer, DESCRIPTION_MAX_LEN + 1);

        printf("Javitas koltsege: ");
        interface_io_fgets_clean(price_buffer, DEFAULT_BUFFER_SIZE + 1);
        sscanf(price_buffer, "%lf", &price);

        return db_add_op(db, client_i, car_i, desc_buffer, price, NULL);
}

int interface_car_rm(const struct database *db, const index client_i,
        const index car_i)
{
        return db_rm_car(db, client_i, car_i);
}

int interface_op_rm(const struct database *db, const index client_i,
        const index car_i, const index op_i)
{
        return db_rm_op(db, client_i, car_i, op_i);
}