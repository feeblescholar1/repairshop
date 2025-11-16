/**
 * @file intf_car.c
 * @brief The car/operation management menu's UI code.
 */

#include "include/intf_car.h"

/**
 * @brief Prints the car/operation management menu's text to \c stdout .
 */
void intf_car_txt(const database *db, idx cl)
{
        client *cl_ = db_cl_get(db, cl);

        puts("\n------------------ Autok kezelese -------------------");
        puts("[0] Vissza");
        puts("[1] Auto hozzadasa");
        puts("[2] Auto adatainak modositasa");
        puts("[3] Auto eltavolitasa");
        puts("[4] Javitas/vizsga hozzadasa");
        puts("[5] Javitas/vizsga adatainak modositasa");
        puts("[6] Javitas/vizsga eltavolitasa");
        puts("------------------------------------------------------");
        printf("[%s][%s][%s]\n", cl_->name, cl_->email, cl_->phone);
        puts("------------------------------------------------------");

        if (cl_->cars->size == 0) {
                puts("Ennek az ugyfelnek nincsenek hozzadott autoi.");
                goto txt_end;
        }

        for (idx i = 0; i < cl_->cars->size; i++) {
                car *car = db_car_get(db, cl, i);
                printf("[%zu][%s][%s]\n", i, car->name, car->plate);

                for (idx j = 0; j < car->operations->size; j++) {
                        operation *op = db_op_get(db, cl, i, j);
                        char date_str[17];
                        date_printf(&op->date_cr, date_str);

                        printf("\t\t[%zu][%s][%.2f][%s]", j, op->desc, op->price, date_str);

                        if (op->date_exp.y != 0) {
                                char date_str2[17];
                                date_printf(&op->date_exp, date_str2);
                                printf("->[%s]", date_str2);
                        }

                        puts("");
                }
        }

        txt_end:
                puts("------------------------------------------------------");
                printf("Opcio: ");

}

/**
 * @brief The car/operation management menu's driver code.
 * @param db The database pointer which the user will address.
 * @param cl The index of the client, who the user will manage.
 * @retval 0 If the user requests to go back.
 * @retval EOOB If the user requested a client, who doesn't exist in the database.
 * @retval EMALLOC If a memory allocation failure is occured.
 */
int intf_car(const database *db, idx cl)
{
        if (!db_cl_get(db, cl))
                return EOOB;

        bool menu_active = true;
        while (menu_active) {
                intf_car_txt(db, cl);
                int s = intf_io_opt();
                int s2 = 0;
                int retval = 0;

                switch (s) {
                        case 0:
                                menu_active = false;
                                break;
                        case 1:
                                retval = intf_car_add_mod(db, cl, false, 0);
                                break;
                        case 2:
                                printf("Auto sorszama: ");
                                s = intf_io_opt();

                                retval = intf_car_add_mod(db, cl, true, s);
                                break;
                        case 3:
                                printf("Auto sorszama: ");
                                s = intf_io_opt();

                                retval = db_car_rm(db, cl, s);
                                break;
                        case 4:
                                printf("Auto sorszama: ");
                                s = intf_io_opt();

                                retval = intf_op_add_mod(db, cl, s, false, 0);
                                break;
                        case 5:
                                printf("Auto sorszama: ");
                                s = intf_io_opt();

                                printf("Javitas sorszama: ");
                                s2 = intf_io_opt();

                                retval = intf_op_add_mod(db, cl, s, true, s2);
                                break;

                        case 6:
                                printf("Auto sorszama: ");
                                s = intf_io_opt();

                                printf("Javitas sorszama: ");
                                s2 = intf_io_opt();

                                retval = db_op_rm(db, cl, s, s2);
                                break;
                        default:
                                puts("\nErvenytelen opcio.\n");
                                break;
                }

                if (retval == EMALLOC)
                        return EMALLOC;

                if (retval == EOOB)
                        puts("\nAz auto/javitas nem talalhato.");
        }
        return 0;
}

/**
 * @brief The frontend for car addition/modification.
 * @param db The destination database.
 * @param cl The client's index in the database, which the user currenly manages.
 * @param mod Set to true if the user requests modification.
 * @param car The car's index if \c mod is \c true .
 * @return \c db_car_add() with the user given parameters.
 */
int intf_car_add_mod(const database *db, idx cl, bool mod, idx car)
{
        if (mod && !db_car_get(db, cl, car))
                return EOOB;

        char name_buffer[NAME_SIZE + 1] = "\0";
        char plate_buffer[PLATE_SIZE + 1] = "\0";

        printf("Auto tipusa (max. %d karakter, formatum: nincs): ", NAME_SIZE);
        intf_io_fgets(name_buffer, NAME_SIZE + 1);

        printf("Auto rendszama (max. %d karakter, formatum: ABCD123): ", PLATE_SIZE);
        intf_io_fgets(plate_buffer, PLATE_SIZE + 1);

        if (mod)
                return db_car_mod(db, cl, car, name_buffer, plate_buffer);

        return db_car_add(db, cl, name_buffer, plate_buffer);
}

/**
 * @brief The frontend for operation addition/modification.
 * @param db The destination database.
 * @param cl The client's index in the database, which the user currenly manages.
 * @param car The car's index in the database.
 * @param mod Set to true if the user requests modification.
 * @param op The operation's index if \c mod is \c true .
 * @return \c db_op_add() with the user given parameters.
 */
int intf_op_add_mod(const database *db, idx cl, idx car, bool mod, idx op)
{
        if (!db_car_get(db, cl, car) || (mod && !db_op_get(db, cl, car, op)))
                return EOOB;

        char desc_buffer[DESC_SIZE + 1] = "\0";
        char price_buffer[DEFAULT_BUF_SIZE + 1] = "\0";
        char date_buffer[DEFAULT_BUF_SIZE + 1] = "\0";
        double price = 0;

        printf("Javitas/vizsga leirasa (max. %d karakter, formatum: nincs): ", DESC_SIZE);
        intf_io_fgets(desc_buffer, DESC_SIZE + 1);

        printf("Javitas/vizsga (forintban, formatum: csak szam): ");
        intf_io_fgets(price_buffer, DEFAULT_BUF_SIZE + 1);
        price = strtod(price_buffer, NULL);

        printf("Vizsga eseten ervenyesseg lejarta (formatum: EEEE-HH-NN OO-PP): ");
        intf_io_fgets(date_buffer, DEFAULT_BUF_SIZE + 1);

        if (mod) {
                if (date_buffer[0] == '\n')
                        return db_op_mod(db, cl, car, op, desc_buffer, price, NULL);

                return db_op_mod(db, cl, car, op, desc_buffer, price, date_buffer);
        }

        if (date_buffer[0] == '\n')
                return db_op_add(db, cl, car, desc_buffer, price, NULL);

        return db_op_add(db, cl, car, desc_buffer, price, date_buffer);
}