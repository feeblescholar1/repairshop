#include <stdio.h>

#include "module-database/include/database.h"
#include "module-interface/include/intf.h"
#include "module-filehandler/include/fh_export.h"
#include "module-filehandler/include/fh_import.h"

int main(void)
{
        struct database *db = db_init("assd", "ddd");
        fh_import(db);
        intf_main(db);
        fh_export(db);
        db_del(db);
        return 0;
}
