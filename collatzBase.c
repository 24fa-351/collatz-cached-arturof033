#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "collatzBase.h"

unsigned long long collatz(unsigned long long condition)
{
    unsigned long long counter = 0;

    while (condition != 1)
    {
        if(condition % 2 == 0){
            condition = condition / 2;
        }
        else{
            condition = (condition * 3) + 1;
        }

        counter++;
    }

    return counter;
}