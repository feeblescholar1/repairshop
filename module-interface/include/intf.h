/**
 * @file intf.h
 * @brief Module header file. Include this for the user interface.
 */
#ifndef REPAIRSHOP_INTF_H
#define REPAIRSHOP_INTF_H

#include "intf_io.h"
#include "intf_client.h"
#include "intf_search.h"

void intf_main_txt(struct database *db);
int intf_main(struct database *db);

#endif //REPAIRSHOP_INTF_H
