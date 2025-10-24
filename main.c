#include <stdio.h>

#include "module-database/include/database.h"
#include "module-interface/include/interface.h"

int main(void)
{
        struct database *db = db_init("assd", "ddd");
        mainloop(db);
        db_del(db);
        return 0;
}
