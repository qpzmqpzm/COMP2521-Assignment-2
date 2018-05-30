#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(){
    printf("Create PQ\n");
    PQ myPQ = newPQ();
    printf("assert myPQ is empty\n");
    assert(PQEmpty(myPQ));
    printf("Add Item (key: 4, value: 10)\n");
    ItemPQ myItem;
    myItem.key = 4; myItem.value = 10;
    addPQ(myPQ, myItem);
    showPQ(myPQ);
    printf("Add Item (key: 2, value: 20)\n");
    myItem.key = 2; myItem.value = 20;
    addPQ(myPQ, myItem);
    showPQ(myPQ);
    printf("Update key: 4 to value: 1\n");
    myItem.key = 4; myItem.value = 1;
    updatePQ(myPQ, myItem);
    showPQ(myPQ);
    printf("assert PQ is not empty\n");
    assert(!PQEmpty(myPQ));
    printf("Pop off key: 4, value: 1\n");
    myItem = dequeuePQ(myPQ);
    showPQ(myPQ);
    printf("assert item popped off key == 4 and value == 1\n");
    assert(myItem.key == 4);
    assert(myItem.value == 1);
    printf("SUCCESS! FreePQ");
    freePQ(myPQ);
    printf("\n finish testing... \n");



  return EXIT_SUCCESS;
}
