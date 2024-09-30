#include "cacheLFU.h"
#include "collatzBase.h"

LinkedListLFU *collatz_LFU;
hit_miss_LFU LFU = {.hit = 0, .miss = 0};

void hit_ratioLFU(){
    printf("hit: %llu\n", LFU.hit);
    printf("miss: %llu\n", LFU.miss);
    double hitRatio = (double) LFU.hit/(LFU.hit + LFU.miss);
    printf("hit ratio: %.5f\n", hitRatio);
}

void LFU_initializer(unsigned long long array_size, unsigned long long cache_size){
       
    collatz_LFU = (LinkedListLFU *)malloc(sizeof(LinkedListLFU));
    collatz_LFU->first = NULL;
    collatz_LFU->last = NULL;
    collatz_LFU->size = 0;
    collatz_LFU->maxSize = cache_size;
    collatz_LFU->condition_list_size = array_size;
    collatz_LFU->condition_list = (nodeLFU **)malloc((array_size) * sizeof(nodeLFU *));

    for(int ix = 0; ix < array_size; ix++)
    {
        collatz_LFU->condition_list[ix] = NULL;
    }
    
}

bool cacheLFU_has(unsigned long long condition){
    
    if (condition >= collatz_LFU->condition_list_size) {
        printf("Error: Condition out of bounds in cache_has\n");
        return false;
    }

    if (collatz_LFU->condition_list[condition] != NULL)
    {
        LFU.hit++;
    }
    else {
        LFU.miss++;
    }

    return collatz_LFU->condition_list[condition] != NULL;
}

bool cacheLFU_full(){
    return collatz_LFU->size == collatz_LFU->maxSize;
}

unsigned long long cacheLFU_value_for(unsigned long long condition){

    if (condition >= collatz_LFU->condition_list_size || 
        collatz_LFU->condition_list[condition] == NULL) {
        printf("Error: Condition out of bounds in cache_value_for\n");
        return -1;
    }

    collatz_LFU->condition_list[condition]->counter++;

    // **********************************
    // ORDERING HAS YET TO BE IMPLEMENTED
    // **********************************
    // //if the list has one node or if the cached for node is at the front
    // if(collatz_LFU->condition_list_size == 1 || 
    //     collatz_LFU->condition_list[condition] == collatz_LFU->first){
        
    //     return collatz_LFU->condition_list[condition]->value; 
    // }
    // else if(collatz_LFU->condition_list[condition]->next->counter >= 
    //         collatz_LFU->condition_list[condition]->counter){

    //     if(collatz_LFU->condition_list[condition]->next->counter == 
    //         collatz_LFU->condition_list[condition]->counter){

                
    //         }

        
    //  }

    return collatz_LFU->condition_list[condition]->value;
}

void cacheLFU_insert(unsigned long long condition, unsigned long long value){

    if (condition >= collatz_LFU->condition_list_size) {
        printf("Error: Condition out of bounds in LFU cache_insert\n");
        return;
    }

    //create new node based on given data
    nodeLFU *newNode = (nodeLFU *)malloc(sizeof(nodeLFU));
    newNode->condition = condition;
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->counter = 1;
    collatz_LFU->condition_list[condition] = newNode;

    //if the cache is full
    if(cacheLFU_full())
    {
        //cache of size one
        if(collatz_LFU->first == collatz_LFU->last)
        {
            collatz_LFU->first = newNode;
            collatz_LFU->last = newNode;
        }
        //cache of size two
        else if (collatz_LFU->size == 2){
            collatz_LFU->last = newNode;
            newNode->next = collatz_LFU->first;
            collatz_LFU->first->prev = newNode;
        }
        else{
            // if all elements have beeen referenced the same amount
            if(collatz_LFU->first->counter == collatz_LFU->last->counter){

                //insert newNode at the front of the list
                collatz_LFU->first->prev = newNode;
                newNode->next = collatz_LFU->first;
                collatz_LFU->first = newNode;

                //remove last node
                nodeLFU *tempNode = collatz_LFU->last;
                collatz_LFU->last = collatz_LFU->last->prev;
                collatz_LFU->last->next = NULL;

                free(tempNode);
                tempNode = NULL;
            }
            // if node to insert belongs at the end of the list
            else if(collatz_LFU->last->prev->counter != 1) {

                nodeLFU *tempNode = collatz_LFU->last;
                nodeLFU *prevNode = collatz_LFU->last->prev;

                //swap newNode with last node
                prevNode->next = newNode;
                newNode->prev = prevNode;
                collatz_LFU->last = newNode;

                free(tempNode);
                tempNode = NULL;
                free(prevNode);
                prevNode = NULL;
            }
            // if node to insert belongs in the middle of the list
            else{
                nodeLFU *current = collatz_LFU->first;
                nodeLFU *nextNode = collatz_LFU->first->next;

                // search for position for newNode
                while(nextNode->counter != 1){
                    current = current->next;
                    nextNode = nextNode->next;
                }

                //insert newNode in list position
                nextNode->prev = newNode;
                newNode->next = nextNode;
                current->next = newNode;
                newNode->prev = current;

                // remove last node
                nodeLFU *tempNode = collatz_LFU->last;
                collatz_LFU->last = collatz_LFU->last->prev;
                collatz_LFU->last->next = NULL;
                                 
                free(tempNode);
                tempNode = NULL;
                free(current);
                current = NULL;
                free(nextNode);
                nextNode = NULL;
            }
        }
    }
    //if the cache is not full
    else{
        //if the list is empty
        if (collatz_LFU->first == NULL){
            collatz_LFU->first = newNode;
            collatz_LFU->last->next = newNode;
        }
        //if there is only one element in the array
        else if (collatz_LFU->first == collatz_LFU->last){
            collatz_LFU->first = newNode; 
            collatz_LFU->last->next = collatz_LFU->first;
            collatz_LFU->first->prev = collatz_LFU->last;
        }
        //if first and last are assigned
        else{
            nodeLFU *tempNode = collatz_LFU->first;
            collatz_LFU->first = newNode;
            newNode->prev = tempNode;
            tempNode->next = newNode;
            free(tempNode);
            tempNode = NULL;
        }

        //increment size
        collatz_LFU->size++;
    }

}

void cacheLFU_free(){

    nodeLFU *current = collatz_LFU->first;
    nodeLFU *nextNode;

    for(int ix = 0; ix < collatz_LFU->condition_list_size; ix++)
    {
        collatz_LFU->condition_list[ix] = NULL;
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
    free(collatz_LFU->condition_list);
    free(collatz_LFU);

}


unsigned long long LFU_collatz(unsigned long long condition){
    
    if(cacheLFU_has(condition)){
        return cacheLFU_value_for(condition);
    }

    unsigned long long counter = collatz(condition);

    cacheLFU_insert(condition, counter);

    return counter;
}

unsigned long long (*get_cacheLFU(unsigned long long array_size, unsigned long long cache_size))(unsigned long long){
    LFU_initializer(array_size, cache_size);
    return LFU_collatz;
}

void (*get_LFU_deactivator())(){
    return cacheLFU_free;
}

void (*get_LFU_hit_ratio())(){
    return hit_ratioLFU;
}