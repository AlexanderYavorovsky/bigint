/* Yavorovsky Alexander, 22.02.2024 */

#include <stdlib.h>

#include "bigint.h"

BigInt *bigint_init(void)
{
    BigInt *res = malloc(sizeof(BigInt));

    if (res == NULL)
        return NULL;

    /* digits? sign? */

    return res;
}