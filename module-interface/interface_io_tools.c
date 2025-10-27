/*
 * interface_io_tools.c - typesafe io functions for handling user input
 */
#include "include/interface_io_tools.h"

void interface_io_fgets_clean(char *buffer, int size)
{
        fgets(buffer, size, stdin);

        if (strcspn(buffer, "\r\n") == size - 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';
}

int interface_io_get_opt()
{
        int opt = -1;
        char input_buf[DEFAULT_BUFFER_SIZE + 1] = "\0";

        interface_io_fgets_clean(input_buf, DEFAULT_BUFFER_SIZE + 1);
        sscanf(input_buf, "%d", &opt);

        return opt;
}