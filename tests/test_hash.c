#include "hash.h"
#include "hash_function.h"

START_TEST (test_octo_hash_create)
{
    octo_hash hash;

    octo_hash_init(&hash, octo_hash_murmur3, 0); 

    octo_hash_destroy(&hash);
}
END_TEST

TCase* octo_hash_tcase()
{
    TCase* tc_octo_hash = tcase_create("octo_hash");
    tcase_add_test(tc_octo_hash, test_octo_hash_create);
    /*
    tcase_add_test(tc_octo_hash, test_octo_hash_add_destroy);
    tcase_add_test(tc_octo_hash, test_octo_hash_prepend);
    tcase_add_test(tc_octo_hash, test_octo_hash_append);
    tcase_add_test(tc_octo_hash, test_octo_hash_remove);
    tcase_add_test(tc_octo_hash, test_octo_hash_foreach);
    */
    return tc_octo_hash;
}
