/*
 * interface_io_tools.c - typesafe io functions for handling user input
 */
#include "include/intf_io.h"

void intf_io_fgets(char *buffer, int size)
{
        fgets(buffer, size, stdin);

        if (strcspn(buffer, "\r\n") == size - 1) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';
}

int intf_io_opt()
{
        int opt = -1;
        char input_buf[DEFAULT_BUF_SIZE + 1] = "\0";

        intf_io_fgets(input_buf, DEFAULT_BUF_SIZE + 1);
        sscanf(input_buf, "%d", &opt);

        return opt;
}