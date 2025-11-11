/**
 * @file intf_search.h
 * @brief Header for user search UI.
 */

#ifndef REPAIRSHOP_INTF_SEARCH_H
#define REPAIRSHOP_INTF_SEARCH_H

#include "../../module-database/include/database.h"
#include "../../include/search.h"
#include "intf_io.h"

int intf_search(struct database *db);
int intf_search_cl(struct database *db);
int intf_search_plate(struct database *db);

#endif //REPAIRSHOP_INTF_SEARCH_H