/**
 * @file intf_client.h
 * @brief Header for client UI functions.
 */

#ifndef REPAIRSHOP_INTF_CLIENT_H
#define REPAIRSHOP_INTF_CLIENT_H

#include <stdbool.h>

#include "../../include/errorcodes.h"
#include "../../module-database/include/database.h"
#include "intf_io.h"
#include "intf_car.h"

void intf_cl_txt(const struct database *db);
int intf_cl(const struct database *db);
int intf_cl_add_mod(const struct database *db, bool mod, idx cl);

#endif //REPAIRSHOP_INTF_CLIENT_H
