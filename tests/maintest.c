/* Yavorovsky Alexander, 23.02.2024 */

#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main(int argc, char *argv[])
{
    BigInt *x, *y;
    char *str;
    printf("test\n");

    x = strtobi("3214");
    str = bitostr(x);
    y = strtobi("-13");

    printf("Is less: %i\n", bigint_isless(x, y));

    printf("Sign: %i; Len: %li\n", x->sign, x->len);
    for (size_t i = 0; i < x->len; i++)
    {
        printf("%i", x->digits[x->len - i - 1]);
    }

    printf("\n\n%s\n", str);

    bigint_fillzero(x, 0, 2);
    free(str);
    str = bitostr(x);
    printf("\n\n%s\n", str);

    bigint_resize(x, 3);
    free(str);
    str = bitostr(x);
    printf("\nlen: %li\n%s\n", x->len, str);

    if (!bigint_normalize(x))
    {
        free(str);
        str = bitostr(x);
        printf("\nlen: %li\n%s\n", x->len, str);
    }

    bigint_free(x);
    bigint_free(y);
    free(str);

    return 0;
}