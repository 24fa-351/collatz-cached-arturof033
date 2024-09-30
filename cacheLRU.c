#include "cacheLRU.h"
#include "collatzBase.h"

LinkedListLRU *collatz_LRU;
hit_miss_LRU LRU = {.hit = 0, .miss = 0};

void hit_ratioLRU(){
    printf("hit: %llu\n", LRU.hit);
    printf("miss: %llu\n", LRU.miss);
    double hitRatio = (double) LRU.hit/(LRU.hit + LRU.miss);
    printf("hit ratio: %.5f\n", hitRatio);
}

void LRU_initializer(unsigned long long array_size, unsigned long long cache_size){
    
    collatz_LRU = (LinkedListLRU *)malloc(sizeof(LinkedListLRU));
    collatz_LRU->first = NULL;
    collatz_LRU->last = NULL;
    collatz_LRU->size = 0;
    collatz_LRU->maxSize = cache_size;
    collatz_LRU->condition_list_size = array_size;
    collatz_LRU->condition_list = (node **)malloc((array_size) * sizeof(node *));

    for(int ix = 0; ix < array_size; ix++)
    {
        collatz_LRU->condition_list[ix] = NULL;
    }
}

bool cacheLRU_has(unsigned long long condition){
    
    if (condition >= collatz_LRU->condition_list_size) {
        printf("Error: Condition out of bounds in LRU cache_has %llu\n", condition);
        return false;
    }

    if (collatz_LRU->condition_list[condition] != NULL)
    {
        LRU.hit++;
    }
    else {
        LRU.miss++;
    }

    return collatz_LRU->condition_list[condition] != NULL;
}

bool cacheLRU_full(){
    return collatz_LRU->size == collatz_LRU->maxSize;
}

unsigned long long cacheLRU_value_for(unsigned long long condition){
    
    if (condition >= collatz_LRU->condition_list_size || 
        collatz_LRU->condition_list[condition] == NULL) {
        printf("Error: Condition out of bounds in LRU cache_value_for %llu\n", condition);
        return -1;
    }

    return collatz_LRU->condition_list[condition]->value;
}

void cacheLRU_insert(unsigned long long condition, unsigned long long value){

    if (condition >= collatz_LRU->condition_list_size) {
        printf("Error: Condition out of bounds in LRU cache_insert %llu\n", condition);
        return;
    }

    //create new node based on given data
    node *newNode = (node *)malloc(sizeof(node));
    newNode->condition = condition;
    newNode->value = value;
    newNode->next = NULL;
    collatz_LRU->condition_list[condition] = newNode;

    //if the cache is full
    if(cacheLRU_full())
    {
        if(collatz_LRU->first == collatz_LRU->last)
        {
            node *tempNode = collatz_LRU->first;
            collatz_LRU->first = newNode;
            collatz_LRU->last = newNode;
            free(tempNode);
            tempNode = NULL;
        }
        else{
            node *tempNode = collatz_LRU->last;
            collatz_LRU->condition_list[collatz_LRU->last->condition] = NULL;
            collatz_LRU->last = collatz_LRU->last->next;
            collatz_LRU->first->next = newNode;
            collatz_LRU->first = newNode;
            free(tempNode);
            tempNode = NULL;
        }
    

    }
    //if the cache is not full
    else{
        //if the list is empty
        if (collatz_LRU->first == NULL){
            collatz_LRU->first = newNode;
            collatz_LRU->last = newNode;
        }
        //if first and last are assigned
        else{
            collatz_LRU->first->next = newNode;
            collatz_LRU->first = newNode;
        }

        //increment size
        collatz_LRU->size++;
    }

}

void cacheLRU_free(){
    
    node *current = collatz_LRU->first;
    node *nextNode;

    for(int ix = 0; ix < collatz_LRU->condition_list_size; ix++)
    {
        collatz_LRU->condition_list[ix] = NULL;
    }

    while (current != NULL) {
        nextNode = current->next;
        free(current);

        current = nextNode;
    }

    free(current);
    current = NULL;
    free(nextNode);
    nextNode = NULL;
    free(collatz_LRU->condition_list);
    free(collatz_LRU);
}

unsigned long long LRU_collatz(unsigned long long condition){
    
    if(cacheLRU_has(condition)){
        return cacheLRU_value_for(condition);
    }

    unsigned long long counter = collatz(condition);

    cacheLRU_insert(condition, counter);

    return counter;
}

unsigned long long (*get_cacheLRU(unsigned long long array_size, unsigned long long cache_size))(unsigned long long){
    LRU_initializer(array_size, cache_size);
    return LRU_collatz;
}

void (*get_LRU_deactivator())(){
    return cacheLRU_free;
}

void (*get_LRU_hit_ratio())(){
    return hit_ratioLRU;
}