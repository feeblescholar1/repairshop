/**
 * @file fh_export.c
 * @brief Function definitions for exporting a database into a txt file.
 * @details The exporting is performed by interating through the database and writing out all data by datatype to new
 *          line, with and ID char and its contents separated by a pipe (|).\n
 *          ID char: \c U - for clients, \c A - for cars and \c J - for operations. The ID char is followed by a \c >
 *          instead of a \c |.
 * @note The first line is the database name and description with the ID of \c D .
 */

#include "include/fh.h"

/**
 * @brief Exports an operation to a file.
 * @param op Pointer to the operation structure to be exported.
 * @param target Pointer to target file.
 * @note  If \c date_exp is uninintialized (marked by \c date_exp.y being \c 0)
 *        the function will write a \c 0 in place of \c date_exp to indicate that.
 */
void fh_op_export(operation *op, FILE *target)
{
        fprintf(target, "J>%s|%f|%d-%02d-%02d %02d:%02d|", op->desc, op->price, op->date_cr.y, op->date_cr.mon,
                op->date_cr.d, op->date_cr.h, op->date_cr.min);

        if (op->date_exp.y != 0)
                fprintf(target, "%d-%02d-%02d %02d:%02d\n", op->date_exp.y, op->date_exp.mon, op->date_exp.d,
                        op->date_exp.h, op->date_exp.min);
        else
                fprintf(target, "0\n");
}

/**
 * @brief Exports a database to file called \c export.txt .
 * @details Exports objects in the following format:\n
 *          Clients: \c U>name|email|phone \n
 *          Cars: \c A>name|plate \n
 *          Objects: \c J>desc|price|date_cr|date_exp
 * @param db Pointer to the database to be exported.
 * @retval 0 On success.
 * @retval EFPERM If the file cannot be opened/created for writing.
 * @note If \c export.txt doesn't exsist, this function creates it.
 */
int fh_export(database *db)
{
        FILE *target = fopen("export.txt", "w");
        if (!target)
                return EFPERM;

        fprintf(target, "D>%s|%s\n", db->name, db->desc);

        for (idx i = 0; i < db->cl->size; i++) {
                client *cl = db_cl_get(db, i);
                fprintf(target, "U>%s|%s|%s\n", cl->name, cl->email, cl->phone);

                for (idx j = 0; j < cl->cars->size; j++) {
                        car *cr = db_car_get(db, i, j);
                        fprintf(target, "A>%s|%s\n", cr->name, cr->plate);

                        for (idx k = 0; k < cr->operations->size; k++) {
                                operation *op = db_op_get(db, i, j, k);
                                fh_op_export(op, target);
                        }

                }
        }

        fclose(target);
        return 0;
}
