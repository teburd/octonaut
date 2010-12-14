/**
 * Copyright (c) 2010 Tom Burdick <thomas.burdick@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <octonaut/hash_function.h>
#include <octonaut/hash.h>
#include <check.h>

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

    bool has = false;
    has = octo_hash_has(&hash, &key, sizeof(key));

    fail_unless(has == true,
        "hash table failed to find correct entry");

    key = 3;
    entry = octo_hash_get(&hash, &key, sizeof(key));

    fail_unless(entry == NULL,
        "hash table failed to retrieve correct entry");

    octo_hash_destroy(&hash);
}
END_TEST

START_TEST (test_octo_hash_chaining)
{
    bool putted = false;

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

    test_hash_struct s3 = 
        {
            .value = 3,
        };
    octo_hash_entry_init(&s3.hash, &s3.value, sizeof(s3.value));


    octo_hash_init(&hash, octo_default_hash_function, 0, 1);

    putted = octo_hash_put(&hash, &s1.hash);
    fail_unless(putted == true,
        "put should succeed");
 
    putted = octo_hash_put(&hash, &s2.hash);
    fail_unless(putted == true,
        "put should succeed");
 
    putted = octo_hash_put(&hash, &s3.hash);
    fail_unless(putted == true,
        "put should succeed");

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

    fail_unless(octo_hash_size(&hash) == 2,
        "hash table size is incorrect");

    key = -1;
    entry = octo_hash_get(&hash, &key, sizeof(key));

    fail_unless(entry == NULL,
        "hash table failed to retrieve correct entry");

    key = 1;
    entry = octo_hash_pop(&hash, &key, sizeof(key));

    fail_unless(entry == &(s1.hash),
        "hash table failed to retrieve correct entry");

    fail_unless(octo_hash_size(&hash) == 1,
        "hash table size is incorrect");

    putted = octo_hash_put(&hash, &s3.hash);

    fail_unless(putted == false,
        "double put of the same key should result in failing to put");

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

    test_hash_sstring s3 = 
        {
            .key = "Jack",
        };
    octo_hash_entry_init(&s3.hash, s3.key, strlen(s3.key));

    octo_hash_init(&hash, octo_default_hash_function, 0, 1);

    octo_hash_put(&hash, &s1.hash);
    octo_hash_put(&hash, &s2.hash);
    octo_hash_put(&hash, &s3.hash);
    
    char king[] = "King";
    char queen[] = "Queen";
    char jack[] = "Jack";
    char ace[] = "Ace";

    octo_hash_entry *entry = octo_hash_get(&hash, queen, strlen(queen)); 

    fail_unless(entry == &(s1.hash),
        "hash table failed to retrieve correct entry");

    entry = octo_hash_get(&hash, &king, strlen(king));

    fail_unless(entry == &(s2.hash),
        "hash table failed to retrieve correct entry");

    entry = octo_hash_pop(&hash, queen, strlen(queen));

    fail_unless(entry == &(s1.hash),
        "hash table failed to retrieve correct entry");

    fail_unless(octo_hash_size(&hash) == 2,
        "hash table size is incorrect");

    entry = octo_hash_get(&hash, ace, strlen(ace));

    fail_unless(entry == NULL,
        "hash table failed to retrieve correct entry");

    entry = octo_hash_pop(&hash, king, strlen(king));

    fail_unless(entry == &(s2.hash),
        "hash table failed to retrieve correct entry");

    fail_unless(octo_hash_size(&hash) == 1,
        "hash table size is incorrect");

    entry = octo_hash_pop(&hash, king, strlen(king));

    fail_unless(entry == NULL,
        "hash table failed to retrieve NULL entry");

    fail_unless(octo_hash_size(&hash) == 1,
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
