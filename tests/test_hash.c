#include "hash.h"
#include "hash_function.h"

START_TEST (test_octo_hash_create)
{
    octo_hash hash;

    octo_hash_init(&hash, octo_default_hash_function, 0, 4); 

    octo_hash_destroy(&hash);
}
END_TEST

typedef struct test_hash_struct
{
    int value;
    octo_hash_entry hash;
} test_hash_struct;

START_TEST (test_octo_hash_put_get_remove)
{
    octo_hash hash;

    test_hash_struct s1 = 
        {
            .value = 1,
        };
    octo_hash_entry_init(&s1.hash, &s1.value, sizeof(s1.value));

    test_hash_struct s2 = 
        {
            .value = 2,
        };

    octo_hash_entry_init(&s2.hash, &s2.value, sizeof(s2.value));

    octo_hash_init(&hash, octo_default_hash_function, 0, 8);

    octo_hash_put(&hash, &s1.hash);
    octo_hash_put(&hash, &s2.hash);
    
    uint32_t key = 1;
    octo_hash_entry *entry = octo_hash_get(&hash, &key, sizeof(key));

    fail_unless(entry == &(s1.hash),
        "hash table failed to retrieve correct entry");

    key = 2;
    entry = octo_hash_get(&hash, &key, sizeof(key));

    fail_unless(entry == &(s2.hash),
        "hash table failed to retrieve correct entry");

    key = 3;
    entry = octo_hash_get(&hash, &key, sizeof(key));

    fail_unless(entry == NULL,
        "hash table failed to retrieve correct entry");

    octo_hash_destroy(&hash);
}
END_TEST

START_TEST (test_octo_hash_chaining)
{
    octo_hash hash;

    test_hash_struct s1 = 
        {
            .value = 1,
        };
    octo_hash_entry_init(&s1.hash, &s1.value, sizeof(s1.value));

    test_hash_struct s2 = 
        {
            .value = 2,
        };

    octo_hash_entry_init(&s2.hash, &s2.value, sizeof(s2.value));

    octo_hash_init(&hash, octo_default_hash_function, 0, 1);

    octo_hash_put(&hash, &s1.hash);
    octo_hash_put(&hash, &s2.hash);
    
    uint32_t key = 1;
    octo_hash_entry *entry = octo_hash_get(&hash, &key, sizeof(key));

    fail_unless(entry == &(s1.hash),
        "hash table failed to retrieve correct entry");

    key = 2;
    entry = octo_hash_get(&hash, &key, sizeof(key));

    fail_unless(entry == &(s2.hash),
        "hash table failed to retrieve correct entry");

    key = 2;
    entry = octo_hash_pop(&hash, &key, sizeof(key));

    fail_unless(entry == &(s2.hash),
        "hash table failed to retrieve correct entry");

    fail_unless(octo_hash_size(&hash) == 1,
        "hash table size is incorrect");

    key = 3;
    entry = octo_hash_get(&hash, &key, sizeof(key));

    fail_unless(entry == NULL,
        "hash table failed to retrieve correct entry");

    key = 1;
    entry = octo_hash_pop(&hash, &key, sizeof(key));

    fail_unless(entry == &(s1.hash),
        "hash table failed to retrieve correct entry");

    fail_unless(octo_hash_size(&hash) == 0,
        "hash table size is incorrect");

    octo_hash_destroy(&hash);
}
END_TEST

typedef struct test_hash_sstring
{
    char key[10];
    octo_hash_entry hash;
} test_hash_sstring;

START_TEST (test_octo_hash_strings)
{
    octo_hash hash;

    test_hash_sstring s1 = 
        {
            .key = "Queen",
        };
    octo_hash_entry_init(&s1.hash, s1.key, strlen(s1.key));

    test_hash_sstring s2 = 
        {
            .key = "King",
        };
    octo_hash_entry_init(&s2.hash, s2.key, strlen(s2.key));

    octo_hash_init(&hash, octo_default_hash_function, 0, 1);

    octo_hash_put(&hash, &s1.hash);
    octo_hash_put(&hash, &s2.hash);
    
    char king[] = "King";
    char queen[] = "Queen";
    char jack[] = "Jack";

    octo_hash_entry *entry = octo_hash_get(&hash, queen, strlen(queen)); 

    fail_unless(entry == &(s1.hash),
        "hash table failed to retrieve correct entry");

    entry = octo_hash_get(&hash, &king, strlen(king));

    fail_unless(entry == &(s2.hash),
        "hash table failed to retrieve correct entry");

    entry = octo_hash_pop(&hash, king, strlen(king));

    fail_unless(entry == &(s2.hash),
        "hash table failed to retrieve correct entry");

    fail_unless(octo_hash_size(&hash) == 1,
        "hash table size is incorrect");

    entry = octo_hash_get(&hash, jack, strlen(jack));

    fail_unless(entry == NULL,
        "hash table failed to retrieve correct entry");

    entry = octo_hash_pop(&hash, queen, strlen(queen));

    fail_unless(entry == &(s1.hash),
        "hash table failed to retrieve correct entry");

    fail_unless(octo_hash_size(&hash) == 0,
        "hash table size is incorrect");

    octo_hash_destroy(&hash);
}
END_TEST



TCase* octo_hash_tcase()
{
    TCase* tc_octo_hash = tcase_create("octo_hash");
    tcase_add_test(tc_octo_hash, test_octo_hash_create);
    tcase_add_test(tc_octo_hash, test_octo_hash_put_get_remove);
    tcase_add_test(tc_octo_hash, test_octo_hash_chaining);
    tcase_add_test(tc_octo_hash, test_octo_hash_strings);
    /*
    tcase_add_test(tc_octo_hash, test_octo_hash_prepend);
    tcase_add_test(tc_octo_hash, test_octo_hash_append);
    tcase_add_test(tc_octo_hash, test_octo_hash_remove);
    tcase_add_test(tc_octo_hash, test_octo_hash_foreach);
    */
    return tc_octo_hash;
}
