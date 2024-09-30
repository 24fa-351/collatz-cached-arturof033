#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "collatzBase.h"

#ifndef CACHELFU_H
#define CACHELFU_H

typedef struct nodeLFU{
    unsigned long long condition;
    unsigned long long value;
    unsigned long long counter;
    struct nodeLFU *next;
    struct nodeLFU *prev;
} nodeLFU;

typedef struct LinkedListLFU{
    nodeLFU *first;
    nodeLFU *last;
    unsigned long long size;
    unsigned long long maxSize;
    unsigned long long condition_list_size;
    struct nodeLFU **condition_list;
} LinkedListLFU;

typedef struct hit_miss_LFU{
    unsigned long long hit;
    unsigned long long miss;
} hit_miss_LFU;

void hit_ratioLFU();

void LFU_initializer(unsigned long long array_size, unsigned long long cache_size);

bool cacheLFU_has(unsigned long long condition);

bool cacheLFU_full();

unsigned long long cacheLFU_value_for(unsigned long long condition);

void cacheLFU_insert(unsigned long long condition, unsigned long long value);

void cacheLFU_free();

unsigned long long LFU_collatz(unsigned long long condition);

CollatzCache get_cacheLFU(unsigned long long array_size, unsigned long long cache_size);

CollatzFunc get_LFU_deactivator();

CollatzFunc get_LFU_hit_ratio();

#endif