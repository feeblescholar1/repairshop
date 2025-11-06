#include <stdio.h>

#include "module-database/include/database.h"
#include "module-interface/include/interface.h"
#include "module-filehandler/include/filehandler_export.h"
#include "module-filehandler/include/filehandler_import.h"

int main(void)
{
        struct database *db = db_init("assd", "ddd");
        fh_import(db);
        mainloop(db);
        fh_export(db);
        db_del(db);
        return 0;
}
