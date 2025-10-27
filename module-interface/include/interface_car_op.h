#ifndef REPAIRSHOP_INTERFACE_CAR_OP_H
#define REPAIRSHOP_INTERFACE_CAR_OP_H

#include "../../module-database/include/database.h"
#include "interface_io_tools.h"

void interface_car_op_text(const struct database *db, index client_i);
int interface_car_op(const struct database *db, index client_i);
int interface_car_add(const struct database *db, index client_i);
int interface_op_add(const struct database *db, index client_i, index car_i);
int interface_car_rm(const struct database *db, index client_i, index car_i);
int interface_op_rm(const struct database *db, index client_i, index car_i,
        index op_i);

#endif //REPAIRSHOP_INTERFACE_CAR_OP_H