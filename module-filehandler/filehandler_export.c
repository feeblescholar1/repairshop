#include "include/filehandler_export.h"

void fh_client_export(struct client *client, FILE *target)
{
        fprintf(target, "U>%s|%s|%s\n", client->name, client->email,
                client->phone);
}

void fh_car_export(struct car *car, FILE *target)
{
        fprintf(target, "A>%s|%s\n", car->name, car->plate);
}

void fh_op_export(struct operation *op, FILE *target)
{
        fprintf(target, "J>%s|%f|%d-%02d-%02d %02d:%02d\n", op->desc,
                op->price, op->date.y, op->date.mon, op->date.d, op->date.h,
                op->date.min);
}

int fh_export(const struct database *db)
{
        FILE *target = fopen("export.txt", "w");
        if (!target)
                return ERR_FILE_ACCESS;

        for (index i = 0; i < db->clients->size; i++) {
                struct client *client = db_get_client(db, i);
                fh_client_export(client, target);

                for (index j = 0; j < client->cars->size; j++) {
                        struct car *car = db_get_car(db, i, j);
                        fh_car_export(car, target);

                        for (index k = 0; k < car->operations->size; k++) {
                                struct operation *op = db_get_op(db, i, j, k);
                                fh_op_export(op, target);
                        }

                }
        }

        fclose(target);
        return 0;
}