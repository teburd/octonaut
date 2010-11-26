#include <stdlib.h>
#include <stdio.h>
#include <check.h>

START_TEST (test_transport_create)
{
    printf("test_transport_create\n");
}
END_TEST

START_TEST (test_transport_destroy)
{
    printf("test_transport_destroy\n");
}
END_TEST


Suite* octonaut_suite(void)
{
    Suite* s = suite_create("Octonaut");

    TCase* tc_transport = tcase_create("Transport");
    tcase_add_test(tc_transport, test_transport_create);
    tcase_add_test(tc_transport, test_transport_destroy);
    suite_add_tcase(s, tc_transport);

    return s;
}


int main(void)
{   
    int number_failed = 0;
    Suite* s = octonaut_suite();
    SRunner* sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return 0;
}
