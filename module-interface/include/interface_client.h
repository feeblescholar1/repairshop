#ifndef REPAIRSHOP_SUBINTERFACE_CLIENT_H
#define REPAIRSHOP_SUBINTERFACE_CLIENT_H

#include <stdbool.h>

#include "../../include/errorcodes.h"
#include "../../module-database/include/database.h"
#include "interface_io_tools.h"
#include "interface_car_op.h"

void interface_client_text(const struct database *db);
int interface_client(const struct database *db);
int interface_client_modify(const struct database *db, index client_i);
int interface_client_add(const struct database *db);
int interface_client_rm(const struct database *db, index client_i);

#endif //REPAIRSHOP_SUBINTERFACE_CLIENT_H