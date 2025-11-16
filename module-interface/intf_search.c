#include "include/intf_search.h"

/**
 * @brief Prints the search menu's text and the last search's result to \c stdout .
 * @param db The source database pointer.
 * @param res Pointer to last search result.
 * @param depth Determines how many database indexes \c res->map->items contains per index.
 * @return  -
 */
void intf_search_txt(database *db, sres *res, int depth)
{
        puts("----------------------- Kereses ----------------------");
        puts("[0] Vissza");
        puts("[1] Ugyfel keresese");
        puts("[2] Rendszam keresese");
        puts("[3] 30 napon belul lejaro vizsgak listazasa");
        puts("-------------------");
        puts("[4] Tovabblepes az ugyfelek kezelehez.");
        puts("[5] Tovabblepes az autok/javitasok kezelesehez.");
        puts("------------------------------------------------------");

        if (depth == 0) {
                puts("A talalatok *itt* fognak megjelenni.");
                goto txt_end;
        }

        if (res->map->size == 0) {
                puts("Nincs talalat.");
                goto txt_end;
        }

        for (idx i = 0; i < res->map->size; i++) {
                idx *db_idx = vct_subptr(res->map, i);

                client *cl = db_cl_get(db, db_idx[0]);
                printf("[%zu][%s][%s][%s]\n", db_idx[0], cl->name, cl->email, cl->phone);

                if (depth > 1) {
                        car *car = db_car_get(db, *db_idx, db_idx[1]);
                        printf("\t[%zu][%s][%s]\n", db_idx[1], car->name, car->plate);

                        if (depth > 2) {
                                operation *op = db_op_get(db, db_idx[0], db_idx[1], db_idx[2]);

                                char date_cr[17] = "\0";
                                char date_exp[17] = "\0";
                                date_printf(&op->date_cr, date_cr);
                                date_printf(&op->date_exp, date_exp);

                                printf("\t\t[%zu][%s][%lf][%s]->[%s]\n", db_idx[2], op->desc, op->price, date_cr,
                                        date_exp);
                        }
                }
        }

        txt_end:
                puts("------------------------------------------------------");
                printf("Opcio: ");
}

/**
 * @brief The search menu's driver code.
 * @param db The database pointer which the user will address.
 * @retval 0 If the user requests to go back.
 * @retval EMALLOC If a memory allocation failure is occured.
 */
int intf_search(database *db)
{
        bool active = true;
        sres result = {.map = NULL, .err = 0};
        int depth = 0;

        while (active) {
                if (result.err == EMALLOC)
                        return EMALLOC;

                intf_search_txt(db, &result, depth);
                int s = intf_io_opt();
                int retval = 0;

                switch (s) {
                        case 0:
                                active = false;
                                if (result.map)
                                        vct_del(result.map);
                                break;
                        case 1:
                                if (result.map)
                                        vct_del(result.map);
                                result = intf_search_cl(db);
                                depth = 1;
                                break;
                        case 2:
                                if (result.map)
                                        vct_del(result.map);
                                result = intf_search_plate(db);
                                depth = 2;
                                break;
                        case 3:
                                if (result.map)
                                        vct_del(result.map);
                                result = search_expiration(db);
                                depth = 3;
                                break;
                        case 4:
                                retval = intf_cl(db);
                                break;
                        case 5:
                                printf("Ugyfelazonosito (nem kell talalatnak lennie): ");
                                s = intf_io_opt();
                                retval = intf_car(db, s);
                                break;
                        default:
                                puts("Nincs ilyen opcio.");
                                break;
                }

                if (retval == EOOB)
                        puts("Az ugyfel nem talalhato.");

                if (retval == EMALLOC)
                        return EMALLOC;
        }

        return 0;
}

/**
 * Frontend for user search by client name.
 * @param db The database pointer which the user will search in.
 * @return A search result structure with corresponding database indexes.
 */
sres intf_search_cl(database *db)
{
        /* Ask for the term */
        printf("Ugyfel neve (max. %d karakter): ", NAME_SIZE);
        char term[NAME_SIZE + 1] = "\0";
        intf_io_fgets(term, NAME_SIZE + 1);

        return search_cl(db, term);
}

/**
 * Frontend for user search by car plate number.
 * @param db The database pointer which the user will search in.
 * @return A search result structure with corresponding database indexes.
 */
sres intf_search_plate(database *db)
{
        /* Ask for the term */
        printf("Rendszam (max. %d karakter): ", PLATE_SIZE);
        char term[PLATE_SIZE + 1] = "\0";
        intf_io_fgets(term, PLATE_SIZE + 1);

        return search_plate(db, term);
}
