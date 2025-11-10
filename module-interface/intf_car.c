#include "include/intf_car.h"

void intf_car_txt(const struct database *db, idx cl)
{
        struct client *cl_ = db_cl_get(db, cl);

        puts("\n---------------- Autok kezelese ----------------");
        puts("[0] Vissza");
        puts("[1] Auto hozzadasa");
        puts("[2] Auto adatainak modositasa");
        puts("[3] Auto eltavolitasa");
        puts("[4] Javitas/vizsga hozzadasa");
        puts("[5] Javitas/vizsga adatainak modositasa");
        puts("[6] Javitas/vizsga eltavolitasa");
        puts("-------------------------------------------------");
        printf("[%s][%s][%s]\n", cl_->name, cl_->email, cl_->phone);
        puts("-------------------------------------------------");
        if (cl_->cars->size == 0) {
                puts("Ennek az ugyfelnek nincsenek hozzadott autoi.");
                goto txt_end;
        }
        for (idx i = 0; i < cl_->cars->size; i++) {
                struct car *car = db_car_get(db, cl, i);
                printf("[%zu][%s][%s]\n", i, car->name, car->plate);

                for (idx j = 0; j < car->operations->size; j++) {
                        struct operation *op = db_op_get(db, cl, i, j);
                        char date_str[17];
                        date_printf(&op->date_cr, date_str);

                        printf("\t\t[%zu][%s][%.2f][%s]", j, op->desc,
                                op->price, date_str);

                        if (op->date_exp.y != 0) {
                                char date_str2[17];
                                date_printf(&op->date_exp, date_str2);
                                printf("->[%s]", date_str2);
                        }

                        puts("");
                }
        }

        txt_end:
                puts("--------------------------------------------------");
                printf("Opcio: ");

}


int intf_car(const struct database *db, idx cl)
{
        if (db_cl_get(db, cl) == NULL)
                return EOOB;

        bool submenu_active = true;
        while (submenu_active) {
                intf_car_txt(db, cl);
                int opt = intf_io_opt();
                int opt2 = 0;
                int response = 0;

                switch (opt) {
                        case 0:
                                submenu_active = false;
                                break;
                        case 1:
                                response = intf_car_add(db, cl);
                                if (response == EMALLOC)
                                        return EMALLOC;
                                break;
                        case 2:
                                printf("Auto sorszama: ");
                                opt = intf_io_opt();

                                response = intf_car_mod(db, cl, opt);
                                if (response == EOOB)
                                        puts("\nAz auto nem talalhato");
                                break;
                        case 3:
                                printf("Auto sorszama: ");
                                opt = intf_io_opt();

                                response = intf_car_rm(db, cl, opt);
                                if (response == EOOB)
                                        puts("\nAz auto nem talalhato");
                                break;
                        case 4:
                                printf("Auto sorszama: ");
                                opt = intf_io_opt();

                                response = intf_op_add(db, cl, opt);
                                if (response == EOOB)
                                        puts("\nAz auto nem talalhato.");
                                else if (response == EMALLOC)
                                        return EMALLOC;
                                break;
                        case 5:
                                printf("Auto sorszama: ");
                                opt = intf_io_opt();

                                printf("Javitas sorszama: ");
                                opt2 = intf_io_opt();

                                response = intf_op_mod(db, cl, opt,opt2);
                                if (response == EOOB)
                                        puts("\nAz elem nem talalhato.");
                                break;

                        case 6:
                                printf("Auto sorszama: ");
                                opt = intf_io_opt();

                                printf("Javitas sorszama: ");
                                opt2 = intf_io_opt();

                                response = intf_op_rm(db, cl, opt, opt2);
                                if (response == EOOB)
                                        puts("\nAz elem nem talalhato.");
                                break;
                        default:
                                puts("\nErvenytelen opcio.\n");
                                break;
                }
        }
        return 0;
}

int intf_car_add(const struct database *db, idx cl)
{
        char name_buffer[NAME_SIZE + 1] = "\0";
        char plate_buffer[PLATE_SIZE + 1] = "\0";

        printf("Auto tipusa (max. %d karakter): ", NAME_SIZE);
        intf_io_fgets(name_buffer, NAME_SIZE + 1);

        printf("Auto rendszama (max. %d karakter): ", PLATE_SIZE);
        intf_io_fgets(plate_buffer, PLATE_SIZE + 1);

        return db_car_add(db, cl, name_buffer, plate_buffer);
}

int intf_op_add(const struct database *db, idx cl, idx car)
{
        if (db_car_get(db, cl, car) == NULL)
                return EOOB;

        char desc_buffer[DESC_SIZE + 1] = "\0";
        char price_buffer[DEFAULT_BUF_SIZE + 1] = "\0";
        char date_buffer[DEFAULT_BUF_SIZE + 1] = "\0";
        double price = 0;

        printf("Javitas/vizsga leirasa (max. %d karakter): ", DESC_SIZE);
        intf_io_fgets(desc_buffer, DESC_SIZE + 1);

        printf("Javitas/vizsga koltsege: ");
        intf_io_fgets(price_buffer, DEFAULT_BUF_SIZE + 1);
        price = strtod(price_buffer, NULL);

        printf("Vizsga eseten ervenyesseg lejarta (formatum: EEEE-HH-NN OO-PP): ");
        intf_io_fgets(date_buffer, DEFAULT_BUF_SIZE + 1);

        if (date_buffer[0] == '\n')
                return db_op_add(db, cl, car, desc_buffer, price, NULL);

        return db_op_add(db, cl, car, desc_buffer, price, date_buffer);
}

int intf_car_mod(const struct database *db, idx cl, idx car)
{
        char name_buffer[NAME_SIZE + 1] = "\0";
        char plate_buffer[PLATE_SIZE + 1] = "\0";

        printf("Auto tipusa (max. %d karakter): ", NAME_SIZE);
        intf_io_fgets(name_buffer, NAME_SIZE + 1);

        printf("Auto rendszama (max. %d karakter, formatum: ABC123 vagy ABCD123): ",
                PLATE_SIZE);
        intf_io_fgets(plate_buffer, PLATE_SIZE + 1);

        return db_car_mod(db, cl, car, name_buffer, plate_buffer);
}

int intf_op_mod(const struct database *db, idx cl, idx car, idx op)
{
        if (db_car_get(db, cl, car) == NULL)
                return EOOB;

        char desc_buffer[DESC_SIZE + 1] = "\0";
        char price_buffer[DEFAULT_BUF_SIZE + 1] = "\0";
        double price = 0;

        printf("Javitas leirasa (max. %d karakter): ", DESC_SIZE);
        intf_io_fgets(desc_buffer, DESC_SIZE + 1);

        puts("Javitas koltsege (forintban): ");
        intf_io_fgets(price_buffer, DEFAULT_BUF_SIZE + 1);
        price = strtod(price_buffer, NULL);

        return db_op_mod(db, cl, car, op, desc_buffer, price, NULL);
}

int intf_car_rm(const struct database *db, idx cl, idx car)
{
        return db_car_rm(db, cl, car);
}

int intf_op_rm(const struct database *db, idx cl, idx car, const idx op)
{
        return db_op_rm(db, cl, car, op);
}