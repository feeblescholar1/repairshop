#ifndef REPAIRSHOP_SUBINTERFACE_CLIENT_H
#define REPAIRSHOP_SUBINTERFACE_CLIENT_H

#include <stdbool.h>

#include "../../include/errorcodes.h"
#include "../../module-database/include/database.h"
#include "interface_io_tools.h"

void subinterface_client_text(struct database *db);
int subinterface_client(struct database *db);
int subinterface_client_add(struct database *db);
int subinterface_client_rm(struct database *db);

#endif //REPAIRSHOP_SUBINTERFACE_CLIENT_H