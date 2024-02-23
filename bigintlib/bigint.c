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

    for (size_t i = 0; i < len - offset; i++)
    {
        x->digits[i] = str[len - i - 1] - '0';
    }
    
    return x;
}

char *bitostr(const BigInt *x)
{
    int offset = 0;
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