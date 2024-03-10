# Long arithmetics (a-la bigint) library
## What is it
This is a static library in C which provides BigInt arithmetics.

## Getting Started
### Requirements
* CMake (version 3.1 or higher)
* GCC

### Build
Firstly, clone this repository:
```
https://github.com/AlexanderYavorovsky/bigint.git
```
Then, navigate to project directory:
```
cd bigint
```
Finally, run this bash script to build the project and run tests:
``` 
./run.sh
```
Or you can also write `./build.sh` to simply build the project. The path to executable file will be: `build/maintest`

## Usage
This library provides following operations: `+` `-` `*` `/` `%` (division and modulo are implemented according to math definitions).\
They are:
* `bigint_sum`
* `bigint_subtract`
* `bigint_multiply`
* `bigint_divide`
* `bigint_mod`

There are also some functions:
* `bigint_factorial(const BigInt *a)` - returns factorial of the given number `a` in the given base `base`.
* `bigint_gcd(const BigInt *a, const BigInt *b)` - returns the greates common divisor of numbers `a` and `b`.

Use `strtobi(char *str)` to convert string to BigInt, and vice versa: `bitostr(const BigInt *x)`. For instance:
```
BigInt *a = strtobi("999");
BigInt *b = strtobi("123");
BigInt *sum = bigint_sum(a, b);
char *strsum = bitostr(sum);

printf("Sum is: %s\n", strsum);

bigint_free(a);
bigint_free(b);
bigint_free(sum);
free(strsum);
```
Here `bigint_free(BigInt *x)` is used to free up space allocated for the given BigInts. Do not forget to always clean up!
