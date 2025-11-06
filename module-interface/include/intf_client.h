#ifndef REPAIRSHOP_INTF_CLIENT_H
#define REPAIRSHOP_INTF_CLIENT_H

#include <stdbool.h>

#include "../../include/errorcodes.h"
#include "../../module-database/include/database.h"
#include "intf_io.h"
#include "intf_car.h"

void intf_cl_txt(const struct database *db);
int intf_cl(const struct database *db);
int intf_cl_mod(const struct database *db, idx cl);
int intf_cl_add(const struct database *db);
int interface_client_rm(const struct database *db, idx cl);

#endif //REPAIRSHOP_INTF_CLIENT_H