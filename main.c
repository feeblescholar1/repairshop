#include <stdio.h>
#include "include/database.h"

int main(void)
{
        // test for latest commit
        struct database *db = db_init("db1", "test database");
        for (int i = 0; i < 10; i++) {
                db_add_client(db, "testname", "testmail", "testnum");
        }
        for (int i = 0; i < 10; i++) {
                db_add_car(db, i, "lambo", "AAAA-TEST");
        }
        for (int i = 0; i < 10; i++) {
                db_add_op(db, i, 0, "brakeswap", 600, NULL);
                db_add_op(db, i, 0, "transmission swap", 800, NULL);
        }

        for (int i = 0; i < 10; i++) {
                struct client *cl = db_get_client(db, i);
                struct car *ca = db_get_car(db, i, 0);
                struct operation *op1 = db_get_op(db, i, 0, 0);
                struct operation *op2 = db_get_op(db, i, 0, 1);
                printf("client %s, %s, %s :\n", cl->name, cl->email, cl->phone);
                printf("\t %s %s:\n", ca->name, ca->plate);
                printf("\t\t %s %lf %s\n", op1->desc, op1->price, asctime(op1->date));
                printf("\t\t %s %lf %s\n", op2->desc, op2->price, asctime(op2->date));
                printf("-------\n\n");
        }

        for (int i = 0; i < 10; i++) {
                db_rm_client(db, 0);
        }

        free(db);

        return 0;
}
