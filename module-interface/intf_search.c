#include "include/intf_search.h"

void intf_search_txt()
{
        puts("\n----- Kereses -----");
        puts("[0] Vissza");
        puts("[1] Ugyfel keresese");
        puts("[2] Rendszam keresese");
        puts("-------------------");
}

int intf_search(struct database *db)
{

        bool submenu_active = true;
        while (submenu_active) {
                intf_search_txt();
                printf("Opcio: ");
                int opt = intf_io_opt();
                int resp = 0;

                switch (opt) {
                        case 0:
                                submenu_active = false;
                                break;
                        case 1:
                                resp = intf_search_cl(db);
                                if (resp == EMALLOC)
                                        return EMALLOC;
                                break;
                        case 2:
                                resp = intf_search_plate(db);
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

int intf_search_cl(struct database *db)
{
        /* Ask for the term */
        printf("Ugyfel neve (max. %d karakter): ", NAME_SIZE);
        char term[NAME_SIZE + 1] = "\0";
        intf_io_fgets(term, NAME_SIZE + 1);

        struct sres res = search_cl(db, term);
        /* Handling these will be the same so the error code doesn't matter */
        if (res.err == EREALLOC || res.err == EMALLOC)
                return EMALLOC;

        puts("--------------------------");

        /* No results */
        if (res.map->size == 0) {
                puts("Nincs talalat.");
                puts("--------------------------");
                vct_del(res.map);
                return 0;
        }

        /* Print the results to stdout */
        for (idx i = 0; i < res.map->size; i++) {
                /* Get the index of the result item */
                idx *db_idx = vct_subptr(res.map, i);

                struct client *cl = db_cl_get(db, *db_idx);
                printf("[%zu][%s][%s][%s]\n", *db_idx, cl->name, cl->email,
                        cl->phone);
        }
        puts("--------------------------");

        vct_del(res.map);
        return 0;
}


int intf_search_plate(struct database *db)
{
        /* Ask for the term */
        printf("Rendszam (max. %d karakter): ", PLATE_SIZE);
        char term[PLATE_SIZE + 1] = "\0";
        intf_io_fgets(term, PLATE_SIZE + 1);

        struct sres res = search_plate(db, term);
        /* Handling these will be the same so the error code doesn't matter */
        if (res.err == EREALLOC || res.err == EMALLOC)
                return EMALLOC;

        puts("--------------------------");

        /* No results */
        if (res.map->size == 0) {
                puts("Nincs talalat.");
                puts("--------------------------");
                vct_del(res.map);
                return 0;
        }

        /* Print the results to stdout */
        for (idx i = 0; i < res.map->size; i++) {
                /* Get the index of the result item */
                idx *db_idx = vct_subptr(res.map, i);

                struct client *cl = db_cl_get(db, db_idx[0]);
                struct car *car = db_car_get(db, db_idx[0], db_idx[1]);

                printf("[%zu][%s][%s][%s]\n", db_idx[0], cl->name, cl->email,
                        cl->phone);
                printf("\t[%zu][%s][%s]\n", db_idx[1], car->name, car->plate);
        }
        puts("--------------------------");

        vct_del(res.map);
        return 0;
}