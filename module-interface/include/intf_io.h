/**
 * @file intf_io.h
 * @brief Header for interface IO functions.
 */

#ifndef REPAIRSHOP_INTF_IO_H
#define REPAIRSHOP_INTF_IO_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * A general buffer size, must not be lower than 32 to avoid breaking
 * functionality.
 */
#define DEFAULT_BUF_SIZE 32

void intf_io_fgets(char *buffer, size_t size);
int intf_io_opt(void);

#endif //REPAIRSHOP_INTF_IO_H
