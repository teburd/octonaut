#include "hash.h"
#include "hash_function.h"

START_TEST (test_octo_hash_create)
{
    octo_hash hash;

    octo_hash_init(&hash, octo_hash_murmur3, 0, 4); 

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
    s1.hash.key = &s1.value;
    s1.hash.keylen = sizeof(s1.value);

    test_hash_struct s2 = 
        {
            .value = 2,
        };
    s2.hash.key = &s2.value;
    s2.hash.keylen = sizeof(s2.value);

    octo_hash_init(&hash, octo_hash_murmur3, 0, 8);

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

TCase* octo_hash_tcase()
{
    TCase* tc_octo_hash = tcase_create("octo_hash");
    tcase_add_test(tc_octo_hash, test_octo_hash_create);
    tcase_add_test(tc_octo_hash, test_octo_hash_put_get_remove);
    /*
    tcase_add_test(tc_octo_hash, test_octo_hash_prepend);
    tcase_add_test(tc_octo_hash, test_octo_hash_append);
    tcase_add_test(tc_octo_hash, test_octo_hash_remove);
    tcase_add_test(tc_octo_hash, test_octo_hash_foreach);
    */
    return tc_octo_hash;
}
