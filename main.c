#include "include/vector.h"
#include "include/objects.h"
#include <stdio.h>

int main(void)
{
        // test for latest commit
        struct vector *db = v_init();
        client_create(db, "john doe", "a@a.com", "+3630628344");
        car_create(db->items[0], "porsche911", "SAAASSS-WWWWW");
        op_create(((struct client*)(db->items[0]))->cars->items[0], "brakeswap",
                2001, NULL);
        client_create(db, "mary jane", "maryjane2@asd.com", "+40004");
        car_create(db->items[0], "lambo huracan", "S2222DWDD-WW");
        op_create(((struct client*)(db->items[0]))->cars->items[0], "brakeswap", 1201, NULL);


        client_remove(db, 0);
        client_remove(db, 0);
        return 0;
}
