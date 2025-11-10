/*
 * interface.h - include this to use the project ui
 */
#ifndef REPAIRSHOP_INTF_H
#define REPAIRSHOP_INTF_H

#include "intf_io.h"
#include "intf_client.h"
#include "intf_search.h"

void intf_main_txt();
int intf_main(struct database *db);

#endif //REPAIRSHOP_INTF_H