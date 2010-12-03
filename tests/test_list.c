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

START_TEST (test_octo_list_add_destroy)
{
    octo_list list;
    octo_list item1;

    octo_list_init(&list); 

    fail_unless(list.next == &list,
        "list does not point to itself as the next item.");

    fail_unless(list.prev == &list,
        "list does not point to itself as the previous item.");

    octo_list_add(&list, &item1);

    fail_unless(item1.next == &list,
        "item1 does not point to list head as the next item.");

    fail_unless(item1.prev == &list,
        "item1 does not point to list head as the previous item.");

    fail_unless(list.next == &item1,
        "item1 does not point to list head as the next item.");

    fail_unless(list.prev == &item1,
        "item1 does not point to list head as the previous item.");

    octo_list_destroy(&list);

    fail_unless(list.next == &list,
        "list does not point to itself as the next item.");

    fail_unless(list.prev == &list,
        "list does not point to itself as the previous item.");

    fail_unless(item1.next == &item1,
        "item1 does not point to itself as the next item.");

    fail_unless(list.prev == &list,
        "item1 does not point to itself as the previous item.");
}
END_TEST

START_TEST (test_octo_list_prepend)
{
    octo_list list;
    octo_list item1;
    octo_list item2;

    octo_list_init(&list); 
    octo_list_add(&list, &item1);
    octo_list_prepend(&list, &item2);

    fail_unless(list.next == &item2,
        "list does not point to item2 as the next item.");

    fail_unless(list.prev == &item1,
        "list does not point to item1 as the previous item.");

    fail_unless(item1.next == &list,
        "item1 does not point to list as the next item.");

    fail_unless(item1.prev == &item2,
        "item1 does not point to item2 as the previous item.");

    fail_unless(item2.next == &item1,
        "item2 does not point to item1 as the next item.");

    fail_unless(item2.prev == &list,
        "item2 does not point to list as the previous item.");

    octo_list_destroy(&list);
}
END_TEST

START_TEST (test_octo_list_append)
{
    octo_list list;
    octo_list item1;
    octo_list item2;

    octo_list_init(&list); 
    octo_list_add(&list, &item1);
    octo_list_append(&list, &item2);

    fail_unless(list.next == &item1,
        "list does not point to item1 as the next item.");

    fail_unless(list.prev == &item2,
        "list does not point to item2 as the previous item.");

    fail_unless(item2.next == &list,
        "item2 does not point to list as the next item.");

    fail_unless(item2.prev == &item1,
        "item2 does not point to item1 as the previous item.");

    fail_unless(item1.next == &item2,
        "item1 does not point to item2 as the next item.");

    fail_unless(item1.prev == &list,
        "item1 does not point to list as the previous item.");

    octo_list_destroy(&list);
}
END_TEST

START_TEST (test_octo_list_remove)
{
    octo_list list;
    octo_list item1;
    octo_list item2;

    octo_list_init(&list); 
    octo_list_add(&list, &item1);
    octo_list_add(&list, &item2);
    octo_list_remove(&item2);

    fail_unless(list.next == &item1,
        "list does not point to item1 as the next item.");

    fail_unless(list.prev == &item1,
        "list does not point to item1 as the previous item.");

    fail_unless(item1.next == &list,
        "item1 does not point to list as the next item.");

    fail_unless(item1.prev == &list,
        "item1 does not point to list as the previous item.");

    octo_list_destroy(&list);
}
END_TEST


typedef struct test_list_struct
{
    int value;
    octo_list list;
} test_list_struct;

START_TEST (test_octo_list_foreach)
{
    octo_list list;
    test_list_struct item1;
    test_list_struct item2;

    item1.value = 1;
    item2.value = 2;
    octo_list_init(&item1.list);
    octo_list_init(&item2.list);

    octo_list_init(&list); 
    octo_list_append(&list, &item1.list);
    octo_list_append(&list, &item2.list);

    test_list_struct *pos;
    test_list_struct *next;
    int i = 1;
    octo_list_foreach(pos, next, &list, list)
    {
        fail_unless(i == pos->value,
            "value does not match index");
        i += 1;
    }

    octo_list_destroy(&list);
}
END_TEST


TCase* octo_list_tcase()
{
    TCase* tc_octo_list = tcase_create("octo_list");
    tcase_add_test(tc_octo_list, test_octo_list_create);
    tcase_add_test(tc_octo_list, test_octo_list_add_destroy);
    tcase_add_test(tc_octo_list, test_octo_list_prepend);
    tcase_add_test(tc_octo_list, test_octo_list_append);
    tcase_add_test(tc_octo_list, test_octo_list_remove);
    tcase_add_test(tc_octo_list, test_octo_list_foreach);
    return tc_octo_list;
}
