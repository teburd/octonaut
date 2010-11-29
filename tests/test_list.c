#include "list.h"

START_TEST (test_octo_list_create)
{
    octo_list list;

    octo_list_init(&list); 

    fail_unless(list.next == &list,
        "list does not point to itself as the next item.");

    fail_unless(list.prev == &list,
        "list does not point to itself as the previous item.");

    octo_list_destroy(&list);

    fail_unless(list.next == &list,
        "list does not point to itself as the next item.");

    fail_unless(list.prev == &list,
        "list does not point to itself as the previous item.");

}
END_TEST

TCase* octo_list_tcase()
{
    TCase* tc_octo_list = tcase_create("octo_list");
    tcase_add_test(tc_octo_list, test_octo_list_create);
    return tc_octo_list;
}
