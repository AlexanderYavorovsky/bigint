/* Yavorovsky Alexander, 22.02.2024 */

#include <stdlib.h>
#include <string.h>

#include "bigint.h"

bool bigint_free(BigInt *x)
{
    if (x != NULL)
    {
        if (x->digits != NULL)
        {
            free(x->digits);
        }
        free(x);
    }
    
    return 0;
}

BigInt *bigint_init(void)
{
    BigInt *x = malloc(sizeof(BigInt));

    if (x == NULL)
        return NULL;

    return x;
}

BigInt *bigint_init_n(size_t len)
{
    BigInt *x = bigint_init();
    if(x == NULL)
        return NULL;
    
    x->digits = malloc(len);
    if (x->digits == NULL)
    {
        free(x);
        return NULL;
    }

    x->len = len;
    x->sign = 1;

    return x;
}

bool bigint_fillzero(BigInt *x, size_t beg, size_t n)
{
    if (x == NULL || beg + n > x->len)
        return 1;

    if (x->digits == NULL)
    {
        x->digits = malloc(x->len);
        if (x->digits == NULL)
            return 1;
    }

    for (size_t i = beg; i < beg + n; i++)
    {
        x->digits[i] = 0;
    }

    return 0;
}

bool bigint_resize(BigInt *x, size_t newlen)
{
    uint8_t *tmp;
    size_t oldlen;

    if (newlen <= 0 || x == NULL || x->digits == NULL)
        return 1;

    tmp = realloc(x->digits, newlen);
    if (tmp == NULL)
        return 1;

    x->digits = tmp;

    oldlen = x->len;
    x->len = newlen;

    if (newlen > oldlen)
        return bigint_fillzero(x, oldlen, newlen - oldlen);

    return 0;
}


bool bigint_normalize(BigInt *x)
{
    size_t cnt, newlen;

    if (x == NULL)
        return 1;

    cnt = x->len - 1;
    while (cnt && x->digits[cnt] == 0)
    {
        cnt--;
    }

    newlen = cnt + 1;

    return bigint_resize(x, newlen);
}

BigInt *bigint_copy(const BigInt *x)
{
    BigInt *copy;

    if (x == NULL || x->len <= 0)
        return NULL;

    copy = bigint_init_n(x->len);
    if (copy == NULL)
        return NULL;

    copy->sign = x->sign;

    for (size_t i = 0; i < x->len; i++)
    {
        copy->digits[i] = x->digits[i];
    }

    return copy;
}

bool bigint_isless(const BigInt *a, const BigInt *b)
{
    if (a == NULL || b == NULL)
        return 0;

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

bool bigint_iszero(const BigInt *x)
{
    if (x == NULL || x->digits == NULL)
        return 0;

    for (size_t i = 0; i < x->len; i++)
    {
        if (x->digits[i] != 0)
            return 0;
    }

    return 1;
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
    {
        offset = 1;
    }

    x = bigint_init_n(len - offset);
    if (x == NULL)
        return NULL;

    x->sign = sign;

    for (size_t i = 0; i < len - offset; i++)
    {
        x->digits[i] = str[len - i - 1] - '0';
    }
    
    return x;
}

char *bitostr(const BigInt *x)
{
    uint8_t offset = 0;
    size_t slen = x->len + 1 + (x->sign == -1);
    char *str = malloc(sizeof(char) * slen);

    if (str == NULL)
        return NULL;

    if (x->sign == -1)
    {
        str[0] = '-';
        offset = 1;
    }

    for (size_t i = 0; i < x->len; i++)
    {
        str[i + offset] = x->digits[x->len - i - 1] + '0';
    }

    str[slen - 1] = '\0'; 

    return str;
}

/* restriction: 2 <= base <= 10 */
BigInt *bigint_sum(const BigInt *a, const BigInt *b, uint8_t base)
{
    BigInt *x;
    uint8_t sum = 0;
    uint8_t carry = 0;

    if (a == NULL || b == NULL)
        return NULL;
    
    if (a->digits == NULL || b->digits == NULL)
        return NULL;
    
    if (base < 2 || base > 10)
        return NULL;

    if (bigint_isless(a, b))
    {
        return bigint_sum(b, a, base);
    }
    if (b->sign != 1)
    {
        BigInt *tmp = bigint_copy(b);
        if (tmp == NULL)
            return NULL;

        tmp->sign = 1;

        x = bigint_subtract(a, tmp, base);
        bigint_free(tmp);

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
        
        if (sum >= base)
        {
            carry = sum / base;
            sum %= base;
        }
        else
        {
            carry = 0;
        }

        x->digits[i] = sum;
    }

    if (carry > 0)
    {
        x->digits[x->len - 1] = carry;
    }
    else
    {
        bigint_resize(x, x->len - 1);
    }

    return x;
}

/* 2 <= base <= 10 */
BigInt *bigint_subtract(const BigInt *a, const BigInt *b, uint8_t base)
{
    BigInt *x;
    int8_t diff = 0;
    int8_t borrow = 0;

    if (a == NULL || b == NULL)
        return NULL;
    
    if (a->digits == NULL || b->digits == NULL)
        return NULL;
    
    if (base < 2 || base > 10)
        return NULL;

    if (bigint_isless(a, b))
    {
        x = bigint_subtract(b, a, base);
        x->sign *= -1;

        return x;
    }
    if (b->sign != 1)
    {
        BigInt *tmp = bigint_copy(b);
        if (tmp == NULL)
            return NULL;

        tmp->sign = 1;

        x = bigint_sum(a, tmp, base);
        bigint_free(tmp);

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
                diff += base;
        }
        else
        {
            borrow = 0;
        }

        x->digits[i] = diff;
    }

    bigint_normalize(x);

    return x;
}

BigInt *_multiply_digit(const BigInt *a, uint8_t digit, uint8_t base, size_t offset)
{
    BigInt *x;
    uint8_t prod = 0;
    uint8_t carry = 0;

    if (a == NULL || a->digits == NULL || digit > 9)
        return NULL;

    if (base < 2 || base > 10)
        return NULL;

    x = bigint_init_n(a->len + 1 + offset);
    if (x == NULL)
        return NULL;

    bigint_fillzero(x, 0, offset);

    for (size_t i = 0; i < a->len; i++)
    {
        prod = a->digits[i] * digit + carry;
        if (prod >= base)
        {
            carry = prod / base;
            prod %= base;
        }
        else
        {
            carry = 0;
        }

        x->digits[i + offset] = prod;
    }

    if (carry > 0)
    {
        x->digits[x->len - 1] = carry;
    }
    else
    {
        bigint_resize(x, x->len - 1);
    }

    return x;
}

BigInt *bigint_multiply(const BigInt *a, const BigInt *b, uint8_t base)
{
    BigInt *x;

    if (a == NULL || b == NULL)
        return NULL;

    if (a->digits == NULL || b->digits == NULL)
        return NULL;

    if (base < 2 || base > 10)
        return NULL;

    if (bigint_isless(a, b))
    {
        return bigint_multiply(b, a, base);
    }
    if (b->sign != 1)
    {
        BigInt *tmp = bigint_copy(b);
        if (tmp == NULL)
            return NULL;

        tmp->sign = 1;
        x = bigint_multiply(a, tmp, base);

        if (!bigint_iszero(a))
            x->sign *= -1;

        bigint_free(tmp);

        return x;
    }

    x = bigint_init_n(b->len + 1);
    if (x == NULL)
        return NULL;

    bigint_fillzero(x, 0, x->len);

    for (size_t i = 0; i < b->len; i++)
    {
        if (b->digits[i] == 0)
            continue;

        BigInt *prod = _multiply_digit(a, b->digits[i], base, i);
        BigInt *tmp = x;

        x = bigint_sum(x, prod, base);
        bigint_free(tmp);
        bigint_free(prod);
    }

    bigint_normalize(x);

    return x;
}