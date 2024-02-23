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
    char *str = malloc(sizeof(char) * (x->len + (x->sign == -1)));

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

    return str;
}