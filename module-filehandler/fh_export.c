#include "include/fh.h"

void fh_client_export(struct client *client, FILE *target)
{
        /* export format: U>name|email|phone */
        fprintf(target, "U>%s|%s|%s\n", client->name, client->email,
                 client->phone);
}

void fh_car_export(struct car *car, FILE *target)
{
        /* export format: A>name|plate */
        fprintf(target, "A>%s|%s\n", car->name, car->plate);
}

void fh_op_export(struct operation *op, FILE *target)
{
        /* export format: J>desc|price|date_cr|date_exp */
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