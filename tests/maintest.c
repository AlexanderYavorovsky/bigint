/* Yavorovsky Alexander, 23.02.2024 */

#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main(int argc, char *argv[])
{
    BigInt *x, *y, *r;
    char *sx, *sy, *sr;
    uint8_t base = 10;
    printf("test\n");

    x = strtobi("-102");
    sx = bitostr(x);
    y = strtobi("-19");
    sy = bitostr(y);

    r = bigint_multiply(x, y, base);
    sr = bitostr(r);

    printf("%s * %s = %s\n", sx, sy, sr);

    bigint_free(x);
    bigint_free(y);
    bigint_free(r);
    free(sx);
    free(sy);
    free(sr);

    return 0;
}