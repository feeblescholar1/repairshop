/**
 * @file fh_import.c
 * @brief Function definitions to load a file into a database.
 * @details The import process is based on the same hierarchical logic as the database.\n The source file is parsed
 *          line-by-line. The program looks for an ID char first (U, A or J), this marks the datatype. When the datatype
 *          has been determined, the program will parse the string accordingly. As for the linkage, the program will
 *          link all clients to the destination database. The others will link to the last stored parent object.
 * @warning The implementation does \b not check file or data integrity and \b cannot detect intentional tampering with
 *          the source file.
 */

#include <math.h>

#include "include/fh.h"

/**
 * @brief Fills an array of buffers and checks if it's valid.
 * @param str Pointer to the string to be tokenized.
 * @param buf Pointer to an array of buffers.
 * @param buf_size Pointer to an array that contains the buffer sizes.
 * @param buf_cnt Number of buffers to be filled.
 * @return \c 0 if it's successful, \c EINV if not.
 */
int fh_buffer_filler(char *str, char **buf, size_t *buf_size, size_t buf_cnt)
{
        /* Ignore the ID char */
        strtok(str, ">");

        for (idx i = 0; i < buf_cnt; i++) {
                char *next_token = strtok(NULL, "|\n");

                /* Check if the even token exists. */
                if (!next_token)
                        return EINV;

                /* Check if the token is too long. */
                if (strlen(next_token) > buf_size[i])
                        return EINV;

                strcpy(buf[i], next_token);
        }

        return 0;
}

/**
 * @brief Adds an operation to a database.
 * @details For the filehandler, a special operation import functions is needed, since \c db_op_add() can only parse
 *          \c date_exp and generates \c date_cr during runtime, which would produce inaccurate results, since date of
 *          creation is \b not the current date and time. This fuction can parse \c date_cr too to avoid that.
 * @param db The pointer to destination database.
 * @param cl The client's index in the database.
 * @param cr The car's index in the database.
 * @param desc The operation's description.
 * @param price The operation's price.
 * @param date_cr A date string which will be parsed to \c date_cr .
 * @param date_exp A date string which will be parsed to \c date_exp .
 * @returns vct_push() - if the creation was sucessful.
 * @retval 0 On success.
 * @retval EMALLOC If the operation allocation fails.
 * @retval EINV If the parent car doesn't exist.
 * @note Dates should be in 'YYYY-MM-DD HH:MM' format (or 0 if not used).
 * @note For all return values see \c vct_push.
 */
int fh_db_op_add(database *db, idx cl, idx cr, const char *desc, double price, const char *date_cr,
                 const char *date_exp)
{
        /* Get the parent object to link to */
        car *parent = db_car_get(db, cl, cr);
        if (!parent)
                return EINV;

        operation *op = malloc(sizeof(operation));
        if (!op)
                return EMALLOC;

        strcpy(op->desc, desc);
        op->price = price;
        op->date_cr = date_parse(date_cr);

        /* Check if date_exp is uninitialized (indicated by a 0 in the file) */
        if (date_exp[0] != '0')
                op->date_exp = date_parse(date_exp);
        else
                op->date_exp.y = 0;

        return vct_push(parent->operations, op);
}

/**
 * @brief Parses \c str to a client and links it to the database.
 * @param dst The pointer to the destination database.
 * @param str The string to be parsed.
 * @return db_cl_add() - with the tokenized parameters
 * @note The format of str should be: \c U>name|email|phone
 */
int fh_parse_client(database *dst, char *str)
{
        char name[NAME_SIZE + 1] = "\0";
        char email[EMAIL_SIZE + 1] = "\0";
        char phone[PHNUM_SIZE + 1] = "\0";

        char *buf_ptr[3] = {name, email, phone};
        size_t expected_size[3] = {NAME_SIZE, EMAIL_SIZE, PHNUM_SIZE};

        fh_buffer_filler(str, buf_ptr, expected_size, 3);

        return db_cl_add(dst, name, email, phone);
}

/**
 * @brief Parses \c str to a car structure and links it to the database.
 * @param dst The pointer to the destination database.
 * @param cl The client's index in the database.
 * @param str The string to be parsed.
 * @return db_car_add() - with the tokenized parameters
 * @note The format of str should be: \c A>name|plate
 */
int fh_parse_car(database *dst, idx cl, char *str)
{
        char name[NAME_SIZE + 1] = "\0";
        char plate[PLATE_SIZE + 1] = "\0";

        char *buf_ptr[2] = {name, plate};
        size_t expected_size[2] = {NAME_SIZE, PLATE_SIZE};

        if (fh_buffer_filler(str, buf_ptr, expected_size, 2))
                return EINV;

        return db_car_add(dst, cl, name, plate);
}

/**
 * @brief Parses \c str to an operation structure and links it to the database.
 * @param dst The pointer to the destination database.
 * @param cl The client's index in the database.
 * @param car The car's index in the database.
 * @param str The string to be parsed.
 * @return db_car_add() - with the tokenized parameters
 * @note The format of str should be: \c J>desc|plate|date_cr|date_exp
 */
int fh_parse_op(database *dst, idx cl, idx car, char *str)
{
        char desc[DESC_SIZE + 1] = "\0";
        char price[DEFAULT_BUF_SIZE + 1] = "\0";
        char date_cr[DEFAULT_BUF_SIZE + 1] = "\0";
        char date_exp[DEFAULT_BUF_SIZE + 1] = "\0";

        char *buf_ptr[4] = {desc, price, date_cr, date_exp};
        size_t expected_size[4] = {DESC_SIZE, DEFAULT_BUF_SIZE, DEFAULT_BUF_SIZE, DEFAULT_BUF_SIZE};

        if (fh_buffer_filler(str, buf_ptr, expected_size, 4))
                return EINV;

        double price_ = 0;
        if (sscanf(price, "%lf", &price_) != 1)
                return EINV;

        return fh_db_op_add(dst, cl, car, desc, price_, date_cr, date_exp);
}

int fh_parse_dbinfo(database *db, char *str)
{
        char *dst_ptr[2] = {db->name, db->desc};
        size_t expected_size[2] = {NAME_SIZE, DESC_SIZE};

        if (fh_buffer_filler(str, dst_ptr, expected_size, 2))
                return EINV;

        return 0;
}

/**
 * @brief Imports \c export.txt into a database.
 * @param dst The pointer to the destination database.
 * @retval 0 On success.
 * @retval EFPERM If the file cannot be opened for reading.
 * @retval EMALLOC If the database expansion fails.
 * @warning The destination database must be initilazed with \c db_init() first.
 */
int fh_import(database *dst)
{
        FILE *src = fopen("export.txt", "r");
        if (!src)
                return EFPERM;

        char read_buffer[LONGEST_VALID_LINE] = "\0";
        int last_client_index = -1;
        int last_car_index = -1;

        while (fgets(read_buffer, LONGEST_VALID_LINE, src) != NULL) {
                int retval = 0;

                /* check the char ID */
                switch (read_buffer[0]) {
                        case 'D':
                                retval = fh_parse_dbinfo(dst, read_buffer);
                                break;
                        case 'U':
                                retval = fh_parse_client(dst, read_buffer);
                                last_client_index++;
                                last_car_index = -1;
                                break;
                        case 'A':
                                retval = fh_parse_car(dst, last_client_index, read_buffer);
                                last_car_index++;
                                break;
                        case 'J':
                                retval = fh_parse_op(dst, last_client_index, last_car_index, read_buffer);
                                break;
                        default:
                                break;
                }

                if (retval == EMALLOC) {
                        fclose(src);
                        return EMALLOC;
                }

                if (retval == EINV) {
                        fclose(src);
                        return EINV;
                }
        }

        fclose(src);
        return 0;
}
