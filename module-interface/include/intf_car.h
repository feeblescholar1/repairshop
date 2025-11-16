/**
 * @file intf_car.h
 * @brief Header for car UI functions.
 */

#ifndef REPAIRSHOP_INTF_CAR_H
#define REPAIRSHOP_INTF_CAR_H

#include "../../module-database/include/database.h"
#include "intf_io.h"

void intf_car_txt(const database *db, idx cl);
int intf_car(const database *db, idx cl);

int intf_car_add_mod(const database *db, idx cl, bool mod, idx car);
int intf_op_add_mod(const database *db, idx cl, idx car, bool mod, idx op);

#endif //REPAIRSHOP_INTF_CAR_H
