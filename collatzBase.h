#include <stdbool.h>
#include <stdint.h>

#ifndef COLLATZ_H
#define COLLATZ_H

typedef unsigned long long (*CollatzCache)(unsigned long long);

typedef void (*CollatzFunc)();

unsigned long long collatz(unsigned long long condition);

#endif