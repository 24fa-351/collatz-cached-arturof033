#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "collatzBase.h"

#ifndef CACHELRU_H
#define CACHELRU_H

typedef struct node{
    unsigned long long condition;
    unsigned long long value;
    struct node *next;
} node;

typedef struct LinkedListLRU{
    node *first;
    node *last;
    unsigned long long size;
    unsigned long long maxSize;
    unsigned long long condition_list_size;
    struct node **condition_list;
} LinkedListLRU;

typedef struct hit_miss_LRU{
    unsigned long long hit;
    unsigned long long miss;
} hit_miss_LRU;

void hit_ratioLRU();

void LRU_initializer(unsigned long long array_size, unsigned long long cache_size);

bool cacheLRU_has(unsigned long long condition);

bool cacheLRU_full();

unsigned long long cacheLRU_value_for(unsigned long long condition);

void cacheLRU_insert(unsigned long long condition, unsigned long long value);

void cacheLRU_free();

unsigned long long LRU_collatz(unsigned long long condition);

CollatzCache get_cacheLRU(unsigned long long array_size, unsigned long long cache_size);

CollatzFunc get_LRU_deactivator();

CollatzFunc get_LRU_hit_ratio();

#endif