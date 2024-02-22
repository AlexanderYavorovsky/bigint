/* Yavorovsky Alexander, 22.02.2024 */

#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    char *digits;
    int8_t sign;
    size_t len;
} BigInt;

BigInt *bigint_init(void);
bool bigint_free(BigInt *x);
void bigint_fillzero(BigInt *x, size_t n);
void bigint_normalize(BigInt *x);

bool bigint_isless(BigInt a, BigInt b);

BigInt *strtobi(char *num);
char *bitostr(const BigInt *x);

BigInt *bigint_sum(const BigInt *a, const BigInt *b);
BigInt *bigint_subtract(const BigInt *a, const BigInt *b);
BigInt *_multiply_digit(const BigInt *a, uint8_t digit, uint8_t base, size_t offset);
BigInt *bigint_multiply(const BigInt *a, const BigInt *b);
BigInt *bigint_divide(const BigInt *a, const BigInt *b);
BigInt *bigint_mod(const BigInt *a, const BigInt *b);