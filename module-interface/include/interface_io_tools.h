#ifndef REPAIRSHOP_INTERFACE_IO_TOOLS_H
#define REPAIRSHOP_INTERFACE_IO_TOOLS_H

#include <string.h>
#include <stdio.h>

#define DEFAULT_BUFFER_SIZE 32

void interface_io_fgets_clean(char *buffer, int size);
int interface_io_get_opt();

#endif //REPAIRSHOP_INTERFACE_IO_TOOLS_H