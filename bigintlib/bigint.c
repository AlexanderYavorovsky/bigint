/* Yavorovsky Alexander, 10.03.2024 */

#include <stdlib.h>
#include <string.h>

#include "bigint.h"

void bigint_free(BigInt *x)
{
    if (x->digits != NULL)
        free(x->digits);
    free(x);
}

static BigInt *bigint_init_n(size_t len)
{
    BigInt *x = malloc(sizeof(*x));
    if(x == NULL)
        return NULL;
    
    x->digits = calloc(len, sizeof(x->digits));
    if (x->digits == NULL)
    {
        free(x);
        return NULL;
    }

    x->len = len;
    x->sign = 1;

    return x;
}

BigInt *bigint_copy(const BigInt *x)
{
    BigInt *copy;

    copy = bigint_init_n(x->len);
    if (copy == NULL)
        return NULL;

    copy->sign = x->sign;
    memcpy(copy->digits, x->digits, x->len);

    return copy;
}

uint8_t bigint_fillzero(BigInt *x, size_t beg, size_t n)
{
    if (x == NULL || beg + n > x->len)
        return 0;

    if (x->digits == NULL)
    {
        x->digits = malloc(x->len * sizeof(x->digits));
        if (x->digits == NULL)
            return 0;
    }

    for (size_t i = beg; i < beg + n; i++)
        x->digits[i] = 0;

    return 1;
}

static uint8_t bigint_resize(BigInt *x, size_t newlen)
{
    uint8_t *tmp;
    size_t oldlen;

    if (newlen == 0 || x == NULL)
        return 0;

    tmp = realloc(x->digits, newlen);
    if (tmp == NULL)
        return 0;

    x->digits = tmp;

    oldlen = x->len;
    x->len = newlen;

    if (newlen > oldlen)
        return bigint_fillzero(x, oldlen, newlen - oldlen);

    return 1;
}

static uint8_t bigint_normalize(BigInt *x)
{
    size_t cnt, newlen;

    if (x == NULL)
        return 0;

    cnt = x->len - 1;
    while (cnt && x->digits[cnt] == 0)
        cnt--;

    newlen = cnt + 1;
    bigint_resize(x, newlen);

    if (x->len == 1 && x->digits[0] == 0)
        x->sign = 1;

    return 1;
}

uint8_t bigint_iszero(const BigInt *x)
{
    return x->len == 1 && x->digits[0] == 0;
}

uint8_t bigint_isless(const BigInt *a, const BigInt *b)
{
    if (a->sign != b->sign)
        return a->sign != 1;

    if (a->len < b->len)
        return a->sign == 1;
    if (a->len > b->len)
        return a->sign != 1;

    for (size_t i = 0; i < a->len; i++)
    {
        if (a->digits[a->len - i - 1] < b->digits[b->len - i - 1])
            return a->sign == 1;
            
        if (a->digits[a->len - i - 1] > b->digits[b->len - i - 1])
            return a->sign != 1;
    }
    
    return 0;
}

uint8_t bigint_iseq(const BigInt *a, const BigInt *b)
{
    if (a->sign != b->sign || a->len != b->len)
        return 0;

    return !memcmp(a->digits, b->digits, a->len * sizeof(*a->digits));
}

BigInt *strtobi(char *str)
{
    BigInt *x;
    size_t len = strlen(str);
    int8_t sign = 1;
    uint8_t offset = 0;

    if (str[0] == '-')
    {
        sign = -1;
        offset = 1;
    }
    else if (str[0] == '+')
        offset = 1;

    x = bigint_init_n(len - offset);
    if (x == NULL)
        return NULL;

    x->sign = sign;

    for (size_t i = 0; i < len - offset; i++)
        x->digits[i] = str[len - i - 1] - '0';

    bigint_normalize(x);
    
    return x;
}

char *bitostr(const BigInt *x)
{
    uint8_t offset = 0;
    size_t slen = x->len + 1 + (x->sign == -1);
    char *str = malloc(slen * sizeof(char));

    if (str == NULL)
        return NULL;

    if (x->sign == -1)
    {
        str[0] = '-';
        offset = 1;
    }

    for (size_t i = 0; i < x->len; i++)
        str[i + offset] = x->digits[x->len - i - 1] + '0';

    str[slen - 1] = '\0'; 

    return str;
}

static size_t bigint_poslen(uint8_t n)
{
    size_t len = 0;

    while (n > 0)
    {
        n /= 10;
        len++;
    }

    return len;
}

BigInt *postobi(uint8_t n)
{
    BigInt *x;
    size_t len = bigint_poslen(n);
    size_t cnt = 0;

    x = bigint_init_n(len);
    if (x == NULL)
        return NULL;

    while (n > 0)
    {
        x->digits[cnt++] = n % 10;
        n /= 10;
    }

    return x;
}

BigInt *bigint_sum(const BigInt *a, const BigInt *b)
{
    BigInt *x;
    uint8_t sum = 0, carry = 0;

    if (bigint_isless(a, b))
        return bigint_sum(b, a);
    if (b->sign != 1)
    {
        BigInt *bpos = bigint_copy(b);
        if (bpos == NULL)
            return NULL;

        bpos->sign = 1;
        x = bigint_subtract(a, bpos);
        bigint_free(bpos);

        return x;
    }

    x = bigint_init_n(a->len + 1);
    if (x == NULL)
        return NULL;

    for (size_t i = 0; i < a->len; i++)
    {
        sum = a->digits[i] + carry;
        if (i < b->len)
            sum += b->digits[i];

        carry = sum / BASE;
        x->digits[i] = sum % BASE;
    }

    x->digits[a->len] = carry;
    bigint_normalize(x);

    return x;
}

BigInt *bigint_subtract(const BigInt *a, const BigInt *b)
{
    BigInt *x;
    int8_t diff = 0, borrow = 0;

    if (bigint_isless(a, b))
    {
        x = bigint_subtract(b, a);
        x->sign *= -1;

        return x;
    }
    if (b->sign != 1)
    {
        BigInt *bpos = bigint_copy(b);
        if (bpos == NULL)
            return NULL;

        bpos->sign = 1;
        x = bigint_sum(a, bpos);
        bigint_free(bpos);

        return x;
    }

    x = bigint_init_n(a->len);
    if (x == NULL)
        return NULL;

    for (size_t i = 0; i < a->len; i++)
    {
        diff = a->digits[i] + borrow;
        if (i < b->len)
            diff -= b->digits[i];
        
        if (diff < 0)
        {
            borrow = -1;
            if (i != a->len - 1)
                diff += BASE;
        }
        else
            borrow = 0;

        x->digits[i] = diff;
    }

    bigint_normalize(x);

    return x;
}

BigInt *bigint_multiply(const BigInt *a, const BigInt *b)
{
    BigInt *x;
    uint8_t prod = 0, carry;

    if (bigint_iszero(a) || bigint_iszero(b))
    {
        x = strtobi("0");
        return x;
    }

    x = bigint_init_n(a->len + b->len + 1);
    if (x == NULL)
        return NULL;

    for (size_t i = 0; i < b->len; i++)
    {
        carry = 0;
        for (size_t j = 0; j < a->len; j++)
        {
            prod = a->digits[j] * b->digits[i] + x->digits[i + j] + carry;
            carry = prod / BASE;
            x->digits[i + j] = prod % BASE;
        }
        x->digits[i + a->len] = carry;
    }

    x->sign = a->sign * b->sign;
    bigint_normalize(x);

    return x;
}

BigInt *bigint_factorial(const BigInt *a)
{
    BigInt *res, *mul, *one, *top_bound;

    if (a->sign != 1)
        return NULL;

    res = postobi(1);
    mul = postobi(2);
    one = postobi(1);
    top_bound = bigint_sum(a, one);

    if (res == NULL || mul == NULL || one == NULL || top_bound == NULL)
    {
        bigint_free(res), bigint_free(mul), bigint_free(one), bigint_free(top_bound);
        return NULL;
    }

    while (bigint_isless(mul, top_bound))
    {
        bigint_mul(&res, mul);
        bigint_adddigit(&mul, 1);

        if (res == NULL || mul == NULL)
        {
            bigint_free(res), bigint_free(mul), bigint_free(one), bigint_free(top_bound);
            return NULL;
        }
    }

    bigint_free(mul);
    bigint_free(one);
    bigint_free(top_bound);

    return res;
}

void bigint_add(BigInt **a, const BigInt *b)
{
    BigInt *old;

    old = *a;
    *a = bigint_sum(*a, b);
    bigint_free(old);
}

void bigint_sub(BigInt **a, const BigInt *b)
{
    BigInt *old;

    old = *a;
    *a = bigint_subtract(*a, b);
    bigint_free(old);
}

void bigint_mul(BigInt **a, const BigInt *b)
{
    BigInt *old;

    old = *a;
    *a = bigint_multiply(*a, b);
    bigint_free(old);
}

uint8_t bigint_adddigit(BigInt **a, uint8_t digit)
{
    uint8_t sum, carry = 0;
    size_t len = (*a)->len;

    for (size_t i = 0; i < len; i++)
    {
        sum = (*a)->digits[i] + carry;
        if (i == 0)
            sum += digit;

        carry = sum / BASE;
        (*a)->digits[i] = sum % BASE;
        if (carry == 0)
            break;
    }

    if (carry > 0 && bigint_resize(*a, len + 1))
            (*a)->digits[len] = carry;

    return 1;
}

static void bigint_addoffset(BigInt **x, size_t offset)
{
    BigInt *oldx;

    if (*x == NULL || offset == 0)
        return;

    oldx = *x;
    *x = bigint_init_n(oldx->len + offset);
    if (*x == NULL)
        return;

    memcpy((*x)->digits + offset, oldx->digits, oldx->len);
    bigint_normalize(*x);

    bigint_free(oldx);
}

BigInt *bigint_divide(const BigInt *a, const BigInt *b)
{
    BigInt *q, *cur, *bpos;
    size_t cnt;
    uint8_t endflag = 0, was_division = 0;

    if (bigint_iszero(b))
        return NULL;

    bpos = bigint_copy(b);
    bpos->sign = 1;

    cnt = a->len - 1;
    q = strtobi("0");
    cur = strtobi("0");

    while (!endflag)
    {
        uint8_t divcnt = 0;

        while (!endflag && bigint_isless(cur, bpos))
        {
            uint8_t digit;

            if (cnt == 0)
                endflag = 1;

            digit = a->digits[cnt--];
            bigint_addoffset(&cur, 1);
            bigint_adddigit(&cur, digit);

            if (was_division)
                bigint_addoffset(&q, 1);
        }

        while (bigint_isless(bpos, cur) || bigint_iseq(bpos, cur))
        {
            bigint_sub(&cur, bpos);
            divcnt++;
        }

        was_division = 1;
        bigint_adddigit(&q, divcnt);
    }

    if (a->sign != 1 && !bigint_iszero(cur))
        bigint_adddigit(&q, 1);

    q->sign = a->sign * b->sign;

    bigint_free(cur);
    bigint_free(bpos);

    return q;
}

BigInt *bigint_mod(const BigInt *a, const BigInt *b)
{
    BigInt *r, *q, *bq;

    if (bigint_iszero(b))
        return NULL;

    q = bigint_divide(a, b);
    if (q == NULL)
        return NULL;

    bq = bigint_multiply(b, q);
    if (bq == NULL)
    {
        bigint_free(q);
        return NULL;
    }

    r = bigint_subtract(a, bq);

    bigint_free(q);
    bigint_free(bq);

    return r;
}

void bigint_swap(BigInt **a, BigInt **b)
{
    BigInt *tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

BigInt *bigint_gcd(const BigInt *a, const BigInt *b)
{
    BigInt *apos, *bpos;

    if (bigint_iszero(a) && bigint_iszero(b))
        return NULL;

    /* gcd(a, b) = gcd(-a, -b) = gcd(a, -b) = gcd(-a, b) */
    apos = bigint_copy(a);
    apos->sign = 1;
    bpos = bigint_copy(b);
    bpos->sign = 1;

    /* assume a >= b */
    if (bigint_isless(apos, bpos))
        bigint_swap(&apos, &bpos);

    while (!bigint_iszero(bpos))
    {
        BigInt *old_apos = apos;

        apos = bpos;
        bpos = bigint_mod(old_apos, bpos);

        bigint_free(old_apos);
    }

    bigint_free(bpos);

    return apos;
}