/* Yavorovsky Alexander, 23.02.2024 */

#include <stdio.h>

#include "bigint.h"

int main(int argc, char *argv[])
{
    BigInt *x;
    printf("test\n");

    x = strtobi("123");

    printf("Sign: %i; Len: %li\n", x->sign, x->len);
    for (size_t i = 0; i < x->len; i++)
    {
        printf("%i", x->digits[x->len - i - 1]);
    }

    bigint_free(x);

    return 0;
}