/* Yavorovsky Alexander, 23.02.2024 */

#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "bigint.h"
#include "minunit.h"

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

MU_TEST(test_sum10_zero)
{
    BigInt *zero = strtobi("0");
    BigInt *x = strtobi("123");
    BigInt *xneg = strtobi("-123");
    BigInt *sum_left = bigint_sum(zero, x);
    BigInt *sum_right = bigint_sum(x, zero);
    BigInt *sum_neg_left = bigint_sum(zero, xneg);
    BigInt *sum_neg_right = bigint_sum(xneg, zero);

    mu_check(bigint_iseq(x, sum_left));
    mu_check(bigint_iseq(x, sum_right));

    mu_check(bigint_iseq(xneg, sum_neg_left));
    mu_check(bigint_iseq(xneg, sum_neg_right));

    bigint_free(zero);
    bigint_free(x);
    bigint_free(xneg);
    bigint_free(sum_left);
    bigint_free(sum_right);
    bigint_free(sum_neg_left);
    bigint_free(sum_neg_right);
}

MU_TEST(test_sum10_pospos)
{
    BigInt *a = strtobi("999");
    BigInt *b = strtobi("123");
    BigInt *sum_left = bigint_sum(a, b);
    BigInt *sum_right = bigint_sum(b, a);
    BigInt *actual = strtobi("1122");

    mu_check(bigint_iseq(sum_left, sum_right));
    mu_check(bigint_iseq(actual, sum_left));
    mu_check(bigint_iseq(actual, sum_right));

    bigint_free(a);
    bigint_free(b);
    bigint_free(sum_left);
    bigint_free(sum_right);
    bigint_free(actual);
}

MU_TEST(test_sum10_negpos)
{
    BigInt *a = strtobi("-341");
    BigInt *b = strtobi("283");
    BigInt *sum = bigint_sum(a, b);
    BigInt *actual = strtobi("-58");

    mu_check(bigint_iseq(actual, sum));

    bigint_free(a);
    bigint_free(b);
    bigint_free(sum);
    bigint_free(actual);
}

MU_TEST_SUITE(suite_sum)
{
    MU_RUN_TEST(test_sum10_zero);
    MU_RUN_TEST(test_sum10_pospos);
    MU_RUN_TEST(test_sum10_negpos);
}

MU_TEST(test_sub10_zero)
{
    BigInt *zero = strtobi("0");
    BigInt *x = strtobi("123");
    BigInt *xneg = strtobi("-123");
    BigInt *diff_pos = bigint_subtract(x, zero);
    BigInt *diff_neg = bigint_subtract(xneg, zero);

    mu_check(bigint_iseq(x, diff_pos));
    mu_check(bigint_iseq(xneg, diff_neg));

    bigint_free(zero);
    bigint_free(x);
    bigint_free(xneg);
    bigint_free(diff_pos);
    bigint_free(diff_neg);
}

MU_TEST(test_sub10_pospos)
{
    BigInt *a = strtobi("341");
    BigInt *b = strtobi("932");
    BigInt *actual = strtobi("-591");
    BigInt *diff = bigint_subtract(a, b);

    mu_check(bigint_iseq(actual, diff));

    bigint_free(a);
    bigint_free(b);
    bigint_free(actual);
    bigint_free(diff);
}

MU_TEST_SUITE(suite_sub)
{
    MU_RUN_TEST(test_sub10_zero);
    MU_RUN_TEST(test_sub10_pospos);
}

MU_TEST(test_mul10_zero)
{
    BigInt *zero = strtobi("0");
    BigInt *x = strtobi("123");
    BigInt *xneg = strtobi("-123");
    BigInt *mul_left = bigint_multiply(zero, x);
    BigInt *mul_right = bigint_multiply(x, zero);
    BigInt *mul_neg = bigint_multiply(xneg, zero);

    mu_check(bigint_iseq(zero, mul_left));
    mu_check(bigint_iseq(zero, mul_right));
    mu_check(bigint_iseq(zero, mul_neg));

    bigint_free(zero);
    bigint_free(x);
    bigint_free(xneg);
    bigint_free(mul_left);
    bigint_free(mul_right);
    bigint_free(mul_neg);
}

MU_TEST(test_mul10_one)
{
    BigInt *one = strtobi("1");
    BigInt *x = strtobi("123");
    BigInt *xneg = strtobi("-123");
    BigInt *mul_left = bigint_multiply(one, x);
    BigInt *mul_right = bigint_multiply(x, one);
    BigInt *mul_neg = bigint_multiply(xneg, one);

    mu_check(bigint_iseq(x, mul_left));
    mu_check(bigint_iseq(x, mul_right));
    mu_check(bigint_iseq(xneg, mul_neg));

    bigint_free(one);
    bigint_free(x);
    bigint_free(xneg);
    bigint_free(mul_left);
    bigint_free(mul_right);
    bigint_free(mul_neg);
}

MU_TEST(test_mul10_negpos)
{
    BigInt *a = strtobi("-1109");
    BigInt *b = strtobi("29480");
    BigInt *actual = strtobi("-32693320");
    BigInt *mul = bigint_multiply(a, b);

    mu_check(bigint_iseq(actual, mul));

    bigint_free(a);
    bigint_free(b);
    bigint_free(actual);
    bigint_free(mul);
}

MU_TEST(test_mul10_factorial)
{
    BigInt *x = strtobi("100");
    BigInt *factorial = bigint_factorial(x);
    BigInt *actual = strtobi("93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000");

    mu_check(bigint_iseq(actual, factorial));

    bigint_free(x);
    bigint_free(factorial);
    bigint_free(actual);
}

MU_TEST(test_mul10_factorialzero)
{
    BigInt *zero = strtobi("0");
    BigInt *factorial = bigint_factorial(zero);
    BigInt *one = strtobi("1");

    mu_check(bigint_iseq(one, factorial));

    bigint_free(zero);
    bigint_free(factorial);
    bigint_free(one);
}

MU_TEST(test_mul10_factorialneg)
{
    BigInt *neg = strtobi("-124");
    BigInt *factorial = bigint_factorial(neg);

    mu_check(factorial == NULL);

    bigint_free(neg);
}

MU_TEST_SUITE(suite_mul)
{
    MU_RUN_TEST(test_mul10_zero);
    MU_RUN_TEST(test_mul10_one);
    MU_RUN_TEST(test_mul10_negpos);
    MU_RUN_TEST(test_mul10_factorial);
    MU_RUN_TEST(test_mul10_factorialzero);
    MU_RUN_TEST(test_mul10_factorialneg);
}

MU_TEST(test_add10)
{
    BigInt *a = strtobi("40");
    BigInt *b = strtobi("234");
    BigInt *actual = strtobi("274");

    bigint_add(&a, b);
    mu_check(bigint_iseq(a, actual));

    bigint_free(a);
    bigint_free(b);
    bigint_free(actual);
}

MU_TEST(test_sub10)
{
    BigInt *a = strtobi("40");
    BigInt *b = strtobi("234");
    BigInt *actual = strtobi("-194");

    bigint_sub(&a, b);
    mu_check(bigint_iseq(a, actual));

    bigint_free(a);
    bigint_free(b);
    bigint_free(actual);
}

MU_TEST(test_mul10)
{
    BigInt *a = strtobi("40");
    BigInt *b = strtobi("234");
    BigInt *actual = strtobi("9360");

    bigint_mul(&a, b);
    mu_check(bigint_iseq(a, actual));

    bigint_free(a);
    bigint_free(b);
    bigint_free(actual);
}

MU_TEST(test_postobi)
{
    BigInt *x = postobi(49);
    BigInt *actual = strtobi("49");

    mu_check(bigint_iseq(actual, x));

    bigint_free(x);
    bigint_free(actual);
}

MU_TEST(test_adddigit)
{
    BigInt *x = strtobi("9999999");
    uint8_t digit = 9;
    BigInt *actual = strtobi("10000008");

    bigint_adddigit(&x, digit);
    mu_check(bigint_iseq(actual, x));

    bigint_free(x);
    bigint_free(actual);
}

MU_TEST_SUITE(suite_short)
{
    MU_RUN_TEST(test_add10);
    MU_RUN_TEST(test_sub10);
    MU_RUN_TEST(test_mul10);
    MU_RUN_TEST(test_postobi);
    MU_RUN_TEST(test_adddigit);
}

MU_TEST(test_div10_self)
{
    BigInt *a = strtobi("5");
    BigInt *b = strtobi("5");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("1");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST(test_div10_pospos)
{
    BigInt *a = strtobi("126");
    BigInt *b = strtobi("3");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("42");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST(test_div10_remain)
{
    BigInt *a = strtobi("127");
    BigInt *b = strtobi("32");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("3");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST(test_div10_negpos)
{
    BigInt *a = strtobi("-12");
    BigInt *b = strtobi("6");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("-2");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST(test_div10_negpos_remain)
{
    BigInt *a = strtobi("-127");
    BigInt *b = strtobi("61");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("-3");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST(test_div10_posneg_remain)
{
    BigInt *a = strtobi("127");
    BigInt *b = strtobi("-61");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("-2");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST(test_div10_negneg_remain)
{
    BigInt *a = strtobi("-127");
    BigInt *b = strtobi("-61");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("3");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST(test_div10_negneg)
{
    BigInt *a = strtobi("-952");
    BigInt *b = strtobi("-17");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("56");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST(test_div10_posneg)
{
    BigInt *a = strtobi("952");
    BigInt *b = strtobi("-17");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("-56");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST(test_div10_pospos_reswith0)
{
    BigInt *a = strtobi("24781");
    BigInt *b = strtobi("234");
    BigInt *q = bigint_divide(a, b);
    BigInt *actual = strtobi("105");

    mu_check(bigint_iseq(actual, q));

    bigint_free(a);
    bigint_free(b);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST_SUITE(suite_div)
{
    MU_RUN_TEST(test_div10_self);
    MU_RUN_TEST(test_div10_pospos);
    MU_RUN_TEST(test_div10_remain);
    MU_RUN_TEST(test_div10_negpos);
    MU_RUN_TEST(test_div10_negpos_remain);
    MU_RUN_TEST(test_div10_posneg_remain);
    MU_RUN_TEST(test_div10_negneg_remain);
    MU_RUN_TEST(test_div10_negneg);
    MU_RUN_TEST(test_div10_posneg);
    MU_RUN_TEST(test_div10_pospos_reswith0);
}

MU_TEST(test_mod_pospos)
{
    BigInt *a = strtobi("1242");
    BigInt *b = strtobi("54");
    BigInt *r = bigint_mod(a, b);

    mu_check(bigint_iszero(r));

    bigint_free(a);
    bigint_free(b);
    bigint_free(r);
}

MU_TEST(test_mod_pospos_remain)
{
    BigInt *a = strtobi("11471");
    BigInt *b = strtobi("473");
    BigInt *r = bigint_mod(a, b);
    BigInt *actual = strtobi("119");

    mu_check(bigint_iseq(actual, r));

    bigint_free(a);
    bigint_free(b);
    bigint_free(r);
    bigint_free(actual);
}

MU_TEST(test_mod_negpos_remain)
{
    BigInt *a = strtobi("-24781");
    BigInt *b = strtobi("234");
    BigInt *q = bigint_divide(a, b);
    BigInt *r = bigint_mod(a, b);
    BigInt *actual = strtobi("23");

    mu_check(bigint_iseq(actual, r));

    bigint_free(a);
    bigint_free(b);
    bigint_free(r);
    bigint_free(q);
    bigint_free(actual);
}

MU_TEST_SUITE(suite_mod)
{
    MU_RUN_TEST(test_mod_pospos);
    MU_RUN_TEST(test_mod_pospos_remain);
    MU_RUN_TEST(test_mod_negpos_remain);
}

MU_TEST(test_gcd_pospos)
{
    BigInt *a = strtobi("91672");
    BigInt *b = strtobi("7504");
    BigInt *gcd = bigint_gcd(a, b);
    BigInt *actual = strtobi("56");

    mu_check(bigint_iseq(actual, gcd));

    bigint_free(a);
    bigint_free(b);
    bigint_free(gcd);
    bigint_free(actual);
}

MU_TEST(test_gcd_negneg)
{
    BigInt *a = strtobi("-7504");
    BigInt *b = strtobi("-1341942");
    BigInt *gcd = bigint_gcd(a, b);
    BigInt *actual = strtobi("14");

    mu_check(bigint_iseq(actual, gcd));

    bigint_free(a);
    bigint_free(b);
    bigint_free(gcd);
    bigint_free(actual);
}

MU_TEST(test_gcd_zero)
{
    BigInt *a = strtobi("0");
    BigInt *b = strtobi("123497831145451");
    BigInt *gcd = bigint_gcd(a, b);

    mu_check(bigint_iseq(b, gcd));

    bigint_free(a);
    bigint_free(b);
    bigint_free(gcd);
}

MU_TEST(test_gcd_self)
{
    BigInt *a = strtobi("1985710561205715");
    BigInt *gcd = bigint_gcd(a, a);

    mu_check(bigint_iseq(a, gcd));

    bigint_free(a);
    bigint_free(gcd);
}

MU_TEST_SUITE(suite_gcd)
{
    MU_RUN_TEST(test_gcd_pospos);
    MU_RUN_TEST(test_gcd_negneg);
    MU_RUN_TEST(test_gcd_zero);
    MU_RUN_TEST(test_gcd_self);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(suite_str);
    MU_RUN_SUITE(suite_isless);
    MU_RUN_SUITE(suite_iszero);
    MU_RUN_SUITE(suite_iseq);
    MU_RUN_SUITE(suite_sum);
    MU_RUN_SUITE(suite_sub);
    MU_RUN_SUITE(suite_mul);
    MU_RUN_SUITE(suite_short);
    MU_RUN_SUITE(suite_div);
    MU_RUN_SUITE(suite_mod);
    MU_RUN_SUITE(suite_gcd);

    MU_REPORT();

    return MU_EXIT_CODE;
}