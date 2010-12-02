#include "hash_function.h"

START_TEST (test_murmurhash3_sanity)
{
    const char url1[] = "/hey/man/nice/shot?what&what;";
    const char url2[] = "/hey/man/nice/shot?what&what;";
    const char url3[] = "/hey/man/nice/shot?what&not&what;";
    uint32_t hash1 = 0;
    uint32_t hash2 = 0;
    uint32_t hash3 = 0;

    hash1 = murmurhash3((const uint8_t*)url1, sizeof(url1));
    hash2 = murmurhash3((const uint8_t*)url2, sizeof(url2));
    hash3 = murmurhash3((const uint8_t*)url3, sizeof(url3));
    
    printf("hash1 %u hash2 %u hash3 %u\n", hash1, hash2, hash3);
    fail_unless(hash1 == hash2,
        "hashes do not match");
    fail_unless(hash1 != hash3,
        "hashes match");
}
END_TEST

TCase* octo_hash_function_tcase()
{
    TCase* tc_octo_hash_function = tcase_create("octo_hash_function");
    tcase_add_test(tc_octo_hash_function, test_murmurhash3_sanity);
    return tc_octo_hash_function;
}

