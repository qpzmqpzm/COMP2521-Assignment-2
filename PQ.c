#include "PQ.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define MAX_SIZE_ARRAY 2000

// *************** COST ANALYSIS OF PQ REP *****************
// This PQ ADT uses an array to store items
// Things to Note:
//      When found smallest item, it'll switch with the last item in the list
//      Therefore, the array order holds no information ('random')
//      First found, first picked

// Cost Analysis:
//      Time Analysis: search => O(n)
//                     insert => O(1)
//      Storage Analysis: O(MAX_ARRAY_SIZE * sizeof(ItemPQ)
// First simple implementation of PQ (needs to be reviewed to improve search O())
// Suspect it might be to do with insertion (inserting in a particular order) w/ search algorithm
// *************** COST ANALYSIS OF PQ REP *****************


// ########## Structs ##########
typedef struct PQRep {
    int size;
    ItemPQ queue[MAX_SIZE_ARRAY];
} PQRep;


// ########## PQImplementation ##########
PQ newPQ(void) {
    PQ myPQ = malloc(sizeof(PQRep));
    assert(myPQ != NULL);
    myPQ->size = 0;

    return myPQ;
}

void  addPQ(PQ myPQ, ItemPQ item) {

    myPQ->queue[myPQ->size] = item;
    myPQ->size++;

}

// Warning: Do not pass in an empty PQ
ItemPQ dequeuePQ(PQ myPQ) {
    // Assume first item of the PQ is min
    ItemPQ min = myPQ->queue[0];
    int minPos = 0;
    int i;
    for (i = 0; i < myPQ->size-1; i++) {
        if (min.value > myPQ->queue[i].value) {
            min = myPQ->queue[i];
            minPos = i;
        }
    }

    myPQ->queue[minPos] = myPQ->queue[--myPQ->size];

    return min;
}

void updatePQ(PQ myPQ, ItemPQ item) {
    int i;
    for (i = 0; i < myPQ->size; i++) {
        if (item.key == myPQ->queue[i].key) {
            myPQ->queue[i].value = item.value;
            break;
        }
    }
}

int PQEmpty(PQ myPQ) {
    if (myPQ->size > 0) {
        return false;
    } else {
        return true;
    }
}

void showPQ(PQ myPQ) {
    printf("PQ: ");
    int i;
    for (i = 0; i < myPQ->size; i++) {
        printf("(Key: %d, Value: %d) ", myPQ->queue[i].key, myPQ->queue[i].value);
    }
    printf("\n");
}

void freePQ(PQ myPQ) {
    free(myPQ);
}
