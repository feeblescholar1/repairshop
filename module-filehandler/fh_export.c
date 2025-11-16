/**
 * @file fh_export.c
 * @brief Function definitions for exporting a database into a txt file.
 * @details The exporting is performed by interating through the database and
 *          writing out all data by datatype to new line, with and ID char and
 *          its contents separated by a pipe (|).\n
 *          ID char: \c U - for clients, \c A - for cars and \c J - for operations.
 *          The ID char is followed by a \c > instead of a \c |.
 * @note The first line is the database name and description with the ID of \c D
 */

#include "include/fh.h"

/**
 * @brief Exports a client to a file in the following format: U>name|email|phone
 * @param client Pointer to the client structure to be exported.
 * @param target Pointer to target file.
 */
void fh_client_export(struct client *client, FILE *target)
{
        fprintf(target, "U>%s|%s|%s\n", client->name, client->email,
                 client->phone);
}

/**
 * @brief Exports a car to a file in the following format: A>name|plate
 * @param car Pointer to the car structure to be exported.
 * @param target Pointer to target file.
 */
void fh_car_export(struct car *car, FILE *target)
{
        fprintf(target, "A>%s|%s\n", car->name, car->plate);
}

/**
 * @brief Exports an operation to a file in the following format:\n
 *        U>description|price|date_cr|date_exp
 * @param op Pointer to the operation structure to be exported.
 * @param target Pointer to target file.
 * @note  If \c date_exp is uninintialized (marked by \c date_exp.y being \c 0)
 *        the function will write a \c 0 in place of \c date_exp to indicate that.
 */
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

/**
 * @brief Exports a database to file called \c export.txt .
 * @param db Pointer to the database to be exported.
 * @retval 0 On success.
 * @retval EFPERM If the file cannot be opened for writing.
 * @note If \c export.txt doesn't exsist, this function creates it.
 */
int fh_export(struct database *db)
{
        FILE *target = fopen("export.txt", "w");
        if (!target)
                return EFPERM;

        fprintf(target, "D>%s|%s\n", db->name, db->desc);

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
