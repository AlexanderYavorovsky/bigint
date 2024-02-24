/* Yavorovsky Alexander, 22.02.2024 */

#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    uint8_t *digits;
    int8_t sign;
    size_t len;
} BigInt;

bool bigint_free(BigInt *x);
BigInt *bigint_init(void);
BigInt *bigint_init_n(size_t len);
bool bigint_fillzero(BigInt *x, size_t beg, size_t n);
bool bigint_resize(BigInt *x, size_t newlen);
bool bigint_normalize(BigInt *x);
BigInt *bigint_copy(const BigInt *x);

bool bigint_isless(const BigInt *a, const BigInt *b);

BigInt *strtobi(char *str);
char *bitostr(const BigInt *x);

BigInt *bigint_sum(const BigInt *a, const BigInt *b, uint8_t base);
BigInt *bigint_subtract(const BigInt *a, const BigInt *b, uint8_t base);
BigInt *_multiply_digit(const BigInt *a, uint8_t digit, uint8_t base, size_t offset);
BigInt *bigint_multiply(const BigInt *a, const BigInt *b);
BigInt *bigint_divide(const BigInt *a, const BigInt *b);
BigInt *bigint_mod(const BigInt *a, const BigInt *b);