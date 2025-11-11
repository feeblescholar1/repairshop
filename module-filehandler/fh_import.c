/**
 * @file fh_import.c
 * @brief Function definitions to load a file into a database.
 * @details The import process is based on the same hierarchical logic as the
 *          database.\n The source file is parsed line-by-line. The program looks
 *          for an ID char first (U, A or J), this marks the datatype. When the
 *          datatype has been determined, the program will parse the string
 *          accordingly. As for the linkage, the program will link all clients
 *          to the destination database. As for the others they will link to
 *          their last stored parent object.
 * @warning The implementation does \b not check file or data integrity and
 *          \b cannot detect intentional tampering with the source file.
 */

#include "include/fh.h"

/**
 * @brief Adds an operation to a database.
 * @details For the filehandler, a special operation import functions is needed,
 *          since \c db_op_add() can only parse \c date_exp and generates
 *          \c date_cr during runtime, which would produce inaccurate results,
 *          since date of creation is \b not the current date and time. Most
 *          likely, those operations were created much earlier. This fuction can
 *          parse \c date_cr too to avoid that.
 * @param db The pointer to destination database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @param desc The operation's description.
 * @param price The operation's price.
 * @param date_cr A date string which will be parsed to \c date_cr .
 * @param date_exp A date string which will be parsed to \c date_exp .
 * @returns vct_push() - if the creation was sucessful.
 * @retval 0 On success.
 * @retval EMALLOC If the operation allocation fails.
 * @note Dates should be in 'YYYY-MM-DD HH:MM' format (or 0 if not used).
 * @note For all return values see \c vct_push.
 */
int fh_db_op_add(struct database *db, idx cl, idx car, const char *desc,
                 double price, const char *date_cr, const char *date_exp)
{
        struct operation *op = malloc(sizeof(struct operation));
        if (!op)
                return EMALLOC;

        strcpy(op->desc, desc);
        op->price = price;
        op->date_cr = date_parse(date_cr);

        /* check if date_exp is uninitialized (indicated by a 0 in the file) */
        if (date_exp[0] != '0')
                op->date_exp = date_parse(date_exp);
        else
                op->date_exp.y = 0;

        /* get the parent object to link to */
        struct car *tmp = db_car_get(db, cl, car);

        return vct_push(tmp->operations, op);
}

/**
 * @brief Parses \c str to a client and links it to the database.
 * @param dst The pointer to the destination database.
 * @param str The string to be parsed.
 * @return db_cl_add() - with the tokenized parameters
 * @note The format of str should be: U>name|email|phone
 */
int fh_parse_client(struct database *dst, char *str)
{
        char name[NAME_SIZE + 1] = "\0";
        char email[EMAIL_SIZE + 1] = "\0";
        char phone[PHNUM_SIZE + 1] = "\0";

        char *buf_ptr[3]= {name, email, phone};

        /* the first token is not needed */
        strtok(str, ">");
        /* fill the buffers */
        for (int i = 0; i < 3; i++)
                strcpy(buf_ptr[i], strtok(NULL, "|\n"));

        return db_cl_add(dst, name, email, phone);
}

/**
 * @brief Parses \c str to a car structure and links it to the database.
 * @param dst The pointer to the destination database.
 * @param cl The client's index in the database.
 * @param str The string to be parsed.
 * @return db_car_add() - with the tokenized parameters
 * @note The format of str should be: A>name|plate
 */
int fh_parse_car(struct database *dst, idx cl, char *str)
{
        char name[NAME_SIZE + 1] = "\0";
        char plate[PLATE_SIZE + 1] = "\0";

        /* the first token is not needed */
        strtok(str, ">");
        strcpy(name, strtok(NULL, "|"));
        strcpy(plate, strtok(NULL, "\n"));

        return db_car_add(dst, cl, name, plate);
}

/**
 * @brief Parses \c str to an operation structure and links it to the database.
 * @param dst The pointer to the destination database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @param str The string to be parsed.
 * @return db_car_add() - with the tokenized parameters
 * @note The format of str should be: J>desc|plate|date_cr|date_exp
 */
int fh_parse_op(struct database *dst, idx cl, idx car, char *str)
{
        char desc[DESC_SIZE + 1] = "\0";
        double price = 0;
        char date_cr[DEFAULT_BUF_SIZE + 1];
        char date_exp[DEFAULT_BUF_SIZE + 1];

        /* the first token is not needed */
        strtok(str, ">");

        strcpy(desc, strtok(NULL, "|"));
        sscanf(strtok(NULL, "|"), "%lf", &price);
        strcpy(date_cr, strtok(NULL, "|"));
        strcpy(date_exp, strtok(NULL, "\n"));

        return fh_db_op_add(dst, cl, car, desc, price, date_cr, date_exp);
}

/**
 * @brief Imports \c export.txt into a database.
 * @param dst The pointer to the destination database.
 * @retval 0 On success.
 * @retval EFPERM If the file cannot be opened for reading.
 * @retval EMALLOC If the database expansion fails.
 * @warning The destination database must be initilazed with \c db_init() first.
 */
int fh_import(struct database *dst)
{
        FILE *src = fopen("export.txt", "r");
        if (!src)
                return EFPERM;

        char read_buffer[LONGEST_VALID_LINE] = "\0";
        int last_client_index = -1;
        int last_car_index = -1;

        while (fgets(read_buffer, LONGEST_VALID_LINE, src) != NULL) {
                /* function return value */
                int r = 0;

                /* check the char ID */
                switch (read_buffer[0]) {
                        case 'D':
                                strtok(read_buffer, ">");
                                strcpy(dst->name, strtok(NULL, "|"));
                                strcpy(dst->desc, strtok(NULL, "|"));
                                break;
                        case 'U':
                                r = fh_parse_client(dst, read_buffer);
                                if (r == EMALLOC)
                                        return EMALLOC;
                                last_client_index++;
                                last_car_index = -1;
                                break;
                        case 'A':
                                r = fh_parse_car(dst, last_client_index,
                                        read_buffer);
                                if (r == EMALLOC)
                                        return EMALLOC;
                                last_car_index++;
                                break;
                        case 'J':
                                r = fh_parse_op(dst, last_client_index,
                                        last_car_index, read_buffer);
                                if (r == EMALLOC)
                                        return EMALLOC;
                                break;
                }
        }

        fclose(src);
        return 0;
}
