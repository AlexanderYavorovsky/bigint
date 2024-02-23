/* Yavorovsky Alexander, 23.02.2024 */

#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main(int argc, char *argv[])
{
    BigInt *x;
    char *str;
    printf("test\n");

    x = strtobi("-123");
    str = bitostr(x);

    printf("Sign: %i; Len: %li\n", x->sign, x->len);
    for (size_t i = 0; i < x->len; i++)
    {
        printf("%i", x->digits[x->len - i - 1]);
    }

    printf("\n\n%s\n", str);

    bigint_fillzero(x, 0, 2);
    str = bitostr(x);
    printf("\n\n%s\n", str);

    bigint_resize(x, 5);
    printf("\nlen: %li\n%s\n", x->len, bitostr(x));

    if (!bigint_normalize(x))
        printf("\nlen: %li\n%s\n", x->len, bitostr(x));

    bigint_free(x);
    free(str);

    return 0;
}