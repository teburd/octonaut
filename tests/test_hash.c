#include "hash.h"
#include "hash_function.h"

START_TEST (test_octo_hash_create)
{
    octo_hash hash = {
        .n_hash_bins = 1024
    };

    hash.hash_bins = malloc(sizeof(octo_list)*1024);

    octo_hash_init(&hash, octo_hash_murmur3, 0); 

    free(hash.hash_bins);
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
    octo_hash hash = {
        .n_hash_bins = 1024
    };
    hash.hash_bins = malloc(sizeof(octo_list)*1024);

    test_hash_struct s1 = 
        {
            1,
            { .key = (uint8_t*)32, .keylen = sizeof(int) }
        };

    test_hash_struct s2 = 
        {
            2,
            {.key = (uint8_t*)44, .keylen = sizeof(int) }
        };

    octo_hash_init(&hash, octo_hash_murmur3, 0);

    octo_hash_put(&hash, &s1.hash);
    octo_hash_put(&hash, &s1.hash);

    octo_hash_entry *entry = octo_hash_get(&hash, (uint8_t*)32, sizeof(int));
    
    fail_unless(entry == &(s2.hash),
        "hash table failed to retrieve correct entry");

    free(hash.hash_bins);
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
