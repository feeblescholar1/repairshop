/**
 * @file intf_search.h
 * @brief Header for user search UI.
 */

#ifndef REPAIRSHOP_INTF_SEARCH_H
#define REPAIRSHOP_INTF_SEARCH_H

#include "../../module-database/include/database.h"
#include "../../include/search.h"
#include "intf_io.h"
#include "intf_car.h"
#include "intf_client.h"

int intf_search(database *db);

sres intf_search_cl(database *db);
sres intf_search_plate(database *db);
sres intf_search_exp(database *db);

#endif //REPAIRSHOP_INTF_SEARCH_H
