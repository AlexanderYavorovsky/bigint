/* Yavorovsky Alexander, 22.02.2024 */

#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
    uint8_t *digits;
    int8_t sign;
    size_t len;
} BigInt;

/* free memory allocated for x */
bool bigint_free(BigInt *x);

/* returns copy of x */
BigInt *bigint_copy(const BigInt *x);

/* fill x->digits with zeroes from position beg to position n*/
bool bigint_fillzero(BigInt *x, size_t beg, size_t n);

/* returns 1 if a == 0 */
bool bigint_iszero(const BigInt *x);
/* returns 1 if a < b, 0 otherwise */
bool bigint_isless(const BigInt *a, const BigInt *b);
/* returns 1 if a == b */
bool bigint_iseq(const BigInt *a, const BigInt *b);

/* make BigInt from string */
BigInt *strtobi(char *str);
/* make string from BigInt */
char *bitostr(const BigInt *x);
/* make BigInt from small positive integer */
BigInt *postobi(uint8_t n);

/* a + b */
BigInt *bigint_sum(const BigInt *a, const BigInt *b, uint8_t base);
/* a - b */
BigInt *bigint_subtract(const BigInt *a, const BigInt *b, uint8_t base);
/* a * b */
BigInt *bigint_multiply(const BigInt *a, const BigInt *b, uint8_t base);
/* a! */
BigInt *bigint_factorial(const BigInt *a, uint8_t base);
/* a / b */
BigInt *bigint_divide(const BigInt *a, const BigInt *b, uint8_t base);
/* a % b */
BigInt *bigint_mod(const BigInt *a, const BigInt *b, uint8_t base);

/* a += b */
bool bigint_add(BigInt **a, const BigInt *b, uint8_t base);
/* a -= b */
bool bigint_sub(BigInt **a, const BigInt *b, uint8_t base);
/* a *= b */
bool bigint_mul(BigInt **a, const BigInt *b, uint8_t base);
/* a += digit */
bool bigint_adddigit(BigInt **a, uint8_t digit, uint8_t base);
/* a*= digit */
bool bigint_muldigit(BigInt **a, uint8_t digit, uint8_t base);

/* a, b = b, a */
bool bigint_swap(BigInt **a, BigInt **b);

/* returns the greates common divisor of a and b with base = 10 */
BigInt *bigint_gcd(const BigInt *a, const BigInt *b);