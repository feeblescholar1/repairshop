#ifndef REPAIRSHOP_INTF_IO_H
#define REPAIRSHOP_INTF_IO_H

#include <string.h>
#include <stdio.h>

#define DEFAULT_BUF_SIZE 32

void intf_io_fgets(char *buffer, int size);
int intf_io_opt();

#endif //REPAIRSHOP_INTF_IO_H