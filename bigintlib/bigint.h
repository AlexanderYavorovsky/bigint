/* Yavorovsky Alexander, 10.03.2024 */

#include <stdint.h>

#define BASE 10

typedef struct 
{
    uint8_t *digits;
    int8_t sign;
    size_t len;
} BigInt;

/* free up memory allocated for x */
void bigint_free(BigInt *x);

/* returns copy of x */
BigInt *bigint_copy(const BigInt *x);

/* fill x->digits with zeroes from position beg to position n*/
uint8_t bigint_fillzero(BigInt *x, size_t beg, size_t n);

/* returns 1 if a == 0 */
uint8_t bigint_iszero(const BigInt *x);
/* returns 1 if a < b, 0 otherwise */
uint8_t bigint_isless(const BigInt *a, const BigInt *b);
/* returns 1 if a == b */
uint8_t bigint_iseq(const BigInt *a, const BigInt *b);

/* make BigInt from string */
BigInt *strtobi(char *str);
/* make string from BigInt */
char *bitostr(const BigInt *x);
/* make BigInt from small positive integer */
BigInt *postobi(uint8_t n);

/* a + b */
BigInt *bigint_sum(const BigInt *a, const BigInt *b);
/* a - b */
BigInt *bigint_subtract(const BigInt *a, const BigInt *b);
/* a * b */
BigInt *bigint_multiply(const BigInt *a, const BigInt *b);
/* a! */
BigInt *bigint_factorial(const BigInt *a);
/* a / b */
BigInt *bigint_divide(const BigInt *a, const BigInt *b);
/* a % b */
BigInt *bigint_mod(const BigInt *a, const BigInt *b);

/* a += b */
void bigint_add(BigInt **a, const BigInt *b);
/* a -= b */
void bigint_sub(BigInt **a, const BigInt *b);
/* a *= b */
void bigint_mul(BigInt **a, const BigInt *b);
/* a += digit */
uint8_t bigint_adddigit(BigInt **a, uint8_t digit);

/* a, b = b, a */
void bigint_swap(BigInt **a, BigInt **b);

/* returns the greates common divisor of a and b */
BigInt *bigint_gcd(const BigInt *a, const BigInt *b);