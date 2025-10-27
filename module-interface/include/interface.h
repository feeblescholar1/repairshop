/*
 * interface.h - include this to use the project ui
 */
#ifndef REPAIRSHOP_INTERFACE_H
#define REPAIRSHOP_INTERFACE_H

#include "interface_io_tools.h"
#include "interface_client.h"

void text_mainloop();
int mainloop(struct database *db);

#endif //REPAIRSHOP_INTERFACE_H