#include "include/fh_import.h"


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

int fh_parse_car(struct database *dst, idx cl_i, char *str)
{
        char name[NAME_SIZE + 1] = "\0";
        char plate[PLATE_SIZE + 1] = "\0";

        char *token = strtok(str, ">");
        strcpy(name, strtok(NULL, "|"));
        strcpy(plate, strtok(NULL, "\n"));

        return db_car_add(dst, cl_i, name, plate);
}

int fh_parse_op(struct database *dst, idx cl_i, idx car_i, char *str)
{
        char desc[DESC_SIZE + 1] = "\0";
        double price = 0;
        struct date date;

        char *token = strtok(str, ">");
        strcpy(desc, strtok(NULL, "|"));
        sscanf(strtok(NULL, "|"), "%lf", &price);
        sscanf(strtok(NULL, "\n"), "%d-%d-%d %d:%d",
                &date.y, &date.mon, &date.d, &date.h, &date.min);

        return db_op_add(dst, cl_i, car_i, desc, price, NULL);
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