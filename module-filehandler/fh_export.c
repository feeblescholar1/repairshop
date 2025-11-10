#include "include/fh.h"

/* Exports client in the following format: U>name|email|phone */
void fh_client_export(struct client *client, FILE *target)
{
        fprintf(target, "U>%s|%s|%s\n", client->name, client->email,
                 client->phone);
}

/* Exports car in the following format: A>name|plate */
void fh_car_export(struct car *car, FILE *target)
{
        fprintf(target, "A>%s|%s\n", car->name, car->plate);
}

/* Exports car in the following format: J>desc|price|date_cr|date_exp */
void fh_op_export(struct operation *op, FILE *target)
{
        fprintf(target, "J>%s|%f|%d-%02d-%02d %02d:%02d|", op->desc,
                op->price, op->date_cr.y, op->date_cr.mon,
                op->date_cr.d, op->date_cr.h, op->date_cr.min);

        if (op->date_exp.y != 0)
                fprintf(target, "%d-%02d-%02d %02d:%02d\n", op->date_exp.y,
                        op->date_exp.mon, op->date_exp.d, op->date_exp.h,
                        op->date_exp.min);
        else
                fprintf(target, "0\n");
}

/*
 * f(ile)h(andler) export - exports db to export.txt
 * The export formats can be found in the comments above.
 */
int fh_export(const struct database *db)
{
        FILE *target = fopen("export.txt", "w");
        if (!target)
                return EFPERM;

        for (idx i = 0; i < db->cl->size; i++) {
                struct client *client = db_cl_get(db, i);
                fh_client_export(client, target);

                for (idx j = 0; j < client->cars->size; j++) {
                        struct car *car = db_car_get(db, i, j);
                        fh_car_export(car, target);

                        for (idx k = 0; k < car->operations->size; k++) {
                                struct operation *op = db_op_get(db, i, j, k);
                                fh_op_export(op, target);
                        }

                }
        }

        fclose(target);
        return 0;
}