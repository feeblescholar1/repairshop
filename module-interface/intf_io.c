/**
 * @file intf_io.c
 * @brief Functions for user input management.
 */
#include "include/intf_io.h"

/**
 * @brief \c fgets() wrapper that clears stdin, if the input is too large.
 * @param buffer The destination buffer to write the user input to.
 * @param size The size of \c buffer .
 * @return -
 */
void intf_io_fgets(char *buffer, size_t size)
{
        do {
                fgets(buffer, size, stdin);

                if (strcspn(buffer, "\r\n") == size - 1) {
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                }

                size_t real_end = strcspn(buffer, "\r\n");
                buffer[real_end] = '\0';

                if (strcspn(buffer, "|") != real_end) {
                        puts("A pipe karakter ( | ) foglalt. Kerlek probald ujra.");
                        continue;
                }

                break;
        }
        while (true);
}

/**
 * @brief Gets \c option from the user. Used in interface driver codes.
 * @return The option, that user have entered.
 */
int intf_io_opt(void)
{
        int opt = -1;
        char input_buf[DEFAULT_BUF_SIZE + 1] = "\0";

        intf_io_fgets(input_buf, DEFAULT_BUF_SIZE + 1);
        sscanf(input_buf, "%d", &opt);

        return opt;
}
