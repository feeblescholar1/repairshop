#ifndef REPAIRSHOP_INTF_CAR_H
#define REPAIRSHOP_INTF_CAR_H

#include "../../module-database/include/database.h"
#include "intf_io.h"

void intf_car_txt(const struct database *db, idx cl);
int intf_car(const struct database *db, idx cl);

int intf_car_add(const struct database *db, idx cl);
int intf_op_add(const struct database *db, idx cl, idx car);

int intf_car_mod(const struct database *db, idx cl, idx car);
int intf_op_mod(const struct database *db, idx cl, idx car, idx op);

int intf_car_rm(const struct database *db, idx cl, idx car);
int intf_op_rm(const struct database *db, idx cl, idx car, idx op);

#endif //REPAIRSHOP_INTF_CAR_H