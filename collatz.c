#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collatzBase.h"
#include "cacheLRU.h"
#include "cacheLFU.h"

/*
./collatz VALUES SMALLEST LARGEST CACHE_SIZE METHOD

VALUES = amount of random values to test
SMALLEST = lower range limit of random values to choose from
LARGEST = upper range limit of random value to choose from
CACHE_SIZE = size of cache for LRU or LFU
METHOD = choosing between "LRU", "LFU", or "none"

Outputs a list of the random numbers tested and 
    the number of iterations it took for the number to reach 1
*/

CollatzCache collatz_provider;
CollatzFunc collatz_deactivator;
CollatzFunc collatz_hit_ratio;

int main(int argc, char * argv[]) {

    unsigned long long values = atoi(argv[1]);
    unsigned long long smallest = atoi(argv[2]);
    unsigned long long largest = atoi(argv[3]);
    unsigned long long cache_size = atoi(argv[4]);
    char* method = argv[5];

    // *****************************
    // LFU NOT CURRENRLT FUNCTIONING
    // *****************************
    if (strcmp(method, "LFU") == 0){
        collatz_provider = get_cacheLFU(largest, cache_size);
        collatz_deactivator = get_LFU_deactivator();
        collatz_hit_ratio = get_LFU_hit_ratio(); 
    }
    else if(strcmp(method, "LRU") == 0){
        collatz_provider = get_cacheLRU(largest + 1, cache_size);
        collatz_deactivator = get_LRU_deactivator();
        collatz_hit_ratio = get_LRU_hit_ratio();  
    }
    else if (strcmp(method, "none") == 0) {
        collatz_provider = collatz;
        collatz_deactivator = NULL;
        collatz_hit_ratio = NULL;
    }

    for (int ix = 0; ix < values; ix++) 
    {
        unsigned long long randomValue =  rand() % (largest + 1 - smallest) + smallest;
        unsigned long long collatzCounter = (*collatz_provider)(randomValue);
        printf("%llu, %llu\n", randomValue, collatzCounter);
    }

    if(collatz_deactivator != NULL && collatz_hit_ratio != NULL)
    {
        (*collatz_hit_ratio)();

        (*collatz_deactivator)();
    }

    return 0;
}