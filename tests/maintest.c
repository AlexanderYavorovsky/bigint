/* Yavorovsky Alexander, 23.02.2024 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "bigint.h"
#include "minunit.h"

MU_TEST(test_init_max)
{
    size_t len = UINT_MAX;
    BigInt *x = bigint_init_n(len);

    mu_check(x != NULL);
    mu_check(x->len == len);

    bigint_free(x);
}

MU_TEST(test_init_min)
{
    size_t len = 1;
    BigInt *x = bigint_init_n(len);

    mu_check(x != NULL);
    mu_check(x->len == len);

    bigint_free(x);
}

MU_TEST_SUITE(suite_init)
{
    MU_RUN_TEST(test_init_max);
    MU_RUN_TEST(test_init_min);
}

MU_TEST(test_free)
{
    BigInt *x = bigint_init_n(10);

    bigint_free(x);

    mu_check(x == NULL);
}

MU_TEST_SUITE(suite_free)
{
    MU_RUN_TEST(test_free);
}

MU_TEST(test_str_pos)
{
    char str[] = "1234567890";
    uint8_t sign = 1;
    BigInt *a = strtobi(str);
    char *stra;

    mu_check(a->len == strlen(str));
    mu_check(a->sign == sign);

    stra = bitostr(a);
    mu_assert_string_eq(str, stra);

    bigint_free(a);
    free(stra);
}

MU_TEST(test_str_neg)
{
    char str[] = "-1234567890";
    int8_t sign = -1;
    BigInt *a = strtobi(str);
    char *stra;

    mu_check(a->len == strlen(str) - 1);
    mu_check(a->sign == sign);

    stra = bitostr(a);
    mu_assert_string_eq(str, stra);

    bigint_free(a);
    free(stra);
}

MU_TEST(test_str_plus)
{
    char str[] = "+1234567890";
    char str_without_plus[] = "1234567890";
    uint8_t sign = 1;
    BigInt *a = strtobi(str);
    char *stra;

    mu_check(a->len == strlen(str) - 1);
    mu_check(a->sign == sign);

    stra = bitostr(a);
    mu_assert_string_eq(str_without_plus, stra);

    bigint_free(a);
    free(stra);
}

MU_TEST(test_str_zero)
{
    char str[] = "00000000";
    char str_normalized[] = "0";
    uint8_t sign = 1;
    BigInt *a = strtobi(str);
    char *stra;

    mu_check(a->len == strlen(str_normalized));
    mu_check(a->sign == sign);

    stra = bitostr(a);
    mu_assert_string_eq(str_normalized, stra);

    bigint_free(a);
    free(stra);
}

MU_TEST(test_str_negzero)
{
    char str[] = "-00000000";
    char str_normalized[] = "0";
    uint8_t sign = 1;
    BigInt *a = strtobi(str);
    char *stra;

    mu_check(a->len == strlen(str_normalized));
    mu_check(a->sign == sign);

    stra = bitostr(a);
    mu_assert_string_eq(str_normalized, stra);

    bigint_free(a);
    free(stra);
}

MU_TEST(test_str_big)
{
    char str[] = "123456789098765432101234567890987654321";
    uint8_t sign = 1;
    BigInt *a = strtobi(str);
    char *stra;

    mu_check(a->len == strlen(str));
    mu_check(a->sign == sign);

    stra = bitostr(a);
    mu_assert_string_eq(str, stra);

    bigint_free(a);
    free(stra);
}

MU_TEST_SUITE(suite_str)
{
    MU_RUN_TEST(test_str_pos);
    MU_RUN_TEST(test_str_neg);
    MU_RUN_TEST(test_str_plus);
    MU_RUN_TEST(test_str_zero);
    MU_RUN_TEST(test_str_negzero);
    MU_RUN_TEST(test_str_big);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(suite_init);
    MU_RUN_SUITE(suite_free);
    MU_RUN_SUITE(suite_str);
	MU_REPORT();
	return MU_EXIT_CODE;
    
    
}