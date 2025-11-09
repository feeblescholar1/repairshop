#include "include/fh.h"

int fh_db_op_add(struct database *db, idx cl, idx car, const char *desc,
        double price, const char *date_cr, const char *date_exp)
{
        struct operation *op = malloc(sizeof(struct operation));
        if (!op)
                return EMALLOC;

        strcpy(op->desc, desc);
        op->price = price;
        op->date_cr = date_parse(date_cr);
        if (date_exp[0] != '0')
                op->date_exp = date_parse(date_exp);
        else
                op->date_exp.y = 0;

        struct car *tmp = db_car_get(db, cl, car);

        return vct_push(tmp->operations, op);
}

int fh_parse_client(struct database *dst, char *str)
{
        char name[NAME_SIZE + 1] = "\0";
        char email[EMAIL_SIZE + 1] = "\0";
        char phone[PHNUM_SIZE + 1] = "\0";

        char *buf_ptr[3]= {name, email, phone};
        char *token = strtok(str, ">");

        for (int i = 0; i < 3; i++) {
                token = strtok(NULL, "|\n");
                strcpy(buf_ptr[i], token);
        }

        return db_cl_add(dst, name, email, phone);
}

int fh_parse_car(struct database *dst, idx cl, char *str)
{
        char name[NAME_SIZE + 1] = "\0";
        char plate[PLATE_SIZE + 1] = "\0";

        strtok(str, ">");
        strcpy(name, strtok(NULL, "|"));
        strcpy(plate, strtok(NULL, "\n"));

        return db_car_add(dst, cl, name, plate);
}

int fh_parse_op(struct database *dst, idx cl, idx car, char *str)
{
        char desc[DESC_SIZE + 1] = "\0";
        double price = 0;
        char date_cr[DEFAULT_BUF_SIZE + 1];
        char date_exp[DEFAULT_BUF_SIZE + 1];

        strtok(str, ">");
        strcpy(desc, strtok(NULL, "|"));
        sscanf(strtok(NULL, "|"), "%lf", &price);
        strcpy(date_cr, strtok(NULL, "|"));
        strcpy(date_exp, strtok(NULL, "\n"));

        return fh_db_op_add(dst, cl, car, desc, price, date_cr, date_exp);
}

int fh_import(struct database *dst)
{
        FILE *src = fopen("export.txt", "r");
        if (!src)
                return EFPERM;

        char read_buffer[LONGEST_VALID_LINE] = "\0";
        int last_client_index = -1;
        int last_car_index = -1;

        while (fgets(read_buffer, LONGEST_VALID_LINE, src) != NULL) {
                int r;
                switch (read_buffer[0]) {
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
                                r = fh_parse_op(dst, last_client_index, last_car_index,
                                        read_buffer);
                                if (r == EMALLOC)
                                        return EMALLOC;
                                break;
                }
        }

        fclose(src);
        return 0;
}
