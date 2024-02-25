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

MU_TEST(test_isless_0_0)
{
    BigInt *a = strtobi("0");
    BigInt *b = strtobi("0");

    mu_check(!bigint_isless(a, b));

    bigint_free(a);
    bigint_free(b);
}

MU_TEST(test_isless_pos_0)
{
    BigInt *a = strtobi("124");
    BigInt *b = strtobi("0");

    mu_check(!bigint_isless(a, b));

    bigint_free(a);
    bigint_free(b);
}

MU_TEST(test_isless_pos_pos)
{
    BigInt *a = strtobi("124");
    BigInt *b = strtobi("125");

    mu_check(bigint_isless(a, b));

    bigint_free(a);
    bigint_free(b);
}

MU_TEST(test_isless_neg_neg)
{
    BigInt *a = strtobi("-124");
    BigInt *b = strtobi("-224");

    mu_check(!bigint_isless(a, b));

    bigint_free(a);
    bigint_free(b);
}

MU_TEST_SUITE(suite_isless)
{
    MU_RUN_TEST(test_isless_0_0);
    MU_RUN_TEST(test_isless_pos_0);
    MU_RUN_TEST(test_isless_pos_pos);
    MU_RUN_TEST(test_isless_neg_neg);
}

MU_TEST(test_iszero_0)
{
    BigInt *x = strtobi("0");

    mu_check(bigint_iszero(x));

    bigint_free(x);
}

MU_TEST(test_iszero_pos)
{
    BigInt *x = strtobi("10");

    mu_check(!bigint_iszero(x));

    bigint_free(x);
}

MU_TEST_SUITE(suite_iszero)
{
    MU_RUN_TEST(test_iszero_0);
    MU_RUN_TEST(test_iszero_pos);
}

MU_TEST(test_normalize_pos)
{
    BigInt *x = strtobi("000123");
    char *sx;

    bigint_normalize(x);
    sx = bitostr(x);

    mu_assert_string_eq("123", sx);

    bigint_free(x);
    free(sx);
}

MU_TEST(test_normalize_neg)
{
    BigInt *x = strtobi("-000123");
    char *sx;

    bigint_normalize(x);
    sx = bitostr(x);

    mu_assert_string_eq("-123", sx);

    bigint_free(x);
    free(sx);
}

MU_TEST(test_normalize_plus)
{
    BigInt *x = strtobi("+000123");
    char *sx;

    bigint_normalize(x);
    sx = bitostr(x);

    mu_assert_string_eq("123", sx);

    bigint_free(x);
    free(sx);
}

MU_TEST(test_normalize_negzeros)
{
    BigInt *x = strtobi("-0000000");
    char *sx;

    bigint_normalize(x);
    sx = bitostr(x);

    mu_assert_string_eq("0", sx);

    bigint_free(x);
    free(sx);
}

MU_TEST_SUITE(suite_normalize)
{
    MU_RUN_TEST(test_normalize_pos);
    MU_RUN_TEST(test_normalize_neg);
    MU_RUN_TEST(test_normalize_plus);
    MU_RUN_TEST(test_normalize_negzeros);
}

MU_TEST(test_iseq_reflection)
{
    BigInt *x = strtobi("13");
    BigInt *negx = strtobi("-13");

    mu_check(bigint_iseq(x, x));
    mu_check(bigint_iseq(negx, negx));

    bigint_free(x);
    bigint_free(negx);
}

MU_TEST(test_iseq_sign)
{
    BigInt *x = strtobi("13");
    BigInt *negx = strtobi("-13");

    mu_check(!bigint_iseq(x, negx));

    bigint_free(x);
    bigint_free(negx);
}

MU_TEST_SUITE(suite_iseq)
{
    MU_RUN_TEST(test_iseq_reflection);
    MU_RUN_TEST(test_iseq_sign);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(suite_init);
    MU_RUN_SUITE(suite_str);
    MU_RUN_SUITE(suite_isless);
    MU_RUN_SUITE(suite_iszero);
    MU_RUN_SUITE(suite_normalize);
    MU_RUN_SUITE(suite_iseq);

	MU_REPORT();

	return MU_EXIT_CODE;
}